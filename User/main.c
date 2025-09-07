#include "stm32f10x.h"                  // Device header

#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"
#include "GPIO_init.h"
#include "Key.h"
#include "Serial.h"
#include "NFC_USART.h"
#include "Buzzer.h"
#include "Store.h"

#include <string.h>
#include <time.h>

#define HOME_PAGE  ((uint16_t)1)
#define MENU_PAGE  ((uint16_t)2)
#define ACTIVATE_PAGE  ((uint16_t)3)
#define READ_PAGE ((uint16_t)4)
#define WRITE_PAGE  ((uint16_t)5)
#define DELETE_PAGE  ((uint16_t)6)

#define READ_PAGE_CHOICE  ((uint16_t)0)
#define WRITE_PAGE_CHOICE  ((uint16_t)1)
#define DELETE_PAGE_CHOICE  ((uint16_t)2)

#define MENU_MAX_NUM ((uint16_t)3)

void home_page(void);
void menu_page(void);
void activate_page(void);
void read_page(void);
void write_page(void);
void delete_page(void);
void data_init(void);
void reverse_menu(void);
void Tran_Hex2Str(char* result_srting, uint8_t* Hex_array, uint16_t Len);


// change page must use
uint16_t status = HOME_PAGE; // in PCB it is MENU page?? another usb to TTL
uint16_t menu_choice = READ_PAGE_CHOICE;
// choice in menu page
uint8_t read_cursor = 0;
uint8_t read_reverse = 0, write_reverse = 0, delete_reverse = 0;
// NFC status
uint8_t NFC_waitting_reading = 1; // in order to send command once a time;


// save data
typedef struct {
	char name[10];
	char card[20];
}Name_Struct;

Name_Struct name_struct[15];
uint16_t name_array_len = 0;


/*--------------------home page--------------------*/
void home_page(void){
	// show the screen 
	MyRTC_gettime();
	
	/*
	X:0-127
	Y:0-63
	Clock image 30*30
	127/2  - 2.5*8 = 43.5
	--------------^-----------------
	|
	|
	|
	|
	|
	|					10:43 :59
	|				2025/10/1(small)
	|	
	*/
	OLED_ShowImage(49, 0, 30, 30, Clock);
	OLED_Printf(40, 32 + 1, OLED_8X16, "%02d:%02d", MyRTC_time[3], MyRTC_time[4]);
	OLED_Printf(80, 32 + 8, OLED_6X8, ":%02d", MyRTC_time[5]);
	OLED_Printf(38, 32 + 22, OLED_6X8, "%04d-%02d-%02d", MyRTC_time[0], MyRTC_time[1], MyRTC_time[2]);
	
	// NFC waiting... at background, if read card, turn to activate page
	if(NFC_Status == 0)
		{	
			OLED_Clear();
			OLED_ShowString(00, 30, "NFC wrong!", OLED_8X16);
		}else{
			
			if(NFC_waitting_reading)
				{
					NFC_ReadIDCard();
					NFC_waitting_reading = 0;
				}
	
			if(RxLen >= 7) // this is a lower decision
				{
				// turn to activate
				status = ACTIVATE_PAGE;
				OLED_Clear();
				}
		}
	// if change the status, the next step is clear the screen, and if have read_NFC command, we should clear the Rxlen(must), RxPacket(suggest),and NFC status
	// push the button will make status in MENU
	if(Button_any_close()){
		//Indicator_light_init();
		status = MENU_PAGE;
		menu_choice = READ_PAGE_CHOICE;
		NFC_waitting_reading = 1;
		NFC_Send_SAM_command();// in order to exit finding card mode
		NFC_Clear_status(); //clear ACK 
		OLED_Clear();
	}
}


