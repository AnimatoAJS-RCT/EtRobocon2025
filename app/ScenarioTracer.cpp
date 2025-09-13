#include "ScenarioTracer.h"
#include <stdio.h>
#include <algorithm>  // for std::max, std::min

// 回転数差を補正するためのPゲイン
const double ScenarioTracer::Kp = 0.05;
ScenarioTracer::ScenarioTracer(Walker* walker, int leftPwm, int rightPwm)
  : mWalker(walker), mLeftPwm(leftPwm), mRightPwm(rightPwm), mIsInitialized(false)
{
    mState = UNDEFINED;
}

void ScenarioTracer::run()
{
    printf("ScenarioTracer.run: mState = %d\n", mState);
    switch(mState) {
        case UNDEFINED:
            if(!mIsInitialized) {
                mWalker->init();
                mIsInitialized = true;
            }
            mState = WAITING_FOR_START;
            break;
        case WAITING_FOR_START:
            if(mStarterList.empty()) {
                mWalker->setPwm(mLeftPwm, mRightPwm);
                mState = WALKING;
                return;
            }
            for(auto starter : mStarterList) {
                if(starter->isPushed()) {
                    mWalker->setPwm(mLeftPwm, mRightPwm);
                    mState = WALKING;
                    return;
                }
            }
            break;
        case WALKING:
            execWalking();
            break;
        case TERMINATED:
            // Do nothing
            break;
        default:
            break;
    }
}

void ScenarioTracer::execWalking()
{
    // 目標のPWM比に従って回転数が追従するように補正をかける
    int leftCount = mWalker->getLeftCount();
    int rightCount = mWalker->getRightCount();

    // 目標回転数比と現在回転数比の誤差を計算
    // error = leftCount * mRightPwm - rightCount * mLeftPwm
    // この値が0になるように制御する
    double error = static_cast<double>(leftCount) * mRightPwm - static_cast<double>(rightCount) * mLeftPwm;

    // 補正量を計算 (P制御)
    double correction = Kp * error;

    // 補正後のPWM値を計算
    // error > 0 -> 左が回りすぎ -> 左を弱め、右を強める
    // error < 0 -> 右が回りすぎ -> 右を弱め、左を強める
    int correctedLeftPwm = mLeftPwm - static_cast<int>(correction);
    int correctedRightPwm = mRightPwm + static_cast<int>(correction);

    printf("Correction: L/R Cnt=%d/%d, Err=%.1f, Corr=%.1f, PWM L/R=%d/%d -> %d/%d\n",
           leftCount, rightCount, error, correction, mLeftPwm, mRightPwm, correctedLeftPwm, correctedRightPwm);

    mWalker->setPwm(correctedLeftPwm, correctedRightPwm);
    mWalker->run();


    for(auto terminator : mTerminatorList) {
        if(terminator->isToBeTerminate()) {
            mWalker->stop();
            mState = TERMINATED;
            int l = mWalker->getLeftCount();
            int r = mWalker->getRightCount();
            printf("Stop: LC=%d, RC=%d\n", l, r);
            return;
        }
    }
}
