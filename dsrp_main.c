/*******************************************************************************
 *
 *
 * Simple SmartFusion2 Microcontroller subsystem (MSS) GPIO example program.
 *
 *
 */
#include <stdio.h>
#include "Dsrp-drivers/nand_flash.h"
#include "firmware/drivers/mss_gpio/mss_gpio.h"
#include "firmware/CMSIS/system_m2sxxx.h"

#define NOOP                     1
#define RESET_CC                 2
#define HK_Request               3
#define Time_Code                4
#define User_Data_Request        5
#define RADFET_ON                6
#define RADFET_Change_Freq       7

/*========================================================================
 * main() function.
 */
void ROUTINE_DATA(void);
void CHECK_NAND(void);
void ISPACE_COM(void);

/*========================================================================
 * Variables & Parameters.
 */
//Initial
	uint8_t RADFET = 0;
	uint8_t RADFET_flag = 0;
	uint8_t CRC_ERROR = 0;
	uint8_t COM = 0;
	uint8_t check_address[5] = 0x00;

//Initial HK
	uint8_t HK_telemetry = 0x11;
	uint16_t Data_Size = 0;
	uint8_t Com_Count_Receive = 0;
	uint8_t Com_Count_Error = 0;
	uint8_t Com_Count_Accept = 0;
	uint8_t Com_Last_Accept = 0;
	uint8_t Com_Last_Error = 0;
	double Time = 0;  //SCI的time是uint16? //用double正確?
	uint8_t Protocol_ID = 0;
	uint32_t Beacon_PKT_Num;
	uint8_t Reboot_Count = 0;
	uint8_t Status = 0;
	uint16_t Sample_Rate = 0;
	//HK Write Pointer
	//SCI Write Pointer
	int16_t DCDC5_V = 0;
	int16_t DCDC5_I = 0;
	int16_t DCDC12_V = 0;
	int16_t DCDC12_I = 0;
	uint8_t Health = 0;
	int8_t Temp = 0;  //可能要改成int16，int8最多只有127度
	uint16_t CRC = 0;
	
//Initial SCI
	int16_t RADFET_R1 = 0;
	int16_t RADFET_R2 = 0;
	int16_t RADFET_Diode = 0;
	uint8_t SEU_Count;  //怕不夠用，改成uint16?

//NAND FUNCTION使用
	/*NAND-Flash Basic
		First_Reset();Read_ID();Read_ID_ONFI();
		Read_Parameter_Page();Reset();*/

	/*READ
		First_Reset();Read_Page_Multiplane();
		Read_Page();Read_Page_Cache_Sequential();
		Read_Page_Cache_Random();Read_Page_Cache_Last();*/

	/*WRITE
		First_Reset();Rrogram_Page_Multiplane();
		Rrogram_Page();Rrogram_Page_Cache();Read_Page();*/

	/*ERASE
		First_Reset();Erase_Block();Read_Page();*/

/*========================================================================
 * Initialize & MAIN
 */
nand_init(); //GPIO Initialize
First_Reset(); //NAND-Flash啟用，確定是否後續每次用都要重新reset?
ROUTINE_DATA(); //直接進ROUTINE_DATA

void ROUTINE_DATA(void){
	//生成各HK資料???
	//將資料整理成PKT，或是留到收到COM再做也可以
	if(RADFET == 1 && RADFET_flag == 1){
		//讀RADFET->ADC  //讀GPIO值
		//將讀到的GPIO值存到矩陣
		//將矩陣轉換回GPIO值
		Program_Page();  //寫入NAND
	}
	
	if(/*收到COMMAND*/){ //用get_rx？
		ISPACE_COM(); //進'ISPACE Command'
	}
	else CHECK_NAND(); //未收到就進CHECK_NAND
}
	
void CHECK_NAND(void){
	Read_Page(); //從check_address的目前address開始做
	//將讀到的GPIO值存到矩陣
	//計算其值的CRC是否有錯
	
	if(CRC_ERROR == 1){ //若CRC有錯
		SEU_Count = SEU_Count + 1;  //SEU_Count+1
		Rrogram_Page();  //將新的CRC重新寫入
	}  //若CRC沒錯就繼續往下
	
	if(check_address == ){  //若到了check_address end
		Rrogram_Page();  //將當下SEU_Count寫入
		SEU_Count = 0;  //reset SEU_Count
		check_address[0] = 0x00; //reset check_address
		check_address[1] = 0x00;
		check_address[2] = 0x00;
		check_address[3] = 0x00;
		check_address[4] = 0x00;
	}
	else{
		check_address[0] = 0x00;
		check_address[1] = 0x00;
		check_address[2] = 0x00;
		check_address[3] = 0x00;
		check_address[4] = 0x00;
	}  //若還沒到check_address end，則check_address+1
	
	if(/*收到COMMAND*/){ //用get_rx？
		ISPACE_COM(); //進'ISPACE Command'
	}
	else CHECK_NAND(); //未收到就進CHECK_NAND
}
	
void ISPACE_COM(void){
	//先判斷COM為哪個指令
	switch(COM){
		case NOOP:
			break;
		case RESET_CC:					
			break;
		case HK_Request:
			break;
		case Time_Code:
			break;
		case User_Data_Request:
			break;
		case RADFET_ON:
			break;
		case RADFET_Change_Freq:
			break;
		default: 
			break;
	}
	ROUTINE_DATA(); //做完就回ROUTINE_DATA
}

//一些問題
	//RADFET讀取頻率，未讀取時是否有輸出，還是未讀取時會維持前一次讀取的資料?
	//是否要讓COMMAND弄成interupt，若沒有interupt是否會有太晚回復COMMAND的風險?