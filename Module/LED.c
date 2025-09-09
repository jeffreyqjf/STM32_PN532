#include "stm32f10x.h"                  // Device header
#include "GPIO_init.h"


//PB0, PB1
void LED_init(void){
	GPIOB_init(GPIO_Mode_Out_PP, GPIO_Pin_0 | GPIO_Pin_1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
}


void LED_Turn_ON_Pin_Green(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
}


void LED_Turn_OFF_Pin_Green(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
}


void LED_Turn_ON_Pin_Red(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
}


void LED_Turn_OFF_Pin_Red(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
}


void LED_R_ON_G_OFF(void){
	LED_Turn_ON_Pin_Red();
	LED_Turn_OFF_Pin_Green();
}


void LED_R_OFF_G_ON(void){
	LED_Turn_OFF_Pin_Red();
	LED_Turn_ON_Pin_Green();
}


void LED_ALL_ON(void){
	LED_Turn_ON_Pin_Red();
	LED_Turn_ON_Pin_Green();
}


void LED_ALL_OFF(void){
	LED_Turn_OFF_Pin_Red();
	LED_Turn_OFF_Pin_Green();
}
