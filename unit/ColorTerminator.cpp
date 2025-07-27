/******************************************************************************
 *  ColorTerminator.cpp (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class ColorTerminator
 *  Author: Kazuhiro Kawachi
 *  Modifier : Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "ColorTerminator.h"
#include "ColorSensor.h"

#include <stdio.h>

/**
 * コンストラクタ
 * @param colorSensor ColorSensor
 * @param termColor 停止する色のH成分
 */
ColorTerminator::ColorTerminator(const spikeapi::ColorSensor* colorSensor, uint16_t termColorH)
  : mColorSensor(colorSensor), mTermColorH(termColorH)
{
}

bool ColorTerminator::isToBeTerminate()
{
    spikeapi::ColorSensor::HSV hsv;
    mColorSensor->getHSV(hsv);
    printf("ColorTerminator::isToBeTerminate(): %u\n", hsv.h);
    return hsv.h == mTermColorH;
}
