#include    "includes.h"  


void	IO0PIN_R(uint8	x)
{
}

void	IO0CLR_W(uint8	x)
{
}

void	IO0SET_W(uint8	x)
{
}




void	IO0DIR_IN(uint8	x)   
{
}

void	 IO0DIR_OUT(uint8 x)  
{
}

void	 IO1DIR_OUT(uint8 x)  
{
}

void	 NOP(void)  
{
	FeedDog();
}


void	 IO2DIR_OUT(uint8 x)  
{
}
void	 IO2PIN_W(uint8	x) 
{
}
void	 IO1PIN_W(uint8	x) 
{
}

//void	ReCalPara(uint8	x)
//{
//}

uint8 ReadFlsh(uint32 Addr,uint8 *buf,uint32 Len)
{	
	return	MX25L1602_RD(Addr, Len,(INT8U* )buf);
}

INT8U	WriteFlsh(INT32U Dst, uint8 * sFlshRec, INT32U NByte)
{
	return	MX25L1602_WR(Dst,(INT8U*)sFlshRec,NByte);
}



void IO0PIN_W(uint8	Val,uint8	x)   
{
}


uint8	l_hostevtflg;
uint8	GetHostEvtFlg(void)
{
	return	 l_hostevtflg;
}

#define		START_EVT		31		
//----------------------------------------------------------------------------
// 名    称：   uint8 JudgeStartEvt(void)
// 功    能:    返回开机事件标识
// 入口参数： 
// 出口参数：
//----------------------------------------------------------------------------
uint8 	JudgeStartEvt(void)
{
	uint8	EvtType = RIGHT_DATA;
	static	uint32	Time = 0;
	
	EvtType = RIGHT_DATA;
	
	if( Time++ == 0 )
	{
		EvtType = START_EVT;	

		l_hostevtflg = 	START_EVT;
	}
	
	return	EvtType;
}

//----------------------------------------------------------------------------
// 名    称：   void PrinfSysInfo(void)
// 功    能:    系统初始化
// 入口参数：
// 出口参数：
//----------------------------------------------------------------------------
void	PrinfSysInfo(void)
{
//	stcDeviceInfo		sDeviceInfo;
//	stcReviseHight		sReviseHight;
//	stcFixInfo  		sFix;
//	stcDensity			sDensity;
//	float				Density;
//	int16				ReviseHight;
//	uint8				OilBoxCod;
//
//
//	DisplayTime();
//	DelayX10ms(1*100);
//
//	DisplaySet(100,1,"SV15");		
////	DisplaySet(100,1,"10.08");
//
//	DisplaySet(100,1,"HV10");
//	DelayX10ms(2*100);
//
//
//	ReadHostInfo((stcDeviceInfo *)&sDeviceInfo);		//读出厂信息
//	
//	//printfcom0("\r\n 能耗硬件版本号:	");
//	SendCOM0(sDeviceInfo.HardwareVer,sizeof(sDeviceInfo.HardwareVer));
//	//printfcom0("\r\n 能耗软件版本号:	");
//	SendCOM0(sDeviceInfo.SoftwareVer,sizeof(sDeviceInfo.SoftwareVer));
//	
//	//printfcom0("\r\n ");
//	OilBoxCod = GetOilBoxCod();
//	printfcom0("\r\n 油箱模型号:	%d",OilBoxCod);
//
//	DisplaySet(150,1,"C_%u",OilBoxCod);	
//
//	DelayX10ms(1*100);
//  	if(ReadReviseHight((stcReviseHight *) &sReviseHight))
//  	{	
//  		ReviseHight = sReviseHight.ReviseHight / 10;
//  		//printfcom0("\r\n 修正高度为:	%d",ReviseHight*10);
//  		DisplaySet(100,1,"H_%d.",ReviseHight*10);		
//	}
//	DelayX10ms(1*100);
//		
//  	if( ReadDensity((stcDensity *) &sDensity))			//取密度
//	{
//		Density	=  ((float)sDensity.Density / 1000);
//  		DisplaySet(60,1,"d_%d.",(uint16)(1000*Density));		
//  	}
//  	DelayX10ms(1*100);
//
// 	ReadFixInfo((stcFixInfo *)&sFix);
// 	DisplaySet(100,1,"%d",sFix.LocoTyp);
//	DelayX10ms(1*100);		
// 	//DisplaySet(100,1,"%d",(uint16)*sFix.LocoNum);		
// 	DisplaySet(100,1,"N_%d",sFix.LocoNum[0]+sFix.LocoNum[1]*256);	
//		
//// 	DelayX10ms(2*100);
//
//	DisplaySet(60,1,"        ");
//  	DisplaySet(60,1,"88888888");		
// 	DisplaySet(60,1,"        ");	
//	
////	DelayX10ms(2*100);	
}

