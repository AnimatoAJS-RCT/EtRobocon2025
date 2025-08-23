#include "ScenarioTracer.h"
#include <stdio.h>

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
