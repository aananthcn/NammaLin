/*
 * Created on Mon Dec 19 2022 5:24:49 PM
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
#include <stddef.h>

#include <Lin.h>
#include <Lin_cfg.h>

/**********************************************************/
/***					GLOBAL VARIABLES				***/
/**********************************************************/
// static Lin_ConfigType LinConfigs[LIN_DRIVER_MAX_CHANNEL];

static Lin_Module_State_Type Lin_Module_State = LIN_UNINIT;
static Lin_StatusType Lin_Driver_state[LIN_DRIVER_MAX_CHANNEL];

static boolean Lin_error_detect_flag;

static uint16 g_Vendor_ID;
static uint16 g_Module_ID;
static uint8 g_Major_Version_number;
static uint8 g_Minor_version_number;
static uint8 g_Patch_version_number;

static uint32 Lin_Error_code = 0;

/******************************************************************************************/
/*								PRIVATE FUNCTION DEFINITION			    				  */
/******************************************************************************************/

void LinDrv_Init(void) {

	uint8_t Channel_Index;

	/* Iterating Init call according to the total number of LIN channel in the module */
	for(Channel_Index = 0; Channel_Index < LIN_DRIVER_MAX_CHANNEL; Channel_Index++)
	{
		Lin_Init(&LinConfigs[Channel_Index]);
	}
		
}


/******************************************************************************************/
/*								PUBLIC FUNCTION DEFINITION								  */
/******************************************************************************************/

void Lin_Init(const Lin_ConfigType* Config) {

	uint8 fl_Lin_Index;
	uint16 fl_Timeout_sec;
	boolean fl_Lin_Dev_Error_Detect;
	boolean fl_Lin_Version_Info_API;

	uint16 fl_Lin_Channel_ID;
	uint16 fl_Lin_Channel_Baudrate;
	LinNodeType fl_Lin_Node_Type;
	boolean fl_Wakeup_support;
	const char* fl_Lin_Wakeup_Source;
	const char* fl_Lin_Clock_Ref;

	fl_Lin_Index = Config->general.lin_index;
	fl_Timeout_sec = Config->general.lin_timeout_sec;
	fl_Lin_Dev_Error_Detect = Config->general.lin_dev_error_detect;
	fl_Lin_Version_Info_API = Config->general.lin_version_info_api;

	fl_Lin_Channel_ID = Config->chn_cfg.lin_chan_id;
	fl_Lin_Channel_Baudrate = Config->chn_cfg.lin_chan_baudrate;
	fl_Lin_Node_Type = Config->chn_cfg.lin_node_type;
	fl_Wakeup_support = Config->chn_cfg.lin_wakeup_support;
	fl_Lin_Wakeup_Source = Config->chn_cfg.lin_wakeup_source;
	fl_Lin_Clock_Ref = Config->chn_cfg.lin_clock_ref;
	
	/*Check for LIN Development error detection */
	if (fl_Lin_Dev_Error_Detect == TRUE)
	{
		Lin_error_detect_flag = TRUE;

		/* Check for the valid Configuration parameter */
	 	if (Config == NULL)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_INVALID_POINTER;
	 	}

		/* Check for the LIN module not in Initialized state */
	 	if (Lin_Module_State == LIN_UNINIT)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_STATE_TRANSITION;
	 	}

	}

	Lin_Module_State = LIN_INIT;

	if(fl_Wakeup_support)
	{
		//enable Wake-up detection
		//set the hardware unit to reduced power mode operation if supported
	}

	Lin_Driver_state[fl_Lin_Index] = LIN_CH_SLEEP;
	
}


Std_ReturnType Lin_CheckWakeup(uint8 Channel) {

	Std_ReturnType fl_Lin_error_status;

	fl_Lin_error_status = E_NOT_OK;

	/*Check for LIN Development error detection */
	if (Lin_error_detect_flag == TRUE)
	{

		/* Check the LIN module is Initialized */
	 	if (Lin_Module_State != LIN_INIT)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_UNINIT;
			return fl_Lin_error_status;
	 	}

		/* Check for the valid Channel */
	 	if (Channel > LIN_DRIVER_MAX_CHANNEL)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_INVALID_CHANNEL;
			return fl_Lin_error_status;
	 	}
	}

		//if RXD == constant low level
		//Notify ECU State manager - EcuM_SetWakeupEvent
		//LinIF Callback  - LinIf_WakeupConfirmation

		fl_Lin_error_status = E_OK;
		return fl_Lin_error_status;

}

void Lin_GetVersionInfo(Std_VersionInfoType* versioninfo) {

	if(versioninfo == NULL)
	{
		/*Report error with the error code parameter - Lin_Error_code */
		Lin_Error_code = LIN_E_PARAM_POINTER;
	}

	g_Vendor_ID	= versioninfo->vendorID;
	g_Module_ID = versioninfo->moduleID;
	g_Major_Version_number = versioninfo->sw_major_version;
	g_Minor_version_number = versioninfo->sw_minor_version;
	g_Patch_version_number = versioninfo->sw_patch_version;
	
}

