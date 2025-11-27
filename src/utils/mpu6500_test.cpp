/**
 * MPU6500 IMU Test Utility
 * 
 * Tests the MPU6500 6-axis IMU (gyro + accel) with sensor fusion.
 * Provides absolute roll and pitch angles using complementary filter.
 */

#include <Arduino.h>
#include <Wire.h>
#include "FastIMU.h"

#define I2C_SDA 5
#define I2C_SCL 4
#define IMU_ADDRESS 0x68

MPU6500 imu;

// Calibration data
calData calib = { 0 };
AccelData accelData;
GyroData gyroData;

// Sensor fusion variables
float roll = 0.0;
float pitch = 0.0;
unsigned long lastTime = 0;

// Complementary filter coefficient (0.98 = 98% gyro, 2% accel)
const float alpha = 0.98;

void setup() {
  Serial.begin(115200);
  Serial.println("\nMPU6500 IMU Test with Sensor Fusion");
  
  Wire.begin(I2C_SDA, I2C_SCL);
  delay(100);
  
  int err = imu.init(calib, IMU_ADDRESS);
  if (err != 0) {
    Serial.print("Failed to initialize MPU6500! Error code: ");
    Serial.println(err);
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("MPU6500 initialized successfully!");
  
  // Calibrate gyro
  Serial.println("Calibrating gyro - keep sensor still...");
  delay(1000);
  imu.calibrateAccelGyro(&calib);
  Serial.println("Calibration complete!");
  
  // Initialize time
  lastTime = millis();
  
  Serial.println("\nFormat: Roll, Pitch (degrees)");
}

void loop() {
  imu.update();
  imu.getAccel(&accelData);
  imu.getGyro(&gyroData);
  
  // Calculate time delta
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // Convert to seconds
  lastTime = currentTime;
  
  // Calculate angles from accelerometer (in degrees)
  float accelRoll = atan2(accelData.accelY, accelData.accelZ) * 180.0 / PI;
  float accelPitch = atan2(-accelData.accelX, sqrt(accelData.accelY * accelData.accelY + accelData.accelZ * accelData.accelZ)) * 180.0 / PI;
  
  // Integrate gyroscope data (gyro is in degrees/sec)
  roll += gyroData.gyroX * dt;
  pitch += gyroData.gyroY * dt;
  
  // Apply complementary filter
  roll = alpha * roll + (1.0 - alpha) * accelRoll;
  pitch = alpha * pitch + (1.0 - alpha) * accelPitch;
  
  // Print results
  Serial.print("Roll: ");
  Serial.print(roll, 2);
  Serial.print("°\tPitch: ");
  Serial.print(pitch, 2);
  Serial.println("°");
  
  delay(10);
}
