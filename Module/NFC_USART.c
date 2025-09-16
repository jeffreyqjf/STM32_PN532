#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include "Delay.h"
#include "OLED.h"

void NFC_Send_ACK(void);
uint8_t NFC_Status = 1;
uint8_t Card_UID[7];

void NFC_Clear_status(void){
	RxLen = 0;
	for(int i = 0; i < 40;i++){Serial_RxPacket[i] = 0;}
	for(int i = 0; i < 7;i++){Card_UID[i] = 0;}
}


void NFC_USART_init(void){
	Serial_init();
	//DMA_Config();
	Delay_ms(100);
	uint8_t SAMcommand[24] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
	Serial_SendArray(SAMcommand, 24);
	Delay_ms(100); //1000?
	while( RxLen == 0){

		OLED_ShowString(30, 30, "booting", OLED_8X16);
		OLED_Update();
	}
	if(RxLen == 15){
		NFC_Status = 1;
	}else{
		NFC_Status = 0;
	}
	// to receive another data
	OLED_Clear();
	NFC_Clear_status();
	
}

void NFC_ReadIDCard(void){
	uint8_t Readcommand[11] = {0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x02, 0x00, 0xE0, 0x00};
	Serial_SendArray(Readcommand, 11);
	Delay_ms(10);
}
void NFC_ShowRxPacket(void){
	for(int i = 0;i < 6 ;i ++){
			OLED_ShowHexNum(20*i, 0, Serial_RxPacket[i], 2, OLED_6X8);
			OLED_ShowHexNum(20*i, 10, Serial_RxPacket[i+6], 2, OLED_6X8);
			OLED_ShowHexNum(20*i, 20, Serial_RxPacket[i+12], 2, OLED_6X8);
			OLED_ShowHexNum(20*i, 30, Serial_RxPacket[i+18], 2, OLED_6X8);
			OLED_ShowHexNum(20*i, 40, Serial_RxPacket[i+24], 2, OLED_6X8);
			OLED_ShowHexNum(20*i, 50, Serial_RxPacket[i+30], 2, OLED_6X8);
		}
}

void NFC_Send_ACK(void){
	uint8_t ACKcommand[11] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
	Serial_SendArray(ACKcommand, 6);
}
uint8_t* NFC_GET_UID(void){
	// the formate
	// 00 00 FF 00 FF 00                              
	// 00 00 FF 0C F4 D5 4B 01 01 00 04 08 04 24 D3 4F 59 2F 00
	for(int i = 0; i < 7; i++){
		Card_UID[i] = Serial_RxPacket[18 + i];
	}
	return Card_UID;
}

void NFC_Send_SAM_command(void){
	uint8_t SAMcommand[24] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
	Serial_SendArray(SAMcommand, 24);
	Delay_ms(10);
}