/*--------------------menu page--------------------*/
void reverse_menu(void){
	
	if(read_reverse == 1){
		OLED_ClearArea(0, 19, 128, 32);
		OLED_ShowString(0, 20, "READ", OLED_6X8);
		OLED_ShowString(0, 30, "WRITE", OLED_6X8);
		OLED_ShowString(0, 40, "DELETE", OLED_6X8);
		OLED_ReverseArea(0, 19, 128, 10);
	}
	
	if(write_reverse == 1){
		OLED_ClearArea(0, 19, 128, 32);
		OLED_ShowString(0, 20, "READ", OLED_6X8);
		OLED_ShowString(0, 30, "WRITE", OLED_6X8);
		OLED_ShowString(0, 40, "DELETE", OLED_6X8);
		OLED_ReverseArea(0, 29, 128, 10);
	}
	
	if(delete_reverse == 1){
		OLED_ClearArea(0, 19, 128, 32);
		OLED_ShowString(0, 20, "READ", OLED_6X8);
		OLED_ShowString(0, 30, "WRITE", OLED_6X8);
		OLED_ShowString(0, 40, "DELETE", OLED_6X8);
		OLED_ReverseArea(0, 39, 128, 10);
	}
}


void menu_page(void){
	OLED_ShowString(40, 0, "MENU", OLED_8X16);
	switch(menu_choice){
			case READ_PAGE_CHOICE:
				read_reverse = 1;
				write_reverse = 0;
				delete_reverse = 0;
				//OLED_ShowString(60, 60, "**", OLED_6X8);
				//OLED_ShowString(60, 60, "  ", OLED_6X8);
				//OLED_ShowString(60, 60, "  ", OLED_6X8);
				break;
			case WRITE_PAGE_CHOICE:
				read_reverse = 0;
				write_reverse = 1;
				delete_reverse = 0;
				break;
			case DELETE_PAGE_CHOICE:
				read_reverse = 0;
				write_reverse = 0;
				delete_reverse = 1;
				break;
			default:
				break;
		}
	reverse_menu();
	
	//down
	if(Key_check(GPIOA, GPIO_Pin_5) == 1){
		menu_choice += 1;
		menu_choice = menu_choice % MENU_MAX_NUM; // the MENU_MAX_NUN is for extension
	}
	//up
	if(Key_check(GPIOA, GPIO_Pin_4) == 1){
		if(menu_choice == 0){
			menu_choice = MENU_MAX_NUM;
		}
		menu_choice -= 1;
	}
	//Comfirm
	if(Key_check(GPIOA, GPIO_Pin_3)){
		switch(menu_choice){
			case READ_PAGE_CHOICE:
				status = READ_PAGE;
				read_page();
				break;
			case WRITE_PAGE_CHOICE:
				status = WRITE_PAGE;
				write_page();
				break;
			case DELETE_PAGE_CHOICE:
				status = DELETE_PAGE;
				delete_page();
				break;
			default:
				break;
		}
		menu_choice = READ_PAGE_CHOICE; // when change page, back read_page
		OLED_Clear();
	}
	
	// BACK
	if(Key_check(GPIOA, GPIO_Pin_2)){
		status = HOME_PAGE;
		menu_choice = READ_PAGE_CHOICE;
		OLED_Clear();
	}
}


/*--------------------activate page--------------------*/
uint8_t First_time_in_act_page = 1;

void activate_page(void){
	// could add some other function: delay 5s and back...
	if(NFC_Status == 0)
		{
			OLED_ShowString(00, 30, "NFC wrong!", OLED_8X16);
		}
	else
		{
			// GET UID and show
			NFC_GET_UID();
			
			// Tran_Hex2Str
			char result_string[16];
			Tran_Hex2Str(result_string, Card_UID, 6);
			
			// finding in database, but have bug now... can not find the user I write here
			char user_name[10] = {0};
			strcpy(user_name, "Stranger\0");
			uint8_t find_flag = 0; //not used now
			for(int i = 0; i < name_array_len; i++){
				if(strcmp(name_struct[i].card, result_string) == 0)// find the final same card user 
				{
					strcpy(user_name, "\0");
					strcpy(user_name, name_struct[i].name);
					find_flag = 1;
				}	
			}
			OLED_Printf(0, 15, OLED_8X16, "Hello!%s", user_name);
			OLED_Printf(0, 45, OLED_8X16, "ID:%02X%02X%02X%02X%02X%02X", Card_UID[0], Card_UID[1], Card_UID[2], Card_UID[3], Card_UID[4], Card_UID[5]);
			if(First_time_in_act_page == 1){
				Buzzer_on_500ms();
				First_time_in_act_page = 0;
			}
		}
	if(Key_check(GPIOA, GPIO_Pin_2)){
		status = HOME_PAGE;
		menu_choice = READ_PAGE_CHOICE;
		OLED_Clear();
		NFC_waitting_reading = 1;
		NFC_Clear_status();
		First_time_in_act_page = 1;
	}
}


