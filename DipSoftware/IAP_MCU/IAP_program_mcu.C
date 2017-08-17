#include  "includes.h"

//#define	CVI	0

#define	USER_APP_START_ADDR		0x00010000

//extern	void printfcom0texbox(uint8 *g_textboxbuf);

//IAP 远程485升级

//正常开机进入IAP程序，2秒无数据退出，进入主程序。
//

uint8	GetFramRecEndFlgIAP(void)
{
//	return GetCOM1EndFlg(); 
    return ((GetCOM2EndFlg())|(GetCOM3EndFlg())); 
}

//清接收完成标志  南瑞
void	ClearFramRecEndFlgIAP(void)
{
//	ClearCOM1EndFlg();  
		ClearCOM2EndFlg();  
		ClearCOM3EndFlg();  
}

uint16	ReadRs485DataIAP(uint8 *Buf)
{
	uint16	len = 0;
	 
	if(GetCOM2EndFlg())
	{
		len = ReadCOM2(Buf,256); 
		return	len; 
	}
	
	if(GetCOM3EndFlg())
	{
		len = ReadCOM3(Buf,256);
		return	len;  
	}	
}

			  

__asm void JMP_Boot( uint32_t address ){
   LDR SP, [R0]		;Load new stack pointer address
   LDR PC, [R0, #4]	;Load new program counter address
}

#define zyIrqDisable()  __disable_irq()
#define zyIrqEnable()   __enable_irq()


////启动app
//void Boot( void )
//{
////	 void (*userProgram)();   					           /*函数指针*/

//	SCB->VTOR = USER_APP_START_ADDR & 0x1FFFFF80;	//偏移中断向量

//	JMP_Boot(USER_APP_START_ADDR);
////	userProgram = (void (*)()) (USER_APP_START_ADDR+1);
////	(*userProgram)();													/*启动						*/	 
//	
//}
typedef  void (*pFunction)(void);			    //???????????.
/***********************************************
* ??:??????
*/
pFunction   pApp;

void Boot( void )
{
    uint32     JumpAddress = 0;
    uint8      cpu_sr;

    SCB->VTOR = USER_APP_START_ADDR & 0x1FFFFF80;	//偏移中断向量
    
    JumpAddress   =*(volatile uint32*) (USER_APP_START_ADDR + 4); // ??+4?PC??
    pApp          = (pFunction)JumpAddress;                     // ??????APP
    __set_MSP       (*(volatile uint32*) USER_APP_START_ADDR);    // ????????(MSP)
    __set_PSP       (*(volatile uint32*) USER_APP_START_ADDR);    // ?????????(PSP)
    __set_CONTROL   (0);                                        // ??CONTROL
    
    pApp();                                                     //????
    
    //(*pApp)();
    //zyIrqEnable()
    //CPU_CRITICAL_EXIT();

}


//对flash 进行擦写
//默认进入长度len=128
#define		MAX_SEC_NUM   22
void	IAP_pragramDeal(uint8	*buf,uint32	len)
{
	static	uint32	flashlen = 0;
	static	uint32	curaddr = USER_APP_START_ADDR;
	static	uint8	iapbuf[2048];
	static	uint32	iapbuflen = 0;
	static	uint8		times = 0;
	  uint32_t result[4];
	  uint8_t ver_major, ver_minor;
	  uint32_t i;
	  uint8_t *ptr;
	  uint32_t flash_prog_area_sec_start;
	  uint32_t flash_prog_area_sec_end;
	  IAP_STATUS_CODE status;

		times++;
		memcpy(&iapbuf[iapbuflen],buf,len);
    
		iapbuflen +=len;
	   
		if(times == 1024/128 || len != 128)
		{
			if(len != 128)
			{
				for(i = iapbuflen;i<1024;i++ )
				iapbuf[i] = 0xff;	
			}

			times = 0;
			iapbuflen = 0;
			
			zyIrqDisable();										//关中断

			if(flashlen == 0)									// 第一次
			{
				curaddr = USER_APP_START_ADDR;
	 			status = ReadPartID(result);
				printfcom0("\r\nPartID:%d,status %d",result[0],status);
				 
		  	status = ReadBootCodeVer(&ver_major, &ver_minor);
		  	printfcom0("\r\nBoot Code Version:%d.%d status %d",ver_major,ver_minor,status);
	
		  	status = ReadDeviceSerialNum(result);
		  	i=0;
	 			printfcom0("\r\nUID:%d-%d-%d-%d status %d",result[i++],result[i++],result[i++],result[i++],status);
	 			
	 		 	flash_prog_area_sec_start = 	GetSecNum(curaddr);
		  	flash_prog_area_sec_end 	=  	GetSecNum(curaddr + 1024);
		  	
//		  		status = EraseSector(flash_prog_area_sec_start, flash_prog_area_sec_end);
				status = EraseSector(flash_prog_area_sec_start, MAX_SEC_NUM);
				printfcom0("\r\nErase chip0: Success. status %d",status); 							
		  		//status = BlankCheckSector(flash_prog_area_sec_start, flash_prog_area_sec_end,
                //                  &result[0], &result[1]);
		  		status = BlankCheckSector(flash_prog_area_sec_start, MAX_SEC_NUM,
                                  &result[0], &result[1]);
				printfcom0("\r\nErase chip1: Success. status %d",status);                                  
	 		}

// 		 	flash_prog_area_sec_start 	= 	GetSecNum(curaddr);
//	  		flash_prog_area_sec_end 	=  	GetSecNum(curaddr + 1024);	 		
//	 		if(flash_prog_area_sec_start != flash_prog_area_sec_end)												//跨区，清next区
// 			{
// 				status = EraseSector(flash_prog_area_sec_end, flash_prog_area_sec_end);
//				printfcom0("\r\nErase chip0: Success. status %d ",status); 
//		  		status = BlankCheckSector(flash_prog_area_sec_start, flash_prog_area_sec_end,
//                                  &result[0], &result[1]);
//				printfcom0("\r\nErase chip1: Success. status %d  curaddr %x ",status,curaddr);     				
// 			}

	    	curaddr = USER_APP_START_ADDR + flashlen;
			status 	= CopyRAM2Flash((uint8 *)curaddr, iapbuf,IAP_WRITE_1024);
			flashlen += 1024;

  // Compare
			status =  Compare((uint8 *)curaddr, iapbuf,IAP_WRITE_1024);
			printfcom0("\r\nCompare status %d  curaddr %x ",status,curaddr);

//			printfcom0("\r\n");
//			for(i = 0 ;i<IAP_WRITE_1024;i++)
//			printfcom0("%02x",iapbuf[i]);

			printfcom0("\r\n");
			zyIrqEnable();
		}
		//DelayX10ms(2);	
}

extern	unsigned char  CSNR_GetData(unsigned char	*RecBuf,unsigned char RecLen,unsigned char	*DataBuf,unsigned char	*InfoLen) ;
void    IAP_ProgramCopy(void);

void	IAP_PragramTask(void)
{
		static	uint32  	time = 0;
		static	uint8		praflg = 0;
		
		uint8		recbuf[256];
		uint8		databuf[256];
		uint32		reclen 	= 0;	
		uint32		datalen = 0;
		uint32		tmp;
		char		startstring[]={"IAP_pragram start!"};
		char		pramstring[]={"."};
		char		holdstring[]={"!"};
		char		endstring[]={"IAP_pragram end!"};
		char		*p;
		uint32		i;
		static		uint8	lstrecnum = 0;
		
		time = GetSysTime();
		while(1)
		{
            DelayX10ms(1);
            if(GetFramRecEndFlgIAP())
            {
                tmp = ReadRs485DataIAP(recbuf);
                ClearFramRecEndFlgIAP();

                if(tmp == 0)
                    continue ;

                
                CSNR_GetData(recbuf,tmp,databuf,(uint8 *)&datalen); 

                if(praflg == 0)
                {
                    databuf[datalen] = '\0';
                    
                    if(strcmp((char *)&databuf[0],startstring) == 0)				//编程判定
                    {
                        lstrecnum = GetRecFramNum();
                        SetFramNum(lstrecnum);

                        praflg = 1;
                        DataSend_CSNR(0x80,0xA1,startstring,strlen(startstring));	   
                    
                        printfcom0(" s:%s\r\n",startstring);
                        time = GetSysTime();							
                        continue ;
                        //return;
                    }
                } 
                else																					
                {
                    printfcom0("\r\n lstrecnum %x,%x",lstrecnum, GetFramNum());
                    if(lstrecnum == GetRecFramNum())
                    {
                            DataSend_CSNR(0x80,0xA1,holdstring,strlen(holdstring));	
                            time = GetSysTime();
                    }
                    else
                    {	
                        lstrecnum = GetRecFramNum();
                        SetFramNum(lstrecnum);

                        IAP_pragramDeal(databuf,datalen);					//编程						   	
#ifdef CVI													
                        databuf[datalen] = '\0';
                        printfcom0texbox(databuf);
#endif				
                        DataSend_CSNR(0x80,0xA1,pramstring,strlen(pramstring));	 
                        printfcom0("%s",pramstring);
                        
                        if(datalen != 128)												//结束编程
                        {	
                            DataSend_CSNR(0x80,0xA1,endstring,strlen(endstring));	
                            printfcom0("\r\n len != 128");
                            break;
                        }

                        time = GetSysTime();
                    }
                }
            }
            
            if(praflg == 0)
            {
                if(GetSysTime() - time > 200)										//退出
                {
                    printfcom0("\r\nGetSysTime() - time > 1000");
                    break;
                }
            }
            else
            {
                if(GetSysTime() - time > 2000)										//退出
                {
                    printfcom0("\r\nGetSysTime() - time > 1000");
                    break;
                }
            }
		}	
		
//		p =  (uint8 *)USER_APP_START_ADDR;
//		for(i = 0;i < 0x20000;i++ )
//		{
//			printfcom0("%02x",*p);
//			p++;
//		}

        //20170812 新增备份区拷贝功能。
        
        IAP_ProgramCopy();
        
		printfcom0("\r\n boot end");
        FeedDog();
		Boot();
}

typedef  struct   _stcIAPPara_
{
    uint16  hardver;        //????
    uint16  softver;        //????
    uint32  softsize;       //????
    uint32  addr;           //????
    uint32  framenum;       //???
    uint16  code;           //??? 01,????????
    uint16  crc16;
}stcIAPPara;

#define	IAP_PARA_START_ADDR     0x00070000	
#define	IAP_PARA_PRO_SIZE		0x0000FFFF	



/*******************************************************************************
 * ?    ?: IAP_WriteParaFlash
 * ?    ?: ?IAP??????Flash?
 * ????: stcIAPCtrl
 * ????: ?
 * ?  ?: redmorningcn.
 * ????: 2017-08-08
 * ?    ?:
 * ????:
 *******************************************************************************/
void    IAP_ReadParaFlash(stcIAPPara *sIAPPara)
{
    IAP_STATUS_CODE status;
    uint32_t result[4];
    
    uint32_t flash_prog_area_sec_start;
    uint32_t flash_prog_area_sec_end;
    
    zyIrqDisable();                                 //???
                                                    //??copy
    memcpy((uint8 *)sIAPPara,(uint8 *)IAP_PARA_START_ADDR,sizeof(stcIAPPara));
            
    zyIrqEnable();                                  //?????
}


/*******************************************************************************
 * ?    ?: IAP_WriteParaFlash
 * ?    ?: ?IAP??????Flash?
 * ????: stcIAPCtrl
 * ????: ?
 * ?  ?: redmorningcn.
 * ????: 2017-08-08
 * ?    ?:
 * ????:
 *******************************************************************************/
void    IAP_WriteParaFlash(stcIAPPara *sIAPPara)
{
    IAP_STATUS_CODE status;
    uint32_t result[4];

    uint32_t flash_prog_area_sec_start;
    uint32_t flash_prog_area_sec_end;
    
    zyIrqDisable();										                                //???

    flash_prog_area_sec_start   = 	GetSecNum(IAP_PARA_START_ADDR);
    flash_prog_area_sec_end 	=  	GetSecNum(IAP_PARA_START_ADDR + IAP_PARA_PRO_SIZE); //????

    status = EraseSector(flash_prog_area_sec_start, flash_prog_area_sec_end);           //??????
    status = BlankCheckSector(flash_prog_area_sec_start, flash_prog_area_sec_end,
                &result[0], &result[1]);
    
    //??flash??,??????
    status 	= CopyRAM2Flash( (uint8_t *)IAP_PARA_START_ADDR,
                                  (uint8 *)sIAPPara,
                                   256
                            );                                                          //????
    
    status =  Compare((uint8_t *)IAP_PARA_START_ADDR,
                            (uint8 *)sIAPPara,
                            sizeof(stcIAPPara)
                    );                                                                  //????
            
    zyIrqEnable();                                                                      //?flash???,?????
}


#define	USER_APP_START_ADDR		0x00010000	
#define	USER_APP_PRO_SIZE		0x0002FFFF	

#define	USER_BACK_START_ADDR	0x00040000	
/*******************************************************************************
 * ?    ?: IAP_WriteFlash
 * ?    ?: ?????Flash?
 * ????: stcIAPCtrl
 * ????: ?
 * ?  ?: redmorningcn.
 * ????: 2017-08-08
 * ?    ?:
 * ????:
 *******************************************************************************/
void    IAP_CopyAppToFlash(stcIAPPara *sIAPPara)
{
    IAP_STATUS_CODE status;
    uint32_t result[4];
    //读取flash数据
    uint32_t finshlen = 0;
    uint8 buf[1024];
    int i;
    
    uint32_t flash_prog_area_sec_start;
    uint32_t flash_prog_area_sec_end;

    zyIrqDisable();										//???
    
    //插除将要写的区域
    flash_prog_area_sec_start   = 	GetSecNum(USER_APP_START_ADDR);
    flash_prog_area_sec_end 	=  	GetSecNum(USER_APP_START_ADDR + USER_APP_PRO_SIZE);//????

    status = EraseSector(flash_prog_area_sec_start, flash_prog_area_sec_end);           //???????

    status = BlankCheckSector(flash_prog_area_sec_start, flash_prog_area_sec_end,
                  &result[0], &result[1]);


    finshlen = 0;
    //地址越界或已到数据区长度，退出
    while(  finshlen < USER_APP_PRO_SIZE 
        &&  finshlen < sIAPPara->softsize )
    
    {
        memcpy(buf,(uint8 *)(USER_BACK_START_ADDR + finshlen),IAP_WRITE_1024);   //取backup区间数据
        
        status 	= CopyRAM2Flash((uint8 *)(USER_APP_START_ADDR + finshlen),       //将backup区数据写入app区   
                                         buf,
                                         IAP_WRITE_1024
                                );                      
        
        status =  Compare((uint8 *)(USER_APP_START_ADDR + finshlen),             //比较写入内容     
                                     buf,
                                     IAP_WRITE_1024
                                );
        
        finshlen += IAP_WRITE_1024;                     //地址累加  
        
        for(i = 0; i< IAP_WRITE_1024;i++)
        {
            printfcom0("%02x",buf[i]);
        }
        FeedDog();
    }   
    
    zyIrqEnable();                                  //?flash???,?????
}


#define     IAP_COPY_BACK_APP    0x01
/*******************************************************************************
 * ?    ?: IAP_WriteFlash
 * ?    ?: ?????Flash?
 * ????: stcIAPCtrl
 * ????: ?
 * ?  ?: redmorningcn.
 * ????: 2017-08-08
 * ?    ?:
 * ????:
 *******************************************************************************/
void    IAP_ProgramCopy(void)
{
    uint16  crc16;
    
    stcIAPPara  sIapPara;
        
    IAP_ReadParaFlash(&sIapPara);                          //读IAP参数
    
    crc16 = GetCrc16Check((uint8 *)&sIapPara,sizeof(stcIAPPara)-2);  //算校验
    
    printfcom0("\r\n\r\n");
    if( crc16 == sIapPara.crc16 &&                          //校验正确，且copy指令，则执行copy操作,
        sIapPara.code == IAP_COPY_BACK_APP &&               //数据区 数据大小 是否有效
        sIapPara.softsize > 0x100 &&
        sIapPara.softsize < USER_APP_PRO_SIZE
      )
    {
        IAP_CopyAppToFlash(&sIapPara);                      //数据从back区copy至app区
    }
    
    printfcom0("\r\n\r\n");

    sIapPara.code   = 0;
    crc16 = GetCrc16Check((uint8 *)&sIapPara,sizeof(stcIAPPara)-2);  //计算校验
    sIapPara.crc16  = crc16;                                        //校验赋值
    
    //修改copy指令，重新写入
    IAP_WriteParaFlash(&sIapPara);
}

