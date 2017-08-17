/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CO��
DE
*
*                                        IAR Development Kits
*                                               on the
*
*                                            NXP LPC1768
*                                      LPC1766-SK KICKSTART KIT
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : FT
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/



int  main (void)
{
    SystemInit();

    GPIO_Test();

    OneTaskTimes();
//
//	InitWatchDog();
//
////	PrintfTime();
//
//	while(1)
//	{	
////		Diptask();
//
//		EquipmentComTask();
//
//		CalcDip();
//
//		StoreRecTask();
//
//	 	SpeedAndDistTast();
//
//		CheckLocoStaTask();
//
//		ErrDealTask();
//
//		if(GetSysTime() - GetReadDataDisplayTime() > 700)
//		{
//			if(GetSysTime() - GetReadDataDisplayTime() < 800)
//			{
//				ClaerDisplaySetWithoutStore();
//			}
//			DisplayDipVal();
//		}
////		else
////		{
////			if((GetSysTime() - GetReadDataDisplayTime() )%100 <10)
////			{
////				printfcom0("\r\n GetReadDataDisplayTime %d��%d",GetReadDataDisplayTime(),GetSysTime());
////			}
////		}
//
//		LedTast();
//
//		PrintfComSysInfo();
////
////	RecRs485();
//	}
////	GUI_Init();
////
////	OSInit();
////
////    OSTaskCreateExt((void (*)(void *))GPIO_Test,            /* Create the start task                                */                
////					(void          *)0,
////                    (OS_STK        *)&GPIO_TaskStartStk[GPIO_CFG_TASK_START_STK_SIZE - 1],
////                    (INT8U          )APP_CFG_TASK_START_PRIO+1,
////                    (INT16U         )APP_CFG_TASK_START_PRIO+1,
////                    (OS_STK        *)&GPIO_TaskStartStk[0],
////                    (INT32U         )GPIO_CFG_TASK_START_STK_SIZE,
////                    (void          *)0,
////                    (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
////
//////     OSTaskCreateExt((void (*)(void *))uartTask,            /* Create the start task                                */                
//////					(void          *)0,
//////                    (OS_STK        *)&Uart_TaskStartStk[UART_CFG_TASK_START_STK_SIZE - 1],
//////                    (INT8U          )APP_CFG_TASK_START_PRIO+2,
//////                    (INT16U         )APP_CFG_TASK_START_PRIO+2,
//////                    (OS_STK        *)&Uart_TaskStartStk[0],
//////                    (INT32U         )UART_CFG_TASK_START_STK_SIZE,
//////                    (void          *)0,
//////                    (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
////
//// 						
////	OSStart();	
////
////	return(1);

}
