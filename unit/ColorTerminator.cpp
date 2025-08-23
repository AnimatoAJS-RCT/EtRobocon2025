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
#include "Util.h"

#include <stdio.h>

/**
 * コンストラクタ
 * @param colorSensor ColorSensor
 * @param termColor 停止する色
 */
ColorTerminator::ColorTerminator(const spikeapi::ColorSensor* colorSensor, eColor termColor)
  : mColorSensor(colorSensor), mTermColor(termColor)
{
}

bool ColorTerminator::isToBeTerminate()
{
    spikeapi::ColorSensor::HSV hsv;
    mColorSensor->getHSV(hsv);
    eColor c = getColor(hsv.h, hsv.s, hsv.v);
    printf("ColorTerminator::isToBeTerminate(): h=%u\ts=%u\tv=%u\tcolor=%s\n", hsv.h, hsv.s, hsv.v, colorToString(c));
    return  c == mTermColor;
}
