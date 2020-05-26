#include "Bsp_Flash.h"
#include "stm32f10x.h"
#include "MotionTask.h"
#define PageSize 1024
SysConfig_t DispPara;
SysConfig_t SysConfig={20200,//最高频率20.2
                       19500,//最低频率19.2
											 200,//最大功率30%
												10,//软启动时间
												20,//扫频步距3Hz
												0,//强制触发时间，0表示接触触发
											 250,//最大报警功率
                      };

u8 flash_write(u32 StartAddr,u32 *p_data,u32 size)
{	
	volatile FLASH_Status FLASHStatus;	
	u32 EndAddr=StartAddr+size*4;	
	vu32 NbrOfPage = 0;	
	u32 EraseCounter = 0x0, Address = 0x0;
	int i;
	int MemoryProgramStatus=1;
	//?????
	FLASH_Unlock();          //????
	NbrOfPage=((EndAddr-StartAddr)>>10)+1;	//????????	//????????
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	//??
	FLASHStatus=FLASH_COMPLETE;
	for(EraseCounter=0;(EraseCounter<NbrOfPage)&&(FLASHStatus==FLASH_COMPLETE);EraseCounter++)
	{		
		FLASHStatus=FLASH_ErasePage(StartAddr+(PageSize*EraseCounter));	
	}
	//?????
	Address = StartAddr;
	i=0;
	while((Address<EndAddr)&&(FLASHStatus==FLASH_COMPLETE))	
	{	
		FLASHStatus=FLASH_ProgramWord(Address,p_data[i++]);
		Address=Address+4;
	}
	//
	Address = StartAddr;
	i=0;
	while((Address < EndAddr) && (MemoryProgramStatus != 0))
	{	
		if((*(vu32*) Address) != p_data[i++])
		{		MemoryProgramStatus = 0;
				return 1;
		}
		Address += 4;
	}
	return 0;
}

int flash_read(u32 StartAddr,u32 *p_data,u32 size)
{
	u32 EndAddr=StartAddr+size*4;
	int MemoryProgramStatus=1;
	u32 Address = 0x0;
	int i=0;
	Address = StartAddr;
	while((Address < EndAddr) && (MemoryProgramStatus != 0))
	{
		p_data[i++]=(*(vu32*) Address);
		Address += 4;	
	}
	return 0;
}


uint8_t falshCheck()
{
	SysConfig_t sys;
	flash_read (SYSTEM_CONFIG_ADDR,(uint32_t *)(&sys),sizeof(sys)/4);
	if(sys.Hfreq == 0xffffffff)
	{
		flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)(&SysConfig),sizeof(SysConfig)/4);
	}
	else
	{
		memcpy(&SysConfig,&sys,sizeof(SysConfig_t));
	}
	memcpy(&DispPara,&SysConfig,sizeof(SysConfig_t));
}

