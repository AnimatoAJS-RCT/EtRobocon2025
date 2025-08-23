/******************************************************************************
 *  app.cpp (for SPIKE)
 *  Created on: 2025/01/05
 *  Implementation of the Task main_task
 *  Author: Kazuhiro.Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "app.h"
#include "Tracer.h"
#include "LineMonitor.h"
#include "LineTracer.h"
#include "ScenarioTracer.h"
#include "Walker.h"
#include "DistanceTerminator.h"
#include "ColorTerminator.h"
#include "Util.h"

#include "Light.h"
#include "Button.h"
#include "Display.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <string>

#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
// void *__dso_handle=0;

using namespace spikeapi;

// Device objects
// オブジェクトを静的に確保する
ColorSensor gColorSensor(EPort::PORT_E);
ForceSensor gForceSensor(EPort::PORT_D);
Motor gLeftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true);
Motor gRightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true);

// オブジェクトの定義
static LineMonitor* gLineMonitor;
static Walker* gWalker;
static LineTracer* gLineTracer;
static ScenarioTracer* gScenarioTracer;
static Starter* gStarter;
static DistanceTerminator* gDistanceTerminator;
static ColorTerminator* gColorTerminator;

std::vector<Tracer*> tracerList;

void generateTracerList()
{
    // iniファイル読み込み
    int pwm, maxPwm;
    double p, i, d;
    std::vector<std::string> spl;
    size_t result_size;

#ifdef DEBUG_MODE
  // DEBUG_MODEが定義されている場合のコード
  // （例：デバッグ用のログ出力など）
  printf("Debug mode is enabled.\n");
#else
  // DEBUG_MODEが定義されていない場合のコード
  printf("Debug mode is disabled.\n");
#endif

#ifndef MAKE_RASPIKE
    printf("sim\n");
    // シミュレーター環境でファイルを読み込めないため固定文字列で設定値を読み込む
    const std::string lines[] = { "ScenarioTracer 10000 100 90 GREEN",
                                  "LineTracer 1000 50 60 80 LEFT_EDGE 0.8 0.1 0.6 BLUE", "#end" };
    int idx = 0;
    // strcpy((char*)spl, lines[idx]);
    while(lines[idx] != "#end") {
        printf("readini: %s\n", lines[idx].c_str());
        if(lines[idx][0] == '#') {
            idx++;
            continue;
        }

        spl = split(lines[idx], " ");
#else
    printf("notsim\n");
    char iniPath[512];
    getcwd(iniPath, 512);  // カレントディレクトリ取得
    //strcpy(iniPath, "/home/ajspi/work/RasPike-ART/sdk/workspace");

    if(IS_LEFT_COURSE) {
        strcat(iniPath, "/tracer_2025_left.ini");  // カレントディレクトリ配下のiniを指定
    } else {
        strcat(iniPath, "/tracer_2025_right.ini");  // カレントディレクトリ配下のiniを指定
    }

    printf("tracer.ini読み取り:%s\n", iniPath);
    FILE* file;
    file = fopen(iniPath, "r");  // ファイル読み込み
    char line[512];

    // 1行ずつ値を読み取り使用
    fgets(line, 512, file);
    line[strlen(line) - 1] = '\0';  // 改行まで読み込んでいるので末尾を削除（終端文字に変更）
    while(strcmp(line, "#end") != 0) {
        // printf("readini: a%sa, %d\n", line, strcmp(line, "#end"));
        if(line[0] == '#') {
            fgets(line, 512, file);
            line[strlen(line) - 1] = '\0';
            continue;
        }

        spl = split(line, " ");
#endif
        result_size = spl.size();
        for(size_t i = 0; i < result_size; ++i) {
            printf("%lu: %s\n", (unsigned long)i, spl[i].c_str());
        }
        printf("\n");

        if(spl[0] == "ScenarioTracer") {
            double targetDistance;
            int leftPwm, rightPwm;
            targetDistance = atof(spl[1].c_str());
            leftPwm = atof(spl[2].c_str());
            rightPwm = atof(spl[3].c_str());

            gScenarioTracer = new ScenarioTracer(gWalker, leftPwm, rightPwm);
            gScenarioTracer->addStarter(gStarter);

            gDistanceTerminator = new DistanceTerminator(gWalker, targetDistance);
            gScenarioTracer->addTerminator(gDistanceTerminator);

            if(result_size >= 5) {
                eColor stopColor;
                if(spl[4] == "BLACK") {
                    stopColor = BLACK;
                } else if(spl[4] == "BLUE") {
                    stopColor = BLUE;
                } else if(spl[4] == "RED") {
                    stopColor = RED;
                } else if(spl[4] == "GREEN") {
                    stopColor = GREEN;
                } else if(spl[4] == "YELLOW") {
                    stopColor = YELLOW;
                }
                gColorTerminator = new ColorTerminator(&gColorSensor, stopColor);
                gScenarioTracer->addTerminator(gColorTerminator);
            }
            tracerList.push_back(gScenarioTracer);

        } else if(spl[0] == "LineTracer") {
            // TODO:targetDistanceでのterminateを実装
            double targetDistance, p, i, d;
            int targetBrightness, pwm, maxPwm;
            bool isLeftEdge;
            PidGain* pidGain;
            targetDistance = atof(spl[1].c_str());
            gDistanceTerminator = new DistanceTerminator(gWalker, targetDistance);
            targetBrightness = atof(spl[2].c_str());
            pwm = atof(spl[3].c_str());
            maxPwm = atof(spl[4].c_str());
            isLeftEdge = (strcmp(spl[5].c_str(), "LEFT_EDGE") == 0);
            p = atof(spl[6].c_str());
            i = atof(spl[7].c_str());
            d = atof(spl[8].c_str());
            printf("LineTracer(%lf, %d, %d, %d, %s, PidGain(%lf, %lf, %lf)): push\n",
            targetDistance, targetBrightness, pwm, maxPwm, isLeftEdge ? "LEFT_EDGE" :
            "RIGHT_EDGE", p, i, d);
            pidGain = new PidGain(p, i, d);
            gLineTracer = new LineTracer(gLineMonitor, gWalker, pwm, isLeftEdge, pidGain);
            gLineTracer->addStarter(gStarter);
            gLineTracer->addTerminator(gDistanceTerminator);
            if(result_size >= 10) {
                // 色で停止
                eColor stopColor;
                if(spl[9] == "BLUE") {
                    stopColor = BLUE;
                } else if(spl[9] == "RED") {
                    stopColor = RED;
                } else if(spl[9] == "GREEN") {
                    stopColor = GREEN;
                } else if(spl[9] == "YELLOW") {
                    stopColor = YELLOW;
                }
                printf("stopColor: %s\n", colorToString(stopColor));
                gColorTerminator = new ColorTerminator(&gColorSensor, stopColor);
                gLineTracer->addTerminator(gColorTerminator);
            }
            tracerList.push_back(gLineTracer);
        }
        // TODO:難所トレーサーの実装
        //        else if (spl[0] == "RotateTracer")
        //        {
        //            int direction, degree, pwm;
        //            if (spl[1] == "TURN_RIGHT")
        //            { // ^ (!IS_LEFT_COURSE)){
        //                direction = 1;
        //            }
        //            else
        //            {
        //                direction = -1;
        //            }
        //            if (spl[1] == "TURN_LEFT")
        //            { // ^ (!IS_LEFT_COURSE)){
        //                direction = -1;
        //            }
        //            else
        //            {
        //                direction = 1;
        //            }
        //            degree = atof(spl[2].c_str());
        //            if (result_size >= 4)
        //            {
        //                pwm = atof(spl[3].c_str());
        //                printf("RotateTracer(%d, %d, %d): push\n", direction, degree, pwm);
        //                courseList.push_back(new RotateTracer(direction, degree, pwm));
        //            }
        //            else
        //            {
        //                printf("RotateTracer(%d, %d): push\n", direction, degree);
        //                courseList.push_back(new RotateTracer(direction, degree));
        //            }
        //        }
        //        else if (spl[0] == "DifficultScenarioTracer")
        //        {
        //            int direction, maxTimer, maxCnt;
        //            if ((spl[1] == "TURN_RIGHT"))
        //            { // ^ (!IS_LEFT_COURSE)){
        //                direction = 1;
        //            }
        //            else
        //            {
        //                direction = -1;
        //            }
        //            if ((spl[1] == "TURN_LEFT"))
        //            { // ^ (!IS_LEFT_COURSE)){
        //                direction = -1;
        //            }
        //            else
        //            {
        //                direction = 1;
        //            }
        //
        //            if (result_size == 3)
        //            {
        //                maxTimer = atof(spl[2].c_str());
        //                printf("DifficultScenarioTracer(%d, %d): push\n", direction, maxTimer);
        //                courseList.push_back(new DifficultScenarioTracer(direction, maxTimer));
        //            }
        //            else if (result_size == 4)
        //            {
        //                maxTimer = atof(spl[2].c_str());
        //                maxCnt = atof(spl[3].c_str());
        //                printf("DifficultScenarioTracer(%d, %d, %d): push\n", direction, maxTimer,
        //                maxCnt); courseList.push_back(new DifficultScenarioTracer(direction,
        //                maxTimer, maxCnt));
        //            }
        //            else
        //            {
        //                printf("DifficultScenarioTracer(%d): push\n", direction);
        //                courseList.push_back(new DifficultScenarioTracer(direction));
        //            }
        //        }
        else {
            // Tracer名にマッチしなかったらなにもしない
        }
#ifndef MAKE_RASPIKE
        idx++;
#else
        fgets(line, 512, file);
        line[strlen(line) - 1] = '\0';
#endif
    }
#ifdef MAKE_RASPIKE
    fclose(file);  // ファイルを閉じる
#endif
}

/**
 * システム生成
 */
