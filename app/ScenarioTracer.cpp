#include "ScenarioTracer.h"

ScenarioTracer::ScenarioTracer(Walker* walker, int leftPwm, int rightPwm)
    : mWalker(walker), mLeftPwm(leftPwm), mRightPwm(rightPwm), mIsInitialized(false) {
    mState = UNDEFINED;
    mWalker->setPwm(mLeftPwm, mRightPwm);
}

void ScenarioTracer::run() {
    switch (mState) {
        case UNDEFINED:
            if (!mIsInitialized) {
                mWalker->init();
                mIsInitialized = true;
            }
            mState = WAITING_FOR_START;
            break;
        case WAITING_FOR_START:
            if (mStarterList.empty()) {
                mState = WALKING;
                return;
            }
            for (auto starter : mStarterList) {
                if (starter->isPushed()) {
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

void ScenarioTracer::execWalking() {
    mWalker->run();

    for (auto terminator : mTerminatorList) {
        if (terminator->isToBeTerminate()) {
            mWalker->stop();
            mState = TERMINATED;
            return;
        }
    }
}
