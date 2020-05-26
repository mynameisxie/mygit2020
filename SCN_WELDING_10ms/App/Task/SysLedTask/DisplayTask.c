#include "DisplayTask.h"
#include "Bsp_OsTask.h"
#include "Bsp_Led.h"
#include "BSP_595.h"
#include "Bsp_Uart.h"
#include "Bsp_Flash.h"
#include "CycTask.h"
#include  "Bsp_ADC.h"
//LED_RUN
uint8_t DisType;
uint8_t FlushCursor=0;
uint16_t Vout =0;
const uint8_t NumCode[]={0xD7,0x14,0xCD,0x5D,0x1E,0x5B,0xDB,0x15,0xDF,0x5F};
const uint8_t AsciiCode[]={0xD6,//"U"
                           0x08,//"-"
													 0xCD,//"2"
													 0x14,//"1"
													 0x8A,//""
};
uint8_t TubeBuff[]={0x00,0xB3};
uint8_t LedBuff[3]={0xFF,0xFF,0xFF};
//led��̬��ʾ

void ledDisplay()
{	
	if(Vout >=310)//95
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x00;
		LedBuff[2]=0x10;
	}
	else if(Vout >=300)//90
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x00;
		LedBuff[2]=0x30;
	}
	else if(Vout >=290)//85
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x00;
		LedBuff[2]=0x70;
		
	}
	else if(Vout >=280)//80
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x00;
		LedBuff[2]=0xF0;
		
	}
	else if(Vout >=260)//75
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x01;
		LedBuff[2]=0xF0;
		
	}
	else if(Vout >=250)//70
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x03;
		LedBuff[2]=0xF0;
		
	}
		else if(Vout >=240)//65
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x07;
		LedBuff[2]=0xF0;
		
	}
		else if(Vout >=220)//60
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x0F;
		LedBuff[2]=0xF0;
	}
		else if(Vout >=210)//55
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x1F;
		LedBuff[2]=0xF0;
		
	}
		else if(Vout >=190)//50
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x3F;
		LedBuff[2]=0xF0;
		
	}
		else if(Vout >=195)//45
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0x7F;
		LedBuff[2]=0xF0;
		
	}
		else if(Vout >=192)//40
	{
		LedBuff[0]=0x00;
		LedBuff[1]=0xFF;
		LedBuff[2]=0xF0;
		
	}
	else if(Vout >190)//35
	{
		LedBuff[0]=0x01;
		LedBuff[1]=0xFF;
		LedBuff[2]=0xF0;
		
	}
	else if(Vout >=185)//30
	{
		LedBuff[0]=0x03;
		LedBuff[1]=0xFF;
		LedBuff[2]=0xF0;
		
	}
	else if(Vout >=180)//25
	{
		LedBuff[0]=0x07;
		LedBuff[1]=0xFF;
		LedBuff[2]=0xF0;
		
	}
	else if(Vout >=175)//20
	{
		LedBuff[0]=0x0F;
		LedBuff[1]=0xFF;
		LedBuff[2]=0xF0;
		
	}
	else if(Vout >=165)//15
	{
		LedBuff[0]=0x1F;
		LedBuff[1]=0xFF;
		LedBuff[2]=0xF0;
		
	}
	else if(Vout >=160)//10
	{
		LedBuff[0]=0x3F;
		LedBuff[1]=0xFF;
		LedBuff[2]=0xF0;
		
	}
	else if(Vout >10)//5
	{
		LedBuff[0]=0x7F;
		LedBuff[1]=0xFF;
		LedBuff[2]=0xF0;
		
	}
	else
	{
		LedBuff[0]=0xFF;
		LedBuff[1]=0xFF;
		LedBuff[2]=0xF0;
	}
	
	HC595_Send_Multi_Byte(LedBuff,3,T_LED);
}
void SetDisplayType(uint8_t type)
{
	DisType = type;
}
//����ܶ�̬��ʾ
/******************************************************************************
******type  0ʵʱƵ�� 1����Ƶ�� 2����Ƶ�� 3��ֵ
******data 
******point С����
******flush
******************************************************************************/
void DigTubeDisplay()
{
	static uint8_t step = 0;
	static uint16_t counter = 0;
	uint8_t dig[4] = {0};
	switch(DisType)
	{
		case LOGO://��ʾ��������
			dig[3] = AsciiCode[0]; //U
			dig[2] = AsciiCode[1]; //-
			dig[1] = AsciiCode[2]|0x20; //2.
			dig[0] = AsciiCode[3];   //1
			break;
		case FREQUENCY://Ƶ����ʾ
			dig[3] =  NumCode[gfrequency/1000];
			dig[2] =	(NumCode[gfrequency/100%10]| 0x20);//С����
			dig[1] = NumCode[gfrequency/10%10];
			dig[0] = NumCode[gfrequency%10];
			break;
		case UP_LIMIT:
			dig[3] =  0x9E;//H
			dig[2] =	NumCode[DispPara.Hfreq/10000];
			dig[1] = (NumCode[DispPara.Hfreq/1000%10]| 0x20);//С����
			dig[0] = NumCode[DispPara.Hfreq/100%10];
			break;
		case DOWN_LIMIT:
			dig[3] =  0xC2;//L
			dig[2] =	NumCode[DispPara.Lfreq/10000] ;
			dig[1] = (NumCode[DispPara.Lfreq/1000%10]| 0x20);//С����
			dig[0] = NumCode[DispPara.Lfreq/100%10];
			break;
		case ALARM:
			dig[3] =  0xCB;//E
			dig[2] =	0x08;//- ;
			dig[1] = NumCode[AlarmCode/10];//С����
			dig[0] = NumCode[AlarmCode%10];
			break;
		case AMPLIT://���
			dig[3] =  0x8F;//P
			dig[2] =	(NumCode[DispPara.MaxPwmDyty/1000]);
			dig[1] = NumCode[DispPara.MaxPwmDyty/100%10];
			dig[0] = NumCode[DispPara.MaxPwmDyty/10%10];
			break;
		case TRIGGE_TIME://����ʱ��
			dig[3] =  (NumCode[DispPara.triggeTime/1000]);
			dig[2] =	(NumCode[DispPara.triggeTime/100%10])| 0x20;//С����
			dig[1] = NumCode[DispPara.triggeTime/10%10] ;
			dig[0] = NumCode[DispPara.triggeTime%10];
			break;
    case SCAN_INTV:      //ǿ��ɨ�����
			dig[3] =  0x88;//r
			dig[2] =	(NumCode[DispPara.scanIntv/100]);
			dig[1] = NumCode[DispPara.scanIntv/10%10];
			dig[0] = NumCode[DispPara.scanIntv%10];
			break;
		case SOFT_START_TIM ://������ʱ��
			dig[3] =  0xC3;//C
			dig[2] =	(NumCode[DispPara.softStartTim/100]);//С����
			dig[1] = NumCode[DispPara.softStartTim/10%10];
			dig[0] = NumCode[DispPara.softStartTim%10];
			break;
    case MAX_P://�����
			dig[3] =  0x8F;//C
			dig[2] =	(NumCode[DispPara.MaxP/100]) | 0x20;;//
			dig[1] = NumCode[DispPara.MaxP/10%10];
			dig[0] = NumCode[DispPara.MaxP%10];
			break;
		case END_SET:
			dig[3] =  0x08;//-
			dig[2] =	0x08;//-
			dig[1] =  0x08;//-
			dig[0] =  0x08;//-
			break;
		default:
			break;
	}
	if(FlushCursor == 0)
	{
		counter = 0;
	}
	if(counter++ >200)counter =0;
	switch(step)
	{
		case 0://��ʾ��һλ
			step++;

			TubeBuff[0] = dig[0];
			TubeBuff[1] = 0xFE;
			if(FlushCursor==1 && counter > 100)
			{
				TubeBuff[0]=0;
			}
			HC595_Send_Multi_Byte(TubeBuff,2,T_DTBUE);
		break;
		case 1://��ʾ�ڶ�λ
			step++;
			TubeBuff[0] = dig[1];
			TubeBuff[1] = 0xFD;
			if(FlushCursor==2 && counter > 100)
			{
				TubeBuff[0]=0;
			}
			HC595_Send_Multi_Byte(TubeBuff,2,T_DTBUE);
		break;
		case 2://��ʾ����λ
			step++;
			TubeBuff[0] = dig[2];
			TubeBuff[1] = 0xFB;
			if(FlushCursor==3 && counter > 100)
			{
				TubeBuff[0]=0;
			}
			HC595_Send_Multi_Byte(TubeBuff,2,T_DTBUE);
		break;
		case 3://��ʾ����λ
			step=0;
			TubeBuff[0] = dig[3];
			TubeBuff[1] = 0xF7;
			if(FlushCursor==4 && counter > 100)
			{
				TubeBuff[0]=0;
			}
			HC595_Send_Multi_Byte(TubeBuff,2,T_DTBUE);
		break;
		default:
			break;
	}
}
void FlushCursorIncreas()
{
	FlushCursor++;
	if(FlushCursor >= 4)
	{
		FlushCursor = 1;
	}
}
//������������ר��
void S_FlushCursorIncreas()
{
	FlushCursor++;
	if(FlushCursor >= 5)
	{
		FlushCursor = 1;
	}
}
void SysLedRun()
{
	Vout = getRealAdcValue(DTECTOR);
	ledDisplay();
//	DigTubeDisplay();
	
}

void  DisplayTask_Creat(void)
{
	DisType = LOGO;
	gTaskCfgMgEvent.CreatTask(SysLedRun,8,5,Normal_Type);
}
