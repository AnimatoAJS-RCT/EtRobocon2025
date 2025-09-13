#ifndef ETTR_APP_CALIBRATOR_H_
#define ETTR_APP_CALIBRATOR_H_

#include "ColorSensor.h"
#include "ForceSensor.h"

class Calibrator {
public:
    explicit Calibrator(const spikeapi::ColorSensor& colorSensor,
                        const spikeapi::ForceSensor& forceSensor);
    void run();
    int getBlack();
    int getWhite();
    int getTarget();
    bool isFinished();

private:
    void execUndefined();
    void execWaitingForStart();
    void execSettingCourse();
    void execCalibratingBlack();
    void execCalibratingWhite();
    void execTerminated();

    enum State {
        UNDEFINED,
        WAITING_FOR_START,
        SETTING_COURSE,
        CALIBRATING_BLACK,
        CALIBRATING_WHITE,
        TERMINATED
    };
    State mState = UNDEFINED;

    const spikeapi::ColorSensor& mColorSensor;
    const spikeapi::ForceSensor& mForceSensor;
    int mBlack = 0;
    int mWhite = 100;
    bool mIsInitialized = false;
};

#endif  // ETTR_APP_CALIBRATOR_H_