/*--------------------read page--------------------*/
void read_page(void){
	// could add some functions, such as delete one record 
	OLED_ShowString(0, 0, "name", OLED_8X16);
	OLED_ShowString(60, 0, "card", OLED_8X16);

	// kepp the screen roll
	if((Key_check(GPIOA, GPIO_Pin_5) == 1)){
		// -3 is to avoid the null lines
		if(read_cursor < name_array_len - 1 - 3){
			read_cursor += 1;
		}
		OLED_Clear();
	}
	if(Key_check(GPIOA, GPIO_Pin_4) == 1){
		if(read_cursor > 0){
			read_cursor -= 1;
		}
		OLED_Clear();
	}
	
	// show Below 20 Y
	// have memory
	// the X: name is 0 ~ 80;card is 60 ~ 60 + 84(14 * 6).so it will overflow, should change the code
	OLED_ShowString(0, 20, name_struct[0 + read_cursor].name, OLED_6X8);
	OLED_ShowString(60, 20, name_struct[0 + read_cursor].card, OLED_6X8);
	OLED_ShowString(0, 30, name_struct[1 + read_cursor].name, OLED_6X8);
	OLED_ShowString(60, 30, name_struct[1 + read_cursor].card, OLED_6X8);
	OLED_ShowString(0, 40, name_struct[2 + read_cursor].name, OLED_6X8);
	OLED_ShowString(60, 40, name_struct[2 + read_cursor].card, OLED_6X8);
	OLED_ShowString(0, 50, name_struct[3 + read_cursor].name, OLED_6X8);
	OLED_ShowString(60, 50, name_struct[3 + read_cursor].card, OLED_6X8);
	
	// the screen bar len 10 , width 3, roll between 20 ~ 63
	// the screen can show 4, the cursor is between 0 ~ name_array_len - 4
	// when the cursor = 0 , the bar is top 
	// when the cursor = name_array_len - 4, the bar is buttom
	// and the bar is between 20 ~ 50 
	// 20 + cursor / (name_array_len - 4) * 30 is the screen bar Y
	uint16_t Y_top = 20 + (int16_t)((double)read_cursor / (double)(name_array_len - 4) * 30);
	OLED_ClearArea(122, Y_top, 3, 10); // avoid overflower show
	OLED_DrawRectangle(124, Y_top, 3, 10, OLED_FILLED);
	
	if(Key_check(GPIOA, GPIO_Pin_2)){
		status = HOME_PAGE;
		menu_choice = READ_PAGE_CHOICE;
		OLED_Clear();
	}
}


/*--------------------write page--------------------*/
int8_t input_cursor = 0;
uint8_t read_NFC = 0; // have read card or not

char input_char = 'a';
char input_name_array[10];


