/**
 * @file        wiicon.ino
 * @brief       Main file for the Wiicon Remote project
 * 
 * @details     Includes functions for reading sensor data, applying Madgwick filter
 *              and sending Euler angles via Serial in CSV format: R,P,Y
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

#include <Wire.h>
#include "config.h"
#include "bmi160.h"
#include "madgwick.h" 

int accelMap[3] = {0, 1, 2};
int accelSign[3] = {1, 1, 1};
int gyroMap[3] = {0, 1, 2};
int gyroSign[3] = {1, 1, 1};

unsigned long lastTime = 0;

/**
 * Reads sensor data, applies Madgwick filter and sends Euler angles via Serial
 * Output format: roll,pitch,yaw (in degrees)
 */
void sendEuler()
{
    int16_t ax_raw, ay_raw, az_raw;
    int16_t gx_raw, gy_raw, gz_raw;
    
    // Read accelerometer data
    if (!readAccelRaw(&ax_raw, &ay_raw, &az_raw))
    {
        Serial.println("Failed to read ACC data");
        return;
    }
    
    // Read gyroscope data
    if (!readGyroRaw(&gx_raw, &gy_raw, &gz_raw))
    {
        Serial.println("Failed to read GYR data");
        return;
    }

    // Debug: print raw values if all are zero
    if (ax_raw == 0 && ay_raw == 0 && az_raw == 0 && gx_raw == 0 && gy_raw == 0 && gz_raw == 0)
    {
        Serial.println("Raw sensor values are all zero — check wiring, address, or that sensor is powered.");
    }

    // Apply axis remapping and sign inversion before converting to physical units
    int rawA[3] = {ax_raw, ay_raw, az_raw};
    int rawG[3] = {gx_raw, gy_raw, gz_raw};
    float a_mapped[3];
    float g_mapped[3];
    float bias_mapped[3];
    
    for (int i = 0; i < 3; ++i)
    {
        a_mapped[i] = (float)rawA[accelMap[i]] * (float)accelSign[i];
        // Convert accel LSB -> g
        a_mapped[i] = a_mapped[i] / ACC_LSB_PER_G;
        g_mapped[i] = (float)rawG[gyroMap[i]] * (float)gyroSign[i];
        // Bias in deg/s for mapped axis: get raw bias from source axis and apply sign
        bias_mapped[i] = gyroBiasRaw[gyroMap[i]] * (float)gyroSign[i];
        // Convert gyro LSB -> deg/s and remove bias
        g_mapped[i] = g_mapped[i] / GYR_LSB_PER_DPS - bias_mapped[i];
    }

    // Feed the Madgwick with mapped axes (order: X=0, Y=1, Z=2)
    MadgwickAHRSupdate(g_mapped[0], g_mapped[1], g_mapped[2], a_mapped[0], a_mapped[1], a_mapped[2]);

    // Convert quaternion to Euler angles (degrees)
    float roll, pitch, yaw;
    getEulerAngles(&roll, &pitch, &yaw);

    // Optionally swap roll and yaw before sending
#if SWAP_ROLL_YAW
    float outRoll = yaw;
    float outYaw = roll;
#else
    float outRoll = roll;
    float outYaw = yaw;
#endif

    // Send CSV: roll,pitch,yaw
    Serial.print(outRoll, 2);
    Serial.print(',');
    Serial.print(pitch, 2);
    Serial.print(',');
    Serial.println(outYaw, 2);
}

void setup()
{
    Serial.begin(SERIAL_BAUD);
    delay(100);

#ifndef DISABLE_BMI160_SENSOR
    Wire.begin(SDA_PIN, SCL_PIN);

    Serial.println("Starting BMI160 raw reader (Wire-only). Initializing sensor...");
    
    // Warn if using GPIO1 which is UART TX0 and may conflict with Serial
    if (SCL_PIN == 1 || SDA_PIN == 1)
    {
        Serial.println("Warning: using GPIO1 for I2C may conflict with Serial TX (GPIO1). Consider using other pins like 21 (SDA) and 22 (SCL).");
    }

    // Executa scanner I2C para ajudar a debugar fiação/endereço
    i2cScanner();

    // Inicializa o BMI160
    if (!initBMI160())
    {
        Serial.println("Warning: failed to init BMI160 (I2C read/write). Verifique conexões e endereço I2C.");
    }
    else
    {
        Serial.println("BMI160 init attempted (check chip id above).\n");
        // Auto-calibra acelerômetro após inicialização bem-sucedida
        autoCalibrateAccelerometer();
    }

    // Calibração automática do giroscópio: coleta N amostras com o dispositivo imóvel
    Serial.println("Iniciando calibração automática do giroscópio. Mantenha o dispositivo imóvel...");
    
    if (!calibrateGyro(CALIB_SAMPLES, CALIB_DELAY_MS))
    {
        Serial.println("Calibração do giroscópio falhou (leituras I2C). Continuing without bias correction.");
    }
    else
    {
        Serial.print("Gyro raw biases (deg/s): ");
        Serial.print(gyroBiasRaw[0], 4);
        Serial.print(',');
        Serial.print(gyroBiasRaw[1], 4);
        Serial.print(',');
        Serial.println(gyroBiasRaw[2], 4);
        
        // Também imprime biases mapeados de acordo com gyroMap/gyroSign atual
        float mappedBias[3];
        for (int i = 0; i < 3; ++i)
            mappedBias[i] = gyroBiasRaw[gyroMap[i]] * (float)gyroSign[i];
        Serial.print("Gyro mapped biases (deg/s): ");
        Serial.print(mappedBias[0], 4);
        Serial.print(',');
        Serial.print(mappedBias[1], 4);
        Serial.print(',');
        Serial.println(mappedBias[2], 4);
    }
#endif

    lastTime = micros();
}

void loop()
{
#ifndef DISABLE_BMI160_SENSOR
    unsigned long now = micros();
    float dt = (now - lastTime) / 1000000.0f;
    if (dt <= 0)
        return;
    
    float measuredHz = 1.0f / dt;
    sampleFreq = 0.95f * sampleFreq + 0.05f * measuredHz;
    lastTime = now;

        sendEuler();

    delay(10);
#endif
}
