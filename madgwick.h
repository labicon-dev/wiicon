/**
 * @file        madgwick.h
 * @brief       Madgwick filter definitions for the Wiicon Remote project
 * 
 * @author      See AUTHORS file for full list of contributors
 * @date        2025
 * @version     1.0.0
 * 
 * ========================================================================================
 * 
 * MIT License
 * Copyright (c) 2025 Wiicon Remote Contributors
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * ========================================================================================
 */

#ifndef MADGWICK_H
#define MADGWICK_H

#include <Arduino.h>

// Beta: Filter gain. Controls the "confidence" in the gyroscope vs accelerometer.
// - Low (ex: 0.03): Very smooth, corrects drift slowly (good for stability).
// - High (ex: 0.5): Fast response, but lets through accelerometer noise.
extern volatile float beta;

// Estimated sampling frequency (Hz). The loop calculates dynamically,
// but we start with 100Hz as an initial guess.
extern float sampleFreq;

// Quaternion components
extern float q0, q1, q2, q3;

/**
 * Update the quaternion using gyroscope and accelerometer readings
 * @param gx Angular velocity X (deg/s)
 * @param gy Angular velocity Y (deg/s)
 * @param gz Angular velocity Z (deg/s)
 * @param ax Acceleration X (g)
 * @param ay Acceleration Y (g)
 * @param az Acceleration Z (g)
 */
void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az);

/**
 * Convert the current quaternion to Euler angles
 * @param roll Pointer to store the roll angle (degrees)
 * @param pitch Pointer to store the pitch angle (degrees)
 * @param yaw Pointer to store the yaw angle (degrees)
 */
void getEulerAngles(float *roll, float *pitch, float *yaw);

/**
 * Reset the quaternion to the identity orientation (no rotation)
 */
void resetQuaternion();

#endif  // MADGWICK_H