void write_page_nread_card(void){
		OLED_Clear();
		if(NFC_Status == 0){
			OLED_ShowString(00, 30, "NFC wrong!", OLED_8X16);
		}else{
			
			if(NFC_waitting_reading)
				{
					NFC_ReadIDCard();
					NFC_waitting_reading = 0;
				}
				
			OLED_ShowString(00, 30, "waiting NFC card", OLED_8X16); 	// in middle
			// OLED_ShowNum(30, 50, RxLen,2, OLED_6X8);
			
			while(1){
				if(RxLen >= 7){
					// get UID and turn to register an user
					// OLED_ShowString(00, 30, "GET NFC card", OLED_8X16); // not show
					NFC_waitting_reading = 1;
					read_NFC = 1;
					break;
				}
				if(Key_check(GPIOA, GPIO_Pin_2)){
					status = HOME_PAGE;
					menu_choice = READ_PAGE_CHOICE;
					OLED_Clear();
					NFC_waitting_reading = 1;
					read_NFC = 0;
					NFC_Clear_status();
					break;
				}
				// NFC_ShowRxPacket();
				OLED_Update();
			}
			//OLED_ShowNum(30, 50, RxLen, 2, OLED_6X8);
		}
}


uint8_t First_time_in_write_page = 1;

void write_page_read_card(void){
		// if have information, read_NFC = 1
		OLED_Clear();
		// GET UID
		NFC_GET_UID();
		// Tran_Hex2Str
		char result_string[16];
		Tran_Hex2Str(result_string, Card_UID, 6);
		// record ID
		strcpy(name_struct[name_array_len].card, result_string); 
		
		// record name
		uint8_t exit_flag = 0;
		// HEAD
		OLED_Printf(0, 0, OLED_6X8, "ID:%02X%02X%02X%02X%02X%02X", Card_UID[0], Card_UID[1], Card_UID[2], Card_UID[3], Card_UID[4], Card_UID[5]);
		OLED_ShowString(0, 10, "please input name:", OLED_6X8);
		if(First_time_in_write_page == 1){
			Buzzer_on_500ms();
			First_time_in_write_page = 0;
		}
	
		OLED_Update();
		while (!exit_flag){

			// do not want to record and exit
			if((Key_check(GPIOA, GPIO_Pin_2) == 1)){
				status = HOME_PAGE;
				menu_choice = READ_PAGE_CHOICE;
				OLED_Clear();
				exit_flag = 1;
				read_NFC = 0;
				NFC_Clear_status();
				First_time_in_write_page = 1;
				break;
			}
				
			// change the char, only support a-z,input 'a' - 1 to stop
			if((Key_check(GPIOA, GPIO_Pin_5) == 1)){
				input_char += 1;
				OLED_ClearArea(10 * input_cursor, 30, 8, 16);
				}
			if(Key_check(GPIOA, GPIO_Pin_4) == 1){
				input_char -= 1;
				OLED_ClearArea(10 * input_cursor, 30, 8, 16);
				}
			if(Key_check(GPIOA, GPIO_Pin_3) == 1){
				input_name_array[input_cursor] = input_char;
				input_cursor += 1;
				if(input_cursor > 0){OLED_ReverseArea(10 * (input_cursor - 1), 30, 8, 16);}				// reverse the front char
				
				// finish
				if(input_char == 'a' - 1){
					input_name_array[input_cursor - 1] = '\0';
					strcpy(name_struct[name_array_len].name, input_name_array);
					name_array_len += 1;
					Store_Save_name_struct();
					
					// init again
					input_cursor = 0;
					input_char = 'a';
					strcpy(input_name_array, "\0");
					
					status = MENU_PAGE;
					menu_choice = READ_PAGE_CHOICE;
					exit_flag = 1;
					read_NFC = 0;
					NFC_Clear_status();
					OLED_Clear();
					First_time_in_write_page = 1;
					break; 
				}	
				input_char = 'a';
			}
			OLED_ShowChar(10 * input_cursor, 30, input_char, OLED_8X16);
			OLED_ReverseArea(10 * input_cursor, 30, 8, 16);
			OLED_Update();
		}
}


void write_page(void){
	/* write the information into name_array */
	// write function can't avoid the same card now, can achieve it in Flash.c or Database.c
	
	// waiting the NFC information
	if(!read_NFC){
		write_page_nread_card();
	}else{
		write_page_read_card();
	}

	if(Key_check(GPIOA, GPIO_Pin_2)){
		status = HOME_PAGE;
		menu_choice = READ_PAGE_CHOICE;
		OLED_Clear();
		// clean the read_NFC and RxPacket
		read_NFC = 0;
		NFC_waitting_reading = 1; // confirm angain;
		NFC_Clear_status();
	}

}


