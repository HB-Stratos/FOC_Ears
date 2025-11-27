#pragma once
#include <SimpleFOC.h>
#include <encoders/mt6701/MagneticSensorMT6701SSI.h>
#include "MathHelpers.h" // Include MathHelpers for remap_clamped

// Debug flag
#define DEBUG 1

#if DEBUG
#define DEBUG_PRINT(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#endif

class EarController
{
private:
    SPISettings myMT6701SPISettings;
    MagneticSensorMT6701SSI sensor;
    BLDCDriver3PWM driver;
    BLDCMotor motor;
    LowPassFilter lpf;

    float minAngle; // Minimum angle for motion
    float maxAngle; // Maximum angle for motion

    void configureDriver();
    void configurePID();

public:
    EarController(int senCs, int pwm1, int pwm2, int pwm3, int drvEnable, float minAngle, float maxAngle);
    ~EarController();

    void init(float zero_electric_offset = NOT_SET, Direction sensor_direction = Direction::CW);
    void moveTo(float targetAngle);
    void move01(float input);
    void update();
    void setHomePos(float homeAngle);
    float getAngle();

    void setMinAngle(float angle);
    void setMaxAngle(float angle);
};