uint8 OneTask(void)   
{
	UART0Init();
	//UART1Init();

	UART2Init();
	UART3Init();
	
	InitWatchDog();
	
	printfcom0("\r\nboot... ");

	while(1)
	{
		IAP_PragramTask();
	}
//	InitSoftCOM();
//	InitLed();
//	TestSoftCOM();

//	UART2Init();
//	UART3Init();
//
//	InitDisplay();
//	DisplayString("88888888");
//	//ClaerDisplaySetWithoutStore();

//	InitLed();
//	DisplayString("         ");
//	InitCardIO();
//	InitFramIO();
//	InitFlashIO();
//
//	InitDS3231();
//	InitEvtDealSys();

//	InitAdc();
//
//	Time0Init();
//	Time1Init();
//	Time2Init();


	return	1;
}

uint8 HoldTask(void)   
{
//	HoldDisplay();
	HoldLed();
//	HoldCard();								//IC卡维护

	return	1;
}

#define			PLUG_IN_CARD		1
//----------------------------------------------------------------------------
// 名    称：   void IncDisplay(void)
// 功    能:    系统初始化
// 入口参数：
// 出口参数：
//----------------------------------------------------------------------------
void IncDisplay(void)
{ 
//	static	uint32		Time = 0;
////	static	uint32		Times = 0;
//	int16	Prs1,Prs2;	
//	uint8	i;
//
//	Time = GetSysTime();
//
//	for(i =0 ;i<3;)
//	{
////		COM0Task();
//		
//		Prs1 = GetDip1Prs();
//		Prs2 = GetDip2Prs();		
//		
//		//printfcom0("\r\n %4d.%4d",Prs1,Prs2);
//		if(GetSysTime() - Time > 100)
//		{		
//			i++;
//			Time = GetSysTime();
//	
//			if(i%2 == 0)
//			{
//				DisplaySet(100,1,"%4d.%4d.",Prs1,Prs2);
//			}
//			else
//			{
//				DisplaySet(100,1,"%4d.%4d",Prs1,Prs2);
//			}
//		}
//	}
//
//
///*	while(GetPlugFlg() == PLUG_IN_CARD)  //有东西插入
//	{
//		COM0Task();
//
//		if(Times < 1*20)		//1分钟通信
//		{	
//			COM0Task();
//		}
//					
//		Prs1 = GetDip1Prs();
//		Prs2 = GetDip2Prs();
//
//		if(GetSysTime() - Time > 100)
//		{		
//			Time = GetSysTime();
//
//			Times++;
//			
//			if(Prs1<10000 && Prs2 < 10000)
//			{			
//				if(Times%2 == 0)
//				{
//					DisplaySet(100,1,"%4d.%4d.",Prs1,Prs2);
//				}
//				else
//				{
//					DisplaySet(100,1,"%4d.%4d",Prs1,Prs2);
//				}
//			}
//			else
//			{
//				DisplaySet(100,1,"%d",Prs1);
//				DisplaySet(100,1,"%d.",Prs2);
//			}
//		}
//		
//		if(Times > 1*60)		//2分钟退出
//		{	
//			DelayX10ms(200);
//			return;
//		}
//	}	
//	
//	DelayX10ms(200);
//*/	
//	DisplaySet(150,1,"%d",g_CurFlshRecNum);
//	DisplaySet(150,1,"%d.",g_LshRedFlshRecNum);
//	printfcom0("\r\n g_CurFlshRecNum %d, g_LshRedFlshRecNum %d",g_CurFlshRecNum,g_LshRedFlshRecNum);
}

void	OneTaskTimes(void)
{
	OneTask();
	
//	g_CurFlshRecNum 	= GetCurFlshRecNum();
//	g_LshRedFlshRecNum  = GetLstFlshRecNum();
//	g_LshRedFlshRecNum_gprs  = GetLstFlshRecNum_gprs();
//
//
//   	printfcom0("\r\n 当前 %d,已读 %d,已发GPRS %d",g_CurFlshRecNum,g_LshRedFlshRecNum,g_LshRedFlshRecNum_gprs);
// 	PrinfSysInfo();
////	IncDisplay();
////	InitEvtDealSys();
//	InitErrDealSys();
}

