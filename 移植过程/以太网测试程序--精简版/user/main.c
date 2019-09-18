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

#include "ping/ping.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "netif/ethernet.h"
#include "ethernetif.h"

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
//*/
//int main(void)
//{ 
//	uint8_t result1,result2 =0;	

//	#if (!CPU_TS_INIT_IN_DELAY_FUNCTION)      
//		//使用时间戳延时函数前必须先使能计数器
//		CPU_TS_TmrInit();
//	#endif

//	/* 初始化内存保护单元 */
//	BOARD_ConfigMPU();
//	/* 初始化开发板引脚 */
//	BOARD_InitPins();
//	/* 初始化开发板时钟 */
//	BOARD_BootClockRUN();
//	/* 初始化调试串口 */
//	BOARD_InitDebugConsole();
//	/* 打印系统时钟 */
//	PRINTF("\r\n");
//	PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
//	PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
//	PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
//	PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
//	PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
//	PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
//	PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
//	PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
//	PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));	

//	PRINTF("*****测试以太网*****\r\n");	
//	/* 进行以太网测试 */
//	if(lwip_ping_test())
//	{
//		result2 = ENET_TEST_FAIL;
//		PRINTF("*****以太网测试失败*****\r\n");	
//	}
//	else
//	{      
//		result2 = ENET_TEST_OK;
//		PRINTF("*****以太网测试成功*****\r\n");	
//	}		
//	/* 初始化LED */
//	LED_GPIO_Config();
//	
//	while(1);
//	
//}

#define DEST_IP_ADDR0               192
#define DEST_IP_ADDR1               168
#define DEST_IP_ADDR2                 0
#define DEST_IP_ADDR3               232

#define DEST_PORT                  5001

#define UDP_SERVER_PORT            5002   /* define the UDP local connection port */
#define UDP_CLIENT_PORT            5002   /* define the UDP remote connection port */

#define LOCAL_PORT                 5001

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0                    192
#define IP_ADDR1                    168
#define IP_ADDR2                      0
#define IP_ADDR3                    112

/*NETMASK*/
#define NETMASK_ADDR0               255
#define NETMASK_ADDR1               255
#define NETMASK_ADDR2               255
#define NETMASK_ADDR3                 0

/*Gateway Address*/
#define GW_ADDR0                    192
#define GW_ADDR1                    168
#define GW_ADDR2                      0
#define GW_ADDR3                      1
/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS 0x00U
		/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
		
void TCPIP_Init(void)
{
		/*配置网卡信息*/
    ethernetif_config_t fsl_enet_config0 = {
        .phyAddress = EXAMPLE_PHY_ADDRESS,
        .clockName = EXAMPLE_CLOCK_NAME,
        .macAddress = configMAC_ADDR,
    };
 time_init();
		/* 网络接口配置 */
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
		/*LWIP初始化*/
    lwip_init();

		netif_add(&gnetif, &ipaddr, &netmask, &gw, &fsl_enet_config0,
                       ethernetif0_init, ethernet_input);
    netif_set_default(&gnetif);
    netif_set_up(&gnetif);
		ping_init(&gw);

}

int main(void)
{
	BSP_Init();
	TCPIP_Init();
	while(1)
	{
       //调用网卡接收函数
     ethernetif_input(&gnetif);

     //处理LwIP中定时事件
     sys_check_timeouts();

	
	}

}

/****************************END OF FILE**********************/
