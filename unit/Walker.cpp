/******************************************************************************
 *  Walker.cpp (for SPIKE)
 *  Created on: 2025/01/05
 *  Implementation of the Class Walker
 *  Author: Kazuhiro.Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Walker.h"
#include <stdio.h>

/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
Walker::Walker(spikeapi::Motor &leftWheel,
               spikeapi::Motor &rightWheel)
    : mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mTurn(0)
{
}

/**
 * 走行する
 */
void Walker::run()
{
    // 左右モータに回転を指示する
    // printf("pwm:%d, mTurn:%d\n", pwm, mTurn);
    mLeftWheel.setPower(mPwm - mTurn);
    mRightWheel.setPower(mPwm + mTurn);
}

/**
 * 走行に必要なものをリセットする
 */
void Walker::init()
{
    // モータをリセットする
    mLeftWheel.resetCount();
    mRightWheel.resetCount();
}

/**
 * PWMへの加算値を設定する
 * @param forward 前進値
 * @param turn    旋回方向
 */
void Walker::setTurn(float turn)
{
    mTurn = turn;
}

/**
 * PWMを設定する
 * @param pwd PWM値 -100~100
 */
void Walker::setPwm(int pwm)
{
    mPwm = pwm;
}
