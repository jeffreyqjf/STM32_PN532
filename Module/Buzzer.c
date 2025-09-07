#include "stm32f10x.h"                  // Device header
#include "GPIO_init.h"
#include "Delay.h"

#define BUZZER_PIN   	GPIO_Pin_11
#define BUZZER_GPIOx  GPIOB

void Buzzer_init(void){
	// still need change the function if change the GPIOx
	GPIOB_init(GPIO_Mode_Out_PP, BUZZER_PIN);
	GPIO_WriteBit(BUZZER_GPIOx, BUZZER_PIN, Bit_SET);
}


void Buzzer_ON(void){
	GPIO_WriteBit(GPIOB, BUZZER_PIN, Bit_RESET);
}


void Buzzer_OFF(void){
	GPIO_WriteBit(GPIOB, BUZZER_PIN, Bit_SET);
}


void Buzzer_on_500ms(void){
	Buzzer_ON();
	Delay_ms(500);
	Buzzer_OFF();
}
