import re
import numpy as np
import matplotlib.pyplot as plt

# Function to extract the first uncommented calibration LUT from the file
def extract_calibration_lut(file_path):
    with open(file_path, 'r') as file:
        content = file.read()
        # Regular expression to find the first uncommented calibration LUT
        match = re.search(r"^\s*float\s+calibrationLut\[\d+\]\s*=\s*\{([^}]*)\};", content, re.MULTILINE)
        if match:
            # Extract the LUT values and convert them to a list of floats
            lut_values = match.group(1)
            return [float(value.strip()) for value in lut_values.split(',')]
        else:
            raise ValueError("No uncommented calibration LUT found in the file.")

# Path to the file containing the calibration LUT
file_path = "src/utils/openloop_test.cpp"

# Extract the calibration LUT
calibration_lut = extract_calibration_lut(file_path)

# Generate x values from 0 to 2PI
x_values = np.linspace(0, 2 * np.pi, len(calibration_lut))

# Plot the data
plt.figure(figsize=(10, 6))
plt.plot(x_values, calibration_lut, label="Calibration LUT")
plt.title("Calibration Lookup Table")
plt.xlabel("X (0 to 2PI)")
plt.ylabel("Y (Calibration Values)")
plt.grid(True)
plt.legend()
plt.show()