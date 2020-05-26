#include "Bsp_Key.h"
#include "stm32f10x.h"

#define _74H165_CP_H  (GPIO_SetBits(GPIOB,GPIO_Pin_9)) //CLK
#define _74H165_CP_L  (GPIO_ResetBits(GPIOB,GPIO_Pin_9))

#define _74H165_PL_H   (GPIO_SetBits(GPIOB,GPIO_Pin_8)) //EN
#define _74H165_PL_L   (GPIO_ResetBits(GPIOB,GPIO_Pin_8))

#define _74H165_GetData   (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))

struct keyboard5X7
{
	unsigned char   KeyOld;
	unsigned short  KeyTime;        //????;?????
	unsigned char   KeyCode;        //????;???
	unsigned char   KeyReadReady;   //???????
	unsigned char   KeyReading;     //????;??????????
}KeyBoard;

static int8_t KeyBsp_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	  

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;				    
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;				    
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;				    
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	return 0;
}

uint8_t KeyBsp_74ls165Read(void)
{
	uint8_t i;
	uint8_t key = 0;
	
	_74H165_PL_L;
	
	_74H165_CP_L;
	
	__NOP();
	
	__NOP();
	
	_74H165_PL_H;
	
	__NOP();
	
	for(i=0;i<8;i++)
	{	  
	    _74H165_CP_L;
		
		  __NOP();
		
		  if(0x00 == _74H165_GetData)
		  {
				key |= 0x01<<i;
		  }
		  else
		  {
				key &= ~(0x01<<i);
		  }	

			_74H165_CP_H;		

			__NOP();
	}
	
	return (key&0xF8);
}

int8_t KeyBsp_KeyRead(uint8_t byTick)
{
    uint8_t KeyUpdate = 0;
		KeyUpdate = KeyBsp_74ls165Read();
		if(KeyUpdate != 0 && KeyUpdate != 0x08 && KeyUpdate != 0x10 && KeyUpdate != 0x20 && KeyUpdate != 0x40 && KeyUpdate != 0x80)
		return 0;
		if(KeyUpdate == KeyBoard.KeyOld)    	//键一直被按着或者没有按键动作
		{
			if(KeyUpdate!=0)				//键一直被按着            
      {
         KeyBoard.KeyTime ++;
         if(KeyBoard.KeyTime>=1000)	
				 {
					 KeyBoard.KeyTime = 1000;
					 if(KeyUpdate == 0x08)
					 {
						  KeyBoard.KeyCode = 0x06;
							KeyBoard.KeyReadReady = 1;
					 }
				 }
				if(KeyUpdate != 0x08)
				{
					if( KeyBoard.KeyTime==4 )   //有效按键
					{
						KeyBoard.KeyCode = KeyBoard.KeyOld;
						KeyBoard.KeyOld  = KeyUpdate;
						KeyBoard.KeyReadReady = 1;
					}
			  }
      }
		}
		else	//键刚被按下或者刚被释放
    {		
			if(KeyBoard.KeyOld == 0x08)
			{
				if((KeyBoard.KeyTime >= 4 ) && (KeyBoard.KeyTime <500))
				{
				  KeyBoard.KeyCode = 0x08;
					KeyBoard.KeyReadReady = 1;
				}
			}
			KeyBoard.KeyOld  = KeyUpdate;
			KeyBoard.KeyTime = 0;			
    }
		return 0;
}


int8_t KeyBsp_tick(uint8_t byTick)
{
		static uint8_t byTimer = 0;
	
	  byTimer += byTick;
	  
	  if(byTimer >= 5)
		{
			KeyBsp_KeyRead(byTick);
			byTimer = 0;
		}
		
		return 0;
}

uint8_t KeyBsp_read(void)
{

	if(KeyBoard.KeyReadReady == 0)
	{
		return 0;
	}
	else {
	  KeyBoard.KeyReadReady = 0;
		return KeyBoard.KeyCode;
	}
}

Bsp_KeyMgEvent_STR gKeyMgEvent = 
{
	KeyBsp_Init,
	KeyBsp_tick,
	KeyBsp_read
};
void delaynop(uint8_t n)
{
	while(n)
	{
		__NOP();
		n--;
	}
}
