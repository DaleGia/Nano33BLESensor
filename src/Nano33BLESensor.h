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
//#include <Arduino_APDS9960.h>
/* For LPS22HB barometric barometricPressure sensor */
//#include <Arduino_LPS22HB.h>
/* For HTS221 Temperature and humidity sensor */
//#include <Arduino_HTS221.h>

#include <CircularBuffer.h>
#include "MbedOSThreadSafe.h"

/********/
/*MACROS*/
/********/
#define THREAD_STACK_SIZE			(1024U) /* in bytes */

/******************/
/*GLOBAL TYPES*/
/******************/


// struct Nano33BLEPressure_t
// {
// 	float pressure;
// 	uint32_t timeStampMs;
// };

// struct Nano33BLETemperature_t
// {
// 	float temperature;
// 	uint32_t timeStampMs;
// };

// struct Nano33BLEHumidity_t
// {
// 	float humidity;
// 	uint32_t timeStampMs;
// };

// struct Nano33BLEProximity_t
// {
// 	int proximity;
// 	uint32_t timeStampMs;
// };

// struct Nano33BLEGesture_t
// {
// 	int gesture;
// 	uint32_t timeStampMs;
// };

// struct Nano33BLEColour_t
// {
// 	int red;
// 	int green;
// 	int blue;
// 	uint32_t timeStampMs;
// };

/******************/
/*LOCAL VARIABLES*/
/******************/

/******************/
/*GLOBAL VARIABLES*/
/******************/

/*********/
/*CLASSES*/
/*********/

template<class T>
class Nano33BLESensor
{
	public:			
		static T* begin()
		{
			T* instance;
			instance = getInstance();
			initFunction(instance);
			return instance;
		}



	protected:
		Nano33BLESensor() : readThread(
			osPriorityHigh,
			THREAD_STACK_SIZE){};
	
	private:
		static T* getInstance()
		{
		  static T instance;
		  return &instance;
		}

		static void initFunction(T *instance);
		static void readFunction(T *instance);

		rtos::Thread readThread;
};

template<class T> void Nano33BLESensor<T>::initFunction(T *instance)
{
	instance->init();
	instance->readThread.start(mbed::callback(Nano33BLESensor<T>::readFunction, instance));
}

template<class T> void Nano33BLESensor<T>::readFunction(T *instance)
{
	while(1)
	{
		instance->read();
	}
}



// /* variables to hold LPS22HB barometric pressure data */
// class Nano33BLEPressure: public Nano33BLESensor<struct Nano33BLEPressure_t>
// {
// 	public:
// 		Nano33BLEPressure();
// 	private:

// };

// /* variables to hold HTS221 temperature data*/
// class Nano33BLETemperature: public Nano33BLESensor<struct Nano33BLETemperature_t>
// {
// 	public:
// 		Nano33BLETemperature();
// 	private:
// };

// /* variables to hold HTS221 humidity data*/
// class Nano33BLEHumidity: public Nano33BLESensor<struct Nano33BLEHumidity_t>
// {
// 	public:
// 		Nano33BLEHumidity();
// 	private:
// };


// /* variables to hold APDS9960 proximity data*/
// class Nano33BLEProximity: public Nano33BLESensor<struct Nano33BLEProximity_t>
// {
// 	public:
// 		Nano33BLEProximity();
// 	private:
// };
// /* variables to hold APDS9960 gesture data */
// class Nano33BLEGesture: public Nano33BLESensor<struct Nano33BLEGesture_t>
// {
// 	public:
// 		Nano33BLEGesture();
// 	private:
// };

// /* variables to hold APDS9960 colour data */
// class Nano33BLEColour: public Nano33BLESensor<struct Nano33BLEColour_t>
// {
// 	public:
// 		Nano33BLEColour();
// 	private:
// };

#endif /* NANOBLESENSOR_H_ */