/*--------------------delete page--------------------*/
uint8_t delete_cursor = 0; // use to select the choice, point at reverse(choose) data
uint8_t rolling_cursor = 0; // use to show the data, point at first show data


void delete_confirm_page(void){
	OLED_Clear();
	
	// name: xxxx
	// card: xxxx
	// Are you sure to delete?
	// back     confirm        
	OLED_ShowString(0, 0, "name:", OLED_6X8);
	OLED_ShowString(0, 10, "card:", OLED_6X8);
	
	OLED_ShowString(40, 0, name_struct[delete_cursor].name, OLED_6X8);
	OLED_ShowString(40, 10, name_struct[delete_cursor].card, OLED_6X8);
	
	OLED_ShowString(0, 25, "sure to delete?", OLED_6X8);
	
	OLED_ShowString(0, 45, "back", OLED_8X16);
	OLED_ReverseArea(0, 45, 32, 61);
	OLED_ShowString(71, 45, "confirm", OLED_8X16);
	OLED_ReverseArea(71, 45, 127, 61);
	
	OLED_Update();
	
	while(1){
		if(Key_check(GPIOA, GPIO_Pin_2) == 1){
			status = HOME_PAGE;
			menu_choice = READ_PAGE_CHOICE;
			OLED_Clear();
			delete_cursor = 0;
			rolling_cursor = 0;
			break;
		}
	}
}


void delete_page(void){
	/*
	when rolling down the page , first go to buttom of screen, then go down
	when rolling down the page , first go to top of screen, then go up
	*/
	
	// if name_array_len = 7
	// del_cur = 0  then  del_cur = 3  then del_cur = 4  then del_cur = 6 
	// rol_cur = 0        rol_cur = 0   		rol_cur = 1	 ...	rol_cur = 3
	// so the rol_cur in [del_cur, del_cur - 3] 
	// so the del_cur in [rol_cur, rol_cur + 3]
	if((Key_check(GPIOA, GPIO_Pin_5) == 1)){
		
		if(delete_cursor < name_array_len - 1){
			delete_cursor += 1;
			if(delete_cursor > rolling_cursor + 3)
			{
				rolling_cursor += 1;
			}
		}
		
		OLED_Clear();
	}
	
	// del_cur = 6  then del_cur = 3  then del_cur = 0 
	// rol_cur = 3   		 rol_cur = 3	...	 rol_cur = 0
	if(Key_check(GPIOA, GPIO_Pin_4) == 1){

		if(delete_cursor > 0){
			delete_cursor -= 1;
			if(delete_cursor < rolling_cursor)
			{
				rolling_cursor -= 1;
			}
		}
		
		OLED_Clear();
	}
	OLED_Clear();
	
	OLED_ShowString(0, 0, "name", OLED_8X16);
	OLED_ShowString(60, 0, "card", OLED_8X16);
	
	OLED_ShowString(0, 20, name_struct[0 + rolling_cursor].name, OLED_6X8);
	OLED_ShowString(60, 20, name_struct[0 + rolling_cursor].card, OLED_6X8);
	OLED_ShowString(0, 30, name_struct[1 + rolling_cursor].name, OLED_6X8);
	OLED_ShowString(60, 30, name_struct[1 + rolling_cursor].card, OLED_6X8);
	OLED_ShowString(0, 40, name_struct[2 + rolling_cursor].name, OLED_6X8);
	OLED_ShowString(60, 40, name_struct[2 + rolling_cursor].card, OLED_6X8);
	OLED_ShowString(0, 50, name_struct[3 + rolling_cursor].name, OLED_6X8);
	OLED_ShowString(60, 50, name_struct[3 + rolling_cursor].card, OLED_6X8);
	
	// the location is del_cur - rol_cur~[0,3]
	OLED_ReverseArea(0, 20 - 1 + (delete_cursor - rolling_cursor) * 10, 127, 10);
	
	if(Key_check(GPIOA, GPIO_Pin_3)){
		delete_confirm_page();
	}
	
	if(Key_check(GPIOA, GPIO_Pin_2)){
		status = HOME_PAGE;
		menu_choice = READ_PAGE_CHOICE;
		delete_cursor = 0;
		rolling_cursor = 0;
		OLED_Clear();
	}
	
}


