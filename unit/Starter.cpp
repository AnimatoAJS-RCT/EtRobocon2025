/******************************************************************************
 *  Starter.cpp (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class Starter
 *  Author: Kazuhiro Kawachi
 *  Modifier : Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Starter.h"

#include <stdio.h>

/**
 * コンストラクタ
 * @param forceSensor フォースセンサ
 */
Starter::Starter(const spikeapi::ForceSensor& forceSensor) : mForceSensor(forceSensor) {}

/**
 * 押下中か否か
 * @retval true  押下している
 * @retval false 押下していない
 */
bool Starter::isPushed() const
{
    bool pushed = mForceSensor.isTouched();
    //printf("Starter::isPushed(): %s\n", pushed ? "true" : "false");
    return pushed;  // <1>
    // return true;
}
