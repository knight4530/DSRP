/*******************************************************************************
 *
 *
 * Simple SmartFusion2 Microcontroller subsystem (MSS) GPIO example program.
 *
 *
 */
#include "nand_flash.h"
#include "firmware/drivers/mss_gpio/mss_gpio.h"
#include "firmware/CMSIS/system_m2sxxx.h"

/*========================================================================
  Private & MODE functions.
 */
static void nand_init(void){ //Initialize & Configure MSS GPIOs.
	MSS_GPIO_init();
	MSS_GPIO_config( MSS_GPIO_0 , MSS_GPIO_OUTPUT_MODE );  //ALE
	MSS_GPIO_config( MSS_GPIO_1 , MSS_GPIO_OUTPUT_MODE );  //CE#
	MSS_GPIO_config( MSS_GPIO_2 , MSS_GPIO_OUTPUT_MODE );  //CE#2
	MSS_GPIO_config( MSS_GPIO_3 , MSS_GPIO_OUTPUT_MODE );  //CLE
	MSS_GPIO_config( MSS_GPIO_4 , MSS_GPIO_OUTPUT_MODE );  //WP#
	MSS_GPIO_config( MSS_GPIO_8 , MSS_GPIO_OUTPUT_MODE );  //RE#
	MSS_GPIO_config( MSS_GPIO_9 , MSS_GPIO_OUTPUT_MODE );  //WE#
	MSS_GPIO_config( MSS_GPIO_18 , MSS_GPIO_INOUT_MODE );  //DQ7
	MSS_GPIO_config( MSS_GPIO_17 , MSS_GPIO_INOUT_MODE );  //DQ6
	MSS_GPIO_config( MSS_GPIO_16 , MSS_GPIO_INOUT_MODE );  //DQ5
	MSS_GPIO_config( MSS_GPIO_15 , MSS_GPIO_INOUT_MODE );  //DQ4
	MSS_GPIO_config( MSS_GPIO_14 , MSS_GPIO_INOUT_MODE );  //DQ3
	MSS_GPIO_config( MSS_GPIO_13 , MSS_GPIO_INOUT_MODE );  //DQ2
	MSS_GPIO_config( MSS_GPIO_12 , MSS_GPIO_INOUT_MODE );  //DQ1
	MSS_GPIO_config( MSS_GPIO_11 , MSS_GPIO_INOUT_MODE );  //DQ0
	MSS_GPIO_config( MSS_GPIO_23 , MSS_GPIO_INPUT_MODE );  //R/B#
	MSS_GPIO_config( MSS_GPIO_24 , MSS_GPIO_INPUT_MODE );  //R/B#2
}

static void delay(void){ //一個delay約為10us；一個訊號間距1.2us
    volatile uint32_t delay_count = SystemCoreClock/2050000;//count48
    while(delay_count > 0u)
    {
        --delay_count;
    }
}
//1Delay/FOR-1-Delay/FOR-2-Delay/FOR-5-Delay/FOR-10-Delay
//10.04/10.80/19.74/46.50/91.42
//10.02/10.80/19.74/46.48/91.36
//10.02/10.78/19.74/46.48/91.36
//10.02/10.80/19.76/46.50/91.46
//10.02/10.78/19.74/46.46/91.32

static void Reset_DQ(void){
	delay();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_HIGH_Z );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_HIGH_Z );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_HIGH_Z );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_HIGH_Z );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_HIGH_Z );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_HIGH_Z );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_HIGH_Z );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_HIGH_Z );
}

static void Standby(void){ //P.32
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_1, 0xFF );
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_2, 0xFF );
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_8, 0xFF );
	MSS_GPIO_set_output( MSS_GPIO_9, 0xFF );
	MSS_GPIO_set_output( MSS_GPIO_4, 0xFF );
}

static void Write_Enable_ON(void){ //P.32
	MSS_GPIO_set_output( MSS_GPIO_9, 0x00 );
}

static void Write_Enable_OFF(void){ //P.32
	MSS_GPIO_set_output( MSS_GPIO_9, 0xFF );
}

static void Read_Enable(void){ //P.32
	MSS_GPIO_set_output( MSS_GPIO_8, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_8, 0xFF );
}

static void Write_Protect(void){ //P.32
	MSS_GPIO_set_output( MSS_GPIO_4, 0x00 );
}

/*========================================================================
  NAND-Flash Function
 */
static void First_Reset(void){ //P.133
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Command FFh
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	for(int i=0; i<100; i++)delay();
	Standby();
}

static void Reset(void){ //P.133
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Command FFh
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	for(int i=0; i<10; i++)delay();
	Standby();
}

static void Reset_LUN(void){ //P.133
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Command FAh
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 3~5 Cycle
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	for(int i=0; i<10; i++)delay();
	Standby();
}

static void Read_ID(void){ //P.140
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 90h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//Address 00h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	for(int i=0; i<8; i++)Read_Enable();
	Standby();
}

static void Read_ID_ONFI(void){ //P.140
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 90h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//Address 20h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	for(int i=0; i<5; i++)Read_Enable();
	Standby();
}

