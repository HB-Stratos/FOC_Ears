#pragma once
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class IMUHandler
{
private:
    Adafruit_BNO055 bno;               // Owned instance of the BNO055 sensor
    imu::Vector<3> velocity;           // Velocity vector (X, Y, Z)
    imu::Vector<3> linearAcceleration; // Linear acceleration vector (X, Y, Z)
    imu::Vector<3> angularVelocity;    // Angular velocity vector (X, Y, Z)
    imu::Vector<3> absoluteRotation;   // Absolute rotation vector (Pitch, Roll, Yaw)
    unsigned long prevTime;            // Previous timestamp for integration
    float biasStrength;                // Strength of bias to reduce drift

public:
    IMUHandler(TwoWire *wireInstance, float biasStrength = 0.01);
    bool init();                            // Initialize the sensor
    void update();                          // Update all sensor values
    imu::Vector<3> getVelocity();           // Get stored velocity
    imu::Vector<3> getLinearAcceleration(); // Get stored linear acceleration
    imu::Vector<3> getAngularVelocity();    // Get stored angular velocity
    imu::Vector<3> getAbsoluteRotation();   // Get stored absolute rotation
    void setBiasStrength(float strength);   // Set bias strength
};