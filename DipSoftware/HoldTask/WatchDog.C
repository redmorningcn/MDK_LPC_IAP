//#include  "config.h"
#include <includes.h>

//#define		EX_DOG		sp706
#define		WATCHDOG	28		//P028

/****************************************************************************
* ���ƣ�void  InitWatchDog(void)
* ���ܣ���ʼ�����Ź�
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void  InitWatchDog(void)
{
	WDT_Init(WDT_CLKSRC_IRC,WDT_MODE_RESET);
	WDT_Start(4000000);
	FeedDog();
    
    
    
//	GPIO_PinselConfig(WATCHDOG,0);	
//
//	FeedDog();

}

/****************************************************************************
* ���ƣ�void  FeedDog(void)
* ���ܣ����Ź�ι��������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void  FeedDog(void)
{  
	WDT_Feed();
    
    
//   if(GPIO_ReadIoVal(WATCHDOG))
//   {
//   		GPIO_SetOrClearValue(WATCHDOG,0);
//   }
//   else
//   {
//   		GPIO_SetOrClearValue(WATCHDOG,1);	
//   }
}


/****************************************************************************
* ���ƣ�void  FeedDog(void)
* ���ܣ����Ź�ι��������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void  ResetSys(void)
{  
	while(1);	
}