static void Read_Parameter_Page(void){ //P.135
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command ECh
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//Address 00h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	for(int i=0; i<3; i++)delay();
	for(int i=0; i<767; i++)Read_Enable();
	Standby();
}

static void Read_Unique_ID(void){ //P.72
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command EDh
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//Address 00h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	for(int i=0; i<3; i++)delay();
	for(int i=0; i<32; i++)Read_Enable();
	Standby();
}

static void Get_Features(void){ //P.146
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command EEh
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//Address
	//01h:Timing mode
	//10h,80h:Programmable output drive strength
	//81h:Programmable RB# pull-down strength
	//90h:Array operation mode
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	for(int i=0; i<4; i++)Read_Enable();
	Standby();
}

static void Set_Features(void){ //P.144
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command EFh
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//Address
	//01:Timing mode
	//10,80:Programmable output drive strength
	//81:Programmable RB# pull-down strength
	//90:Array operation mode
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Parameters 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	//Parameters 2
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Parameters 3
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Parameters 4
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Standby();
}

static void Read_Status(void){ //P.134
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 70h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	Read_Enable();
	Standby();
}

static void Read_Status_Enhanced(void){ //P.134
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 78h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 3~5 Cycle
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	Read_Enable();
	Standby();
}

static void Change_Read_Column(void){ //P.137
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 05h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~2 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 E0h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	//Dout自定義
	for(int i=0; i<1; i++)Read_Enable();
	Standby();
}

static void Change_Read_Column_Enhanced(void){ //P.82
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 06h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 E0h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	//Dout自定義
	for(int i=0; i<1; i++)Read_Enable();
	Standby();
}

static void Change_Write_Column(void){ //P.159
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 85h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~2 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Din自定義
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Standby();
}

static void Change_Row_Address(void){ //P.84
	//可視為Change_Write_Column_Enhanced
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 06h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Din自定義
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Standby();
}

//static void Read(void){}

static void Read_Page(void){ //P.135
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 00h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 30h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<4; i++)delay();
	for(int i=0; i<8640; i++)Read_Enable();
	Standby();
}

static void Read_Page_Multiplane(void){ //P.156
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 00h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 32h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Standby();
}

static void Read_Page_Cache_Sequential(void){ //P.138
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 31h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<4; i++)delay();
	for(int i=0; i<8640; i++)Read_Enable();
	Standby();
}

static void Read_Page_Cache_Random(void){ //P.139
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 00h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 31h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<4; i++)delay();
	for(int i=0; i<8640; i++)Read_Enable();
	Standby();
}

static void Read_Page_Cache_Last(void){
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 3Fh
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<4; i++)delay();
	for(int i=0; i<8640; i++)Read_Enable();
	Standby();
}

static void Program_Page(void){ //P.140
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 80h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Column Address 2
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Din自定義
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 10h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<60; i++)delay();
	Standby();
}

static void Program_Page_Multiplane(void){ //P.160
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 80h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Column Address 2
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Din自定義
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 11h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<60; i++)delay();
	Standby();
}

static void Program_Page_Cache(void){ //P.142
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 80h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Column Address 2
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Din自定義
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 15h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<60; i++)delay();
	Standby();
}

static void Erase_Block(void){
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 60h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 3~5 Cycle
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 D0h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<700; i++)delay();
	Standby();
}

static void Erase_Block_Multiplane(void){
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 60h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 3~5 Cycle
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 D1h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	Standby();
}

static void Copyback_Read(void){  //P.143
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 00h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 35h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<4; i++)delay();
	for(int i=0; i<8640; i++)Read_Enable();
	Standby();
}

static void Copyback_Read_Multiplane(void){ //P.156
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 00h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 32h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Standby();
}

static void Copyback_Program(void){
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 85h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Din自定義
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 10h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<60; i++)delay();
	Standby();
}

static void Copyback_Program_Multiplane(void){
	MSS_GPIO_set_output( MSS_GPIO_1, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command 85h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	MSS_GPIO_set_output( MSS_GPIO_0, 0xFF );
	//輸入 Address 的 1~5 Cycle
	//Column Address 1
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	Write_Enable_ON();
	//Column Address 2
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 1 (Plane & Page)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 2 (Block)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	//Row Address 3 (Block & LUN)
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_0, 0x00 );
	//Din自定義
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_LOW );
	Write_Enable_OFF();
	MSS_GPIO_set_output( MSS_GPIO_3, 0xFF );
	//Command2 11h
	Write_Enable_ON();
	MSS_GPIO_drive_inout( MSS_GPIO_18, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_17, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_16, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_15, MSS_GPIO_DRIVE_HIGH );
	MSS_GPIO_drive_inout( MSS_GPIO_14, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_13, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_12, MSS_GPIO_DRIVE_LOW );
	MSS_GPIO_drive_inout( MSS_GPIO_11, MSS_GPIO_DRIVE_HIGH );
	Write_Enable_OFF();
	Reset_DQ();
	MSS_GPIO_set_output( MSS_GPIO_3, 0x00 );
	for(int i=0; i<60; i++)delay();
	Standby();
}
