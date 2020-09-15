/*
  NanoBLESensorBuffer.h
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This class implements a way to store and access the circular buffer
  that each sensor will have assigned to it.

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
#ifndef NANO33BLESENSORBUFFER_H_
#define NANO33BLESENSORBUFFER_H_

/*****************************************************************************/
/*INLCUDES                                                                   */
/*****************************************************************************/
#include "Arduino.h"
#include <CircularBuffer.h>

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
#define BUFFER_SIZE    (20U)

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*CLASS DECLARATION                                                          */
/*****************************************************************************/
template<class T>
class Nano33BLESensorBuffer
{
    public:
        uint32_t getAvailableDataSize(void);
        bool pop(T& data);
        uint32_t popMultiple(T& buffer, uint32_t size);
    protected:
        void push(T& data);
    private:
        mbed::CircularBuffer<T, BUFFER_SIZE> buffer;
};

/*****************************************************************************/
/*CLASS MEMBER FUNCTION IMPLEMENTATION                                       */
/*****************************************************************************/
template<class T> uint32_t Nano33BLESensorBuffer<T>::getAvailableDataSize(void)
{
    return this->buffer.size();
}

template<class T> bool Nano33BLESensorBuffer<T>::pop(T& buffer)
{
    return this->buffer.pop(buffer);
}

template<class T> uint32_t Nano33BLESensorBuffer<T>::popMultiple(T& buffer, uint32_t size)
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

template<class T> void Nano33BLESensorBuffer<T>::push(T& data)
{
    this->buffer.push(data);
    return;
}

#endif /* NANO33BLESENSORBUFFER_H_ */
