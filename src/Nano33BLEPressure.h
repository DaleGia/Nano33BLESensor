/*
  Nano33BLEPressure.h
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This class reads pressure data from the on board Nano 33 BLE
  Sense pressure sensor using Mbed OS. It stores the results in a ring 
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
#ifndef NANO33BLEPRESSURE_H_
#define NANO33BLEPRESSURE_H_

/*****************************************************************************/
/*INLCUDES                                                                   */
/*****************************************************************************/
#include "Nano33BLESensorBuffer.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/**
 * This macro is required. It defines the wait period between sensor reads.
 * Update to the value you need based on how fast the sensor can read data.  
 */
#define DEFAULT_PRESSURE_READ_PERIOD_MS                (40U)
#define DEFAULT_PRESSURE_THREAD_STACK_SIZE_BYTES       (1024U) 

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

class Nano33BLEPressureData
{
  public:
    float barometricPressure;
    uint32_t timeStampMs;
};
/**
 * This class declares the init and read functions your sensor will use to 
 * initialise the sensor and get the data. All you have to do is change the
 * class name what a name you like 
 * (currently "Nano33BLEPressure"), and update the 
 * "Nano33BLEPressureData" name to the name you defined in 
 * the section above.
 */
class Nano33BLEPressure: public Nano33BLESensorBuffer<Nano33BLEPressureData>
{
  public:
   /**
     * @brief Initialises the sensor and starts the Mbed OS Thread.
     * 
     */
    void begin()
    {
      init();
      readThread.start(mbed::callback(Nano33BLEPressure::readFunction, this));
    }

    Nano33BLEPressure(
      uint32_t readPeriod_ms = DEFAULT_PRESSURE_READ_PERIOD_MS,
      osPriority threadPriority = osPriorityNormal,
      uint32_t threadSize = DEFAULT_PRESSURE_THREAD_STACK_SIZE_BYTES) :
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

    static void readFunction(Nano33BLEPressure *instance)
    {
      while(1)
      {
          instance->read();
      }
    }

    uint32_t readPeriod;
    rtos::Thread readThread;
};

extern Nano33BLEPressure Pressure;
#endif /* NANO33BLEPRESSURE_H_ */