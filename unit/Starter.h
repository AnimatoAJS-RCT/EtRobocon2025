/******************************************************************************
 *  Starter.h (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class Starter
 *  Author: Kazuhiro Kawachi
 *  Modifier : Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_UNIT_STARTER_H_
#define ETTR_UNIT_STARTER_H_

#include "ForceSensor.h"

class Starter {
public:
    explicit Starter(const spikeapi::ForceSensor& forceSensor);

    bool isPushed();

private:
    const spikeapi::ForceSensor& mForceSensor;
    bool mIsStarted;
};

#endif  // ETTR_UNIT_STARTER_H_
