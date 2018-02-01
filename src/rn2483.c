/*
 * rn2483.c
 *
 *  Created on: 18-jan.-2018
 *      Author: Guus Leenders
 */



#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#include "leuart.h"
#include "usart.h"
#include "delay.h"
#include "util.h"

#include "rn2483.h"

char commandBuffer[RN2483_COMMANDBUFFER_SIZE];
bool otaa = false;
// OTAA addresses
char deviceEUI[17]; 	// arrays size + 1 because of \0 terminator in string
char applicationEUI[17];
char applicationKey[33];
// ABP addresses
char networkSessionKey[33];
char applicationSessionKey[33];
char deviceAddress[9];

void RN2483_Init(char * receiveBuffer, uint8_t bufferSize){ // Setup with autobaud
	USART0_ClearCondition(); 									// >936us low pulse
	USART0_Setup(); 											// Setup serial
	memset(commandBuffer, '\0', RN2483_COMMANDBUFFER_SIZE);
	sprintf(commandBuffer, "U"); 							// Send 0x55
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	DelayMs(1000);
	RN2483_GetSystemVersion(receiveBuffer, bufferSize);
}
void RN2483_Sleep(uint32_t sleepTime, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys sleep %lu\r\n", (unsigned long) sleepTime);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
}
void RN2483_GetHardwareEUI(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac get deveui\r\n");
	Leuart_SendCommand(commandBuffer, strlen(commandBuffer), receiveBuffer, bufferSize);
}/*
void RN2483_GetSystemVersion(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys get ver\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_GetApplicationEUI(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys get appeui\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_MacReset(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac reset 868\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetDeviceEUI(char * eui, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(eui) == 16){
		strcpy(deviceEUI, eui);
		deviceEUI[16] = '\0';
	}
	sprintf(commandBuffer, "mac set deveui %s\r\n", deviceEUI);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetApplicationEUI(char * eui, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(eui) == 16){
		strcpy(applicationEUI, eui);
		applicationEUI[16] = '\0';
	}
	sprintf(commandBuffer, "mac set appeui %s\r\n", applicationEUI);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetApplicationKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(key) == 32){
		strcpy(applicationKey, key);
		applicationKey[32] = '\0';
	}
	sprintf(commandBuffer, "mac set appkey %s\r\n", applicationKey);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetApplicationSessionKey(char * skey, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(skey) == 32){
		strcpy(applicationSessionKey, skey);
		applicationKey[32] = '\0';
	}
	sprintf(commandBuffer, "mac set appskey %s\r\n", applicationSessionKey);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}

void RN2483_SetNetworkSessionKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(key) == 32){
		strcpy(networkSessionKey, key);
		networkSessionKey[32] = '\0';
	}
	sprintf(commandBuffer, "mac set nwkskey %s\r\n", networkSessionKey);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetDeviceAddress(char * address, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(address) == 8){
		strcpy(deviceAddress, address);
		deviceAddress[8] = '\0';
	}
	sprintf(commandBuffer, "mac set devaddr %s\r\n", deviceAddress);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetOutputPower(uint8_t pwr, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set pwridx %i\r\n", pwr);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_DisableAdaptiveDataRate(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set adr off\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_DisableAutomaticReplies(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set ar off\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetDataRate(uint8_t dr, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set dr %i\r\n", dr);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_EnableAdaptiveDataRate(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set adr on\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetBatteryLevel(uint8_t battery, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set bat %i\r\n", battery);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SaveMac(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac save\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
bool RN2483_JoinOTAA(char * receiveBuffer, uint8_t bufferSize){
	otaa = true;
	bool joined = false;
	for(int i=0; i<10 && !joined; i++){
		sprintf(commandBuffer, "mac join otaa\r\n");
		USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
		USART0_ReceiveBuffer(receiveBuffer, bufferSize); // Receive command response
		USART0_ReceiveBuffer(receiveBuffer, bufferSize); // Receive accepted
		if(StringStartsWith(receiveBuffer, "accepted")){
			joined = true;
			DelayMs(500);
		}else{
			DelayMs(500);
		}
	}
	return joined;
}
bool RN2483_JoinABP(char * receiveBuffer, uint8_t bufferSize){
	otaa = false;
	bool joined = false;
	for(int i=0; i<10 && !joined; i++){
		sprintf(commandBuffer, "mac join abp\r\n");
		USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
		USART0_ReceiveBuffer(receiveBuffer, bufferSize); // Receive command response
		USART0_ReceiveBuffer(receiveBuffer, bufferSize); // Receive accepted
		if(StringStartsWith(receiveBuffer, "accepted")){
			joined = true;
			DelayMs(500);
		}else{
			DelayMs(500);
		}
	}
	return joined;
}
bool RN2483_Setup(char * receiveBuffer, uint8_t bufferSize){
	if(otaa){
		return RN2483_SetupOTAA(applicationEUI, applicationKey, deviceEUI, receiveBuffer, bufferSize);
	}else{
		return RN2483_SetupABP(deviceAddress, applicationSessionKey, networkSessionKey, receiveBuffer, bufferSize);
	}
}
bool RN2483_SetupOTAA(char * appEUI, char * appKey, char * devEUI, char * receiveBuffer, uint8_t bufferSize){
	RN2483_MacReset(receiveBuffer, bufferSize);
	if(devEUI == NULL){
		RN2483_GetHardwareEUI(receiveBuffer, bufferSize);
		char address[16];
		sprintf(address, "%s", receiveBuffer);
		RN2483_SetDeviceEUI(address, receiveBuffer, bufferSize);
	}else{
		RN2483_SetDeviceEUI(devEUI, receiveBuffer, bufferSize);
	}
	RN2483_SetApplicationEUI(appEUI, receiveBuffer, bufferSize);
	RN2483_SetApplicationKey(appKey, receiveBuffer, bufferSize);
	RN2483_SetOutputPower(RN2483_POWER_14DBM, receiveBuffer, bufferSize);
	RN2483_DisableAdaptiveDataRate(receiveBuffer, bufferSize);
	RN2483_DisableAutomaticReplies(receiveBuffer, bufferSize);
	RN2483_SetDataRate(5, receiveBuffer, bufferSize);
	RN2483_EnableAdaptiveDataRate(receiveBuffer, bufferSize);
	RN2483_SetBatteryLevel(150, receiveBuffer, bufferSize);
	RN2483_SaveMac(receiveBuffer, bufferSize);
	bool joined = RN2483_JoinOTAA(receiveBuffer, bufferSize);
	return joined;
}
bool RN2483_SetupABP(char * devAddr, char * appKey, char * NwkSKey, char * receiveBuffer, uint8_t bufferSize){
	RN2483_MacReset(receiveBuffer, bufferSize);
	RN2483_SetNetworkSessionKey(NwkSKey, receiveBuffer, bufferSize);
	RN2483_SetApplicationSessionKey(appKey, receiveBuffer, bufferSize);
	RN2483_SetDeviceAddress(devAddr, receiveBuffer, bufferSize);
	RN2483_SetOutputPower(RN2483_POWER_14DBM, receiveBuffer, bufferSize);
	RN2483_DisableAdaptiveDataRate(receiveBuffer, bufferSize);
	RN2483_DisableAutomaticReplies(receiveBuffer, bufferSize);
	RN2483_SaveMac(receiveBuffer, bufferSize);
	bool joined = RN2483_JoinABP(receiveBuffer, bufferSize);
	return joined;
}
uint8_t RN2483_Transmit(char * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize){
	return RN2483_TransmitUnconfirmed(data, payloadSize,  receiveBuffer, bufferSize);
}
uint8_t RN2483_TransmitUnconfirmed(char * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize){
	char * decodedPayload;
	StringToHexString(data, payloadSize/2, &decodedPayload);
	sprintf(commandBuffer, "mac tx uncnf 1 %s\r\n", decodedPayload);
	free(decodedPayload);
	return RN2483_TransmitProcessCommand(payloadSize + 18, receiveBuffer, bufferSize);
}
uint8_t RN2483_TransmitConfirmed(char * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize){
	char * decodedPayload;
	StringToHexString(data, payloadSize/2, &decodedPayload);
	sprintf(commandBuffer, "mac tx cnf 1 %s\r\n", decodedPayload);
	free(decodedPayload);
	return RN2483_TransmitProcessCommand(payloadSize + 17, receiveBuffer, bufferSize);
}
uint8_t RN2483_TransmitProcessCommand(uint8_t commandSize, char * receiveBuffer, uint8_t bufferSize){
	bool sendDone = false;
	uint8_t busyCounter = 0;
	uint8_t retryCounter = 0;

	while(!sendDone){
		retryCounter ++;
		if(retryCounter > 10){
			return TX_FAIL;
		}
		USART0_SendBuffer(commandBuffer, commandSize);
		USART0_ReceiveBuffer(receiveBuffer, bufferSize);
		if(StringStartsWith(receiveBuffer, "ok")){
			USART0_ReceiveBuffer(receiveBuffer, bufferSize);
			if(StringStartsWith(receiveBuffer, "mac_tx_ok")){
				sendDone = true;
				return TX_SUCCESS;
			}else if(StringStartsWith(receiveBuffer, "mac_rx")){
				// TODO: capture confirmation data
				sendDone = true;
				return TX_SUCCESS_CONFIRMED;
			}else if(StringStartsWith(receiveBuffer, "mac_err")){
				RN2483_Setup(receiveBuffer, bufferSize);
			}else if(StringStartsWith(receiveBuffer, "invalid_data_len")){
				sendDone = true;
				return TX_FAIL;
			}else if(StringStartsWith(receiveBuffer, "radio_tx_ok")){
				sendDone = true;
				return TX_SUCCESS;
			}else if(StringStartsWith(receiveBuffer, "radio_err")){
				RN2483_Setup(receiveBuffer, bufferSize);
			}
		}else if(StringStartsWith(receiveBuffer, "invalid_param")){
			sendDone = true;
			return TX_FAIL;
		}else if(StringStartsWith(receiveBuffer, "not_joined")){
			RN2483_Setup(receiveBuffer, bufferSize);
		}else if(StringStartsWith(receiveBuffer, "no_free_ch")){
			DelayMs(500);
		}else if(StringStartsWith(receiveBuffer, "silent")){
			RN2483_Setup(receiveBuffer, bufferSize);
		}else if(StringStartsWith(receiveBuffer, "frame_counter_err_rejoin_needed")){
			RN2483_Setup(receiveBuffer, bufferSize);
		}else if(StringStartsWith(receiveBuffer, "busy")){
			busyCounter ++;
			if(busyCounter >=10){
				RN2483_Setup(receiveBuffer, bufferSize);
			}else{
				DelayMs(500);
			}
		}else if(StringStartsWith(receiveBuffer, "mac_paused")){
			RN2483_Setup(receiveBuffer, bufferSize);
		}else if(StringStartsWith(receiveBuffer, "invalid_data_len")){
			sendDone = true;
			return TX_FAIL;
		}else{
			RN2483_Setup(receiveBuffer, bufferSize);
		}
	}
	return TX_FAIL;
}*/
