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
#include "Walker.h"
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
static Starter* gStarter;
static ColorTerminator* gColorTerminator;

std::vector<Tracer*> tracerList;

void generateTracerList()
{
    // iniファイル読み込み
    int pwm, maxPwm;
    double p, i, d;

    // シミュレーター環境でファイルを読み込めないため固定文字列で設定値を読み込む
    const char* lines[] = {
        "LineTracer 1000 50 60 80 LEFT_EDGE 0.7 0.1 0.6 BLUE",
        "#end"
    };
    char* spl[16];
    size_t result_size;
    int idx = 0;
    // 1行ずつ値を読み取り使用
    strcpy((char*)spl, lines[idx]);
    while(strcmp(lines[idx], "#end") != 0) {
        // printf("readini: a%sa, %d\n", line, strcmp(line, "#end"));
        if(lines[idx][0] == '#') {
            idx++;
            continue;
        }

        result_size = split((char*)lines[idx], " ", spl, SIZE_OF_ARRAY(spl));
        for(size_t i = 0; i < result_size; ++i) {
            printf("%zu: %s\n", i, spl[i]);
        }
        printf("\n");

        if(strcmp(spl[0], "ScenarioTracer") == 0) {
            // TODO:ScenarioTracer生成
            //            double targetDistance;
            //            int leftPwm, rightPwm;
            //            targetDistance = atof(spl[1]);
            //            leftPwm = atoi(spl[2]);
            //            rightPwm = atoi(spl[3]);
            //            if (result_size >= 5)
            //            {
            //                colorid_t stopColor;
            //                if (strcmp(spl[4], "BLACK") == 0)
            //                {
            //                    stopColor = colorid_t::COLOR_BLACK;
            //                }
            //                else if (strcmp(spl[9], "BLUE") == 0)
            //                {
            //                    stopColor = colorid_t::COLOR_BLUE;
            //                }
            //                else if (strcmp(spl[9], "RED") == 0)
            //                {
            //                    stopColor = colorid_t::COLOR_RED;
            //                }
            //                else if (strcmp(spl[9], "GREEN") == 0)
            //                {
            //                    stopColor = colorid_t::COLOR_GREEN;
            //                }
            //                else if (strcmp(spl[9], "YELLOW") == 0)
            //                {
            //                    stopColor = colorid_t::COLOR_YELLOW;
            //                }
            //                printf("ScenarioTracer(%lf, %d, %d, %d): push\n", targetDistance,
            //                leftPwm, rightPwm, stopColor); courseList.push_back(new
            //                ScenarioTracer(targetDistance, leftPwm, rightPwm, stopColor));
            //            }
            //            else
            //            {
            //                printf("ScenarioTracer(%lf, %d, %d): push\n", targetDistance, leftPwm,
            //                rightPwm); courseList.push_back(new ScenarioTracer(targetDistance,
            //                leftPwm, rightPwm));
            //            }
        } else if(strcmp(spl[0], "LineTracer") == 0) {
            //TODO:targetDistanceでのterminateを実装
            double targetDistance, p, i, d;
            int targetBrightness, pwm, maxPwm;
            bool isLeftEdge;
            PidGain* pidGain;
            targetDistance = atof(spl[1]);
            targetBrightness = atoi(spl[2]);
            pwm = atoi(spl[3]);
            maxPwm = atoi(spl[4]);
            isLeftEdge = (strcmp(spl[5], "LEFT_EDGE") == 0);
            p = atof(spl[6]);
            i = atof(spl[7]);
            d = atof(spl[8]);
            // printf("LineTracer(%lf, %d, %d, %d, %s, PidGain(%lf, %lf, %lf)): push\n",
            // targetDistance, targetBrightness, pwm, maxPwm, isLeftEdge ? "LEFT_EDGE" :
            // "RIGHT_EDGE", p, i, d);
            pidGain = new PidGain(p, i, d);
            gWalker->setPwm(pwm);
            gLineTracer = new LineTracer(gLineMonitor, gWalker, isLeftEdge, pidGain);
            gLineTracer->addStarter(gStarter);
            if(result_size >= 10) {
                // 色で停止
                uint16_t stopColor;
                if(strcmp(spl[9], "BLUE") == 0) {
                    stopColor = PBIO_COLOR_HUE_BLUE;
                } else if(strcmp(spl[9], "RED") == 0) {
                    stopColor = PBIO_COLOR_HUE_RED;
                } else if(strcmp(spl[9], "GREEN") == 0) {
                    stopColor = PBIO_COLOR_HUE_GREEN;
                } else if(strcmp(spl[9], "YELLOW") == 0) {
                    stopColor = PBIO_COLOR_HUE_YELLOW;
                }
                gColorTerminator = new ColorTerminator(&gColorSensor, stopColor);
                gLineTracer->addTerminator(gColorTerminator);
            }
            tracerList.push_back(gLineTracer);
        }
        // TODO:難所トレーサーの実装
        //        else if (strcmp(spl[0], "RotateTracer") == 0)
        //        {
        //            int direction, degree, pwm;
        //            if (strcmp(spl[1], "TURN_RIGHT") == 0)
        //            { // ^ (!IS_LEFT_COURSE)){
        //                direction = 1;
        //            }
        //            else
        //            {
        //                direction = -1;
        //            }
        //            if (strcmp(spl[1], "TURN_LEFT") == 0)
        //            { // ^ (!IS_LEFT_COURSE)){
        //                direction = -1;
        //            }
        //            else
        //            {
        //                direction = 1;
        //            }
        //            degree = atoi(spl[2]);
        //            if (result_size >= 4)
        //            {
        //                pwm = atoi(spl[3]);
        //                printf("RotateTracer(%d, %d, %d): push\n", direction, degree, pwm);
        //                courseList.push_back(new RotateTracer(direction, degree, pwm));
        //            }
        //            else
        //            {
        //                printf("RotateTracer(%d, %d): push\n", direction, degree);
        //                courseList.push_back(new RotateTracer(direction, degree));
        //            }
        //        }
        //        else if (strcmp(spl[0], "DifficultScenarioTracer") == 0)
        //        {
        //            int direction, maxTimer, maxCnt;
        //            if ((strcmp(spl[1], "TURN_RIGHT") == 0))
        //            { // ^ (!IS_LEFT_COURSE)){
        //                direction = 1;
        //            }
        //            else
        //            {
        //                direction = -1;
        //            }
        //            if ((strcmp(spl[1], "TURN_LEFT") == 0))
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
        //                maxTimer = atoi(spl[2]);
        //                printf("DifficultScenarioTracer(%d, %d): push\n", direction, maxTimer);
        //                courseList.push_back(new DifficultScenarioTracer(direction, maxTimer));
        //            }
        //            else if (result_size == 4)
        //            {
        //                maxTimer = atoi(spl[2]);
        //                maxCnt = atoi(spl[3]);
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
        idx++;
    }
}

/**
 * システム生成
 */
static void user_system_create()
{
    // コース設定
    IS_LEFT_COURSE = true;
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
        tracerList.front()->run();
        // TODO:terminateしたら次のTracerに行くようにする
    }

    ext_tsk();
}
