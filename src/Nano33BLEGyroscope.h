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

#ifndef NANO33BLEGYROSCOPE_H_
#define NANO33BLEGYROSCOPE_H_

#include "NanoBLESensor.h"
/* For LSM9DS1 9-axis IMU sensor */
#include <Arduino_LSM9DS1.h>

/* IMU sensor gets acceleration data at a rate of 109Hz.
 * 8ms is 125Hz so this should be enough */
#define GYROSCOPE_MONITOR_PERIOD_MS				(8U)
#define GYROSCOPE_MONITOR_THREAD_STACK_SIZE		(512U) /* in bytes */
#define GYROSCOPE_MONITOR_THREAD_PRIORITY		(osPriorityNormal) /* must be osPriority_t enum type */
#define GYROSCOPE_MONITOR_THREAD_NAME			"Gyroscope Monitor"
#define GYROSCOPE_READ_THREAD_STACK_SIZE		(256U) /* in bytes */
#define GYROSCOPE_READ_THREAD_PRIORITY			(osPriorityHigh) /* must be osPriority_t enum type */
#define GYROSCOPE_READ_THREAD_NAME				"Gyroscope reader"

struct NanoBLE33Gyroscope_t
{
	float x;
	float y;
	float z;
	uint32_t timeStampMs;
};

extern rtos::EventFlags sensorAvailableflags;

/* variables to hold LSM9DS1 accelerometer data */
class NanoBLE33Gyroscope: public NanoBLE33Sensor<struct NanoBLE33Gyroscope_t>
{
	public:
    	static NanoBLE33Gyroscope* begin();

	private:
		/* Here will be the instance stored. */
		static NanoBLE33Gyroscope* instance;
		NanoBLE33Gyroscope();


	    static void monitorFunction(void);
	    static void readFunction(NanoBLE33Gyroscope* instance);
};

/* Null, because instance will be initialized on demand. */
NanoBLE33Gyroscope* NanoBLE33Gyroscope::instance = NULL;

NanoBLE33Gyroscope* NanoBLE33Gyroscope::begin()
{
    if (instance == NULL)
    {
        instance = new NanoBLE33Gyroscope();
    }

    return instance;
}

NanoBLE33Gyroscope::NanoBLE33Gyroscope()
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
		/* Already initialised and in use.*/
	}

    rtos::Thread monitorThread = rtos::Thread(
			GYROSCOPE_MONITOR_THREAD_PRIORITY,
			GYROSCOPE_MONITOR_THREAD_STACK_SIZE,
			nullptr,
			GYROSCOPE_MONITOR_THREAD_NAME);

	rtos::Thread readThread = rtos::Thread(
			GYROSCOPE_READ_THREAD_PRIORITY,
			GYROSCOPE_READ_THREAD_STACK_SIZE,
			nullptr,
			GYROSCOPE_READ_THREAD_NAME);

	monitorThread.start(mbed::callback(NanoBLE33Gyroscope::monitorFunction));
	readThread.start(mbed::callback(NanoBLE33Gyroscope::readFunction, this));
}

void NanoBLE33Gyroscope::monitorFunction()
{
	while(1)
	{
		if(IMU.gyroscopeAvailable())
		{
			sensorAvailableflags.set(GYROSCOPE_DATA_READY_FLAG);
		}

		rtos::ThisThread::sleep_for(GYROSCOPE_MONITOR_PERIOD_MS);
	}
}

void NanoBLE33Gyroscope::readFunction(NanoBLE33Gyroscope* instance)
{
	NanoBLE33Gyroscope_t* data;

	while(1)
	{
		sensorAvailableflags.wait_all(ACCELERATION_DATA_READY_FLAG);
		data = instance->buffer + instance->bufferPosition;
		data->timeStampMs = millis();
	    IMU.readGyroscope(data->x, data->y, data->z);
	    instance->bufferPosition++;
    	Serial.print("Buffer Position: ");
    	Serial.println(instance->bufferPosition);

	    if(SENSOR_BUFFER_SIZE < instance->bufferPosition)
	    {
	    	Serial.println("Handling overflow for NanoBLE33Gyroscope");
	    	instance->handleOverflow();
	    }
	}
}


#endif /* NANO33BLEGYROSCOPE_H_ */
