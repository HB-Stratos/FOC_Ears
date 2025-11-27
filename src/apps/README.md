# Apps Directory

This directory contains the main application implementations for the FOC Ears project.

## Files

### Main Application
- **foc_ears_main.cpp** - Full FOC ears implementation with IMU control

### Helper Classes (used by main application)
- **EarController.cpp/.h** - Motor control and ear movement logic
- **IMUHandler.cpp.disabled/.h.disabled** - IMU sensor handling (currently disabled)
- **MathHelpers.h** - Math utility functions

Each .cpp file contains its own `setup()` and `loop()` functions.
