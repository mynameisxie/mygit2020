#include "stm32f10x.h"
#include "Bsp_lcd.h"
#include "ASCII8X16.h"
#include "ASCII16X32.h"
#include "ZiKu16X16.h"
#include "Bsp_lcd.h"
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Pin Definition
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define CLR_CS          GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET)//PA15
#define SET_CS          GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET)//PA15
#define CLR_DC          GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET)//PB4
#define SET_DC          GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET)//PB4
#define CLR_REST        GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET)//PB6
#define SET_REST        GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET)//PB6

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
//#define Max_Column	128
//#define Max_Row		64
#define	Brightness	0xCF

#define Max_Column	63		// 256/4-1
#define Max_Row		63		// 64-1
#define ColumnShift 28		// must be 112/4=28 because SEG112 -> Column0

void spi1_init(void);
void gpio_init(void);

struct  OLED_Palette
{
	unsigned char FrontDisplay;	//Front Display content color
	unsigned char RearDisplay;	//Rear  Display content color
//	unsigned char Brightness;
	unsigned char OLEDSelect;	//select which OLED will be displayed
}DisplayParam;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Patterns
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
unsigned char UniV[][48]={			// UniV OLED
	0x00,0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,0xE0,0xE0,0xE0,0xE0,0x60,0xE0,0xE0,0xE0,0xC0,0x00,0x00,0x00,0xEC,0xEC,0xEC,0xEC,0x00,0x00,0x00,0x04,0x3C,0xFC,0xFC,0xF0,0x80,0x80,0xF0,0xFC,0xFC,0x3C,0x04,0x00,
	0x00,0x00,0x07,0x0F,0x1F,0x1F,0x1C,0x18,0x1C,0x1F,0x1F,0x0F,0x07,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x00,0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x0F,0x1F,0x1F,0x1F,0x1F,0x0F,0x01,0x00,0x00,0x00,
	0x00,0x80,0xE0,0xE0,0xF0,0x70,0x30,0x70,0xF0,0xE0,0xE0,0x80,0x00,0x00,0x00,0xF0,0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0xF0,0xF0,0x30,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0xF0,0xF0,0xF0,0xF0,0x30,0x70,0xF0,0xF0,0xE0,0x80,
	0x00,0x0F,0x3F,0x3F,0x7F,0x70,0x60,0x70,0x7F,0x3F,0x3F,0x0F,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x63,0x63,0x63,0x63,0x60,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x60,0x70,0x7F,0x7F,0x3F,0x0F,
};
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void uDelay(unsigned int l)
{
	while(l--);
}


void Delay(unsigned char n)
{
unsigned char i,j,k;

	for(k=0;k<n;k++)
	{
		for(i=0;i<131;i++)
		{
			for(j=0;j<15;j++)
			{
				uDelay(203);	
			}
		}
	}
}

void Write_Command(unsigned char Data)
{
	CLR_CS;
	CLR_DC;
	uDelay(1);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
  SPI_I2S_SendData(SPI1,Data);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
	uDelay(1);
	SET_CS;
	SET_DC;
}

void Write_Data(unsigned char Data)
{
	CLR_CS;
	SET_DC;
	uDelay(1);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
  SPI_I2S_SendData(SPI1,Data);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
	uDelay(1);
	SET_CS;
	SET_DC;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Start_Column(unsigned char d)
{
	Write_Command(0x00+d%16);		// Set Lower Column Start Address for Page Addressing Mode
						//   Default => 0x00
	Write_Command(0x10+d/16);		// Set Higher Column Start Address for Page Addressing Mode
						//   Default => 0x10
}


void Set_Addressing_Mode(unsigned char d)
{
	Write_Command(0x20);			// Set Memory Addressing Mode
	Write_Command(d);			//   Default => 0x02
						//     0x00 => Horizontal Addressing Mode
						//     0x01 => Vertical Addressing Mode
						//     0x02 => Page Addressing Mode
}


void Set_Column_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x21);			// Set Column Address
	Write_Command(a);			//   Default => 0x00 (Column Start Address)
	Write_Command(b);			//   Default => 0x7F (Column End Address)
}


