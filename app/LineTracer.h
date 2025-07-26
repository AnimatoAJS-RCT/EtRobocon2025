/******************************************************************************
 *  LineTracer.h (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_APP_LINETRACER_H_
#define ETTR_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "Walker.h"
#include "Pid.h"
#include "Tracer.h"

class LineTracer : public Tracer
{
public:
    static const float Kp;
    static const int bias;

    /**
     * コンストラクタ
     * @param lineMonitor LineMonitor
     * @param walker Walker
     * @param _isLeftEdge エッジの左右どちらを走るか(true:左エッジ, false:右エッジ)
     * @param _gain PIDゲイン
     * @param _stopColor 停止条件の色 //TODO:未実装
     */
    LineTracer(const LineMonitor *lineMonitor,
               Walker *walker,
               bool _isLeftEdge,
               PidGain *_gain);

    void run();

private:
    const LineMonitor *mLineMonitor;
    Walker *mWalker;
    int mPwm;
    int mMaxPwm;
    bool mIsLeftEdge;
    PidGain *mPidGain;
    bool mIsInitialized;
    Pid mPid;

    float calcPropValue(int diffReflection);
};

#endif // ETTR_APP_LINETRACER_H_
