/*
 * Created on Mon Dec 19 2022 5:28:31 PM
 *
 * The MIT License (MIT)
 * Copyright (c) 2022 Aananth C N, Abiramy Periasamy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef NAMMA_LIN_H
#define NAMMA_LIN_H

#include <Platform_Types.h>
#include <Std_Types.h>
#include <Lin_cfg.h>   /*auto-generated file, contains structre & macros related to Lin_ConfigType*/ 
#include <Lin_GeneralTypes.h>


/*********************************************************************/
/*                           TYPE DEFINITIONS                        */
/*********************************************************************/
typedef enum Lin_Module_State_Type_enum {
    LIN_UNINIT,
    LIN_INIT
}Lin_Module_State_Type;

typedef struct {
    uint8_t data[8];
} LinDrv_DataBuffer_t;

typedef struct {
    uint16_t data[14];
} Lin_Header_Break_Field;

typedef struct {
    uint8_t data[8];
} Lin_Header_Synch;



/*********************************************************************/
/*                           MACRO DEFINITIONS                       */
/*********************************************************************/

/* Development Error Codes*/
#define LIN_E_UNINIT             0x00
#define LIN_E_INVALID_CHANNEL    0x02
#define LIN_E_INVALID_POINTER    0x03
#define LIN_E_STATE_TRANSITION   0x04
#define LIN_E_PARAM_POINTER      0x05

/*Production Error code assigned by DEM */
#define LIN_E_TIMEOUT            0x06


/*********************************************************************/
/*                      PUBLIC FUNCTION DECLARATION                  */
/*********************************************************************/

/* Service to Initialize  the LIN channels */
void Lin_Init(const Lin_ConfigType* Config);

/* Service to Initialize the LIN module */
void LinDrv_Init(void);

/* Service to check if a wakeup has occurred on the addressed LIN channel */
Std_ReturnType Lin_CheckWakeup(uint8 Channel);

/* Service to get the version information of the LIN module */
void Lin_GetVersionInfo(Std_VersionInfoType* versioninfo);

/* Service for sending a LIN header and a LIN response, if necessary */
Std_ReturnType Lin_SendFrame(uint8 Channel, const Lin_PduType* PduInfoPtr);

/* Service is only applicable for LIN master node */
Std_ReturnType Lin_GoToSleep(uint8 Channel);

/* Service to set the channel state to LIN_CH_SLEEP */
Std_ReturnType Lin_GoToSleepInternal(uint8 Channel);

/* Service to generate the wakeup pulse */
Std_ReturnType Lin_Wakeup(uint8 Channel);

/* Sets the channel state to LIN_CH_OPERATIONAL */
Std_ReturnType Lin_WakeupInternal(uint8 Channel);

/* Gets the status of the LIN driver */
Lin_StatusType Lin_GetStatus(uint8 Channel, const uint8** Lin_SduPtr);

 
#endif