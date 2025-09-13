#include "Calibrator.h"
#include "Util.h"
#include "app.h"  // IS_LEFT_COURSE を参照するため
#include <stdio.h>  // for sprintf
#include <thread>
#include <chrono>

Calibrator::Calibrator(const spikeapi::ColorSensor& colorSensor,
                       const spikeapi::ForceSensor& forceSensor)
  : mColorSensor(colorSensor), mForceSensor(forceSensor)
{
}

void Calibrator::run()
{
    execUndefined();

    // 1. 開始待ち
    execWaitingForStart();

    // 2. コース設定
    execSettingCourse();

    // 3. 黒測定
    execCalibratingBlack();

    // 4. 白測定
    execCalibratingWhite();

    // 5. 終了
    execTerminated();
}

void Calibrator::execTerminated()
{
    printf("Calibration Finished.\n");
    printf("Target: %d\n", getTarget());
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));  // 2s wait to show result
    mState = TERMINATED;
}

int Calibrator::getBlack()
{
    return mBlack;
}

int Calibrator::getWhite()
{
    return mWhite;
}

int Calibrator::getTarget()
{
    return (mWhite + mBlack) / 2;
}

bool Calibrator::isFinished()
{
    return mState == TERMINATED;
}

void Calibrator::execUndefined()
{
    if(mIsInitialized == false) {
        // 初期化処理
        mIsInitialized = true;
    }
    mState = WAITING_FOR_START;
}

void Calibrator::execWaitingForStart()
{
    printf("Calibrate: Push to start\n");
    // ボタンが押されるまで待つ
    while(!mForceSensor.isTouched()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // ボタンが離されるまで待つ（チャタリング防止）
    while(mForceSensor.isTouched()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    mState = SETTING_COURSE;
}

void Calibrator::execSettingCourse()
{
    printf("Set Course: Push to toggle\n");

    // 3秒間入力がなければ次のステップへ
    const int timeout_ms = 3000;
    auto last_input_time = std::chrono::steady_clock::now();

    while (true) {
        // 現在のコース設定を表示
        printf("Course: %s (Confirm in %lds) \n",
               IS_LEFT_COURSE ? "LEFT " : "RIGHT",
               (long)(timeout_ms / 1000
                      - std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::steady_clock::now() - last_input_time)
                            .count()));

        if (mForceSensor.isTouched()) {
            IS_LEFT_COURSE = !IS_LEFT_COURSE;  // コースをトグル
            last_input_time = std::chrono::steady_clock::now(); // タイマーリセット
            // ボタンが離されるまで待つ
            while(mForceSensor.isTouched()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        // タイムアウト判定
        if (std::chrono::steady_clock::now() - last_input_time > std::chrono::milliseconds(timeout_ms)) {
            printf("\nCourse set to %s.\n", IS_LEFT_COURSE ? "LEFT" : "RIGHT");
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    mState = CALIBRATING_BLACK;
}

void Calibrator::execCalibratingBlack()
{
    printf("Set to Black & Push\n");
    // ボタンが押されるまで待つ
    while(!mForceSensor.isTouched()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    mBlack = mColorSensor.getReflection();
    printf("Black: %d\n", mBlack);
    // ボタンが離されるまで待つ
    while(mForceSensor.isTouched()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    mState = CALIBRATING_WHITE;
}

void Calibrator::execCalibratingWhite()
{
    printf("Set to White & Push\n");
    // ボタンが押されるまで待つ
    while(!mForceSensor.isTouched()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    mWhite = mColorSensor.getReflection();
    printf("White: %d\n", mWhite);
    // ボタンが離されるまで待つ
    while(mForceSensor.isTouched()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    mState = TERMINATED;
}
