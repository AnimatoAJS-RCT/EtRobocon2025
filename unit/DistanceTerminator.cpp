/******************************************************************************
 *  DistanceTerminator.cpp (for SPIKE)
 *  Created on: 2025/08/23
 *  Definition of the Class DistanceTerminator
 *  Author: Your Name
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "DistanceTerminator.h"
#include <stdio.h>

const double DistanceTerminator::TIRE_DIAMETER = 81.0;
const double DistanceTerminator::PI = 3.1415926535;

/**
 * コンストラクタ
 * @param walker Walker
 * @param targetDistance 目標距離 (mm)
 */
DistanceTerminator::DistanceTerminator(Walker* walker, double targetDistance)
  : mWalker(walker), mTargetDistance(targetDistance), mInitialDistance(0.0)
{
}

void DistanceTerminator::init()
{
    mInitialDistance = calcCurrentDistance();
}

bool DistanceTerminator::isToBeTerminate()
{
    double currentDistance = calcCurrentDistance() - mInitialDistance;
    printf("DistanceTerminator::isToBeTerminate(): current: %f, target: %f\n", currentDistance, mTargetDistance);
    return currentDistance >= mTargetDistance;
}

double DistanceTerminator::calcCurrentDistance()
{
    double wheelCircumference = TIRE_DIAMETER * PI;
    double leftDistance = (double)mWalker->getLeftCount() * wheelCircumference / 360.0;
    double rightDistance = (double)mWalker->getRightCount() * wheelCircumference / 360.0;
    return (leftDistance + rightDistance) / 2.0;
}

