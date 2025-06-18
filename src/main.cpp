#include "Arduino.h"
#include "SPI.h"
#include "SimpleFOCDrivers.h"
#include "encoders/MT6701/MagneticSensorMT6701SSI.h"
#include "EarController.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <IMUHandler.h>
#include <MathHelpers.h>

#define PIN_BUTTON 13

#define I2C_SDA 27
#define I2C_SCL 14

TwoWire I2CBNO = TwoWire(0);
IMUHandler imuHandler(&I2CBNO, 0.5);

EarController earL = EarController(33, 19, 18, 05, 17, 0.1, -1.9);
EarController earR = EarController(25, 16, 04, 00, 02, 0.1, -1.9);

void setup()
{
    Serial.begin(115200);

    I2CBNO.begin(I2C_SDA, I2C_SCL, 100000);
    /* Initialise the sensor */
    if (!imuHandler.init())
    {
        Serial.println("IMU initialization failed!");
        while (1)
            ; // Halt execution if initialization fails
    }

    Serial.println("IMU initialized successfully!");

    /* Initialize Push Button */
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    /* Initialize Ear Controllers */
    earL.setHomePos(0.5);
    earR.setHomePos(-2.9);

    earL.init(2.14, Direction(-1));
    earR.init(1.98, Direction(-1));

    // earL.init();
    // earR.init();
}

unsigned long lastImuUpdate = 0;
unsigned long lastPrintTime = 0;

void loop()
{
    float targetAngle = 0.1 * PI * _sin(float(millis()) / 500 * PI);
    float targetR = 0;
    float targetL = 0;
    if (digitalRead(PIN_BUTTON) == LOW)
    {
        // targetAngle -= 1.8;
        // targetAngle = 0;

        // targetAngle = imuHandler.getVelocity().x() * 10;

        // float rollAngle = utils::mapf(imuHandler.getAbsoluteRotation().z(), 0, -PI, -1, 1);
        // float pitchAngle = utils::mapf(imuHandler.getAbsoluteRotation().y(), PI / 2, -PI / 2, 1, -1);

        targetL =
            +utils::remap_clamped(imuHandler.getAbsoluteRotation().z(), 0, -PI / 2, 0.9, 0.1) + utils::remap_clamped(imuHandler.getAbsoluteRotation().y(), PI / 2, 0, 0.9, 0.1);

        targetR =
            +utils::remap_clamped(imuHandler.getAbsoluteRotation().z(), -PI, -PI / 2, 0.9, 0.1) + utils::remap_clamped(imuHandler.getAbsoluteRotation().y(), PI / 2, 0, 0.9, 0.1);
    }

    // earL.moveTo(targetAngle);
    // earR.moveTo(targetAngle);

    earL.move01(targetL);
    earR.move01(1 - targetR);

    earL.update();
    earR.update();

    if (millis() - lastImuUpdate >= 10)
    {
        imuHandler.update();
        lastImuUpdate = millis();
    }

    if (millis() - lastPrintTime >= 100)
    {
        // Serial.print("L: ");
        // Serial.print(earL.getAngle());
        // Serial.print(" R: ");
        // Serial.print(earR.getAngle());
        Serial.print(" Target: ");
        Serial.print(targetAngle);
        // Serial.println("");

        lastPrintTime = millis();

        imu::Vector<3> var = imuHandler.getAbsoluteRotation();
        Serial.print("X: ");
        Serial.print(var.x());
        Serial.print(" Y: ");
        Serial.print(var.y());
        Serial.print(" Z: ");
        Serial.print(var.z());
        Serial.println("");
    }

    /* Absolute Rotations:
    X: Pitch, zero @ 180, pitch up is decrease
    Y: Roll, zero @ -90, roll right is increase
    Z: Yaw, zero irrelevant
    */

    /*Angular Velocities:
    X: Yaw
    Y: Roll, roll right is increase
    Z: Pitch, pitch up is increase
    */

    /*Linear Accelerations:
    X: Up, upwards is positive
    Y: Forwards, forward is negative
    Z: Rightwards, right is negative
    */

    // _delay(100);
}