/*--------------------system function--------------------*/
void data_init(void){
	Store_read_name_struct(); // get name_array_len and Data
	/*
	strcpy(name_struct[0].name, "jeffrey\0");
	strcpy(name_struct[0].card, "0424D34F592F\0");
	
	strcpy(name_struct[1].name, "MuSeet\0");
	strcpy(name_struct[1].card, "1\0");
	
	strcpy(name_struct[2].name, "e\0");
	strcpy(name_struct[2].card, "2\0");
	
	strcpy(name_struct[3].name, "ff\0");
	strcpy(name_struct[3].card, "3\0");
	
	strcpy(name_struct[4].name, "rey\0");
	strcpy(name_struct[4].card, "4\0");
	
	strcpy(name_struct[5].name, "jeffre2\0");
	strcpy(name_struct[5].card, "123\0");
	
	strcpy(name_struct[6].name, "jefrey\0");
	strcpy(name_struct[6].card, "123\0");
	*/
}


// maybe this function is wrong, but will not effect thr result, and could be a secret
void Tran_Hex2Str(char* result_srting, uint8_t* Hex_array, uint16_t Len)
{
	uint8_t one_num = 0;
	for(int i = 0; i < Len; i++)
	{
		
		one_num = Hex_array[i] / 16;
		if(one_num < 10)
		{
			result_srting[2 * i] = one_num + '0';
		}
		else{
			result_srting[2 * i] = one_num + 'A' - 10;
		}
		
		one_num = Hex_array[i] % 16;
		if(one_num < 10)
		{
			result_srting[2 * i + 1] = one_num + '0';
		}
		else{
			result_srting[2* i + 1] = one_num + 'A' - 10;
		}
	}
	result_srting[2 * Len] = '\0';
}


// add songer and LED program
int main(void)
{
	Store_init();
	data_init();
	OLED_Init();
	MyRTC_init();
	Key_init();
	NFC_USART_init();
	Buzzer_init();
	
	
	while(1){
		switch(status){
			case HOME_PAGE:
				home_page();
				break;
			case MENU_PAGE:
				menu_page();
				break;
			case ACTIVATE_PAGE:
				activate_page();
				break;
			case READ_PAGE:
				read_page();
				break;
			case WRITE_PAGE:
				write_page();
				break;
			case DELETE_PAGE:
				delete_page();
				break;
			default:
				break;
		}
		
		OLED_Update();
		Indicator_light_init();
	}
}



// #include "I2C.h"
// #include "NFC.h"
	/*
	// I2C test
	
	MyI2C_init();
	MyI2C_Start();
	MyI2C_SendByte(0x48);
	
	Delay_ms(1);
	//MyI2C_W_SCL(1);
	//while(MyI2C_R_SCL() == 0);
	
	uint8_t Ack = MyI2C_ReceiveAck();//it seem like waiting for NFC wake
	MyI2C_Stop();
	
	OLED_ShowNum(60, 30, Ack, 4 ,OLED_8X16);
	
	OLED_Update();
	
	//
	
	NFC_init();
	//NFC_information_frame(0x02,0xFE,0x02,0x2A);

	
	NFC_Read_ACK();
	OLED_ShowNum(30, 30, inter_ack, 4, OLED_6X8);

	for(int i = 0; i < 6; i++){
		OLED_ShowHexNum(20*i, 0, I2C_data[i], 2, OLED_6X8);
	}
	
	OLED_Update();
	return 1;
	*/