static void user_system_create()
{
    // コース設定
    IS_LEFT_COURSE = false;
    // IS_LEFT_COURSE = g_isLeftCourse;

    // オブジェクトの作成
    gWalker = new Walker(gLeftWheel, gRightWheel);
    gLineMonitor = new LineMonitor(gColorSensor);
    gStarter = new Starter(gForceSensor);
    // gPidGain = new PidGain(0.7, 0.1, 0.6);
    // gLineTracer = new LineTracer(gLineMonitor, gWalker, 50, 50, true, gPidGain);
    generateTracerList();

    // 初期化完了通知
    Light light;
    light.turnOnColor(Light::EColor::ORANGE);
}

/**
 * システム破棄
 */
static void user_system_destroy()
{
    gLeftWheel.stop();
    gRightWheel.stop();
    gLeftWheel.resetCount();
    gRightWheel.resetCount();

    delete gLineTracer;
    delete gStarter;
    delete gLineMonitor;
    delete gWalker;
}

/**
 * メインタスク
 */
void main_task(intptr_t unused)
{
    user_system_create();  // センサやモータの初期化処理

    // 周期ハンドラ開始
    sta_cyc(CYC_TRACER);

    slp_tsk();  // バックボタンが押されるまで待つ

    // 周期ハンドラ停止
    stp_cyc(CYC_TRACER);

    user_system_destroy();  // 終了処理

    ext_tsk();
}

/**
 * ライントレースタスク
 */
void tracer_task(intptr_t exinf)
{
    Button button;

    if(button.isLeftPressed()) {
        wup_tsk(MAIN_TASK);  // レフトボタン押下
    } else {
        printf("tracer_task: tracerList size: %d\n", tracerList.size());
        if (!tracerList.empty() && tracerList.front() != nullptr && tracerList.front()->isTerminated()) {
            printf("remove\n");
            Tracer* tracer = tracerList.front();
            tracerList.erase(tracerList.begin());
            delete tracer;
        }

        if (!tracerList.empty() && tracerList.front() != nullptr) {
            tracerList.front()->run();
        }
    }

    ext_tsk();
}