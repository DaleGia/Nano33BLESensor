/*
  Nano33BLEGesture.h
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This class reads gesture data from the on board Nano 33 BLE
  Sense gesture sensor using Mbed OS. It stores the results in a ring 
  buffer (within the Nano33BLESensorBuffer Class) which can be accessed
  in a manner with softer time constraints than other implementations.

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
/*INLCUDE GUARD                                                              */
/*****************************************************************************/
/* Update these names to match the name of the file */ 
#ifndef NANO33BLEGESTURE_H_
#define NANO33BLEGESTURE_H_

/*****************************************************************************/
/*INLCUDES                                                                   */
/*****************************************************************************/
#include "Nano33BLESensorBuffer.h"
#include <Arduino_APDS9960.h>

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/* 
 * As per Arduino_APDS9960.h, 0=100%, 1=150%, 2=200%, 3=300%. Obviously more
 * boost results in more power consumption. 
 */
#define IR_LED_BOOST_VALUE      (0U)
/* 
 * Set sensitivity from 0 to 100. Higher is more sensitive. In
 * my experience it requires quite a bit of experimentation to
 * get this right, as if it is too sensitive gestures will always
 * register as GESTURE_DOWN or GESTURE_UP and never GESTURE_LEFT or
 * GESTURE_RIGHT. This can be called before APDS.begin() as it just
 * sets an internal sensitivity value.
 */
#define IR_GESTURE_SENSITIVITY  (50U)
/**
 * This macro is required. It defines the wait period between sensor reads.
 * Update to the value you need based on how fast the sensor can read data.  
 */
#define DEFAULT_GESTURE_READ_PERIOD_MS                (10U)
#define DEFAULT_GESTURE_THREAD_STACK_SIZE_BYTES       (1024U) 

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*CLASS DECLARATION                                                          */
/*****************************************************************************/
/**
 * This class defines the data types that the sensor will ultimately give us 
 * after a read operation. Update it to your sensor requirements and call it
 * whatever you like. Make sure the members are public.
 */

class Nano33BLEGestureData
{
  public:
    enum  GESTURE
    {
      LEFT = GESTURE_LEFT,
      RIGHT = GESTURE_RIGHT,
      UP = GESTURE_UP,
      DOWN = GESTURE_DOWN
    };

    enum GESTURE gesture;
    uint32_t timeStampMs;
};

/**
 * @brief This class reads gesture data from the on board Nano 33 BLE
 * Sense APDS9960 using Mbed OS. It stores the results in a ring 
 * buffer (within the Nano33BLESensorBuffer Class) which can be accessed
 * in a manner with softer time constraints than other implementations. 
 */
class Nano33BLEGesture: public Nano33BLESensorBuffer<Nano33BLEGestureData>
{
  public:
   /**
     * @brief Initialises the sensor and starts the Mbed OS Thread.
     * 
     */
    void begin()
    {
      init();
      readThread.start(mbed::callback(Nano33BLEGesture::readFunction, this));
    }

    Nano33BLEGesture(
      uint32_t readPeriod_ms = DEFAULT_GESTURE_READ_PERIOD_MS,
      osPriority threadPriority = osPriorityNormal,
      uint32_t threadSize = DEFAULT_GESTURE_THREAD_STACK_SIZE_BYTES) :
        readPeriod(readPeriod_ms),
        readThread(
        threadPriority,
        threadSize){};
  private:
    /**
     * @brief Initialises the accelerometer sensor.
     * 
     */
    void init(void);
    /**
     * @brief Takes one reading from the accelerometer sensor if a reading 
     * is available.
     * 
     */
    void read(void);

    static void readFunction(Nano33BLEGesture *instance)
    {
      while(1)
      {
          instance->read();
      }
    }

    uint32_t readPeriod;
    rtos::Thread readThread;
};


extern Nano33BLEGesture Gesture;
#endif /* NANO33BLEGESTURE_H_ */