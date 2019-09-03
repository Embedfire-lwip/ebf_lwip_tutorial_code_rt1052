/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS V9.0.0 + i.MXRT LwIP
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 i.MX RT 全系列开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "debug.h"
#include "fsl_iomuxc.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "tcpecho.h"
#include "enet_ethernetif.h"
#include "sys_arch.h"
#include "lwip/init.h"
#include "lwip/timeouts.h"

/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */


/******************************* 宏定义 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些宏定义。
 */


/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
extern void TCPIP_Init(void);

/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  ****************************************************************/
int flag = 0;
int main(void)
{	

  /* 开发板硬件初始化 */
	BSP_Init();
	TCPIP_Init();
	PRINTF("LwIP版本：%s\n",LWIP_VERSION_STRING);
	PRINTF("ping实验例程\n");

	PRINTF("使用同一个局域网中的电脑ping开发板的地址，可进行测试\n");

	PRINTF("本地IP和端口: %d.%d.%d.%d\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
	TCP_Echo_Init();
	while (1)
	{  
			/* 从以太网缓冲区中读取数据包，交给LwIP处理 */
			if(flag)
			{
				flag = 0;
				LED2_TOGGLE;
				//调用网卡接收函数
				ethernetif_input(&gnetif);
			}
			/* 处理 LwIP 超时 */
			sys_check_timeouts();
	}	

}
