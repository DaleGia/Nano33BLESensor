/*
  Nano33BLEGesture.cpp
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
/*INLCUDES                                                                   */
/*****************************************************************************/
#include "Nano33BLEGesture.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*CLASS MEMBER FUNCTION IMPLEMENTATION                                       */
/*****************************************************************************/
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
void Nano33BLEGesture::init()
{
  APDS.setGestureSensitivity(IR_GESTURE_SENSITIVITY);
  if (!APDS.begin())
  {
    /* Something went wrong... Put this thread to sleep indefinetely. */
    osSignalWait(0x0001, osWaitForever);
  }
  /* As per Arduino_APDS9960.h, 0=100%, 1=150%, 2=200%, 3=300%. Obviously more
   * boost results in more power consumption. 
   */
  APDS.setLEDBoost(IR_LED_BOOST_VALUE);
  return;
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
void Nano33BLEGesture::read(void)
{
  /* 
   * Place the implementation required to read the sensor
   * once here.
   */
  Nano33BLEGestureData data;

  /* If new proximity data is available on the APDS9960 get the data.*/
  if (APDS.gestureAvailable())
  {
    data.gesture = (enum Nano33BLEGestureData::GESTURE)APDS.readGesture();
    data.timeStampMs = millis();
    push(data);
  }
  /* This is required for the timing of the reading of
   * the sensor. Do not delete it.
   */
  rtos::ThisThread::sleep_for(this->readPeriod);
  return;
}

Nano33BLEGesture Gesture;