/*
  Nano33BLESensorMicrophoneRMS.h
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This class reads RMS microphone data from the on board Nano 33 BLE
  Sense microphone using Mbed OS. It stores the results in a ring 
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
#ifndef NANO33BLEMICROPHONERMS_H_
#define NANO33BLEMICROPHONERMS_H_

/*****************************************************************************/
/*INLCUDES                                                                   */
/*****************************************************************************/
#include "Nano33BLESensorBuffer.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
#define DEFAULT_MICROPHONE_THREAD_STACK_SIZE_BYTES       (1024U) 

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*GLOBAL Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*CLASS DECLARATION                                                          */
/*****************************************************************************/
/**
 * This class defines the data types that the sensor will ultimately give us 
 * after a read operation. Update it to your sensor requirements and call it
 * whatever you like. Make sure the members are public.
 */

class Nano33BLEMicrophoneRMSData
{
  public:
    int16_t RMSValue;
    uint32_t timeStampMs;
};

/**
 * @brief This class reads rms microphone data from the on board Nano 33 BLE
 * Sense microphone using Mbed OS. It stores the results in a ring 
 * buffer (within the Nano33BLESensorBuffer Class) which can be accessed
 * in a manner with softer time constraints than other implementations. 
 */
class Nano33BLEMicrophoneRMS: public Nano33BLESensorBuffer<Nano33BLEMicrophoneRMSData>
{
  public:
   /**
     * @brief Initialises the sensor and starts the Mbed OS Thread.
     * 
     */
    void begin()
    {
      init();
      readThread.start(mbed::callback(Nano33BLEMicrophoneRMS::readFunction, this));
    }

    Nano33BLEMicrophoneRMS(
      osPriority threadPriority = osPriorityNormal,
      uint32_t threadSize = DEFAULT_MICROPHONE_THREAD_STACK_SIZE_BYTES) :
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

    static void readFunction(Nano33BLEMicrophoneRMS *instance)
    {
      while(1)
      {
          instance->read();
      }
    }

    rtos::Thread readThread;
    static void PDM_callback(void);  
};

extern Nano33BLEMicrophoneRMS MicrophoneRMS;
#endif /* NANO33BLEMICROPHONERMS_H_ */