/*
  Nano33BLESensorMicrophoneRMS.h
  Copyright (c) 2020 Dale Giancono. All rights reserved..

`  *** WRITE SOMETHING HERE ***

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
/* These are required, do not remove them */
#include "Nano33BLESensor.h"
#include "Nano33BLESensorBuffer.h"

/* Place includes required for the initialisation and read of the sensor here*/
#include <PDM.h>

#include <arm_math.h>
/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/* This value was also used in the PDM example, seems like a good enough reason to
 * continue using it. With this value and 16kHz sampling frequency, the RMS sampling
 * period will be 16mS */
#define MICROPHONE_BUFFER_SIZE_IN_WORDS (256U)

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/
const uint32_t MICROPHONE_BUFFER_SIZE_IN_BYTES_C = (MICROPHONE_BUFFER_SIZE_IN_WORDS * sizeof(int16_t));

/* MP34DT05 Microphone data buffer with a bit depth of 16. Also a variable for the RMS value */
static int16_t microphoneBuffer[MICROPHONE_BUFFER_SIZE_IN_WORDS];
static rtos::Semaphore bufferReadySemaphore;

/*****************************************************************************/
/*GLOBAL Functions                                                                */
/*****************************************************************************/
void PDM_callback(void);

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
 * This class declares the init and read functions your sensor will use to 
 * initialise the sensor and get the data. All you have to do is change the
 * class name what a name you like 
 * (currently "Nano33BLEYOURCLASSNAMEHERE"), and update the 
 * "Nano33BLEYOURDATACLASSNAMEHERE" name to the name you defined in 
 * the section above.
 */
class Nano33BLEMicrophoneRMS: public Nano33BLESensor<Nano33BLEMicrophoneRMS>, public Nano33BLESensorBuffer<Nano33BLEMicrophoneRMSData>
{
  public:
    void init(void);
    void read(void);     
};

/*****************************************************************************/
/*CLASS MEMBER FUNCTION IMPLEMENTATION                                       */
/*****************************************************************************/
void PDM_callback(void)
{
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  if(bytesAvailable == MICROPHONE_BUFFER_SIZE_IN_BYTES_C)
  {
    PDM.read(microphoneBuffer, MICROPHONE_BUFFER_SIZE_IN_BYTES_C);
    bufferReadySemaphore.release();
  }
}

/**
 * @brief
 * This member function implementation should do everything requred to 
 * initialise the sensor this class is designed for. Immediately after 
 * this function is executed, the RTOS will begin periodically reading
 * values from the sensor.
 * 
 * @param none
 * @return none
 */
void Nano33BLEMicrophoneRMS::init()
{
  /* PDM setup for MP34DT05 microphone */
  /* configure the data receive callback to transfer data to local buffer */
  PDM.onReceive(PDM_callback);
  /* Initialise single PDM channel with a 16KHz sample rate (only 16kHz or 44.1kHz available */
  if (!PDM.begin(1, 16000))
  {
    /* Something went wrong... Put this thread to sleep indefinetely. */
    osSignalWait(0x0001, osWaitForever);
  }
  else
  {
    /* Gain values can be from 0 to 80 (around 38db). Check out nrf_pdm.h
     * from the nRF528x-mbedos core to confirm this. 
     */
    /* 
     * This has to be done after PDM.begin() is called as begin() always
     *  sets the gain as the default PDM.h value (20).
     */
    PDM.setGain(80);
  }
}

/**
 * @brief
 * This member function implementation should do everything requred to 
 * read one reading from the sensor this class is designed for. This 
 * function is put inside an endless while loop so will be called 
 * endlessly, therefore a sleep should be called at the end of the 
 * function. The sleep period should be defined by the READ_PERIOD_MS
 * defined at the start of this file.
 * 
 * @param none
 * @return none
 */
void Nano33BLEMicrophoneRMS::read(void)
{
  /* 
   * Place the implementation required to read the sensor
   * once here.
   */
  bufferReadySemaphore.acquire();
  Nano33BLEMicrophoneRMSData data;
  arm_rms_q15((q15_t*)microphoneBuffer, MICROPHONE_BUFFER_SIZE_IN_WORDS, (q15_t*)&data.RMSValue);
  data.timeStampMs = millis();
  push(data);
}

/* Update these names to match the name of the file */ 
#endif /* NANO33BLEMICROPHONERMS_H_ */