Std_ReturnType Lin_SendFrame(uint8 Channel, const Lin_PduType* PduInfoPtr) {
	
	Std_ReturnType fl_Lin_error_status;
	fl_Lin_error_status = E_NOT_OK;
	LinNodeType fl_Lin_Node = LinConfigs[Channel].chn_cfg.lin_node_type;

	Lin_FramePidType fl_Pid;
	Lin_FrameCsModelType fl_cs_model;
	Lin_FrameResponseType fl_drc;
	Lin_FrameDlType fl_Dl;

	/*Check for the LIN Node Type as Master */
	if(fl_Lin_Node != MASTER)
	{
		return fl_Lin_error_status;
	}

	/*Check for LIN Development error detection */
	if (Lin_error_detect_flag == TRUE)
	{
		/* Check if the LIN module state */
	 	if (Lin_Module_State != LIN_INIT)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_UNINIT;
			return fl_Lin_error_status;
	 	}

		/* Check for Channel is valid */
	 	if (Channel > LIN_DRIVER_MAX_CHANNEL)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_INVALID_CHANNEL;
			return fl_Lin_error_status;
	 	}

		/* Check for the parameter is a NULL pointer */
		if(PduInfoPtr == NULL)
		{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_INVALID_POINTER;
			return fl_Lin_error_status;
		}

		/* Check the Driver state is Operational*/
		if(Lin_Driver_state[Channel] == LIN_CH_SLEEP || Lin_Driver_state[Channel] == LIN_NOT_OK)
		{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_STATE_TRANSITION;
			return fl_Lin_error_status;
		}
	}

	fl_Pid = PduInfoPtr->Pid;
	fl_cs_model = PduInfoPtr->Cs;
	fl_drc = PduInfoPtr->Drc;
	fl_Dl = PduInfoPtr->Dl;
	/*UART Tx - LIN Header - Break field, Synch byte, pid

	if( LinDriverstate[Channel] == LIN_TX_BUSY || LIN_TX_ERROR || LIN_RX_BUSY || LIN_RX_ERROR)
	{
		LinDriverstate[Channel] = LIN_OPERATIONAL;
	}

	PID = PduInfoPtr->PID
	Length = PduInfoPtr->Dl
	
	
	*/
	/*Response*/

	/* If Drc = LIN_FRAMERESPONSE_TX = Master Response  
		Lin_Driver_state[Channel] = LIN_TX_BUSY;
		Clear ready flag before send
		Enable frame complete
		Enable Tx IRQ
		if (PduInfoPtr->DI > 0){
			TxCurrPtr[Channel] = LinBufTx[Channel];
			TxSize[Channel] = PduInfoPtr->DI;
			memcpy(TxCurrPtr[Channel],PduInfoPtr->SduPtr,PduInfoPtr->DI);
		}
		write to transmit reg


		if(PduInfoPtr->Cs == Enhanced)
		Append checksum to Tx

		Wait until Lin_GetStatus() == LIN_TX_OK;

	*/


	/* If Drc = LIN_FRAMERESPONSE_RX = Slave Response
	Lin_Driver_state[Channel] = LIN_RX_BUSY;
	RxCurrPtr[Channel] = LinBufRx[Channel];
	RxSize[Channel] = ptr->Dl

	write to transmit reg
	Clear ready flag before send
	write to transmit reg
	Enable Rx IRQ


	Receive data followed by checksum

	Wait until Lin_GetStatus() == LIN_RX_OK;

	*/
	//If Drc =  LIN_FRAMERESPONSE_IGNORE, ignore Rx

	//Set status of LIN driver according to Tx, Rx & Error 

	

	fl_Lin_error_status = E_OK;
	return fl_Lin_error_status;
}

Std_ReturnType Lin_GoToSleep(uint8 Channel) {

	Std_ReturnType fl_Lin_error_status;
	fl_Lin_error_status = E_NOT_OK;

	LinNodeType fl_Lin_Node = LinConfigs[Channel].chn_cfg.lin_node_type;

	Lin_PduType fl_Pdu_Info;

	uint8 data[8] = {0x00, 0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF};
	
	/*Check for LIN Development error detection */
	if (Lin_error_detect_flag == TRUE)
	{
		/* Check if the LIN module state */
	 	if (Lin_Module_State != LIN_INIT)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_UNINIT;
			return fl_Lin_error_status;
	 	}

		/* Check for Channel is valid */
	 	if (Channel > LIN_DRIVER_MAX_CHANNEL)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_INVALID_CHANNEL;
			return fl_Lin_error_status;
	 	}

		/*Check for the LIN Node Type as Master */
		if(fl_Lin_Node != MASTER)
		{
			return fl_Lin_error_status;
		}

	}
	/*
	Add macro for Sleep Request ID - 0x3C
	*/
	Lin_Driver_state[Channel] = LIN_CH_SLEEP;

	/* Diagnostic Frame - Master Sleep Request */

	fl_Pdu_Info.Cs = LIN_CLASSIC_CS; // ID-60, 61 always use classic checksum
	fl_Pdu_Info.Pid = 0x3C;
	fl_Pdu_Info.SduPtr = data;
	fl_Pdu_Info.Dl = 8;
	fl_Pdu_Info.Drc = LIN_FRAMERESPONSE_TX;

	/*Tx Go-to-sleep-command*/
	Lin_SendFrame(Channel, &fl_Pdu_Info);
	
	
	//In case of Tx error - Set Channel state to LIN_CH_SLEEP_PENDING
	// Shall termination ongoing Frame TX 

	//Wake-up detection enable
	//Reduced Power mode enable

	fl_Lin_error_status = E_OK;
	return fl_Lin_error_status;
}



