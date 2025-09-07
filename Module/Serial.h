#ifndef __SERIAL_H
#define __SERIAL_H

extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];
extern uint16_t pRxPacket;
extern uint8_t RxLen;

void Serial_init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
// uint8_t Serial_GetRxFlag(void);
// void DMA_Config(void);


#endif
