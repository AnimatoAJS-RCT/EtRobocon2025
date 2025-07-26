/******************************************************************************
 *  LineTracerWithStarter.h  (for SPIKE)
 *  Created on: 2025/01/05
 *  Implementation of the Class LineTracerWithStarter
 *  Author: Kazuhiro.Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_APP_LINETRACERWITHSTARTER_H_
#define ETTR_APP_LINETRACERWITHSTARTER_H_

#include "Starter.h"
#include "LineTracer.h"

class LineTracerWithStarter {
public:
    LineTracerWithStarter(LineTracer* lineTracer,
               const Starter* starter);

    void run();

private:
    enum State { // <1>
        UNDEFINED,
        WAITING_FOR_START,
        WALKING
    };

    LineTracer* mLineTracer;
    const Starter* mStarter;
    State mState; // <2>

    void execUndefined(); // <3>
    void execWaitingForStart();
    void execWalking();
};

#endif  // ETTR_APP_LINETRACERWITHSTARTER_H_
