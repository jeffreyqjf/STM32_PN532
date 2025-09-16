#include "stm32f10x.h"                  // Device header
#include "math.h"
#include "OLED.h"
#include "Delay.h"

void Interrupt_GPIOA_init(uint16_t x){
	//  use GPIO_Pin_x as EXIT
	
    double re = pow((double) 2.0, (double) x);
    uint16_t tmp = (uint16_t) re;
	uint16_t EXTIx_IRQn = 0;
	
	/*
	EXTI0_IRQn                        
    EXTI1_IRQn
    EXTI2_IRQn                       
    EXTI3_IRQn                       
    EXTI4_IRQn     
	EXTI15_10_IRQn
	EXTI9_5_IRQn
	*/
	switch(x){
		case 0:
			EXTIx_IRQn = EXTI0_IRQn;
			break;
		case 1:
			EXTIx_IRQn = EXTI1_IRQn;
			break;
		case 2:
			EXTIx_IRQn = EXTI2_IRQn;
			break;
		case 3:
			EXTIx_IRQn = EXTI3_IRQn;
			break;
		case 4:
			EXTIx_IRQn = EXTI4_IRQn;
			break;
		default:
			if(x <= 9 && x >= 5){
				EXTIx_IRQn = EXTI9_5_IRQn;
			}else if(x <= 15 && x>= 10){
				EXTIx_IRQn = EXTI15_10_IRQn;
			}	
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // AFPO is used for EXIT
	
	// GPIO init
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 * tmp; // GPIO_Pin_x = GPIO_Pin_0 * 2**x
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0 + x); // PinSourcex = Pinsource0 + x
	
	// EXIT init
	EXTI_InitTypeDef EXIT_InitStructure;
	EXIT_InitStructure.EXTI_Line = EXTI_Line0 * tmp; // Linex = Line0 * 2**x  
	EXIT_InitStructure.EXTI_LineCmd = ENABLE;
	EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXIT_InitStructure);
	
	// NVIC init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}


void Interrupt_GPIOB_init(uint16_t x){
	// GPIOx
	
	double re = pow((double) 2.0, (double) x);
	uint16_t tmp = (uint16_t) re;
	uint16_t EXTIx_IRQn = 0;
	
	switch(x){
		case 0:
			EXTIx_IRQn = EXTI0_IRQn;
			break;
		case 1:
			EXTIx_IRQn = EXTI1_IRQn;
			break;
		case 2:
			EXTIx_IRQn = EXTI2_IRQn;
			break;
		case 3:
			EXTIx_IRQn = EXTI3_IRQn;
			break;
		case 4:
			EXTIx_IRQn = EXTI4_IRQn;
			break;
		default:
			if(x <= 9 && x >= 5){
				EXTIx_IRQn = EXTI9_5_IRQn;
			}else if(x <= 15 && x>= 10){
				EXTIx_IRQn = EXTI15_10_IRQn;
			}
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 * tmp;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0 + x);

	
	EXTI_InitTypeDef EXIT_InitStructure;
	EXIT_InitStructure.EXTI_Line = EXTI_Line0 * tmp;
	EXIT_InitStructure.EXTI_LineCmd = ENABLE;
	EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXIT_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}


void Interrupt_GPIOC_init(uint16_t x){
	// GPIOx
	
	double re = pow((double) 2.0, (double) x);
	uint16_t tmp = (uint16_t) re;
	uint16_t EXTIx_IRQn = 0;
	
	switch(x){
		case 0:
			EXTIx_IRQn = EXTI0_IRQn;
			break;
		case 1:
			EXTIx_IRQn = EXTI1_IRQn;
			break;
		case 2:
			EXTIx_IRQn = EXTI2_IRQn;
			break;
		case 3:
			EXTIx_IRQn = EXTI3_IRQn;
			break;
		case 4:
			EXTIx_IRQn = EXTI4_IRQn;
			break;
		default:
			if(x <= 9 && x >= 5){
				EXTIx_IRQn = EXTI9_5_IRQn;
			}else if(x <= 15 && x>= 10){
				EXTIx_IRQn = EXTI15_10_IRQn;
			}	
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 * tmp;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0 + x);
	
	EXTI_InitTypeDef EXIT_InitStructure;
	EXIT_InitStructure.EXTI_Line = EXTI_Line0 * tmp;
	EXIT_InitStructure.EXTI_LineCmd = ENABLE;
	EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXIT_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}
