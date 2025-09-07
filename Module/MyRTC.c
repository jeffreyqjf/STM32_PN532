#include "stm32f10x.h"                  // Device header
#include <time.h>

void MyRTC_settime(void);
void MyRTC_gettime(void);

uint16_t MyRTC_time[] = {2025, 8, 20, 23, 59, 59};

void MyRTC_init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	// keep time going on
	if(BKP_ReadBackupRegister(BKP_DR1) != 0x1111){
		
		// make sure the LSE is already
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		
		RTC_SetPrescaler(32768 - 1);
		RTC_WaitForLastTask();
		
		MyRTC_settime();
		BKP_WriteBackupRegister(BKP_DR1, 0x1111);
	}else{
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
}


void MyRTC_settime(void){
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = MyRTC_time[0] - 1900;
	time_date.tm_mon = MyRTC_time[1] -1;
	time_date.tm_mday = MyRTC_time[2];
	time_date.tm_hour = MyRTC_time[3];
	time_date.tm_min = MyRTC_time[4];
	time_date.tm_sec = MyRTC_time[5];
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();
}

void MyRTC_gettime(void){
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;
	time_date = *localtime(&time_cnt);
	
	MyRTC_time[0] = time_date.tm_year + 1900;
	MyRTC_time[1] = time_date.tm_mon + 1;
	MyRTC_time[2] = time_date.tm_mday;
	MyRTC_time[3] = time_date.tm_hour;
	MyRTC_time[4] = time_date.tm_min;
	MyRTC_time[5] = time_date.tm_sec;

}
