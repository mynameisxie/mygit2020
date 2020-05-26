#ifndef	__OLED_H
#define	__OLED_H
extern struct  OLED_Palette
{
	unsigned char FrontDisplay;	//Front Display content color
	unsigned char RearDisplay;	//Rear  Display content color
	unsigned char Brightness;
	unsigned char OLEDSelect;	//select which OLED will be displayed
}   DisplayParam;

void Set_Master_Current(unsigned char Data);	//set brightness
void Set_Display_On_Off(unsigned char Data);
void Draw_Rectangle(unsigned char Data, unsigned char LinewIDTH, 
					unsigned char ColumnStartAddress, unsigned char ColumneEndAddress, 
					unsigned char RowStartAddress, unsigned char RowEndAddress);
void Fill_RAM(unsigned char Data);
void Fill_Block(unsigned char Data, unsigned char ColumnStartAddress, unsigned char ColumneEndAddress, 
				unsigned char RowStartAddress, unsigned char RowEndAddress);
void Checkerboard(void);
void Grayscale(void);
void Show_Word16X16(unsigned char Front, unsigned short Word_ASCII,
		           unsigned char StartXAddress, unsigned char StartYAddress);
void Show_String8X16(unsigned char Front, char *Data_Pointer, 
				     unsigned char StartXAddress, unsigned char StartYAddress);
void Show_String16X32(unsigned char Front, char *Data_Pointer,
					  unsigned char StartXAddress, unsigned char StartYAddress);		
void Show_Words16X16(unsigned char Front, char *Data_Pointer, 
				     unsigned char StartXAddress, unsigned char StartYAddress);								 
void Show_Picture(const unsigned char *Data_Pointer, unsigned char ColumnStartAddress, unsigned char ColumneEndAddress, 
				  unsigned char RowStartAddress, unsigned char RowEndAddress);
void Vertical_Scroll(unsigned char ScrollingDirection, unsigned char NumbersPerStep, unsigned char StepTime);
void Vertical_FadeScroll(unsigned char ScrollingDirection, unsigned char NumbersPerStep, unsigned char StepTime);
void FullScreen_FadeIn(void);
void FullScreen_FadOut(void);
void Sleep(unsigned char SleepMode);
void Set_Linear_Gray_Scale_Table(void);
void OLEDDisplayPanelSwitch(unsigned char DisplayPanel);
void StartUp_CenterToSide(void);
void TopOLED_Init(void);
void BottomOLED_Init(void);
#endif
