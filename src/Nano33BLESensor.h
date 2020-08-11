/*
  NanoBLESensor.h
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

/**********/
/*INCLUDES*/
/**********/

#ifndef NANOBLESENSOR_H_
#define NANOBLESENSOR_H_

#include <Arduino.h>

/* For APDS9960 Gesture, light, and proximity sensor */
#include <Arduino_APDS9960.h>
/* For LPS22HB barometric barometricPressure sensor */
#include <Arduino_LPS22HB.h>
/* For HTS221 Temperature and humidity sensor */
#include <Arduino_HTS221.h>

/********/
/*MACROS*/
/********/
#define SENSOR_BUFFER_SIZE	20

#define GYROSCOPE_DATA_MONITOR_PERIOD_MS		8

#define ACCELERATION_DATA_READY_FLAG	0x0001
#define GYROSCOPE_DATA_READY_FLAG		0x0002
#define MAGNETIC_DATA_READY_FLAG		0x0004
#define PRESSURE_DATA_READY_FLAG		0x0008
#define TEMPERATURE_DATA_READY_FLAG		0x0010
#define HUMIDITY_DATA_READY_FLAG		0x0020
#define PROXIMITY_DATA_READY_FLAG		0x0040
#define GESTURE_DATA_READY_FLAG			0x0080
#define COLOUR_DATA_READY_FLAG			0x0100

/******************/
/*GLOBAL TYPES*/
/******************/


struct NanoBLE33Pressure_t
{
	float pressure;
	uint32_t timeStampMs;
};

struct NanoBLE33Temperature_t
{
	float temperature;
	uint32_t timeStampMs;
};

struct NanoBLE33Humidity_t
{
	float humidity;
	uint32_t timeStampMs;
};

struct NanoBLE33Proximity_t
{
	int proximity;
	uint32_t timeStampMs;
};

struct NanoBLE33Gesture_t
{
	int gesture;
	uint32_t timeStampMs;
};

struct NanoBLE33Colour_t
{
	int red;
	int green;
	int blue;
	uint32_t timeStampMs;
};

/******************/
/*LOCAL VARIABLES*/
/******************/

/******************/
/*GLOBAL VARIABLES*/
/******************/
rtos::EventFlags sensorAvailableflags;

/*********/
/*CLASSES*/
/*********/
/* Flags to indicate whether the various sensors are being used */
static bool MP34DT05InitialisedFlag = false;
static bool IMUInitialisedFlag = false;
static bool APDS9960InitialisedFlag = false;
static bool LPS22HBInitialisedFlag = false;
static bool HTS221InitialisedFlag = false;

template<class T>
class NanoBLE33Sensor
{
	public:
		uint32_t getAvailableDataSize(void);
		bool getLast(T& data);
		uint32_t getData(T& buffer, uint32_t size);

	protected:
		NanoBLE33Sensor(
				osPriority_t monitorThreadPriority,
				uint32_t monitorThreadStackSize,
				const char* monitorThreadName,
				osPriority_t readThreadPriority,
				uint32_t readThreadStackSize,
				const char* readThreadName) :
					monitorThread(
						monitorThreadPriority,
						monitorThreadStackSize,
						nullptr,
						monitorThreadName),
					readThread(
						readThreadPriority,
						readThreadStackSize,
						nullptr,
						readThreadName),
					dataEventFlags(),
					dataLock_mutex()
		{
			Serial.println("NanoBLE33Sensor inistialised");
		};

		//T buffer[SENSOR_BUFFER_SIZE];
		mbed::CircularBuffer<T, SENSOR_BUFFER_SIZE> buffer;

		rtos::Thread monitorThread;
		rtos::Thread readThread;

		enum dataEventFlags_e
		{
			dataAvailableFlag = 0x000001,
		};

		rtos::EventFlags dataEventFlags;
		rtos::Mutex dataLock_mutex;

	private:




};

template<class T> uint32_t NanoBLE33Sensor<T>::getAvailableDataSize(void)
{
	return this->buffer.size();
}

template<class T> bool NanoBLE33Sensor<T>::getLast(T& buffer)
{
	return this->buffer.pop(buffer);
}

template<class T> uint32_t NanoBLE33Sensor<T>::getData(T& buffer, uint32_t size)
{
	uint32_t availableData;
	uint32_t readData;
	uint32_t ii;

	availableData = this->buffer.size();
	if(availableData < size)
	{
		readData = availableData;
	}
	else
	{
		readData = size;
	}

	for(ii = 0; ii < readData; ii++)
	{
		this->buffer.pop(buffer);
	}

	return readData;
}




/* variables to hold LPS22HB barometric pressure data */
class NanoBLE33Pressure: public NanoBLE33Sensor<struct NanoBLE33Pressure_t>
{
	public:
		NanoBLE33Pressure();
	private:

};

/* variables to hold HTS221 temperature data*/
class NanoBLE33Temperature: public NanoBLE33Sensor<struct NanoBLE33Temperature_t>
{
	public:
		NanoBLE33Temperature();
	private:
};

/* variables to hold HTS221 humidity data*/
class NanoBLE33Humidity: public NanoBLE33Sensor<struct NanoBLE33Humidity_t>
{
	public:
		NanoBLE33Humidity();
	private:
};


/* variables to hold APDS9960 proximity data*/
class NanoBLE33Proximity: public NanoBLE33Sensor<struct NanoBLE33Proximity_t>
{
	public:
		NanoBLE33Proximity();
	private:
};
/* variables to hold APDS9960 gesture data */
class NanoBLE33Gesture: public NanoBLE33Sensor<struct NanoBLE33Gesture_t>
{
	public:
		NanoBLE33Gesture();
	private:
};

/* variables to hold APDS9960 colour data */
class NanoBLE33Colour: public NanoBLE33Sensor<struct NanoBLE33Colour_t>
{
	public:
		NanoBLE33Colour();
	private:
};

#endif /* NANOBLESENSOR_H_ */
