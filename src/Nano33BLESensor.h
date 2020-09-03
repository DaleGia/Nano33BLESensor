/*
  NanoBLESensor.h
  Copyright (c) 2020 Dale Giancono. All rights reserved..

`    *** WRITE SOMETHING HERE ***

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
#define DEFAULT_THREAD_STACK_SIZE            (1024U) /* in bytes */

/******************/
/*GLOBAL TYPES*/
/******************/

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
        Nano33BLESensor(
            osPriority threadPriority = osPriorityNormal,
            uint32_t threadSize = DEFAULT_THREAD_STACK_SIZE) : readThread(
            threadPriority,
            threadSize){};
    
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

#endif /* NANOBLESENSOR_H_ */