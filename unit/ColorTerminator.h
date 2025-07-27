/******************************************************************************
 *  ColorTerminator.h (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class ColorTerminator
 *  Author: Kazuhiro Kawachi
 *  Modifier : Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_UNIT_ColorTerminator_H_
#define ETTR_UNIT_ColorTerminator_H_

#include "ColorSensor.h"
#include "Terminator.h"

class ColorTerminator : public Terminator {
   public:
    /**
     * コンストラクタ
     * @param colorSensor ColorSensor
     * @param termColor 停止する色のH成分
     */
    ColorTerminator(const spikeapi::ColorSensor* colorSensor, const uint16_t termColorH);

    bool isToBeTerminate();

   private:
    const spikeapi::ColorSensor* mColorSensor;
    const uint16_t mTermColorH;
};

#endif  // ETTR_UNIT_ColorTerminator_H_