Std_ReturnType Lin_GoToSleepInternal(uint8 Channel) {

	Std_ReturnType fl_Lin_error_status;
	fl_Lin_error_status = E_NOT_OK;
	LinNodeType fl_Lin_Node = LinConfigs[Channel].chn_cfg.lin_node_type;
	
	/*Check for LIN Development error detection */
	if (Lin_error_detect_flag == TRUE)
	{
		/* Check if the LIN module state */
	 	if (Lin_Module_State != LIN_INIT)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_UNINIT;
			return fl_Lin_error_status;
	 	}

		/* Check for Channel is valid */
	 	if (Channel > LIN_DRIVER_MAX_CHANNEL)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_INVALID_CHANNEL;
			return fl_Lin_error_status;
	 	}

	}

	//Wake-up detection enable
	//Reduced Power mode enable

	Lin_Driver_state[Channel] = LIN_CH_SLEEP;

	fl_Lin_error_status = E_OK;
	return fl_Lin_error_status;
}

Std_ReturnType Lin_Wakeup(uint8 Channel) {
	
	Std_ReturnType fl_Lin_error_status;
	fl_Lin_error_status = E_NOT_OK;

	Lin_PduType fl_Pdu_Info;

	uint8 data[2] = {0x00, 0x00};  //Need to verify


	/*Check for LIN Development error detection */
	if (Lin_error_detect_flag == TRUE)
	{
		/* Check if the LIN module state */
	 	if (Lin_Module_State != LIN_INIT)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_UNINIT;
			return fl_Lin_error_status;
	 	}

		/* Check for Channel is valid */
	 	if (Channel > LIN_DRIVER_MAX_CHANNEL)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_INVALID_CHANNEL;
			return fl_Lin_error_status;
	 	}

		/* Check for the LIN Driver is in SLEEP state*/
		if(Lin_Driver_state[Channel] != LIN_CH_SLEEP)
		{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_STATE_TRANSITION;
			return fl_Lin_error_status;
		}
	}

	/* Diagnostic Frame - Master Wakeup Request */

	fl_Pdu_Info.Cs = LIN_CLASSIC_CS; // ID-60, 61 always use classic checksum
	fl_Pdu_Info.Pid = 0x00;  //Need to verify
	fl_Pdu_Info.SduPtr = data;
	fl_Pdu_Info.Dl = 2;
	fl_Pdu_Info.Drc = LIN_FRAMERESPONSE_RX;


	/*Tx Go-to-sleep-command*/
	Lin_SendFrame(Channel, &fl_Pdu_Info);

	Lin_Driver_state[Channel] = LIN_OPERATIONAL;

	fl_Lin_error_status = E_OK;
	return fl_Lin_error_status;

}

Std_ReturnType Lin_WakeupInternal(uint8 Channel) {
		Std_ReturnType fl_Lin_error_status;
	fl_Lin_error_status = E_NOT_OK;

	/*Check for LIN Development error detection */
	if (Lin_error_detect_flag == TRUE)
	{
		/* Check if the LIN module state */
	 	if (Lin_Module_State != LIN_INIT)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_UNINIT;
			return fl_Lin_error_status;
	 	}

		/* Check for Channel is valid */
	 	if (Channel > LIN_DRIVER_MAX_CHANNEL)
	 	{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_INVALID_CHANNEL;
			return fl_Lin_error_status;
	 	}

		/* Check for the LIN Driver is in SLEEP state*/
		if(Lin_Driver_state[Channel] != LIN_CH_SLEEP)
		{
			/*Report error with the error code parameter - Lin_Error_code */
			Lin_Error_code = LIN_E_STATE_TRANSITION;
			return fl_Lin_error_status;
		}
	}

	Lin_Driver_state[Channel] = LIN_OPERATIONAL;

	fl_Lin_error_status = E_OK;
	return fl_Lin_error_status;
}

Lin_StatusType Lin_GetStatus(uint8 Channel, const uint8** Lin_SduPtr) {

	Lin_StatusType fl_Lin_status;
	fl_Lin_status = Lin_Driver_state[Channel];

	//Code - Need to improvise

	//Lin_SduPtr->SDU until next Lin_sendFrame() call

	Lin_Driver_state[Channel] = LIN_CH_SLEEP;

	return fl_Lin_status;
}

