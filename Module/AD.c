#include "stm32f10x.h"                  // Device header
#include "GPIO_init.h"
#include "OLED.h"

void AD_init(void){
		
	// GPIO init
	GPIOA_init(GPIO_Mode_AIN, GPIO_Pin_2);
	
	// ADCCLK init
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	// ADC init
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	// why ADC do not have to init NVIC?
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
}


uint16_t Get_AD_Vaule(void){
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	// while( ADC_GetSoftwareStartConvStatus(ADC1) == SET);
	OLED_ShowNum(1, 1, ADC_GetConversionValue(ADC1), 6, OLED_6X8);
	return ADC_GetConversionValue(ADC1);
}
