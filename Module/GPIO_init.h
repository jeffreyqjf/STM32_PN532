#ifndef __GPIO_H
#define __GPIO_H
      
void GPIOA_init(GPIOMode_TypeDef GPIO_Mode, uint16_t GPIO_Pin_x);
void GPIOB_init(GPIOMode_TypeDef GPIO_Mode, uint16_t GPIO_Pin_x);
void GPIOC_init(GPIOMode_TypeDef GPIO_Mode, uint16_t GPIO_Pin_x);
void Indicator_light_init(void);
// void NFC_GPIO_init(void);
// uint8_t Button_all_close(void);
uint8_t Button_any_close(void);

#endif
