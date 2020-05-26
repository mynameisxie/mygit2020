#ifndef _OlcdTask_H
#define _OlcdTask_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#include  "types.h"
typedef struct MenuItem        
{
	uint8_t mux;
	int8_t msg;
	uint8_t cnt;
	uint8_t init;
//	uint8_t dKLeft;
//	uint8_t dKRight;
//	uint8_t dKUp;
//	uint8_t dKDown;
//	uint8_t dKMenu;
	char *Page1String;//第1行显示
	char *Page2String;//第2行显示
	char *Page3String;//第3行显示
	char *Page4String;//第4行显示
	void (*Subs)(uint8_t key);
	struct MenuItem *ChildrenMenus[5];
	struct MenuItem *ParentMenus;
}MenuItem_t;




#ifdef Olcd_Task 
MenuItem_t    MainMenu;
MenuItem_t    ModifyMenu;
MenuItem_t    SyscfgMenu;
MenuItem_t       StBaudMenu;
MenuItem_t       StAddrMenu;
MenuItem_t       StDevcMenu;
MenuItem_t       StTimeMenu;
MenuItem_t       ShowVrMenu;
MenuItem_t         CfmDevMenu;
MenuItem_t   *MenuPoint = &MainMenu;
#else
extern MenuItem_t  MainMenu;
extern MenuItem_t    ModifyMenu;
extern MenuItem_t    SyscfgMenu;
extern MenuItem_t       StBaudMenu;
extern MenuItem_t       StAddrMenu;
extern MenuItem_t       StDevcMenu;
extern MenuItem_t         CfmDevMenu;

#endif

extern void OlcdTask_Creat(void);
extern void decodeHmitoPusher(void);
extern void decodeHmitoDouBl(void);
extern void decodeHmitoSglBl(void);	
extern void OlcdTaskInit(void);
extern int8_t clrHmiEvent(uint8_t chg);

	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

