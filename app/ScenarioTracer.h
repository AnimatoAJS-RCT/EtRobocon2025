#ifndef ETTR_APP_SCENARIOTRACER_H_
#define ETTR_APP_SCENARIOTRACER_H_

#include "Tracer.h"
#include "Walker.h"

class ScenarioTracer : public Tracer {
public:
    ScenarioTracer(Walker* walker, int leftPwm, int rightPwm);
    void run() override;

private:
    Walker* mWalker;
    int mLeftPwm;
    int mRightPwm;
    bool mIsInitialized;
    static const double Kp;  // ゲイン

    void execWalking();
};

#endif // ETTR_APP_SCENARIOTRACER_H_