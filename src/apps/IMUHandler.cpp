#include "IMUHandler.h"

IMUHandler::IMUHandler(TwoWire *wireInstance, float biasStrength)
    : bno(Adafruit_BNO055(55, 0x29, wireInstance)), // Initialize BNO055 with I2C address and wire instance
      velocity(0, 0, 0),
      linearAcceleration(0, 0, 0),
      angularVelocity(0, 0, 0),
      absoluteRotation(0, 0, 0),
      prevTime(0),
      biasStrength(biasStrength)
{
    // Constructor does not initialize the sensor
}

bool IMUHandler::init()
{
    // Initialize the sensor
    if (!bno.begin())
    {
        Serial.println("BNO055 Init failed! Check wiring or I2C address.");
        return false; // Return failure
    }

    // configure BNO055 to be vertically mounted (rotated 90 deg around Y)
    bno.setAxisRemap(Adafruit_BNO055::REMAP_CONFIG_P0);
    bno.setAxisSign(Adafruit_BNO055::REMAP_SIGN_P0);

    // Enable external crystal for better accuracy
    bno.setExtCrystalUse(true);

    return true; // Return success
}

void IMUHandler::update()
{
    // Get current time
    unsigned long curTime = millis();
    float deltaTime = (curTime - prevTime) / 1000.0; // Convert to seconds
    prevTime = curTime;

    // Get acceleration data and integrate to calculate velocity
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    velocity.x() += accel.x() * deltaTime;
    velocity.y() += accel.y() * deltaTime;
    velocity.z() += accel.z() * deltaTime;

    // Apply bias to reduce drift
    velocity.x() *= (1.0 - biasStrength * deltaTime);
    velocity.y() *= (1.0 - biasStrength * deltaTime);
    velocity.z() *= (1.0 - biasStrength * deltaTime);

    // Store linear acceleration
    linearAcceleration = accel;

    // Get angular velocity
    angularVelocity = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

    // Get quaternion data
    imu::Quaternion quat = bno.getQuat();

    // Convert quaternion to Euler angles
    absoluteRotation = quat.toEuler();
}

imu::Vector<3> IMUHandler::getVelocity()
{
    return velocity;
}

imu::Vector<3> IMUHandler::getLinearAcceleration()
{
    return linearAcceleration;
}

imu::Vector<3> IMUHandler::getAngularVelocity()
{
    return angularVelocity;
}

imu::Vector<3> IMUHandler::getAbsoluteRotation()
{
    return absoluteRotation;
}

void IMUHandler::setBiasStrength(float strength)
{
    biasStrength = strength;
}