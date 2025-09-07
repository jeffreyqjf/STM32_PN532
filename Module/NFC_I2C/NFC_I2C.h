#ifndef __NFC_I2C_H
#define __NFC_I2C_H

void NFC_information_frame(uint8_t LEN, uint8_t LCS, uint8_t PDn, uint8_t DCS);
void NFC_ACK_frame(void);
void NFC_NACK_frame(void);
void NFC_Error_frame(void);
extern uint8_t I2C_data[40];
extern uint8_t point;
extern uint8_t Data;
extern uint8_t inter_ack;
void NFC_Read_ACK(void);
void NFC_init(void);


#endif
