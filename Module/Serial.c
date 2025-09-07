#include "stm32f10x.h"  // Device header
#include "GPIO_init.h"


uint8_t RxLen = 0;
// uint8_t Serial_TxPacket[40];
uint8_t Serial_RxPacket[40];
// uint8_t Serial_RxFlag = 0;
// uint16_t pRxPacket = 0;


// it seem like teh USART3 can not receive data
void Serial_init(void){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART1, &USART_InitStructure);
	
		//USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
		
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_Init(&NVIC_InitStructure);
		
		USART_Cmd(USART1, ENABLE);
}


void Serial_SendByte(uint8_t Byte){
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}


void Serial_SendArray(uint8_t *Array, uint16_t Length){
	for(int i = 0; i < Length; i++){
		Serial_SendByte(Array[i]); 
	}
}

/*
uint8_t Serial_GetRxFlag(void){
	 if (Serial_RxFlag == 1){
			Serial_RxFlag = 0;
		  return 1;
	 }
	 return 0;
}
*/


/*
void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    DMA_DeInit(DMA1_Channel3);  

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;   
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Serial_RxPacket;         
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                
    DMA_InitStructure.DMA_BufferSize = 40;                  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                  
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel3, ENABLE);
}
*/


void USART1_IRQHandler(void){
     
   uint16_t RxData;
	
  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET){
    RxData = USART_ReceiveData(USART1);
		Serial_RxPacket[RxLen ++ ] = (uint8_t)RxData;
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
}

