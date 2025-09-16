#include "stm32f10x.h"                  // Device header
#include "I2C.h"
#include "Delay.h"
#define NFC_Address 0x48

void NFC_information_frame(uint8_t LEN, uint8_t LCS, uint8_t PDn, uint8_t DCS){
	MyI2C_Start();
	MyI2C_SendByte(NFC_Address);Delay_ms(1);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0xFF);MyI2C_ReceiveAck();
	// LEN = TFI and PD0 to PDn
	MyI2C_SendByte(LEN);MyI2C_ReceiveAck();
	// lower byte of [LEN + LCS] = 0x00
	MyI2C_SendByte(LCS);MyI2C_ReceiveAck();
	// TFI D5h: PN532 -> HOST
	MyI2C_SendByte(0xD4);MyI2C_ReceiveAck();
	// PD0 is command code
	MyI2C_SendByte(PDn);MyI2C_ReceiveAck();
	// lower byte of [TFI + PD0 + ... + PDn + DCS] = 0x00
	MyI2C_SendByte(DCS);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_Stop();
}


void NFC_ACK_frame(void){
	MyI2C_Start();
	MyI2C_SendByte(NFC_Address);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0xFF);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0xFF);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_Stop();
}


void NFC_NACK_frame(void){
	MyI2C_Start();
	MyI2C_SendByte(NFC_Address);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0xFF);MyI2C_ReceiveAck();
	MyI2C_SendByte(0xFF);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_Stop();
}



void NFC_Error_frame(void){
	MyI2C_Start();
	MyI2C_SendByte(NFC_Address);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_SendByte(0xFF);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x01);MyI2C_ReceiveAck();
	MyI2C_SendByte(0xFF);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x7F);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x81);MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t I2C_data[40];
uint8_t point = 0;
uint8_t Data;
uint8_t inter_ack;
void NFC_Read_ACK(void){

	MyI2C_Start();
	MyI2C_SendByte(NFC_Address | 0x01);
	inter_ack = MyI2C_ReceiveAck();

	for(int i = 0;i < 6; i++){
		Data = MyI2C_ReceiveByte();
		I2C_data[point] = Data;
		point += 1;
		// answer PN532
		if(i < 5){MyI2C_SendAck(0);}else{MyI2C_SendAck(1);}
	}
	MyI2C_Stop();
}


void NFC_init(void){
	MyI2C_init();
}
