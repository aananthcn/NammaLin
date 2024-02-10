/*
 * Created on Mon Dec 19 2022 5:25:54 PM
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
#ifndef LIN_GENERAL_TYPES_H
#define LIN_GENERAL_TYPES_H

#include <Platform_Types.h>
#include <Std_Types.h>

typedef uint8 Lin_FramePidType;  /*Represents all valid protected identifier*/

typedef enum {
    LIN_ENHANCED_CS,
    LIN_CLASSIC_CS
} Lin_FrameCsModelType;

typedef enum {
    LIN_FRAMERESPONSE_TX,
    LIN_FRAMERESPONSE_RX,
    LIN_FRAMERESPONSE_IGNORE
} Lin_FrameResponseType;

typedef uint8 Lin_FrameDlType;

typedef struct {
    Lin_FramePidType Pid;
    Lin_FrameCsModelType Cs;
    Lin_FrameResponseType Drc;
    Lin_FrameDlType Dl;
    const uint8* SduPtr;
} Lin_PduType;

typedef enum {
    LIN_NOT_OK,
    LIN_TX_OK,
    LIN_TX_BUSY,
    LIN_TX_HEADER_ERROR,
    LIN_TX_ERROR,
    LIN_RX_OK,
    LIN_RX_BUSY,
    LIN_RX_ERROR,
    LIN_RX_NO_RESPONSE,
    LIN_OPERATIONAL,
    LIN_CH_SLEEP
} Lin_StatusType;

typedef enum {
    LIN_ERR_HEADER,
    LIN_ERR_RESP_STOPBIT,
    LIN_ERR_RESP_CHKSUM,
    LIN_ERR_RESP_DATABIT,
    LIN_ERR_NO_RESP,
    LIN_ERR_INC_RESP
} Lin_SlaveErrorType;


#endif