void Set_Page_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x22);			// Set Page Address
	Write_Command(a);			//   Default => 0x00 (Page Start Address)
	Write_Command(b);			//   Default => 0x07 (Page End Address)
}


void Set_Start_Line(unsigned char d)
{
	Write_Command(0x40|d);			// Set Display Start Line
						//   Default => 0x40 (0x00)
}


void Set_Contrast_Control(unsigned char d)
{
	Write_Command(0x81);			// Set Contrast Control
	Write_Command(d);			//   Default => 0x7F
}


void Set_Charge_Pump(unsigned char d)
{
	Write_Command(0x8D);			// Set Charge Pump
	Write_Command(0x10|d);			//   Default => 0x10
						//     0x10 (0x00) => Disable Charge Pump
						//     0x14 (0x04) => Enable Charge Pump
}


void Set_Segment_Remap(unsigned char d)
{
	Write_Command(0xA0|d);			// Set Segment Re-Map
						//   Default => 0xA0
						//     0xA0 (0x00) => Column Address 0 Mapped to SEG0
						//     0xA1 (0x01) => Column Address 0 Mapped to SEG127
}


void Set_Entire_Display(unsigned char d)
{
	Write_Command(0xA4|d);			// Set Entire Display On / Off
						//   Default => 0xA4
						//     0xA4 (0x00) => Normal Display
						//     0xA5 (0x01) => Entire Display On
}


void Set_Inverse_Display(unsigned char d)
{
	Write_Command(0xA6|d);			// Set Inverse Display On/Off
						//   Default => 0xA6
						//     0xA6 (0x00) => Normal Display
						//     0xA7 (0x01) => Inverse Display On
}


void Set_Multiplex_Ratio(unsigned char d)
{
	Write_Command(0xA8);			// Set Multiplex Ratio
	Write_Command(d);			//   Default => 0x3F (1/64 Duty)
}


