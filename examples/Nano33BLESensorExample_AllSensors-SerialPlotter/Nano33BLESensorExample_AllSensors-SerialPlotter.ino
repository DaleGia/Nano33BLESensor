/*
  Nano33BLESensorExample_AllSensors-SerialPlotter.ino
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This program is an example program showing some of the cababilities of the 
  Nano33BLESensor Library. In this case it outputs sensor data and from all 
  of the Arduino Nano 33 BLE Sense's on board sensors via serial in a format 
  that can be displayed on the Arduino IDE serial plotter.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*****************************************************************************/
/*INCLUDES                                                                   */
/*****************************************************************************/
#include "Arduino.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLEMagnetic.h"
#include "Nano33BLEProximity.h"
#include "Nano33BLEColour.h"
#include "Nano33BLEGesture.h"
#include "Nano33BLEPressure.h"
#include "Nano33BLETemperature.h"
#include "Nano33BLEMicrophoneRMS.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/
/* 
 * Objects which we will store data in each time we read
 * the each sensor. 
 */ 
Nano33BLEMagneticData magneticData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLEAccelerometerData accelerometerData;
Nano33BLEProximityData proximityData;
Nano33BLEColourData colourData;
Nano33BLEGestureData gestureData;
Nano33BLEPressureData pressureData;
Nano33BLETemperatureData temperatureData;
Nano33BLEMicrophoneRMSData MicrophoneRMSData;

char buffer[300];  
/*****************************************************************************/
/*SETUP (Initialisation)                                                          */
/*****************************************************************************/
void setup()
{
    /* Serial setup for UART debugging */
    Serial.begin(115200);
    /* 
     * Initialises the all the sensor, and starts the periodic reading 
     * of the sensor using a Mbed OS thread. The data is placed in a 
     * circular buffer and can be read whenever.
     */
    Magnetic.begin();
    Gyroscope.begin();
    Accelerometer.begin();
    Proximity.begin();
    Colour.begin();
    Gesture.begin();
    Pressure.begin();
    Temperature.begin();
    MicrophoneRMS.begin();

    delay(500);
}

/*****************************************************************************/
/*LOOP (runtime super loop)                                                          */
/*****************************************************************************/
void loop()
{
    /*
     * This gets all the data from each sensor. Note that each sensor gets data
     * at different frequencies. Seeing as this super loop runs every 50mS, not
     * all the sensors will have new data. If a sensor does not have new data, 
     * the old data will just be printed out again. This is a little sloppy, but
     * allows the data to be printed in a coherrent way inside serial plotter.
     */
    Magnetic.pop(magneticData);
    Gyroscope.pop(gyroscopeData);
    Accelerometer.pop(accelerometerData);
    Proximity.pop(proximityData);
    Colour.pop(colourData);
    Gesture.pop(gestureData);
    Pressure.pop(pressureData);
    Temperature.pop(temperatureData);
    MicrophoneRMS.pop(MicrophoneRMSData);


    snprintf(
      buffer, 
      sizeof(buffer),
      "%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d", 
      magneticData.x, 
      magneticData.y, 
      magneticData.z,
      gyroscopeData.x, 
      gyroscopeData.y, 
      gyroscopeData.z,
      accelerometerData.x, 
      accelerometerData.y, 
      accelerometerData.z,
      proximityData.proximity,
      colourData.r, 
      colourData.g, 
      colourData.b, 
      colourData.c,
      gestureData.gesture,
      pressureData.barometricPressure,
      temperatureData.temperatureCelsius, 
      temperatureData.humidity,
      MicrophoneRMSData.RMSValue);

    Serial.println(buffer);

    delay(50);
}