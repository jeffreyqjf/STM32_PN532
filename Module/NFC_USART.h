#ifndef __NFC_USART_H
#define __NFC_USART_H

extern uint8_t NFC_Status;
extern uint8_t Card_UID[7];
void NFC_Clear_status(void);
void NFC_USART_init(void);
void NFC_ReadIDCard(void);
void NFC_ShowRxPacket(void);
void NFC_Send_ACK(void);
uint8_t* NFC_GET_UID(void);
void NFC_Send_SAM_command(void);
#endif