void Set_Display_On_Off(unsigned char d)	
{
	Write_Command(0xAE|d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE (0x00) => Display Off
						//     0xAF (0x01) => Display On
}


void Set_Start_Page(unsigned char d)
{
	Write_Command(0xB0|d);			// Set Page Start Address for Page Addressing Mode
						//   Default => 0xB0 (0x00)
}


void Set_Common_Remap(unsigned char d)
{
	Write_Command(0xC0|d);			// Set COM Output Scan Direction
						//   Default => 0xC0
						//     0xC0 (0x00) => Scan from COM0 to 63
						//     0xC8 (0x08) => Scan from COM63 to 0
}


void Set_Display_Offset(unsigned char d)
{
	Write_Command(0xD3);			// Set Display Offset
	Write_Command(d);			//   Default => 0x00
}


void Set_Display_Clock(unsigned char d)
{
	Write_Command(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	Write_Command(d);			//   Default => 0x80
						//     D[3:0] => Display Clock Divider
						//     D[7:4] => Oscillator Frequency
}


void Set_Precharge_Period(unsigned char d)
{
	Write_Command(0xD9);			// Set Pre-Charge Period
	Write_Command(d);			//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


void Set_Common_Config(unsigned char d)
{
	Write_Command(0xDA);			// Set COM Pins Hardware Configuration
	Write_Command(0x02|d);			//   Default => 0x12 (0x10)
						//     Alternative COM Pin Configuration
						//     Disable COM Left/Right Re-Map
}


void Set_VCOMH(unsigned char d)
{
	Write_Command(0xDB);			// Set VCOMH Deselect Level
	Write_Command(d);			//   Default => 0x20 (0.77*VCC)
}


void Set_NOP()
{
	Write_Command(0xE3);			// Command for No Operation
}

void Set_Write_RAM(void)
{
	Write_Command(0x5C);		// Enable MCU to Write into RAM
}

void Set_Row_Address(unsigned char RowStartAddress, unsigned char RowEndAddress)
{
	Write_Command(0x75);			// Set Row Address
	Write_Data(RowStartAddress);	// Default => 0x00
	Write_Data(RowEndAddress);		// Default => 0x7F = 127(128 Piex)
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char Data)
{
unsigned char i,j;

	for(i=0;i<8;i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(0x00);

		for(j=0;j<128;j++)
		{
			Write_Data(Data);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Start Page
//    b: End Page
//    c: Start Column
//    d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char i,j;
	
	for(i=a;i<(b+1);i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(c);

		for(j=0;j<d;j++)
		{
			Write_Data(Data);
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard()
{
unsigned char i,j;
	
	for(i=0;i<8;i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(0x00);

		for(j=0;j<64;j++)
		{
			Write_Data(0x55);
			Write_Data(0xaa);
		}
	}
}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Start Page
//    b: End Page
//    c: Start Column
//    d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char *Src_Pointer;
unsigned char i,j;

	Src_Pointer=Data_Pointer;
	for(i=a;i<(b+1);i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(c);

		for(j=0;j<d;j++)
		{
			Write_Data(*Src_Pointer);
			Src_Pointer++;
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In()
{
unsigned int i;	

	Set_Display_On_Off(0x01);
	for(i=0;i<(Brightness+1);i++)
	{
		Set_Contrast_Control(i);
		uDelay(200);
		uDelay(200);
		uDelay(200);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out()
{
unsigned int i;	

	for(i=(Brightness+1);i>0;i--)
	{
		Set_Contrast_Control(i-1);
		uDelay(200);
		uDelay(200);
		uDelay(200);
	}
	Set_Display_On_Off(0x00);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


void OLED_Init_I()				// VCC Generated by Internal DC/DC Circuit
{
  unsigned int i=0;

	CLR_REST;
	for(i=0;i<2000;i++)
	{
		uDelay(200);
	}
	SET_REST;
  uDelay(200);
	Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock(0x80);		// Set Clock as 100 Frames/Sec
	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
	Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
	Set_Charge_Pump(0x04);			// Enable Embedded DC/DC Converter (0x00/0x04)
	Set_Addressing_Mode(0x02);		// Set Page Addressing Mode (0x00/0x01/0x02)
	Set_Segment_Remap(0x01);		// Set SEG/Column Mapping (0x00/0x01)
	Set_Common_Remap(0x08);			// Set COM/Row Scan Direction (0x00/0x08)
	Set_Common_Config(0x10);		// Set Sequential Configuration (0x00/0x10)
	Set_Contrast_Control(Brightness);	// Set SEG Output Current
	Set_Precharge_Period(0xF1);		// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	Set_VCOMH(0x40);			// Set VCOM Deselect Level
	Set_Entire_Display(0x00);		// Disable Entire Display On (0x00/0x01)
	Set_Inverse_Display(0x00);		// Disable Inverse Display On (0x00/0x01)

//	Fill_RAM(0x00);				// Clear Screen
  
	Set_Display_On_Off(0x01);		// Display On (0x00/0x01)
}



/////////////////////////////////////////////////////////
unsigned long ZiKu_Word_Address_Convert( unsigned short Word_ASCII) 
{
	unsigned long 	ZiKu_Address;
	unsigned long   i;
	for(i=0;i<sizeof(ZiKu16X16);i+=34)
	{   
		  ZiKu_Address = (*(ZiKu16X16+i))*256+*(ZiKu16X16+i+1);
	    if( ZiKu_Address == Word_ASCII)
	    {
			ZiKu_Address = i+2;
			return(ZiKu_Address);
	    }
	}
	return 0;
}
/////////////////////////////////////////////////////////
//  Show Character (8x16)
/////////////////////////////////////////////////////////
void Show_Font8X16(uint8_t Ascii, uint8_t StartXAddress, uint8_t StartYAddress, uint8_t flag)
{
	const unsigned char *Pointer;
	unsigned char RowY,ColX;
	unsigned char DisplyRAM[4];
	Pointer = &ASCII8X16[(Ascii-0x20)][0];

	for(RowY=0;RowY<2;RowY++)
	{
		Set_Start_Page(StartXAddress+RowY);
		Set_Start_Column(StartYAddress);
		for(ColX=0;ColX<8;ColX++)
		{
			DisplyRAM[0]=*(Pointer+ColX+RowY*8);
			if(flag >= 1)DisplyRAM[0] = ~DisplyRAM[0];
			Write_Data(DisplyRAM[0]);
		}
	}
		
	
}

/////////////////////////////////////////////////////////

//  Show Word (16x16)
/////////////////////////////////////////////////////////
void Show_Word16X16(uint16_t Word_ASCII, uint8_t StartXAddress, uint8_t StartYAddress, uint8_t flag)
{
	const unsigned char *Pointer;
	unsigned char RowY,ColX;
	unsigned char DisplyRAM[16];
  Pointer = &ZiKu16X16[0] + ZiKu_Word_Address_Convert(Word_ASCII);
	for(RowY=0;RowY<2;RowY++)
	{	
		Set_Start_Page(StartXAddress+RowY);
			Set_Start_Column(StartYAddress);
		for(ColX=0;ColX<16;ColX++)
		{
			DisplyRAM[0]=*(Pointer+ColX+RowY*16);
			if(flag >= 1)DisplyRAM[0]=~DisplyRAM[0];
				Write_Data(DisplyRAM[0]);
		}
		
	}
	
}

void PrintfCustom(char *Data_Pointer,uint8_t StartXAddress, uint8_t StartYAddress, uint8_t flag)
{
    unsigned short Word_ASCII;
	  while(1)
		{			
			if(*Data_Pointer >= 0xB0)
			{
			Word_ASCII = *Data_Pointer*256 + *(Data_Pointer+1);
			Show_Word16X16(Word_ASCII,StartXAddress,StartYAddress,flag);
			StartYAddress += 16;
			Data_Pointer +=2;
			}
			else if( (*Data_Pointer>=0x20) && (*Data_Pointer<=0x7E) )
			{
				Show_Font8X16((unsigned char)*Data_Pointer,StartXAddress,StartYAddress,flag);
				StartYAddress += 8;
				Data_Pointer +=1;
			}
			else
			{
				break;
			}
	  }
}
void lcd_init()
{



	gpio_init();
	spi1_init();
  
	OLED_Init_I();
	Fill_RAM(0x00);			// Clear Screen
}
void spi1_init()
{
  SPI_InitTypeDef    spi_def;
		
		/*--------------- Reset SPI init structure parameters values -----------------*/
		/* Initialize the SPI_Direction member */
		spi_def.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		/* initialize the SPI_Mode member */
		spi_def.SPI_Mode = SPI_Mode_Master;
		/* initialize the SPI_DataSize member */
		spi_def.SPI_DataSize = SPI_DataSize_8b;
		/* Initialize the SPI_CPOL member */
		spi_def.SPI_CPOL = SPI_CPOL_High;
		/* Initialize the SPI_CPHA member */
		spi_def.SPI_CPHA = SPI_CPHA_2Edge;
		/* Initialize the SPI_NSS member */
		spi_def.SPI_NSS = SPI_NSS_Soft;
		/* Initialize the SPI_BaudRatePrescaler member */
		spi_def.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
		/* Initialize the SPI_FirstBit member */
		spi_def.SPI_FirstBit = SPI_FirstBit_MSB;
		/* Initialize the SPI_CRCPolynomial member */
		spi_def.SPI_CRCPolynomial = 10;
	  SPI_Init(SPI1,&spi_def);
		SPI_Cmd(SPI1,ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
}
void gpio_init()
{
	  GPIO_InitTypeDef   GPIO_InitStruct;
    /**SPI2 GPIO Configuration    
    PB3     ------> SPI_SCK
    PB4     ------> SPI_MISO
    PB5     ------> SPI_MOSI 
	  PB6     ------> RESET
    */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);	
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}
