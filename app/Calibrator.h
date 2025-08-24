#ifndef ETTR_APP_CALIBRATOR_H_
#define ETTR_APP_CALIBRATOR_H_

#include "ColorSensor.h"
#include "TouchSensor.h"

class Calibrator {
public:
    explicit Calibrator(const spikeapi::ColorSensor& colorSensor,
                        const spikeapi::TouchSensor& touchSensor);
    void run();
    int getBlack();
    int getWhite();
    int getTarget();
    bool isFinished();

private:
    void execUndefined();
    void execWaitingForStart();
    void execCalibratingBlack();
    void execWaitingForWhite();
    void execCalibratingWhite();
    void execWaitingForFinish();
    void execTerminated();

    enum State {
        UNDEFINED,
        WAITING_FOR_START,
        CALIBRATING_BLACK,
        WAITING_FOR_WHITE,
        CALIBRATING_WHITE,
        WAITING_FOR_FINISH,
        TERMINATED
    };
    State mState = UNDEFINED;

    const spikeapi::ColorSensor& mColorSensor;
    const spikeapi::TouchSensor& mTouchSensor;
    int mBlack = 0;
    int mWhite = 100;
    bool mIsInitialized = false;
};

#endif  // ETTR_APP_CALIBRATOR_H_
