#include  "testTask.h"
#include  "Bsp_OsTask.h"
#include  "Bsp_Uart.h"
#include  "Bsp_DO.h"
#include  "Bsp_DI.h"
uint8_t uuu[128] = {9,7,8,1,2,3,4,5,6};
uint8_t  sss =0;

uint8_t uuu1 = 0x00;

uint8_t uuu2 = 0x00;

void DOTest(void)
{
	static uint8_t saaa = 0;
	
	saaa++;
	
	if(saaa %2)
	{
		gDoMgEvent.wirte(&uuu1,4);
	}
	else
	{
	  gDoMgEvent.wirte(&uuu2,4);
	}
}
void TestTask(void)
{
//	uint16_t len;

//	if(0x00 == gSerialMgEvent.read(2,uuu,&len))
//	{
//		gSerialMgEvent.wirte(2,uuu,len);
//	}
//	DOTest();
	gDiMgEvent.read(&sss);
	
}
void  testTask_Creat(void)
{
//	gTaskCfgMgEvent.CreatTask(TestTask,2,500,Normal_Type);
}
