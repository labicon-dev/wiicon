/**
 * @file        madgwick.cpp
 * @brief       Implementation of the Madgwick AHRS filter for the Wiicon Remote project
 *
 * @details     Implementation of the Madgwick AHRS filter for the Wiicon Remote project
 *
 * @author      See AUTHORS file for full list of contributors
 * @date        2025
 * @version     1.0.0
 *
 * @see         madgwick.h for full license and attribution information
 *
 * ========================================================================================
 * DERIVED WORK - See madgwick.h for complete attribution chain
 * ========================================================================================
 *
 * Copyright (c) 2011 Sebastian Madgwick
 * Copyright (c) 2025 Wiicon Remote Contributors (this implementation)
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "madgwick.h"

volatile float beta       = 0.1f;
float          sampleFreq = 100.0f;
float          q0         = 1.0f;
float          q1         = 0.0f;
float          q2         = 0.0f;
float          q3         = 0.0f;

void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az) {
    float SEq_1 = q0;
    float SEq_2 = q1;
    float SEq_3 = q2;
    float SEq_4 = q3;

    float deltat = 1.0f / sampleFreq;

    float w_x = gx * (PI / 180.0f);
    float w_y = gy * (PI / 180.0f);
    float w_z = gz * (PI / 180.0f);

    float a_x = ax;
    float a_y = ay;
    float a_z = az;

    /*
    * START OF APPENDIX A IMPLEMENTATION
    *
    * The following code is the implementation of the Madgwick AHRS filter as described in the paper.
    * It is adapted directly from "Appendix A: IMU filter implementation optimised in C"
    * of Sebastian Madgwick's internal report.
    *
    * The code is implemented as described in the paper, with the following changes:
    * - The code is implemented in C++
    */

    float norm;                                                            /**< Vector norm */
    float SEqDot_omega_1, SEqDot_omega_2, SEqDot_omega_3, SEqDot_omega_4;  /**< Quaternion derivative from gyroscopes */
    float f_1, f_2, f_3;                                                   /**< Objective function elements */
    float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33;              /**< Objective function Jacobian elements */
    float SEqHatDot_1, SEqHatDot_2, SEqHatDot_3, SEqHatDot_4;              /**< Estimated direction of the gyroscope error */

    // Auxiliary variables to avoid repeated calculations
    float halfSEq_1 = 0.5f * SEq_1;
    float halfSEq_2 = 0.5f * SEq_2;
    float halfSEq_3 = 0.5f * SEq_3;
    float halfSEq_4 = 0.5f * SEq_4;
    float twoSEq_1  = 2.0f * SEq_1;
    float twoSEq_2  = 2.0f * SEq_2;
    float twoSEq_3  = 2.0f * SEq_3;

    norm = sqrt(a_x * a_x + a_y * a_y + a_z * a_z);
    if (norm == 0.0f) return;
    a_x /= norm;
    a_y /= norm;
    a_z /= norm;

    // Compute the objective function and Jacobian [Equation 25 and 26]
    f_1 = twoSEq_2 * SEq_4 - twoSEq_1 * SEq_3 - a_x;
    f_2 = twoSEq_1 * SEq_2 + twoSEq_3 * SEq_4 - a_y;
    f_3 = 1.0f - twoSEq_2 * SEq_2 - twoSEq_3 * SEq_3 - a_z;

    J_11or24 = twoSEq_3;
    J_12or23 = 2.0f * SEq_4;
    J_13or22 = twoSEq_1;
    J_14or21 = twoSEq_2;
    J_32     = 2.0f * J_14or21;
    J_33     = 2.0f * J_11or24;

    // Compute the gradient (matrix multiplication) [Equation 42 and 43]
    SEqHatDot_1 = J_14or21 * f_2 - J_11or24 * f_1;
    SEqHatDot_2 = J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3;
    SEqHatDot_3 = J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1;
    SEqHatDot_4 = J_14or21 * f_1 + J_11or24 * f_2;

    // Normalise the gradient
    norm = sqrt(SEqHatDot_1 * SEqHatDot_1 + SEqHatDot_2 * SEqHatDot_2 + SEqHatDot_3 * SEqHatDot_3 +
                SEqHatDot_4 * SEqHatDot_4);
    SEqHatDot_1 /= norm;
    SEqHatDot_2 /= norm;
    SEqHatDot_3 /= norm;
    SEqHatDot_4 /= norm;

    // Compute the quaternion derivative measured by gyroscopes [Equation 12]
    SEqDot_omega_1 = -halfSEq_2 * w_x - halfSEq_3 * w_y - halfSEq_4 * w_z;
    SEqDot_omega_2 = halfSEq_1 * w_x + halfSEq_3 * w_z - halfSEq_4 * w_y;
    SEqDot_omega_3 = halfSEq_1 * w_y - halfSEq_2 * w_z + halfSEq_4 * w_x;
    SEqDot_omega_4 = halfSEq_1 * w_z + halfSEq_2 * w_y - halfSEq_3 * w_x;

    // Compute then integrate the estimated quaternion derivative [Equation 42 and 43]
    SEq_1 += (SEqDot_omega_1 - (beta * SEqHatDot_1)) * deltat;
    SEq_2 += (SEqDot_omega_2 - (beta * SEqHatDot_2)) * deltat;
    SEq_3 += (SEqDot_omega_3 - (beta * SEqHatDot_3)) * deltat;
    SEq_4 += (SEqDot_omega_4 - (beta * SEqHatDot_4)) * deltat;

    // Normalise quaternion
    norm = sqrt(SEq_1 * SEq_1 + SEq_2 * SEq_2 + SEq_3 * SEq_3 + SEq_4 * SEq_4);
    q0   = SEq_1 / norm;
    q1   = SEq_2 / norm;
    q2   = SEq_3 / norm;
    q3   = SEq_4 / norm;
}

void getEulerAngles(float *roll, float *pitch, float *yaw) {
    *roll  = atan2f(2.0f * (q0 * q1 + q2 * q3), 1.0f - 2.0f * (q1 * q1 + q2 * q2));
    *pitch = asinf(2.0f * (q0 * q2 - q3 * q1));
    *yaw   = atan2f(2.0f * (q0 * q3 + q1 * q2), 1.0f - 2.0f * (q2 * q2 + q3 * q3));

    *roll *= 180.0f / PI;
    *pitch *= 180.0f / PI;
    *yaw *= 180.0f / PI;
}

void resetQuaternion() {
    q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
}
