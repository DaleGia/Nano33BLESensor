/*
  Nano33BLESensorExample_IMU.ino
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This program is an example program showing some of the cababilities of the 
  Nano33BLESensor Library. In this case it outputs IMU microphone data from 
  one of the Arduino Nano 33 BLE Sense's on board sensors via serial in a 
  format that can be displayed on the Arduino IDE serial plotter. It also 
  outputs the data via BLE in a string format that can be viewed using a 
  variety of BLE scanning software.

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
/* For the bluetooth funcionality */
#include <ArduinoBLE.h>
/* For all of the sensors */
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLEMagnetic.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/* 
 * We use strings to transmit the data via BLE, and this defines the buffer
 * size used to transmit these strings. Only 20 bytes of data can be 
 * transmitted in one packet with BLE, so a size of 20 is chosen the the data 
 * can be displayed nicely in whatever application we are using to monitor the
 * data.
 */
#define BLE_BUFFER_SIZES             20
/* Device name which can be scene in BLE scanning software. */
#define BLE_DEVICE_NAME                "Arduino Nano 33 BLE Sense"
/* Local name which should pop up when scanning for BLE devices. */
#define BLE_LOCAL_NAME                "Sensors"

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

/* 
 * Declares the BLEService and characteristics we will need for the BLE 
 * transfer. The UUID was randomly generated using one of the many online 
 * tools that exist. It was chosen to use BLECharacteristic instead of 
 * BLEFloatCharacteristic (and other characteristic types) as it is hard 
 * to view non-string data in most BLE scanning software. Strings can be 
 * viewed easiler enough. In an actual application you might want to 
 * transfer specific data types directly.
 */
BLEService BLESensors("590d65c7-3a0a-4023-a05a-6aaf2f22441c");
BLECharacteristic magneticBLEX("0001", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic magneticBLEY("0002", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic magneticBLEZ("0003", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);

BLECharacteristic gyroscopeBLEX("0004", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic gyroscopeBLEY("0005", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic gyroscopeBLEZ("0006", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);

BLECharacteristic accelerometerBLEX("0007", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic accelerometerBLEY("0008", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic accelerometerBLEZ("0009", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);

/* Common global buffer will be used to write to the BLE characteristics. */
char bleBuffer[BLE_BUFFER_SIZES];
/*****************************************************************************/
/*SETUP (Initialisation)                                                     */
/*****************************************************************************/
void setup()
{
    /* 
     * Serial setup. This will be used to transmit data for viewing on serial 
     * plotter 
     */
    Serial.begin(115200);
    while(!Serial);


    /* BLE Setup. For information, search for the many ArduinoBLE examples.*/
    if (!BLE.begin()) 
    {
        while (1);    
    }
    else
    {
        BLE.setDeviceName(BLE_DEVICE_NAME);
        BLE.setLocalName(BLE_LOCAL_NAME);
        BLE.setAdvertisedService(BLESensors);
        /* A seperate characteristic is used for each sensor data type. */
        BLESensors.addCharacteristic(magneticBLEX);
        BLESensors.addCharacteristic(magneticBLEY);
        BLESensors.addCharacteristic(magneticBLEZ);
        BLESensors.addCharacteristic(gyroscopeBLEX);
        BLESensors.addCharacteristic(gyroscopeBLEY);
        BLESensors.addCharacteristic(gyroscopeBLEZ);
        BLESensors.addCharacteristic(accelerometerBLEX);
        BLESensors.addCharacteristic(accelerometerBLEY);
        BLESensors.addCharacteristic(accelerometerBLEZ);

        BLE.addService(BLESensors);
        BLE.advertise();
        /* 
         * Initialises the all the sensor, and starts the periodic reading 
         * of the sensor using a Mbed OS thread. The data is placed in a 
         * circular buffer and can be read whenever.
         */
        Magnetic.begin();
        Gyroscope.begin();
        Accelerometer.begin();

        /* Plots the legend on Serial Plotter */
        Serial.println("MagX, MagY, MagZ, GyX, GyY, GyZ, AccX, AccY, AccZ");
    }
}

/*****************************************************************************/
/*LOOP (runtime super loop)                                                  */
/*****************************************************************************/
void loop()
{
    BLEDevice central = BLE.central();
    if(central)
    {
        int writeLength;
        bool dataGotFlag = false;
        /* 
         * If a BLE device is connected, accelerometer data will start being read, 
         * and the data will be written to each BLE characteristic. The same 
         * data will also be output through serial so it can be plotted using 
         * Serial Plotter. 
         */
        while(central.connected())
        {    
            /* 
             * sprintf is used to convert the read float value to a string 
             * which is stored in bleBuffer. This string is then written to 
             * the BLE characteristic. 
             */        
            if(Magnetic.pop(magneticData))
            {
                writeLength = sprintf(bleBuffer, "%f", magneticData.x);
                magneticBLEX.writeValue(bleBuffer, writeLength); 
                writeLength = sprintf(bleBuffer, "%f", magneticData.y);
                magneticBLEY.writeValue(bleBuffer, writeLength);                
                writeLength = sprintf(bleBuffer, "%f", magneticData.z);
                magneticBLEZ.writeValue(bleBuffer, writeLength);                
                dataGotFlag = true;
            }

            if(Gyroscope.pop(gyroscopeData))
            {
                writeLength = sprintf(bleBuffer, "%f", gyroscopeData.x);
                gyroscopeBLEX.writeValue(bleBuffer, writeLength);   
                writeLength = sprintf(bleBuffer, "%f", gyroscopeData.y);
                gyroscopeBLEY.writeValue(bleBuffer, writeLength);   
                writeLength = sprintf(bleBuffer, "%f", gyroscopeData.z);
                gyroscopeBLEZ.writeValue(bleBuffer, writeLength);   
                dataGotFlag = true;
            }

            if(Accelerometer.pop(accelerometerData))
            {
                writeLength = sprintf(bleBuffer, "%f", accelerometerData.x);
                accelerometerBLEX.writeValue(bleBuffer, writeLength);   
                writeLength = sprintf(bleBuffer, "%f", accelerometerData.y);
                accelerometerBLEY.writeValue(bleBuffer, writeLength);   
                writeLength = sprintf(bleBuffer, "%f", accelerometerData.z);
                accelerometerBLEZ.writeValue(bleBuffer, writeLength);   
                dataGotFlag = true;
            }

            if(dataGotFlag)
            {
                Serial.printf(
                    "%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", 
                    magneticData.x, 
                    magneticData.y, 
                    magneticData.z, 
                    gyroscopeData.x, 
                    gyroscopeData.y, 
                    gyroscopeData.z,
                    accelerometerData.x, 
                    accelerometerData.y, 
                    accelerometerData.z);
                dataGotFlag = false;
            }
        }
    }
}
