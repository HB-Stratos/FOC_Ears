Warning AI Written

# FOC Ears - Build System Guide

## Project Structure

This project uses a multi-environment setup to easily switch between the main application and various utility tools.

```
src/
├── main.cpp                    # Dispatcher (routes to correct implementation)
├── apps/                       # Main application implementations
│   └── foc_ears_main.cpp      # Full FOC ears with IMU control
└── utils/                      # Utility and debugging tools
    ├── i2c_scanner.cpp        # I2C bus scanner
    ├── motor_test.cpp         # Motor testing utility
    └── sensor_test.cpp        # Sensor testing utility
```

## How to Switch Modes

### Using VS Code PlatformIO Extension

1. Look at the bottom status bar of VS Code
2. Click on the environment name (e.g., "Default (foc_ears)")
3. Select the environment you want from the dropdown:
   - **foc_ears** - Main application
   - **i2c_scanner** - I2C debugging
   - **motor_test** - Motor calibration
   - **sensor_test** - Sensor debugging
4. Build and upload as normal

### Using Command Line

```bash
# Build a specific environment
pio run -e foc_ears

# Upload to a specific environment
pio run -e i2c_scanner -t upload

# Upload and monitor
pio run -e motor_test -t upload -t monitor
```

## Available Environments

### `foc_ears` - Main Application

The complete FOC ears implementation with:

- IMU integration (BNO055)
- Motor control (SimpleFOC)
- FastLED support
- Full ear movement control

**Libraries**: SimpleFOC, SimpleFOCDrivers, FastLED, Adafruit BNO055

### `i2c_scanner` - I2C Scanner

Simple utility to scan the I2C bus and find connected devices.

**Libraries**: None (Arduino core only)

### `motor_test` - Motor Testing

Test and calibrate motors without full system overhead.

**Libraries**: SimpleFOC, SimpleFOCDrivers

### `sensor_test` - Sensor Testing

Test encoder and sensor readings without full system overhead.

**Libraries**: SimpleFOC, SimpleFOCDrivers

## Adding a New Environment

1. **Create the implementation file** in `src/apps/` or `src/utils/`:

   ```cpp
   // src/utils/my_new_tool.cpp
   #include <Arduino.h>

   void setup() {
       Serial.begin(115200);
       Serial.println("My new tool starting...");
   }

   void loop() {
       // Your code here
   }
   ```

2. **Add to main.cpp dispatcher**:

   ```cpp
   #elif defined(USE_MY_NEW_TOOL)
       #include "utils/my_new_tool.cpp"
   ```

3. **Add environment to platformio.ini**:

   ```ini
   [env:my_new_tool]
   lib_deps =
       ; Add any required libraries
   build_flags =
       -D USE_MY_NEW_TOOL
   ```

4. **Select and build** the new environment!

## Configuration Details

### Base Configuration (`[env]`)

All environments inherit these settings:

- Platform: ESP32-S3
- Board: ESP32-S3-DevKitC-1-N16R8
- Framework: Arduino
- Monitor speed: 115200 baud
- Monitor filters: Exception decoder, printable

### Per-Environment Settings

Each environment can override or add:

- **lib_deps**: Project-specific libraries
- **build_flags**: Defines to select the correct main file
- **Any other PlatformIO settings**

## Benefits of This Approach

✅ **No file renaming** - Switch instantly via environment selection  
✅ **No config duplication** - Base config is inherited by all  
✅ **Minimal dependencies** - Each tool only loads what it needs  
✅ **Easy to add new tools** - Just create a file and add 3 lines  
✅ **Clean separation** - Apps vs utilities clearly organized  
✅ **Version control friendly** - All implementations stay in the repo

## Tips

- The dispatcher (`main.cpp`) should never be edited unless adding new environments
- Each `.cpp` file in `apps/` and `utils/` must contain both `setup()` and `loop()`
- Use the `#error` directive in the dispatcher to catch configuration mistakes early
- Keep utility tools simple and focused on one task
- Document what each environment does in platformio.ini comments
