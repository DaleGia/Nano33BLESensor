/*
  Nano33BLESensor_template.h
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
#ifndef NANO33BLEYOURFILENAMEHERE_H_
#define NANO33BLEYOURFILENAMEHERE_H_

/*****************************************************************************/
/*INLCUDES                                                                   */
/*****************************************************************************/
/* These are required, do not remove them */
#include "Nano33BLESensor.h"
#include "Nano33BLESensorBuffer.h"

/* Place includes required for the initialisation and read of the sensor here*/

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/**
 * This macro is required. It defines the wait period between sensor reads.
 * Update to the value you need based on how fast the sensor can read data.  
 */
#define READ_PERIOD_MS          (8U)

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

class Nano33BLEYOURDATACLASSNAMEHERE
{
  public:
};

/**
 * This class declares the init and read functions your sensor will use to 
 * initialise the sensor and get the data. All you have to do is change the
 * class name what a name you like 
 * (currently "Nano33BLEYOURCLASSNAMEHERE"), and update the 
 * "Nano33BLEYOURDATACLASSNAMEHERE" name to the name you defined in 
 * the section above.
 */
class Nano33BLEYOURCLASSNAMEHERE: public Nano33BLESensor<Nano33BLEYOURCLASSNAMEHERE>, public Nano33BLESensorBuffer<Nano33BLEYOURDATACLASSNAMEHERE>
{
  public:
    void init(void);
    void read(void);
};

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
void Nano33BLEYOURCLASSNAMEHERE::init()
{
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
void Nano33BLEYOURCLASSNAMEHERE::read(void)
{
  /* 
   * Place the implementation required to read the sensor
   * once here.
   */


  /* This is required for the timing of the reading of
   * the sensor. Do not delete it.
   */
  rtos::ThisThread::sleep_for(READ_PERIOD_MS);
}

/* Update these names to match the name of the file */ 
#endif /* NANO33BLEYOURFILENAMEHERE_H_ */