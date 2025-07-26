/******************************************************************************
 *  LineTracer.cpp (for SPIKE)
 *  Created on: 2025/01/05
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineTracer.h"

// 定数宣言
const float LineTracer::Kp = 0.83;
const int LineTracer::bias = 0;

/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param walker 走行
 */
LineTracer::LineTracer(const LineMonitor *lineMonitor,
                       Walker *walker,
                       bool isLeftEdge,
                       PidGain *pidGain)
    : mLineMonitor(lineMonitor),
      mWalker(walker),
      mIsLeftEdge(isLeftEdge),
      mPidGain(pidGain),
      mIsInitialized(false),
      mPid(pidGain, 2)
{
}

/**
 * ライントレースする
 */
void LineTracer::run()
{
  if (mIsInitialized == false)
  {
    mWalker->init();
    mIsInitialized = true;
  }

  int diffReflection = mLineMonitor->calDiffReflection();

  // 走行体の操作量を計算する
  float turn = calcPropValue(diffReflection);
  mWalker->setTurn(turn);

  // 走行を行う
  mWalker->run();
}

/**
 * 走行体の操作量を計算する
 * @param diffBrightness ラインから外れた度合い（ライン閾値との差）
 */
float LineTracer::calcPropValue(int diffBrightness)
{
  // float turn = LineTracer::Kp * diffBrightness + LineTracer::bias;
  double d = mPid.calculatePid(diffBrightness);
  float turn = d;
  return turn;
}
