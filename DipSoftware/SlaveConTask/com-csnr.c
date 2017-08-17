#include <includes.h>


uint8	GetHdlcFrameEndFlg(void)
{
	return	Gett0HdlcFramEndFlg();	
}

void	ClearHdlcFrameEndFlg(void)
{
	Cleart0HdlcFramEndFlg();   
}

uint8		ReadHdlcFrame(uint8	*buf)  
{
	uint32	reclen;
	reclen 	= ReadSoftCOM(buf);
	return	reclen;	
}
//
//
//uint8	l_hdlcreccomdata = 0;
//uint8	GetHdlcReccomDataNum(void)
//{
//	return l_hdlcreccomdata;
//}

uint8	GetSendHdlcAddr(void)
{
	return	0x75;
}

//void	ClearHdlcReccomDataNum(void)
//{
//	l_hdlcreccomdata = 0;
//}

uint16	g_higval = 500;
uint8	GetSetHdlcDataBuf(uint8 *buf)  
{
	uint8	len;

	len = 0;
	buf[len++] = 0xc5;
	buf[len++] = 0x67;
	buf[len++] = 0x01;

	memcpy(&buf[len],(uint8 *)&g_higval,sizeof(g_higval));
	len += sizeof(g_higval);
	return len;
}
//
//extern	uint8	hdlcreccombuf[256];
void  CSNR_RecDealHostDip(void)     									//����
{
	uint8	RecBuf[256] = {0};								//���ջ�����
	uint8	DataBuf[256] = {0};								//
	uint8	souraddr;
	uint32	RecLen,DataLen;
	uint16	dip,hig = 0;
	static	uint16 	lsthig = 0;
	static	uint32	time;
			
	if( GetFramRecEndFlgDip() == 1)						//���µ�֡β���������ݴ���	  ////////////////////////
	{
		ClearFramRecEndFlg();									//���־��������ȡ��

		RecLen 	= ReadRs485DataDip(RecBuf);
		
		souraddr = CSNR_GetData(RecBuf,RecLen,DataBuf,(unsigned char*)&DataLen); 

	 	if(souraddr == 	0x80)
		{ 
			dip = DataBuf[3]+ DataBuf[2]*256;	
	
			hig = DipConvertHig(dip);
			
			if(GetSysTime()-time > 500)	 		//ˢ���ٶȱ���
			{
				time = GetSysTime();
				if(lsthig)	  					
				{
					if(abs(hig - lsthig) < 2)	//������ֵ��С
					{
						g_higval = hig;
					}
					else
					{
						g_higval = (lsthig + hig )/2;
					}
				}
				lsthig = hig;
			}

//			l_hdlcreccomdata = DataLen;
//			memcpy(hdlcreccombuf,DataBuf,l_hdlcreccomdata);
		 }
	}
}
