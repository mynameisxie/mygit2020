#define Olcd_Task 1
#include  "MotionTask.h"
#include  "DouDirBlower.h"
#include  "SglDirBlower.h"
#include  "Windpusher.h"
#include  "Bsp_OsTask.h"
#include  "Bsp_Di.h"
#include  "Bsp_Do.h"
#include  "Bsp_Key.h"
#include  "Bsp_Uart.h"
#include  "OlcdTask.h"
#include  "Bsp_Key.h"
#include  "Bsp_lcd.h"
#include  "Bsp_Flash.h"
#include  "MotionTask.h"
#include  "MbSlave.h"
#define UP           0x80
#define DOWN         0x40
#define LEFT         0x20
#define RIGHT        0x10
#define MENU         0x08
#define LONG_MENU    6
extern uint8_t OpenWindDo[2]; 
extern uint8_t ClosWindDo[2]; 
extern uint8_t OpenWindDi[2]; 
extern uint8_t ClosWindDi[2]; 
uint8_t getKeyValue(void);
uint8_t KeyVlue = 0;
uint32_t flag = 0;
static uint8_t adress = 1;
static uint16_t timeout = 30;
int8_t baiw,shiw,gew;	
static uint8_t keyLeftFunChg[4] = {0};
static uint8_t keyRightFunChg[4] = {0};
void DeviceSwitch(void);
void dealInMainMenu(uint8_t keyvl);
void dealInModifyMenu(uint8_t keyvl);
void dealInAddrStMenu(uint8_t keyvl);
void dealInDevcMenu(uint8_t keyvl);
void dealInBaudMenu(uint8_t keyvl);
void dealInTimeStMenu(uint8_t keyvl);
void dealInVrMenu(uint8_t keyvl);
void dealInCfmMenu(uint8_t keyvl);
void ShowDevState(uint8_t Di,uint8_t Do);
void ShowDevSelct(uint8_t cnt);
void showMenuSelct(uint8_t cnt);
void showBaudSelct(uint8_t cnt);
void showDevSelct(uint8_t cnt);
void showAddrSelct(uint8_t cnt);
void showTimeSelct(uint8_t cnt);
void showCfmDev(uint8_t cnt);
void ShowDevStateSgl(uint8_t Di, uint8_t Do);
int8_t setHmiEvent(uint8_t chg, uint8_t event);
void OlcdTask(void)
{

	static uint16_t tiomeout = 0;
	KeyVlue = gKeyMgEvent.read();
	
	if(KeyVlue != 0)
	{
	  tiomeout = 0;
	}
	else
	{
		if(tiomeout < 300)
		{
			  tiomeout++;

		}
		else if(tiomeout == 300)
		{
			MenuPoint = &MainMenu;
			MenuPoint->init = 0;		
			tiomeout++;
		}
		else
		{
		  tiomeout = 400;
		}
	}
	switch(KeyVlue)
	{
		case UP:
			if(MenuPoint == &StAddrMenu)
			{
				if(MenuPoint->msg == 0)
				{
					if(++baiw > 9)baiw = 0;
					if(baiw*100+shiw*10+gew > 247 )baiw = 0;
				}
				else if(MenuPoint->msg == 1)
				{
					if(++shiw > 9)shiw = 0;
					if(baiw*100+shiw*10+gew > 247 )shiw = 0;

				}
				else if(MenuPoint->msg == 2)
				{
				  if(++gew > 9)gew = 0;
          if(baiw*100+shiw*10+gew > 247 )gew = 0;
				}
				adress = baiw*100+shiw*10+gew;
			}
			else if(MenuPoint == &StTimeMenu)
			{
			  if(MenuPoint->msg == 0)
				{
					if(++baiw > 9)baiw = 0;
				}
				else if(MenuPoint->msg == 1)
				{
					if(++shiw > 9)shiw = 0;
				}
				else if(MenuPoint->msg == 2)
				{
				  if(++gew > 9)gew = 0;
				}
				timeout = baiw*100+shiw*10+gew;
				if(timeout == 0) timeout = 1;
			}
			//
			else if(MenuPoint == &ModifyMenu)
			{
				if(SysCfg.type == SGL_DIRBLOWER || SysCfg.type == NORMAL_IO)
				{
					setHmiEvent(ModifyMenu.msg, 1);
				}
				else{
					if(--MenuPoint->msg < 0)
					{
						MenuPoint->msg = MenuPoint->mux-1;
					}
				}
			}
			else if(MenuPoint != &CfmDevMenu)
			{
				if(--MenuPoint->msg < 0)
				{
					MenuPoint->msg = MenuPoint->mux-1;
				}
			}
			break;
		case DOWN:
			if(MenuPoint == &StAddrMenu)
			{
				if(MenuPoint == &StAddrMenu)
				{
					if(MenuPoint->msg == 0)
					{
						if(--baiw < 0)baiw = 9;
						if(baiw*100+shiw*10+gew > 247 )baiw = 0;
					}
					else if(MenuPoint->msg == 1)
					{
						if(--shiw < 0)shiw = 9;
						if(baiw*100+shiw*10+gew > 247 )shiw = 0;
					}
					else if(MenuPoint->msg == 2)
					{
						if(--gew < 0)gew = 9;
						if(baiw*100+shiw*10+gew > 247 )gew = 0;
					}
					adress = baiw*100+shiw*10+gew;
				}
			}
			else if(MenuPoint == &StTimeMenu)
			{
				if(MenuPoint->msg == 0)
				{
					if(--baiw < 0)baiw = 9;
				}
				else if(MenuPoint->msg == 1)
				{
					if(--shiw < 0)shiw = 9;
				}
				else if(MenuPoint->msg == 2)
				{
					if(--gew < 0)gew = 9;
				}
				timeout = baiw*100+shiw*10+gew;
				if(timeout == 0) timeout = 1;
			}
			else if(MenuPoint == &ModifyMenu)
			{
				if(SysCfg.type == SGL_DIRBLOWER || SysCfg.type == NORMAL_IO)
				{
					setHmiEvent(ModifyMenu.msg, 3);
				}
				else
				{
					if(++MenuPoint->msg >= MenuPoint->mux)
					{
						MenuPoint->msg = 0;
					}
				}
			}
			else if(MenuPoint != &CfmDevMenu)
			{
				if(++MenuPoint->msg >= MenuPoint->mux)
				{
					MenuPoint->msg = 0;
				}
			}
			break;
		case LEFT:
			if(MenuPoint == &SyscfgMenu || MenuPoint == &StBaudMenu || MenuPoint == &StAddrMenu || MenuPoint == &StDevcMenu || MenuPoint == &StTimeMenu || MenuPoint ==&ShowVrMenu)
			{
				if(MenuPoint->ParentMenus != NULL)
				{
					MenuPoint = MenuPoint->ParentMenus;
					MenuPoint->init = 0;
				}
			}
			else if(MenuPoint == &ModifyMenu)
			{
				if(SysCfg.type == SGL_DIRBLOWER || SysCfg.type == NORMAL_IO)
				{
					if(--MenuPoint->msg < 0)
					{
						MenuPoint->msg = MenuPoint->mux-1;
					}
				}
				else
				{
					uint8_t equip = Controller.Sys->type;
					uint8_t state = 0;
					switch(equip)
					{
						case WIND_PUSHER:
							state = getPusherState(&Controller,ModifyMenu.msg);
							if(state == WIND_CLOSED || state == WIND_CLOSING || state == WIND_PAUSE )
							{
								setHmiEvent(ModifyMenu.msg,1);
							}
							else if(state == WIND_OPENING)
							{
								setHmiEvent(ModifyMenu.msg,3);
							}
							break;
						case DOU_DIRBLOWER:
							state = getPusherState(&Controller,ModifyMenu.msg);
							if(state == DBL_NGTV_RUNING || state == DBL_STOP)
							{
								setHmiEvent(ModifyMenu.msg,1);
							}
							else if(state == DBL_PSTV_RUNING)
							{
								setHmiEvent(ModifyMenu.msg,3);
							}
							break;
						default:
							break;
					}
				}
				
			}
			else if(MenuPoint == &CfmDevMenu)
			{
				if(--MenuPoint->msg < 0)
				{
					MenuPoint->msg = MenuPoint->mux-1;
				}				
			}
			break;
		case RIGHT:
			if(MenuPoint == &SyscfgMenu)
			{
			  if(MenuPoint->ChildrenMenus[MenuPoint->msg] != NULL)
				{
				  MenuPoint = MenuPoint->ChildrenMenus[MenuPoint->msg];
					MenuPoint->init = 0;
				}
			}
			else if(MenuPoint == &StAddrMenu || MenuPoint == &StTimeMenu)
			{
			   if(++MenuPoint->msg >= MenuPoint->mux)
				 {
				   MenuPoint->msg = 0;
				 }
			}
			else if(MenuPoint == &ModifyMenu)
			{
				
				if(SysCfg.type == SGL_DIRBLOWER || SysCfg.type == NORMAL_IO)
				{
					if(++MenuPoint->msg >= MenuPoint->mux)
					{
						MenuPoint->msg = 0;
					}
				}
				else
				{
					uint8_t equip = Controller.Sys->type;
					uint8_t state = 0;
					switch(equip)
					{
						case WIND_PUSHER:
							state = getPusherState(&Controller,ModifyMenu.msg);
							if(state == WIND_OPENED || state == WIND_OPENING || state == WIND_PAUSE )
							{
								setHmiEvent(ModifyMenu.msg,2);
							}
							else if(state == WIND_CLOSING)
							{
								setHmiEvent(ModifyMenu.msg,3);
							}
							break;
						case DOU_DIRBLOWER:
							state = getPusherState(&Controller,ModifyMenu.msg);
							if(state == DBL_PSTV_RUNING || state == DBL_STOP)
							{
								setHmiEvent(ModifyMenu.msg,2);
							}
							else if(state == DBL_NGTV_RUNING)
							{
								setHmiEvent(ModifyMenu.msg,3);
							}
							break;
						default:
							break;
					}
				}
			}
			else if(MenuPoint == &CfmDevMenu)
			{
				if(++MenuPoint->msg >= MenuPoint->mux)
				{
					MenuPoint->msg = 0;
				}				
			}
			break;
		case MENU:
			//进入修改状态
			if(MenuPoint == &MainMenu)
			{
				if(MenuPoint->ChildrenMenus[0] != NULL)
				{
					MenuPoint = MenuPoint->ChildrenMenus[0];
					MenuPoint->init = 0;
				}
			}
			//退出修改状态
			else if(MenuPoint == &ModifyMenu)
			{
				if(MenuPoint->ParentMenus != NULL)
				{
					MenuPoint = MenuPoint->ParentMenus;
					MenuPoint->init = 0;
				}
			}
			else if(MenuPoint == &SyscfgMenu)
			{
				MenuPoint = MenuPoint->ChildrenMenus[MenuPoint->msg];
				MenuPoint->init = 0;
			}
			else if(MenuPoint == &StAddrMenu)
			{
				if(adress != SysCfg.addr)
				{
					SysCfg.addr = adress;
					SysCfg.crc  = (uint16_t)(SysCfg.addr+SysCfg.baud+SysCfg.type+SysCfg.rback+SysCfg.tmout);
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
					flash_write(SYSTEM_CONFIG_ADDR2,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
					//设置通讯地址
					gMbSlaveMgEvent.setAddr((uint8_t)SysCfg.addr);

				}
			  MenuPoint = MenuPoint->ParentMenus;
				MenuPoint->init = 0;
			}
			else if(MenuPoint == &StTimeMenu)
			{
				if(timeout != SysCfg.tmout)
				{
					if(timeout == 0)
						timeout = 30;
					SysCfg.tmout = timeout;
					SysCfg.crc  = (uint16_t)(SysCfg.addr+SysCfg.baud+SysCfg.type+SysCfg.rback+SysCfg.tmout);
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
					flash_write(SYSTEM_CONFIG_ADDR2,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
				}
			  MenuPoint = MenuPoint->ParentMenus;
				MenuPoint->init = 0;			
			}
			else if(MenuPoint == &StDevcMenu)
			{
//			  if(StDevcMenu.msg != SysCfg.type-1)
//				{
//					SysCfg.type = StDevcMenu.msg+1;
//					SysCfg.crc  = (uint16_t)(SysCfg.addr+SysCfg.baud+SysCfg.type+SysCfg.rback+SysCfg.tmout);
//					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
//					flash_write(SYSTEM_CONFIG_ADDR2,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
//					DeviceSwitch();
//				}
			  MenuPoint = MenuPoint->ChildrenMenus[StDevcMenu.msg];
				MenuPoint->init = 0;
			}
			else if(MenuPoint == &StBaudMenu)
			{
				if(StBaudMenu.msg != SysCfg.baud)
				{
				  SysCfg.baud = StBaudMenu.msg;
					SysCfg.crc  = (uint16_t)(SysCfg.addr+SysCfg.baud+SysCfg.type+SysCfg.rback+SysCfg.tmout);
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);				
					flash_write(SYSTEM_CONFIG_ADDR2,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);

					//设置通讯波特率
					gSerialMgEvent.setbaud(2,SysCfg.baud);	
				}
				MenuPoint = MenuPoint->ParentMenus;
				MenuPoint->init = 0;
			}
			else if(MenuPoint == &CfmDevMenu)
			{
				if(MenuPoint->msg == 0)
				{	
					if(StDevcMenu.msg == 3)
					{
						SysCfg.type = 4;
						SysCfg.rback = 1;
						SysCfg.crc  = (uint16_t)(SysCfg.addr+SysCfg.baud+SysCfg.type+SysCfg.rback+SysCfg.tmout);
						flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
						flash_write(SYSTEM_CONFIG_ADDR2,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
						DeviceSwitch();
					}
					else if(StDevcMenu.msg == 4)
					{
					  SysCfg.type = 4;
						SysCfg.rback = 0;
						SysCfg.crc  = (uint16_t)(SysCfg.addr+SysCfg.baud+SysCfg.type+SysCfg.rback+SysCfg.tmout);
						flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
						flash_write(SYSTEM_CONFIG_ADDR2,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
						DeviceSwitch();
					}	
					else if(StDevcMenu.msg != SysCfg.type-1)
					{
						SysCfg.type = StDevcMenu.msg+1;
						SysCfg.rback = 1;
						SysCfg.crc  = (uint16_t)(SysCfg.addr+SysCfg.baud+SysCfg.type+SysCfg.rback+SysCfg.tmout);
						flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
						flash_write(SYSTEM_CONFIG_ADDR2,(uint32_t *)&SysCfg,sizeof(SysCfg_STR)/4);
						DeviceSwitch();
					}
					MenuPoint = MenuPoint->ParentMenus;
					MenuPoint->init = 0;
				}

				else if(MenuPoint->msg == 1)
				{

					MenuPoint = &StDevcMenu;
					MenuPoint->init = 0;

				}
			}
			break;
		
		case LONG_MENU:
			//进入系统设置
			if(MenuPoint == &MainMenu)
			{
			  if(MenuPoint->ChildrenMenus[1] != NULL)
				{
				  MenuPoint = MenuPoint->ChildrenMenus[1];
					MenuPoint->init = 0;
				}
			}
		  break;
		
		default:
			
			break;
	}
	KeyVlue = 0;
	MenuPoint->Subs(KeyVlue);
}

void  OlcdTask_Creat(void)
{
	gTaskCfgMgEvent.CreatTask(OlcdTask,6,200,Normal_Type);
}

void dealInMainMenu(uint8_t keyvl)
{
	
  uint8_t Di;
	uint8_t Do;
	if(MainMenu.init == 0)
	{
	  Fill_RAM(0x00);
		PrintfCustom(MainMenu.Page1String,0,0,0);
		PrintfCustom(MainMenu.Page2String,2,0,0);
		PrintfCustom(MainMenu.Page3String,4,0,0);
		PrintfCustom(MainMenu.Page4String,6,0,0);
		MainMenu.init = 1;
	}
	gDiMgEvent.read(&Di);
	gDoMgEvent.read(&Do);
	ShowDevState(Di,Do);
//	AutoHandFlag = 0;
}
void dealInModifyMenu(uint8_t keyvl)
{
  uint8_t Di;
	uint8_t Do;
	
	if(ModifyMenu.init == 0)
	{
		Fill_RAM(0x00);
		PrintfCustom(ModifyMenu.Page1String,0,0,0);
		PrintfCustom(ModifyMenu.Page2String,2,0,0);
		PrintfCustom(ModifyMenu.Page3String,4,0,0);
		PrintfCustom(ModifyMenu.Page4String,6,0,0);
		
		ModifyMenu.init = 1;
	}
	gDiMgEvent.read(&Di);
	gDoMgEvent.read(&Do);
	ShowDevState(Di,Do);
	ShowDevSelct(ModifyMenu.msg);

}
void dealInSyscfgMenu(uint8_t keyvl)
{
  if(SyscfgMenu.init == 0)
	{
		Fill_RAM(0x00);
	  PrintfCustom(SyscfgMenu.Page1String,0,0,1);
		PrintfCustom(SyscfgMenu.Page2String,2,0,0);
		PrintfCustom(SyscfgMenu.Page3String,4,0,0);
		PrintfCustom(SyscfgMenu.Page4String,6,0,0);
		SyscfgMenu.init = 1;
		SyscfgMenu.msg = 0;
	}
	
	showMenuSelct(SyscfgMenu.msg);
	
}
void dealInBaudMenu(uint8_t keyvl)
{
  if(StBaudMenu.init == 0)
	{
		Fill_RAM(0x00);
	  PrintfCustom(StBaudMenu.Page1String,0,0,0);
		PrintfCustom(StBaudMenu.Page2String,2,0,0);
		PrintfCustom(StBaudMenu.Page3String,4,0,0);
		PrintfCustom(StBaudMenu.Page4String,6,0,0);
		StBaudMenu.init = 1;
		StBaudMenu.msg = SysCfg.baud;	
	}
	showBaudSelct(StBaudMenu.msg);
}
void dealInAddrStMenu(uint8_t keyvl)
{
  if(StAddrMenu.init == 0)
	{
		Fill_RAM(0x00);
	  PrintfCustom(StAddrMenu.Page1String,0,0,0);
		PrintfCustom(StAddrMenu.Page2String,2,0,0);
		PrintfCustom(StAddrMenu.Page3String,4,0,1);
		PrintfCustom(StAddrMenu.Page4String,6,0,0);
		StAddrMenu.init = 1;
		StAddrMenu.msg = 0;
		adress = SysCfg.addr;
	}
	showAddrSelct(StAddrMenu.msg);
}

void dealInDevcMenu(uint8_t keyvl)
{
  if(StDevcMenu.init == 0)
	{
		Fill_RAM(0x00);
	  PrintfCustom(StDevcMenu.Page1String,0,0,0);
		PrintfCustom(StDevcMenu.Page2String,2,0,1);
		PrintfCustom(StDevcMenu.Page3String,4,0,0);
		PrintfCustom(StDevcMenu.Page4String,6,0,0);
		StDevcMenu.init = 1;
		if(SysCfg.type == 4)
		{
			if(SysCfg.rback == 0)
			  StDevcMenu.msg = 4;
			else
				StDevcMenu.msg = 3;
		}
		else
		{
			StDevcMenu.msg = SysCfg.type-1;
		}
	}
	showDevSelct(StDevcMenu.msg);
}
void dealInTimeStMenu(uint8_t keyvl)
{
  if(StTimeMenu.init == 0)
	{
		Fill_RAM(0x00);
	  PrintfCustom(StTimeMenu.Page1String,0,0,0);
		PrintfCustom(StTimeMenu.Page2String,2,0,0);
		PrintfCustom(StTimeMenu.Page3String,4,0,0);
		PrintfCustom(StTimeMenu.Page4String,6,0,0);
		StTimeMenu.init = 1;
		StTimeMenu.msg = 0;
		timeout = SysCfg.tmout;
	}
	showTimeSelct(StTimeMenu.msg);
}
void dealInVrMenu(uint8_t keyvl)
{
  if(ShowVrMenu.init == 0)
	{
		Fill_RAM(0x00);
	  PrintfCustom(ShowVrMenu.Page1String,0,0,0);
		PrintfCustom(ShowVrMenu.Page2String,2,0,0);
		PrintfCustom(ShowVrMenu.Page3String,4,0,0);
		PrintfCustom(ShowVrMenu.Page4String,6,0,0);
		ShowVrMenu.init = 1;
		ShowVrMenu.msg = 0;
	}
}
void dealInCfmMenu(uint8_t keyvl)
{
  if(CfmDevMenu.init == 0)
	{
		Fill_RAM(0x00);
	  PrintfCustom(CfmDevMenu.Page1String,0,0,0);
		PrintfCustom(CfmDevMenu.Page2String,2,0,0);
		PrintfCustom(CfmDevMenu.Page3String,4,0,0);
		PrintfCustom(CfmDevMenu.Page4String,6,0,0);
		CfmDevMenu.init = 1;
		CfmDevMenu.msg = 1;
	}
	showCfmDev(CfmDevMenu.msg);
}
void MainMenuInit()
{
	MainMenu.msg = 0;//状态查看
	MainMenu.mux = 2;
	switch(SysCfg.type)
	{
	  case WIND_PUSHER:
			MainMenu.Page1String = "门窗1:按M修改   ";
			MainMenu.Page2String = "开:          :关";
			MainMenu.Page3String = "门窗2:          ";
			MainMenu.Page4String = "开:          :关";			
			break;
	  case DOU_DIRBLOWER:
			MainMenu.Page1String = "风机1:按M修改   ";
			MainMenu.Page2String = "正:          :反";
			MainMenu.Page3String = "风机2:          ";
			MainMenu.Page4String = "正:          :反";			
			break;
	  case SGL_DIRBLOWER:
			MainMenu.Page1String = "单向风机 按M修改";
		  MainMenu.Page2String = "DO0_3:          ";
			MainMenu.Page3String = "DI0_3:          ";
			MainMenu.Page4String = "DI4_7:          ";
			break;
	  case NORMAL_IO:
			MainMenu.Page1String = "通用IO  按M修改 ";
		  MainMenu.Page2String = "DO0_3:          ";
			MainMenu.Page3String = "DI0_3:          ";
			MainMenu.Page4String = "DI4_7:          ";			
			break;
	  default:
			break;
	}

	MainMenu.Subs = dealInMainMenu;
	MainMenu.ParentMenus = NULL;
	MainMenu.ChildrenMenus[0] = &ModifyMenu;
	MainMenu.ChildrenMenus[1] = &SyscfgMenu;
	MainMenu.ChildrenMenus[2] = NULL;
}


void ModifyMenuInit()
{
	ModifyMenu.msg = 0;//设置动作
	if(SysCfg.type == WIND_PUSHER || SysCfg.type == DOU_DIRBLOWER)
		ModifyMenu.mux = 2;
	else
		ModifyMenu.mux = 4;
	switch(SysCfg.type)
	{
	  case WIND_PUSHER:
			ModifyMenu.Page1String = "门窗1:按M退出    ";
			ModifyMenu.Page2String = "开左        右关";
			ModifyMenu.Page3String = "门窗2:          ";
			ModifyMenu.Page4String = "开左        右关";			
			break;
	  case DOU_DIRBLOWER:
			ModifyMenu.Page1String = "风机1:按M退出    ";
			ModifyMenu.Page2String = "正左        右反";
			ModifyMenu.Page3String = "风机2:          ";
			ModifyMenu.Page4String = "正左        右反";			
			break;
	  case SGL_DIRBLOWER:
			ModifyMenu.Page1String = "按M退出 上开关下";
		  ModifyMenu.Page2String = "DO0_3:          ";
			ModifyMenu.Page3String = "DI0_3:          ";
			ModifyMenu.Page4String = "DI4_7:          ";
			break;
	  case NORMAL_IO:
			ModifyMenu.Page1String = "按M退出 上开关下";
		  ModifyMenu.Page2String = "DO0_3:          ";
			ModifyMenu.Page3String = "DI0_3:          ";
			ModifyMenu.Page4String = "DI4_7:          ";			
			break;
	  default:
			break;
	}
	ModifyMenu.Subs = dealInModifyMenu;
	ModifyMenu.ParentMenus = &MainMenu;
	ModifyMenu.ChildrenMenus[0] = NULL;
	ModifyMenu.ChildrenMenus[1] = NULL;
	ModifyMenu.ChildrenMenus[2] = NULL;
}


void SyscfgMenuInit()
{
	SyscfgMenu.msg = 0;//系统设置
	SyscfgMenu.mux= 5; 
	SyscfgMenu.Page1String = "1通讯波特率   ";
	SyscfgMenu.Page2String = "2通讯地址     ";
	SyscfgMenu.Page3String = "3控制器类型   ";
	SyscfgMenu.Page4String = "4门窗超时时间 ";
	SyscfgMenu.Subs = dealInSyscfgMenu;
	SyscfgMenu.ParentMenus = &MainMenu;
	SyscfgMenu.ChildrenMenus[0] = &StBaudMenu;
	SyscfgMenu.ChildrenMenus[1] = &StAddrMenu;
	SyscfgMenu.ChildrenMenus[2] = &StDevcMenu;
	SyscfgMenu.ChildrenMenus[3] = &StTimeMenu;
	SyscfgMenu.ChildrenMenus[4] = &ShowVrMenu;
}


void StBaudMenuInit()
{
	StBaudMenu.msg = 0;//通信波特率设置
	StBaudMenu.mux = 5;
	StBaudMenu.Page1String = "通讯波特率设置";
	StBaudMenu.Page2String = "115200  57600";
	StBaudMenu.Page3String = "38400   19200";
	StBaudMenu.Page4String = "9600";
	StBaudMenu.Subs = dealInBaudMenu;
	StBaudMenu.ParentMenus = &SyscfgMenu;
	StBaudMenu.ChildrenMenus[0] = NULL;
	StBaudMenu.ChildrenMenus[1] = NULL;
	StBaudMenu.ChildrenMenus[2] = NULL;
}
void StAddrMenuInit()
{
	StAddrMenu.msg = 0;//通信地址设置
	StAddrMenu.mux = 3;
	StAddrMenu.Page1String = "控制器地址设置:";
	StAddrMenu.Page2String = "               ";
	StAddrMenu.Page3String = "001";
	StAddrMenu.Page4String = "设置范围: 1_247";
	StAddrMenu.Subs = dealInAddrStMenu;
	StAddrMenu.ParentMenus = &SyscfgMenu;
	StAddrMenu.ChildrenMenus[0] = NULL;
	StAddrMenu.ChildrenMenus[1] = NULL;
	StAddrMenu.ChildrenMenus[2] = NULL;
}
void StDevcMenuInit()
{
	StDevcMenu.msg = 0;//控制器类型设置
	StDevcMenu.mux = 5;
	StDevcMenu.Page1String = "控制器类型设置: ";
	StDevcMenu.Page2String = "通用IO控制器    ";
	StDevcMenu.Page3String = "双向风机控制器  ";
	StDevcMenu.Page4String = "单向风机控制器  ";
	StDevcMenu.Subs = dealInDevcMenu;
	StDevcMenu.ParentMenus = &SyscfgMenu;
	StDevcMenu.ChildrenMenus[0] = &CfmDevMenu;
	StDevcMenu.ChildrenMenus[1] = &CfmDevMenu;
	StDevcMenu.ChildrenMenus[2] = &CfmDevMenu;
	StDevcMenu.ChildrenMenus[3] = &CfmDevMenu;
	StDevcMenu.ChildrenMenus[4] = &CfmDevMenu;
}
void StTimeMenuInit()
{
	StTimeMenu.msg = 0;//通信地址设置
	StTimeMenu.mux = 3;
	StTimeMenu.Page1String = "门窗超时设置: ";
	StTimeMenu.Page2String = "               ";
	StTimeMenu.Page3String = "030   秒";
	StTimeMenu.Page4String = "设置范围: 1_999";
	StTimeMenu.Subs = dealInTimeStMenu;
	StTimeMenu.ParentMenus = &SyscfgMenu;
	StTimeMenu.ChildrenMenus[0] = NULL;
	StTimeMenu.ChildrenMenus[1] = NULL;
	StTimeMenu.ChildrenMenus[2] = NULL;
}
void ShowVrMenuInit()
{
	ShowVrMenu.msg = 0;//通信地址设置
	ShowVrMenu.mux = 3;
	ShowVrMenu.Page1String = "软件版本: ";
	ShowVrMenu.Page2String = "                ";
	ShowVrMenu.Page3String = version;
	ShowVrMenu.Page4String = "                ";
	ShowVrMenu.Subs = dealInVrMenu;
	ShowVrMenu.ParentMenus = &SyscfgMenu;
	
	
	
	
	
	
	
	ShowVrMenu.ChildrenMenus[0] = NULL;
	ShowVrMenu.ChildrenMenus[1] = NULL;
	ShowVrMenu.ChildrenMenus[2] = NULL;
}
void CfmDevMenuInit()
{
	CfmDevMenu.msg = 0;//确认控制器类型设置
	CfmDevMenu.mux = 2;
	CfmDevMenu.Page1String = "                ";
	CfmDevMenu.Page2String = "  修改设备类型? ";
	CfmDevMenu.Page3String = "     是  否     ";
	CfmDevMenu.Page4String = "                ";
	CfmDevMenu.Subs = dealInCfmMenu;
	CfmDevMenu.ParentMenus = &SyscfgMenu;
	CfmDevMenu.ChildrenMenus[0] = NULL;
	CfmDevMenu.ChildrenMenus[1] = NULL;
	CfmDevMenu.ChildrenMenus[2] = NULL;
}

void OlcdTaskInit(void)
{
	lcd_init();
  MainMenuInit();
	ModifyMenuInit();
	SyscfgMenuInit();
	StBaudMenuInit();
	StAddrMenuInit();
	StDevcMenuInit();
	StTimeMenuInit();
	ShowVrMenuInit();
	CfmDevMenuInit();
}
void showDevSelct(uint8_t cnt)
{
	uint8_t slt;
	if(cnt == 3)
	{
		PrintfCustom("有反馈门窗控制器",2,0,1);
		
		PrintfCustom("无反馈门窗控制器",4,0,0);
		PrintfCustom("                ",6,0,0);		
	}
	else if(cnt == 4)
	{
		PrintfCustom("有反馈门窗控制器",2,0,0);
		
		PrintfCustom("无反馈门窗控制器",4,0,1);
		PrintfCustom("                ",6,0,0);		
	}
	else
	{
		slt = (0x01 << cnt)&0x01;
		PrintfCustom(StDevcMenu.Page2String,2,0,slt);
		slt = (0x01 << cnt)&0x02;
		PrintfCustom(StDevcMenu.Page3String,4,0,slt);
		slt = (0x01 << cnt)&0x04;
		PrintfCustom(StDevcMenu.Page4String,6,0,slt);
	}
}
void ShowDevState(uint8_t Di,uint8_t Do)
{
	uint8_t slt;
	static uint8_t cnt = 0,cnt2 = 0;
	switch(SysCfg.type)
	{
		case WIND_PUSHER:
		case DOU_DIRBLOWER:
			//设备1开关状态
			if(CtrlErr.pusher[1] == 1 && SysCfg.rback == 0)
			{
				Di |= OpenWindDi[1]; 
			}
			else if(CtrlErr.pusher[1] == 2 && SysCfg.rback == 0)
			{
				Di |= ClosWindDi[1]; 
			}
			else if(SysCfg.rback == 0)
			{
			  Di &= ~OpenWindDi[1]; 
				Di &= ~ClosWindDi[1]; 
			}
			
			if(CtrlErr.pusher[0] == 1 && SysCfg.rback == 0)
			{
				Di |= OpenWindDi[0]; 
			}
			else if(CtrlErr.pusher[0] == 2 && SysCfg.rback == 0)
			{
				Di |= ClosWindDi[0]; 
			}
			else if(SysCfg.rback == 0)
			{
			  Di &= ~OpenWindDi[0]; 
				Di &= ~ClosWindDi[0]; 
			}
			
			if(Di & OpenWindDi[0])
			{
				PrintfCustom("拳",2,32,0);
			}
			else
			{
				PrintfCustom("圈",2,32,0);
			}
			if(Di & ClosWindDi[0])
			{
				PrintfCustom("拳",2,80,0);
			}
			else
			{
				PrintfCustom("圈",2,80,0);
			}
			//设备2开关状态
			if(Di & OpenWindDi[1])
			{
				PrintfCustom("拳",6,32,0);
			}
			else
			{
				PrintfCustom("圈",6,32,0);
			}
			if(Di & ClosWindDi[1])
			{
				PrintfCustom("拳",6,80,0);
			}
			else
			{
				PrintfCustom("圈",6,80,0);
			}
			//设备1开关输出
			if(Do& OpenWindDo[0])
			{
				PrintfCustom("腥",2,48,0);
			}
			else
			{
				if(SysCfg.type == WIND_PUSHER && CtrlErr.pusher[0] == 4 && SysCfg.rback == 1)
				{
				  PrintfCustom("错",2,48,0);
				}
				else
				{
					PrintfCustom("  ",2,48,0);
				}
			}
			if(Do & ClosWindDo[0])
			{
				PrintfCustom("腥",2,64,0);
			}
			else
			{
				if(SysCfg.type == WIND_PUSHER && CtrlErr.pusher[0] == 5 && SysCfg.rback == 1)
				{
				  PrintfCustom("错",2,64,0);
				}
				else
				{
					PrintfCustom("  ",2,64,0);
				}
			}
			//设备2开关输出
			if(Do& OpenWindDo[1])
			{
				PrintfCustom("腥",6,48,0);
			}
			else
			{
				if(SysCfg.type == WIND_PUSHER && CtrlErr.pusher[1] == 4 &&SysCfg.rback == 1)
				{
				  PrintfCustom("错",6,48,0);
				}
				else
				{
					PrintfCustom("  ",6,48,0);
				}
			}
			if(Do & ClosWindDo[1])
			{
				PrintfCustom("腥",6,64,0);
			}
			else
			{
				if(SysCfg.type == WIND_PUSHER && CtrlErr.pusher[1] == 5 && SysCfg.rback == 1)
				{
				  PrintfCustom("错",6,64,0);
				}
				else
				{
					PrintfCustom("  ",6,64,0);
				}
			}
			
			if(((SysCfg.type == WIND_PUSHER)&&(Controller.Mcnt[0] || Controller.Pcnt[0])) || \
				  ((SysCfg.type == DOU_DIRBLOWER)&&(Controller.Pcnt[0])) )
			{
				if(cnt++ >= 8)cnt =1;
				if(cnt<4)
					PrintfCustom("原",0,112,0);
				else
					PrintfCustom("  ",0,112,0);
			}
			else if(((SysCfg.type == DOU_DIRBLOWER)&&(SysCfg.rback == 1)&&(CtrlErr.dblower[0] == 4 || CtrlErr.dblower[0] == 5)))
			{
			   PrintfCustom("错",0,112,0);
			}
			else
			{
				PrintfCustom("  ",0,112,0);
			}
			if(((SysCfg.type == WIND_PUSHER)&&(Controller.Mcnt[1] || Controller.Pcnt[1])) || \
				  ((SysCfg.type == DOU_DIRBLOWER)&&(Controller.Pcnt[1])) )
			{
				if(cnt2++ >= 8)cnt2 =1;
				if(cnt2<4)
					PrintfCustom("原",4,112,0);
				else
					PrintfCustom("  ",4,112,0);
			}
			else if(((SysCfg.type == DOU_DIRBLOWER)&&(SysCfg.rback == 1)&&(CtrlErr.dblower[1] == 4 || CtrlErr.dblower[1] == 5)))
			{
			   PrintfCustom("错",4,112,0);
			}
			else
			{
				PrintfCustom("  ",4,112,0);
			}

			break;
		case SGL_DIRBLOWER:
		case NORMAL_IO    :
			if(Di&0x01)//DI0
			{
				PrintfCustom("拳",4,56,0);
			}
			else
			{
				PrintfCustom("圈",4,56,0);
			}
			if(Di&0x02)//DI1
			{
				PrintfCustom("拳",4,72,0);
			}
			else
			{
				PrintfCustom("圈",4,72,0);
			}
			if(Di&0x04)//DI2
			{
				PrintfCustom("拳",4,88,0);
			}
			else
			{
				PrintfCustom("圈",4,88,0);
			}
			if(Di&0x08)//DI3
			{
				PrintfCustom("拳",4,104,0);
			}
			else
			{
				PrintfCustom("圈",4,104,0);
			}
			if(Di&0x10)//DI4
			{
				PrintfCustom("拳",6,56,0);
			}
			else
			{
				PrintfCustom("圈",6,56,0);
			}
			if(Di&0x20)//DI5
			{
				PrintfCustom("拳",6,72,0);
			}
			else
			{
				PrintfCustom("圈",6,72,0);
			}
			if(Di&0x40)//DI6
			{
				PrintfCustom("拳",6,88,0);
			}
			else
			{
				PrintfCustom("圈",6,88,0);
			}
			if(Di&0x80)//DI7
			{
				PrintfCustom("拳",6,104,0);
			}
			else
			{
				PrintfCustom("圈",6,104,0);
			}
			
			slt = (0x01 << ModifyMenu.msg)&0x01;
			if(Do&0x01)//DO0
			{
				if(MenuPoint == &MainMenu)
					PrintfCustom("腥",2,56,0);
				else
					PrintfCustom("开",2,56,slt);
			}
			else
			{
				if(MenuPoint == &MainMenu)
					PrintfCustom("星",2,56,0);
				else
					PrintfCustom("关",2,56,slt);
			}
			slt = (0x01 << ModifyMenu.msg)&0x02;
			if(Do&0x02)//DO1
			{
				if(MenuPoint == &MainMenu)
					PrintfCustom("腥",2,72,0);
				else
					PrintfCustom("开",2,72,slt);
			}
			else
			{
				if(MenuPoint == &MainMenu)
					PrintfCustom("星",2,72,0);
				else
					PrintfCustom("关",2,72,slt);
			}
			slt = (0x01 << ModifyMenu.msg)&0x04;
			if(Do&0x04)//DO2
			{
				if(MenuPoint == &MainMenu)
					PrintfCustom("腥",2,88,0);
				else
					PrintfCustom("开",2,88,slt);
			}
			else
			{
				if(MenuPoint == &MainMenu)
					PrintfCustom("星",2,88,0);
				else
					PrintfCustom("关",2,88,slt);
			}
			slt = (0x01 << ModifyMenu.msg)&0x08;
			if(Do&0x08)//DO3
			{
				if(MenuPoint == &MainMenu)
					PrintfCustom("腥",2,104,0);
				else
					PrintfCustom("开",2,104,slt);
			}
			else
			{
				if(MenuPoint == &MainMenu)
					PrintfCustom("星",2,104,0);
				else
					PrintfCustom("关",2,104,slt);
			}
			break;
		default:
			break;
		
	}
}
void ShowDevStateSgl(uint8_t Di, uint8_t Do)
{

}
void ShowDevSelct(uint8_t cnt)
{
	switch(SysCfg.type)
	{
	  case WIND_PUSHER:
			if(cnt == 1)
			{
				PrintfCustom("门窗1",0,0,0);
				PrintfCustom("门窗2",4,0,1);
			}
			else
			{
				PrintfCustom("门窗1",0,0,1);
				PrintfCustom("门窗2",4,0,0);
			}
			break;
	  case DOU_DIRBLOWER:
			if(cnt == 1)
			{
				PrintfCustom("风机1",0,0,0);
				PrintfCustom("风机2",4,0,1);
			}
			else
			{
				PrintfCustom("风机1",0,0,1);
				PrintfCustom("风机2",4,0,0);
			}		
			break;
	  case SGL_DIRBLOWER:

			break;
	  case NORMAL_IO:

			break;
		
	  default:
			break;
	}
}
void showMenuSelct(uint8_t cnt)
{
	uint8_t slt;
	if(cnt == 4)
	{
		PrintfCustom("5软件版本     ",0,0,1);			
		PrintfCustom("              ",2,0,0);
		PrintfCustom("              ",4,0,0);
		PrintfCustom("              ",6,0,0);			
	}
	else
	{
		slt = (0x01 << cnt)&0x01;
		PrintfCustom(SyscfgMenu.Page1String,0,0,slt);
		slt = (0x01 << cnt)&0x02;
		PrintfCustom(SyscfgMenu.Page2String,2,0,slt);
		slt = (0x01 << cnt)&0x04;
		PrintfCustom(SyscfgMenu.Page3String,4,0,slt);
		slt = (0x01 << cnt)&0x08;
		PrintfCustom(SyscfgMenu.Page4String,6,0,slt);
	}
}
void showBaudSelct(uint8_t cnt)
{
	uint8_t slt;
	slt = (0x01 << cnt)&0x01;
	PrintfCustom("115200",2,0,slt);
	slt = (0x01 << cnt)&0x02;
	PrintfCustom("57600 ",2,64,slt);
	slt = (0x01 << cnt)&0x04;
	PrintfCustom("38400 ",4,0,slt);
	slt = (0x01 << cnt)&0x08;
	PrintfCustom("19200 ",4,64,slt);
	slt = (0x01 << cnt)&0x10;
	PrintfCustom("9600  ",6,0,slt);
}
void showAddrSelct(uint8_t cnt)
{
  uint8_t slt;
	char *anum[10]={"零","一","二","三","四","五","六","七","八","九",};
	if(adress > 247) adress = 247;
	baiw = adress/100;
	shiw = adress/10%10;
	gew = adress%10;
	slt = (0x01 << cnt)&0x01;
  PrintfCustom(anum[baiw],4,0,slt);
	slt = (0x01 << cnt)&0x02;
  PrintfCustom(anum[shiw],4,16,slt);
	slt = (0x01 << cnt)&0x04;
  PrintfCustom(anum[gew],4,32,slt);
}
void showTimeSelct(uint8_t cnt)
{
 uint8_t slt;
	char *anum[10]={"零","一","二","三","四","五","六","七","八","九",};
	if(timeout > 999) timeout = 999;
	baiw = timeout/100;
	shiw = timeout/10%10;
	gew = timeout%10;
	slt = (0x01 << cnt)&0x01;
  PrintfCustom(anum[baiw],4,0,slt);
	slt = (0x01 << cnt)&0x02;
  PrintfCustom(anum[shiw],4,16,slt);
	slt = (0x01 << cnt)&0x04;
  PrintfCustom(anum[gew],4,32,slt);
}
void showCfmDev(uint8_t cnt)
{
	uint8_t slt;
	slt = (0x01 << cnt)&0x01;
	PrintfCustom("是",4,40,slt);
	slt = (0x01 << cnt)&0x02;
	PrintfCustom("否",4,70,slt);
}

void DeviceSwitch()
{
	uint8_t uu = 0;
	uint8_t tmp[4] = {0};
	gDoMgEvent.wirte(&uu,4);	//清除DO数据
	gMbSlaveMgEvent.write(_eCOILS,8,tmp,4);//清除MODBUS内存
  keyLeftFunChg[0] = 0;
	memset(keyLeftFunChg,0,sizeof(keyLeftFunChg));
	memset(keyRightFunChg,0,sizeof(keyRightFunChg));
	MainMenuInit();
	ModifyMenuInit();
	delaynop(100);
	checkSysStatus(&Controller);
	Controller.Pcnt[0] = 0;
	Controller.Pcnt[1] = 0;
	Controller.Mcnt[0] = 0;
	Controller.Mcnt[1] = 0;
	memset(&CtrlErr,0,sizeof(CtrlErr_STR));
}

int8_t setHmiEvent(uint8_t chg, uint8_t event)
{
	
	if(chg>4 || event>4) return -1;
//	AutoHandFlag = 1;
	LcdKeyE[chg].cmd = event;
	LcdKeyE[chg].flag = 1;
	return 1;
}

int8_t clrHmiEvent(uint8_t chg)
{
	if(chg>4) return -1;
	LcdKeyE[chg].cmd  = 0;
	LcdKeyE[chg].flag = 0;
	return 1;
}

uint8_t getKeyValue()
{
  return 0;
}
