/*
  Nano33BLEAccelerometer.h
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

#ifndef NANO33BLEACCELEROMETER_H_
#define NANO33BLEACCELEROMETER_H_

#include "NanoBLESensor.h"
/* For LSM9DS1 9-axis IMU sensor */
#include <Arduino_LSM9DS1.h>

/* IMU sensor gets acceleration data at a rate of 109Hz.
 * 8ms is 125Hz so this should be enough */
#define ACCELERATION_MONITOR_PERIOD_MS				(8U)
#define ACCELERATION_MONITOR_THREAD_STACK_SIZE		(512U) /* in bytes */
#define ACCELERATION_MONITOR_THREAD_PRIORITY		(osPriorityNormal) /* must be osPriority_t enum type */
#define ACCELERATION_MONITOR_THREAD_NAME			"Acceleration Monitor"
#define ACCELERATION_READ_THREAD_STACK_SIZE			(256U) /* in bytes */
#define ACCELERATION_READ_THREAD_PRIORITY			(osPriorityHigh) /* must be osPriority_t enum type */
#define ACCELERATION_READ_THREAD_NAME				"Acceleration reader"


struct Nano33BLEAccelerometer_t
{
	float x;
	float y;
	float z;
	uint32_t timeStampMs;
};

extern rtos::EventFlags sensorAvailableflags;

/* variables to hold LSM9DS1 accelerometer data */
class Nano33BLEAccelerometer: public NanoBLE33Sensor<struct Nano33BLEAccelerometer_t>
{
	public:
		static Nano33BLEAccelerometer* begin();

	private:
		/* Here will be the instance stored. */
		static Nano33BLEAccelerometer* instance;
    	Nano33BLEAccelerometer();

	    static void monitorFunction(void);
	    static void readFunction(Nano33BLEAccelerometer* instance);
};

/* Null, because instance will be initialized on demand. */
Nano33BLEAccelerometer* Nano33BLEAccelerometer::instance = NULL;

Nano33BLEAccelerometer* Nano33BLEAccelerometer::begin()
{
    if (instance == NULL)
    {
        instance = new Nano33BLEAccelerometer();
    }

    return instance;
}

Nano33BLEAccelerometer::Nano33BLEAccelerometer()
{
	if(false == ImuInitialisedFlag_g)
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

		ImuInitialisedFlag_g = true;
	}
	else
	{
		/* Already initialised */
	}

	rtos::Thread monitorThread = rtos::Thread(
			ACCELERATION_MONITOR_THREAD_PRIORITY,
			ACCELERATION_MONITOR_THREAD_STACK_SIZE,
			nullptr,
			ACCELERATION_MONITOR_THREAD_NAME);

	rtos::Thread readThread = rtos::Thread(
			ACCELERATION_READ_THREAD_PRIORITY,
			ACCELERATION_READ_THREAD_STACK_SIZE,
			nullptr,
			ACCELERATION_READ_THREAD_NAME);

	monitorThread.start(mbed::callback(Nano33BLEAccelerometer::monitorFunction));
	readThread.start(mbed::callback(Nano33BLEAccelerometer::readFunction, this));
}

void Nano33BLEAccelerometer::monitorFunction()
{
	while(1)
	{
		if(IMU.accelerationAvailable())
		{
			sensorAvailableflags.set(ACCELERATION_DATA_READY_FLAG);
		}

		rtos::ThisThread::sleep_for(ACCELERATION_MONITOR_PERIOD_MS);
	}
}

void Nano33BLEAccelerometer::readFunction(Nano33BLEAccelerometer* instance)
{
	Nano33BLEAccelerometer_t* data;

	while(1)
	{
		sensorAvailableflags.wait_all(ACCELERATION_DATA_READY_FLAG);
		data = instance->buffer + instance->bufferPosition;
		data->timeStampMs = millis();
	    IMU.readAcceleration(data->x, data->y, data->z);
	    instance->bufferPosition++;
	    if(SENSOR_BUFFER_SIZE < instance->bufferPosition)
	    {
	    	instance->handleOverflow();
	    }
	}
}


#endif /* NANO33BLEACCELEROMETER_H_ */
