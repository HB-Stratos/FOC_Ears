#include "EarController.h"

// Constructor
EarController::EarController(int senCs, int pwm1, int pwm2, int pwm3, int drvEnable, float minAngle, float maxAngle)
    : myMT6701SPISettings(4000000, MT6701_BITORDER, SPI_MODE2),
      sensor(senCs, myMT6701SPISettings),
      driver(pwm1, pwm2, pwm3, drvEnable),
      motor(7, 11.2, 80, 0.003),
      lpf(0.02),
      minAngle(minAngle), // Initialize minAngle
      maxAngle(maxAngle)  // Initialize maxAngle
{
    DEBUG_PRINT("EarController initialized.");
}

// Destructor
EarController::~EarController()
{
    DEBUG_PRINT("EarController destroyed.");
}

// Public function: Initialize the controller
void EarController::init(float zero_electric_offset, Direction sensor_direction)
{
    DEBUG_PRINT("Initializing EarController...");

    motor.zero_electric_angle = zero_electric_offset;
    motor.sensor_direction = sensor_direction;

    // MOSI is not used at all, but has to be specified
    SPI.begin(32, 35, 33); // TODO clean up SPI bus
    configureDriver();
    configurePID();

    motor.linkSensor(&sensor);
    motor.linkDriver(&driver);

    sensor.init();
    driver.init();
    motor.init();

    if (motor.initFOC())
        Serial.println("FOC init success!");
    else
    {
        Serial.println("FOC init failed!");
        return;
    }

    DEBUG_PRINT("Sensor electric offset: ");
    DEBUG_PRINT(motor.zero_electric_angle);
    DEBUG_PRINT("Sensor Direction: ");
    DEBUG_PRINT(motor.sensor_direction);

    DEBUG_PRINT("Initialization complete.");
}

// Public function: Move to a specific angle
void EarController::moveTo(float targetAngle)
{
    motor.move(lpf(targetAngle));
}

// Public function: Move between minAngle and maxAngle based on input [0, 1]
void EarController::move01(float input)
{
    float targetAngle = utils::remap_clamped(input, 0.0f, 1.0f, minAngle, maxAngle);
    motor.move(targetAngle);
}

// Public function: Update the motor state
void EarController::update()
{
    motor.loopFOC();
}

void EarController::setHomePos(float homeAngle)
{
    motor.sensor_offset = homeAngle;
}

float EarController::getAngle()
{
    sensor.update();
    return sensor.getAngle();
}

// Setters for minAngle and maxAngle
void EarController::setMinAngle(float angle)
{
    minAngle = angle;
}

void EarController::setMaxAngle(float angle)
{
    maxAngle = angle;
}

// Private function: Configure the driver
void EarController::configureDriver()
{
    DEBUG_PRINT("Configuring driver...");

    // pwm frequency to be used [Hz]
    driver.pwm_frequency = 24000;
    // power supply voltage [V]
    driver.voltage_power_supply = 15;
    // Max DC voltage allowed - default voltage_power_supply
    driver.voltage_limit = 15;
}

// Private function: Configure PID settings
void EarController::configurePID()
{
    DEBUG_PRINT("Configuring PID...");

    // set control loop type to be used
    motor.controller = MotionControlType::angle;

    // velocity PID controller parameters
    // default P=0.5 I = 10 D =0
    motor.PID_velocity.P = 0.02;
    motor.PID_velocity.I = 2;
    motor.PID_velocity.D = 0.00;
    // jerk control using voltage voltage ramp
    // default value is 300 volts per sec  ~ 0.3V per millisecond
    motor.PID_velocity.output_ramp = 0;

    // velocity low pass filtering
    // default 5ms - try different values to see what is the best. m,
    // the lower the less filtered
    motor.LPF_velocity.Tf = 1 / (6.28 * 200);

    // setting the limits
    // either voltage
    // motor.voltage_limit = 10; // Volts - default driver.voltage_limit
    // // of current
    // motor.current_limit = 2; // Amps - default 0.2Amps

    // angle PID controller
    // default P=20
    motor.P_angle.P = 20;
    motor.P_angle.I = 0; // usually only P controller is enough
    motor.P_angle.D = 0; // usually only P controller is enough
    // acceleration control using output ramp
    // this variable is in rad/s^2 and sets the limit of acceleration
    motor.P_angle.output_ramp = 0; // default 1e6 rad/s^2

    // angle low pass filtering
    // default 0 - disabled
    // use only for very noisy position sensors - try to avoid and keep the values very small
    motor.LPF_angle.Tf = 0; // default 0

    // setting the limits
    //  maximal velocity of the position control
    motor.velocity_limit = 10000; // rad/s - default 20

    // motor.PID_velocity.limit = 1;
    // motor.P_angle.limit = 200;
}
