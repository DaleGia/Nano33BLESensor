# Nano33BLESensor
See [The Arduino Nano 33 BLE Sense Sensor Library You Have Been Waiting For](https://dalegi.com/2020/09/04/the-arduino-nano-33-ble-sense-sensor-library-you-have-been-waiting-for/).

An Arduino library for the [Nano 33 BLE Sense](https://store.arduino.cc/usa/nano-33-ble-sense) that leverages [Mbed OS](https://os.mbed.com/mbed-os/) to automatically place sensor measurements in a ring buffer that can be integrated into programs in a simple manner.

Nano33BLESensor was created to help make easy to understand examples for the Arduino Nano 33 BLE Sense that utilise Mbed OS. For an example that does not use Mbed OS, please see [nano-33-sense-serial-example](https://github.com/DaleGia/nano-33-sense-serial-example)

## Features
- Class implementation with common interface for the following sensor measurements
  - 3-axis Accelerometer
  - 3-axis Gyroscope
  - 3-axis Magnetic
  - RMS Microphone
  - Barometric Pressure
  - Temperature (with humidity)
  - Proximity
  - RGBC Colour
  - Gesture
- Mbed OS usage, allowing easy integration with programs.
- Ring buffer usage, allowing the softening of time constraints in regard to the reading sensor measurements.
- Excellent examples for all sensors designed for BLE and Serial Plotter that help you to get started.

## Why Would I Want This?
- Super simple initialisation of on board sensors.
- No code required beyond initialisation for collection of sensor data.
- Super simple usage of sensor data.
- Common interface among different sensors.
- Using Mbed OS effectively makes the reading of sensor measurements happen "in the background", and keeps it out of the main program loop.
  
## Installation
Download this file as a zip, and extract the resulting folder into your Arduino Libraries folder. See [Installing Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries). Alternatively, use the Arduino IDE and library manager to find and install Nano33BLESensor.
 
## Note on Usage
Nano33BLESensor can be used with both the [ArduinoCore-nRF528x-mbedos](https://github.com/arduino/ArduinoCore-nRF528x-mbedos) and [ArduinoCore-mbed](https://github.com/arduino/ArduinoCore-mbed) cores, however the [Nano33BLESensorExample_microphoneRMS.ino](examples/Nano33BLESensorExample_microphoneRMS/Nano33BLESensorExample_microphoneRMS.ino) example only currently compiles when using the ArduinoCore-nRF528x-mbedos core.

By default the ring buffer is only size of 20. No indication of buffer overflow is available, so it is up to the user to ensure the buffer is being read reguarly enough. Each sensor is read at differing intervals that are dependant on the sensors capabilities.

## Examples
- Initialisation and starting of all sensors
```c++
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLEMagnetic.h"
#include "Nano33BLEProximity.h"
#include "Nano33BLEColour.h"
#include "Nano33BLEGesture.h"
#include "Nano33BLEPressure.h"
#include "Nano33BLETemperature.h"
#include "Nano33BLEMicrophoneRMS.h"

void setup()
{
  Magnetic.begin();
  Gyroscope.begin();
  Accelerometer.begin();
  Proximity.begin();
  Colour.begin();
  Gesture.begin();
  Pressure.begin();
  Temperature.begin();
  MicrophoneRMS.begin();
}
```
- Get last sensor values taken from Accelerometer and print them
```c++
Nano33BLEAccelerometerData accelerometerData;

if(Accelerometer.pop(accelerometerData))
{
  //Sensor value read from buffer
}
else
{
  //Sensor value not ready (try again later)
}
```
- Get all available sensor values taken from Colour sensor.
```c++
uint32_t availableColourData;
availableColourData = Colour.availableDataSize();
if(availableColourData > 0)
{
  Nano33BLEColourData colourData[availableColourData];
  Colour.popMultiple(colourData, availableColourData);
}
```

pop() and popMultiple() can be used in a similar manner for all other sensors.


## Further Examples  
[3-axis Accelerometer with BLE and serial output](examples/Nano33BLESensorExample_accelerometer/Nano33BLESensorExample_accelerometer.ino)

[3-axis Gyroscope with BLE and serial output](examples/Nano33BLESensorExample_gyroscope/Nano33BLESensorExample_gyroscope.ino)

[3-axis Magnetic with BLE and serial output](examples/Nano33BLESensorExample_magnetic/Nano33BLESensorExample_magnetic.ino)

[IMU sensors with BLE and serial output](examples/Nano33BLESensorExample_IMU/Nano33BLESensorExample_IMU.ino)

[RMS Microphone output with BLE and serial output](examples/Nano33BLESensorExample_microphoneRMS/Nano33BLESensorExample_microphoneRMS.ino)

[Barometric pressure with BLE and serial output](examples/Nano33BLESensorExample_pressure/Nano33BLESensorExample_pressure.ino)

[Temperature and humidity with BLE and serial output](examples/Nano33BLESensorExample_temperature/Nano33BLESensorExample_temperature.ino)

[Proximity with BLE and serial output](examples/Nano33BLESensorExample_proximity/Nano33BLESensorExample_proximity.ino)

[RGBC Colour with BLE and serial output](examples/Nano33BLESensorExample_colour/Nano33BLESensorExample_colour.ino)

[Gesture with BLE and serial output](examples/Nano33BLESensorExample_gesture/Nano33BLESensorExample_gesture.ino)

[All sensors with serial output](examples/Nano33BLESensorExample_AllSensors-SerialPlotter/Nano33BLESensorExample_AllSensors-SerialPlotter.ino)


