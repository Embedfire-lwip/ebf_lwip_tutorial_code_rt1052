/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   液晶显示英文
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include <stdio.h>

#include "fsl_debug_console.h"
#include "fsl_elcdif.h"


#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "./delay/core_delay.h"   

#include "./led/bsp_led.h" 


extern int lwip_ping_test(void);

/*******************************************************************
 * Prototypes
 *******************************************************************/
#define SDRAM_TEST_FAIL  0
#define SDRAM_TEST_OK    1

#define ENET_TEST_FAIL   0
#define ENET_TEST_OK     1

/*******************************************************************
 * Variables
 *******************************************************************/

/*******************************************************************
 * Code
 *******************************************************************/

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{ 
		uint8_t result1,result2 =0;	

    #if (!CPU_TS_INIT_IN_DELAY_FUNCTION)      
      //使用时间戳延时函数前必须先使能计数器
      CPU_TS_TmrInit();
    #endif
  
	  /* 初始化内存保护单元 */
    BOARD_ConfigMPU();
		/* 初始化开发板引脚 */
    BOARD_InitPins();
		/* 初始化开发板时钟 */
    BOARD_BootClockRUN();
		/* 初始化调试串口 */
		BOARD_InitDebugConsole();
		/* 打印系统时钟 */
		PRINTF("\r\n");
		PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
		PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
		PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
		PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
		PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
		PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
		PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
		PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
		PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));	

		PRINTF("*****测试以太网*****\r\n");	
		/* 进行以太网测试 */
    if(lwip_ping_test())
    {
      result2 = ENET_TEST_FAIL;
      PRINTF("*****以太网测试失败*****\r\n");	
    }
    else
    {      
      result2 = ENET_TEST_OK;
      PRINTF("*****以太网测试成功*****\r\n");	
    }		
    /* 初始化LED */
    LED_GPIO_Config();
    
    while(1);
    
}

/****************************END OF FILE**********************/
