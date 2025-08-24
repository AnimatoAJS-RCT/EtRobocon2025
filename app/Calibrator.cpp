#include "Calibrator.h"
#include "Util.h"

Calibrator::Calibrator(const spikeapi::ColorSensor& colorSensor,
                       const spikeapi::TouchSensor& touchSensor)
  : mColorSensor(colorSensor), mTouchSensor(touchSensor)
{
}

void Calibrator::run()
{
    while(1) {
        switch(mState) {
            case UNDEFINED:
                execUndefined();
                break;
            case WAITING_FOR_START:
                execWaitingForStart();
                break;
            case CALIBRATING_BLACK:
                execCalibratingBlack();
                break;
            case WAITING_FOR_WHITE:
                execWaitingForWhite();
                break;
            case CALIBRATING_WHITE:
                execCalibratingWhite();
                break;
            case WAITING_FOR_FINISH:
                execWaitingForFinish();
                break;
            case TERMINATED:
                // Do nothing
                return;
            default:
                break;
        }
    }
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
    msg_f("Calibrate: Push to start", 2);
    while(!mTouchSensor.isPressed()) {
        mState = CALIBRATING_BLACK;
        tslp_tsk(500);  // 500ms wait
    }
}

void Calibrator::execCalibratingBlack()
{
    msg_f("Calibrating black...", 1);
    mBlack = mColorSensor.getReflection();
    char msg[32];
    sprintf(msg, "Black: %d", mBlack);
    msg_f(msg, 2);
    tslp_tsk(1000);  // 1s wait
    mState = WAITING_FOR_WHITE;
}

void Calibrator::execWaitingForWhite()
{
    msg_f("Set white", 1);
    msg_f("Push to start", 2);
    if(mTouchSensor.isPressed()) {
        mState = CALIBRATING_WHITE;
        tslp_tsk(500);  // 500ms wait
    }
}

void Calibrator::execCalibratingWhite()
{
    msg_f("Calibrating white...", 1);
    mWhite = mColorSensor.getReflection();
    char msg[32];
    sprintf(msg, "White: %d", mWhite);
    msg_f(msg, 2);
    tslp_tsk(1000);  // 1s wait
    mState = WAITING_FOR_FINISH;
}

void Calibrator::execWaitingForFinish()
{
    msg_f("Finished.", 1);
    char msg[32];
    sprintf(msg, "Target: %d", getTarget());
    msg_f(msg, 2);
    if(mTouchSensor.isPressed()) {
        mState = TERMINATED;
    }
}
