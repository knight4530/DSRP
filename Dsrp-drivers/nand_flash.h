/*******************************************************************************
 *
 *
 * Simple SmartFusion2 Microcontroller subsystem (MSS) GPIO example program.
 *
 *
 */
#include "firmware/drivers/mss_gpio/mss_gpio.h"
#include "firmware/CMSIS/system_m2sxxx.h"

/*========================================================================
  Private & MODE functions.
 */
static void nand_init(void);
static void delay(void);
static void Reset_DQ(void);
static void Standby(void);
static void Write_Enable_ON(void);
static void Write_Enable_OFF(void);
static void Read_Enable(void);
static void Write_Protect(void);

/*========================================================================
  NAND-Flash functions.
 */
static void First_Reset(void);
static void Reset(void);
static void Reset_LUN(void);
static void Read_ID(void);
static void Read_ID_ONFI(void);
static void Read_Parameter_Page(void);
static void Read_Unique_ID(void);
static void Get_Features(void);
static void Set_Features(void);
static void Read_Status(void);
static void Read_Status_Enhanced(void);
static void Change_Read_Column(void);
static void Change_Read_Column_Enhanced(void);
static void Change_Write_Column(void);
static void Change_Row_Address(void); //可視為Change_Write_Column_Enhanced
static void Read_Page(void);
static void Read_Page_Multiplane(void);
static void Read_Page_Cache_Sequential(void);
static void Read_Page_Cache_Random(void);
static void Read_Page_Cache_Last(void);
static void Program_Page(void);
static void Program_Page_Multiplane(void);
static void Program_Page_Cache(void);
static void Erase_Block(void);
static void Erase_Block_Multiplane(void);
static void Copyback_Read(void);
static void Copyback_Read_Multiplane(void);
static void Copyback_Program(void);
static void Copyback_Program_Multiplane(void);