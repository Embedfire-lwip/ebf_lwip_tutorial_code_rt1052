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
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "./systick/bsp_systick.h"
#include "./led/bsp_led.h"   

#include "ping.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "sys_arch.h"
#include "tcpecho.h"



/*******************************************************************
 * Prototypes
 *******************************************************************/
int flag = 0;

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
	  /* 开发板硬件初始化 */
		BSP_Init();
	
		struct netif gnetif;
		ip4_addr_t ipaddr;
		ip4_addr_t netmask;
		ip4_addr_t gw;
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
		/*ping 初始化*/
    ping_init(&gw);
		
		/*打印提示信息*/
		PRINTF("LwIP版本：%s\n",LWIP_VERSION_STRING);
		PRINTF("ping实验例程\n");

		PRINTF("使用同一个局域网中的电脑ping开发板的地址，可进行测试\n");

		PRINTF("本地IP和端口: %d.%d.%d.%d\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
		TCP_Echo_Init();
    while (1)
    {
			//调用网卡接收函数
        ethernetif_input(&gnetif);
				/* 处理 LwIP 超时 */
        sys_check_timeouts();
    }

}

