/*
  Nano33BLEMagnetic.h
  Copyright (c) 2020 Dale Giancono. All rights reserved..

`	*** WRITE SOMETHING HERE ***

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

#ifndef NANO33BLEMAGNETIC_H_
#define NANO33BLEMAGNETIC_H_

#include "NanoBLESensor.h"
/* For LSM9DS1 9-axis IMU sensor */
#include <Arduino_LSM9DS1.h>
#include "critical_section_api.h"

/* IMU sensor gets acceleration data at a rate of 109Hz.
 * 8ms is 125Hz so this should be enough */
#define MAGNETIC_MONITOR_PERIOD_MS				(8U)
#define MAGNETIC_MONITOR_THREAD_STACK_SIZE		(1024U) /* in bytes */
#define MAGNETIC_MONITOR_THREAD_PRIORITY		(osPriorityHigh) /* must be osPriority_t enum type */
#define MAGNETIC_MONITOR_THREAD_NAME			"Magnetic Monitor"
#define MAGNETIC_READ_THREAD_STACK_SIZE			(1024U) /* in bytes */
#define MAGNETIC_READ_THREAD_PRIORITY			(osPriorityHigh) /* must be osPriority_t enum type */
#define MAGNETIC_READ_THREAD_NAME				"Magnetic reader"

struct Nano33BLEMagnetic_t
{
	float x;
	float y;
	float z;
	uint32_t timeStampMs;
};


/* variables to hold LSM9DS1 accelerometer data */
class Nano33BLEMagnetic: public NanoBLE33Sensor<struct Nano33BLEMagnetic_t>
{
	public:
		static void begin()
		{
			getInstance();
		}

	private:
		static Nano33BLEMagnetic* getInstance()
		{
			static Nano33BLEMagnetic instance;
			return &instance;
		}

		/* Here will be the instance stored. */
		Nano33BLEMagnetic();

	    static void monitorFunction(Nano33BLEMagnetic* instance);
	    static void readFunction(Nano33BLEMagnetic* instance);
};

Nano33BLEMagnetic::Nano33BLEMagnetic() : NanoBLE33Sensor{
											MAGNETIC_MONITOR_THREAD_PRIORITY,
											MAGNETIC_MONITOR_THREAD_STACK_SIZE,
											MAGNETIC_MONITOR_THREAD_NAME,
											MAGNETIC_READ_THREAD_PRIORITY,
											MAGNETIC_READ_THREAD_STACK_SIZE,
											MAGNETIC_READ_THREAD_NAME}
{
	if(false == IMUInitialisedFlag)
	{
		/* IMU setup for LSM9DS1*/
		/* default setup has all sensors active in continous mode. Sample rates
		 *  are as follows: magneticFieldSampleRate = 20Hz, gyroscopeYroscopeSampleRate = 109Hz,
		 * accelerationSampleRate = 109Hz */

		if (!IMU.begin())
		{
			/* Hacky way of stopping task executation in event of failure. */
			while(1);
		}

		IMUInitialisedFlag = true;
	}
	else
	{
		/* Already initialised */
	}
	this->monitorThread.start(mbed::callback(Nano33BLEMagnetic::monitorFunction, this));
	this->readThread.start(mbed::callback(Nano33BLEMagnetic::readFunction, this));
}

void Nano33BLEMagnetic::monitorFunction(Nano33BLEMagnetic* instance)
{
	while(1)
	{
    	if(IMU.accelerationAvailable())
		{
	    	instance->dataEventFlags.set(dataEventFlags_e::dataAvailableFlag);
		}
    	else
    	{
    	}

		rtos::ThisThread::sleep_for(MAGNETIC_MONITOR_PERIOD_MS);
	}
}

void Nano33BLEMagnetic::readFunction(Nano33BLEMagnetic* instance)
{
	Nano33BLEMagnetic_t data;

	while(1)
	{
    	instance->dataEventFlags.wait_all(dataEventFlags_e::dataAvailableFlag);

    	data.timeStampMs = millis();
    	IMU.readMagneticField(data.x, data.y, data.z);
    	instance->buffer.push(data);
	}
}


#endif /* NANO33BLEACCELEROMETER_H_ */
