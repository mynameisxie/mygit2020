//  UG-5664ALBDF01(SAS1-E017-A)(Light Blue)) Reference Code
//  Dot Matrix: 256*64
//  GRAM :480*128*4Bits
//  Driver IC : SSD1322 (Solomom Systech)
//	Interface : 3-wire SPI

#include "stm32f10x.h"
#include "ASCII8X16.h"
#include "ASCII16X32.h"
#include "ZiKu16X16.h"

#define Max_Column	63		// 256/4-1
#define Max_Row		63		// 64-1
#define ColumnShift 28		// must be 112/4=28 because SEG112 -> Column0


struct  OLED_Palette
{
	unsigned char FrontDisplay;	//Front Display content color
	unsigned char RearDisplay;	//Rear  Display content color
	unsigned char Brightness;
	unsigned char OLEDSelect;	//select which OLED will be displayed
}   DisplayParam;

///////////////////////////////////////
//Read/Write Sequence
///////////////////////////////////////
void Write_Command(unsigned char Data)
{

}
///////////////////////////////////
void Write_Data(unsigned char Data)
{

}


/////////////////////////////////////////////////////////
void Set_Column_Address(unsigned char ColumnStartAddress, unsigned char ColumneEndAddress)
{
	Write_Command(0x15);			// Set Column Address
	Write_Data(ColumnShift + ColumnStartAddress);	// Default => 0x00
	Write_Data(ColumnShift + ColumneEndAddress);	// Default => 0x77 = 119(120*4=480 Piex)
}
/////////////////////////////////////////////////////////
void Set_Row_Address(unsigned char RowStartAddress, unsigned char RowEndAddress)
{
	Write_Command(0x75);			// Set Row Address
	Write_Data(RowStartAddress);	// Default => 0x00
	Write_Data(RowEndAddress);		// Default => 0x7F = 127(128 Piex)
}
/////////////////////////////////////////////////////////
void Set_Write_RAM(void)
{
	Write_Command(0x5C);		// Enable MCU to Write into RAM
}
/////////////////////////////////////////////////////////
void Set_Read_RAM(void)
{
	Write_Command(0x5D);		// Enable MCU to Read from RAM
}
/////////////////////////////////////////////////////////
void Set_Remap_Format(unsigned char Data)
{
	Write_Command(0xA0);		// Set Re-Map / Dual COM Line Mode
	Write_Data(Data);			// Default => 0x40
								// Set Horizontal Address Increment
								// Column Address 0 Mapped to SEG0
								// Disable Nibble Remap
								// Scan from COM[N-1] to COM0
								// Disable COM Split Odd Even
	Write_Data(0x11);			// Enable Dual COM Line Mode
}

/////////////////////////////////////////////////////////
void Set_Start_Line(unsigned char Data)
{
	Write_Command(0xA1);		// Set Vertical Scroll by RAM
	Write_Data(Data);			// Default => 0x00
}
/////////////////////////////////////////////////////////
void Set_Display_Offset(unsigned char Data)
{
	Write_Command(0xA2);		// Set Vertical Scroll by Row
	Write_Data(Data);			// Default => 0x00
}
/////////////////////////////////////////////////////////
void Set_Display_Mode(unsigned char Data)
{
	Write_Command(0xA4|Data);	// Set Display Mode
								// Default => 0xA4
								// 0xA4 (0x00) => Entire Display Off, All Pixels Turn Off
								// 0xA5 (0x01) => Entire Display On, All Pixels Turn On at GS Level 15
								// 0xA6 (0x02) => Normal Display
								// 0xA7 (0x03) => Inverse Display
}
/////////////////////////////////////////////////////////
void Set_Partial_Display(unsigned char Command, unsigned char Data1, unsigned char Data2)
{
	Write_Command(0xA8|Command);
								// Default => 0x8F
								// Select Internal Booster at Display On
	if(Command == 0x00)
	{
		Write_Data(Data1);
		Write_Data(Data2);
	}
}
/////////////////////////////////////////////////////////
void Set_Function_Selection(unsigned char Data)
{
	Write_Command(0xAB);		// Function Selection
	Write_Data(Data);			// Default => 0x01
								// Enable Internal VDD Regulator
}
/////////////////////////////////////////////////////////
void Set_Display_On_Off(unsigned char Data)
{
	Write_Command(0xAE|Data);	// Set Display On/Off
								// Default => 0xAE
								// 0xAE (0x00) => Display Off (Sleep Mode On)
								// 0xAF (0x01) => Display On  (Sleep Mode Off)
}
/////////////////////////////////////////////////////////
void Set_Phase_Length(unsigned char Data)
{
	Write_Command(0xB1);		// Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment
	Write_Data(Data);			// Default => 0x74 (7 Display Clocks [Phase 2] / 9 Display Clocks [Phase 1])
								// D[3:0] => Phase 1 Period in 5~31 Display Clocks
								// D[7:4] => Phase 2 Period in 3~15 Display Clocks
}
/////////////////////////////////////////////////////////
void Set_Display_Clock(unsigned char Data)
{
	Write_Command(0xB3);		// Set Display Clock Divider / Oscillator Frequency
	Write_Data(Data);			// Default => 0xD0
								// A[3:0] => Display Clock Divider
								// A[7:4] => Oscillator Frequency
}
/////////////////////////////////////////////////////////
void Set_Display_Enhancement_A(unsigned char Data1, unsigned char Data2)
{
	Write_Command(0xB4);		// Display Enhancement
	Write_Data(0xA0|Data1);		// Default => 0xA2
								// 0xA0 (0x00) => Enable External VSL
								// 0xA2 (0x02) => Enable Internal VSL (Kept VSL Pin N.C.)
	Write_Data(0x05|Data2);		// Default => 0xB5
								// 0xB5 (0xB0) => Normal
								// 0xFD (0xF8) => Enhance Low Gray Scale Display Quality
}
/////////////////////////////////////////////////////////
void Set_GPIO(unsigned char Data)
{
	Write_Command(0xB5);		// General Purpose IO
	Write_Data(Data);			// Default => 0x0A (GPIO Pins output Low Level.)
}
/////////////////////////////////////////////////////////
void Set_Precharge_Period(unsigned char Data)
{
	Write_Command(0xB6);		// Set Second Pre-Charge Period
	Write_Data(Data);				// Default => 0x08 (8 Display Clocks)
}
/////////////////////////////////////////////////////////
void Set_Precharge_Voltage(unsigned char Data)
{
	Write_Command(0xBB);		// Set Pre-Charge Voltage Level
	Write_Data(Data);			// Default => 0x17 (0.50*VCC)
}
/////////////////////////////////////////////////////////
void Set_VCOMH(unsigned char Data)
{
	Write_Command(0xBE);		// Set COM Deselect Voltage Level
	Write_Data(Data);			// Default => 0x04 (0.80*VCC)
}
/////////////////////////////////////////////////////////
void Set_Contrast_Current(unsigned char Data)
{
	Write_Command(0xC1);		// Set Contrast Current
	Write_Data(Data);			// Default => 0x7F
}
/////////////////////////////////////////////////////////
void Set_Master_Current(unsigned char Data)
{
	Write_Command(0xC7);		// Master Contrast Current Control
	Write_Data(Data & 0x0F);	// Default => 0x0f (Maximum)
}
/////////////////////////////////////////////////////////
void Set_Multiplex_Ratio(unsigned char Data)
{
	Write_Command(0xCA);		// Set Multiplex Ratio
	Write_Data(Data);			// Default => 0x7F (1/128 Duty)
}
/////////////////////////////////////////////////////////
void Set_Display_Enhancement_B(unsigned char Data)
{
	Write_Command(0xD1);		// Display Enhancement
	Write_Data(0x82|Data);		// Default => 0xA2
								// 0x82 (0x00) => Reserved
								// 0xA2 (0x20) => Normal
	Write_Data(0x20);
}
/////////////////////////////////////////////////////////
void Set_Command_Lock(unsigned char Data)
{
	Write_Command(0xFD);		// Set Command Lock
	Write_Data(0x12|Data);		// Default => 0x12
								// 0x12 => Driver IC interface is unlocked from entering command.
								// 0x16 => All Commands are locked except 0xFD.
}
/////////////////////////////////////////////////////////
//  Graphic Acceleration (Partial or Full Screen)
//
//    LinewIDTH: Line Width
//    ColumnStartAddress: Column Address of Start
//    ColumneEndAddress: Column Address of End
//    RowStartAddress: Row Address of Start
//    RowEndAddress: Row Address of End
/////////////////////////////////////////////////////////
void Draw_Rectangle(unsigned char Data, unsigned char LinewIDTH, 
					unsigned char ColumnStartAddress, unsigned char ColumneEndAddress, 
					unsigned char RowStartAddress, unsigned char RowEndAddress)
{
	unsigned char i,j,k,l;

	k=LinewIDTH%4;
	if(k == 0)
	{
		l=(LinewIDTH/4)-1;
	}
	else
	{
		l=LinewIDTH/4;
	}

	Set_Column_Address(ColumnStartAddress,ColumneEndAddress);
	Set_Row_Address(RowStartAddress,(RowStartAddress+LinewIDTH-1));
	Set_Write_RAM();
	for(j=0;j<LinewIDTH;j++)
	{
		for(i=0;i<(ColumneEndAddress-ColumnStartAddress+1);i++)
		{
			Write_Data(Data);
			Write_Data(Data);
		}
	}

	Set_Column_Address((ColumneEndAddress-l),ColumneEndAddress);
	Set_Row_Address(RowStartAddress+LinewIDTH,RowEndAddress-LinewIDTH);
	Set_Write_RAM();
	for(i=0;i<(RowEndAddress-RowStartAddress+1);i++)
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == 0)
			{
				switch(k)
				{
					case 0:
						Write_Data(Data);
						Write_Data(Data);
						break;
					case 1:
						Write_Data(0x00);
						Write_Data(Data&0x0F);
						break;
					case 2:
						Write_Data(0x00);
						Write_Data(Data);
						break;
					case 3:
						Write_Data(Data&0x0F);
						Write_Data(Data);
						break;
				}
			}
			else
			{
				Write_Data(Data);
				Write_Data(Data);
			}
		}
	}

	Set_Column_Address(ColumnStartAddress,ColumneEndAddress);
	Set_Row_Address((RowEndAddress-LinewIDTH+1),RowEndAddress);
	Set_Write_RAM();
	for(j=0;j<LinewIDTH;j++)	
	{
		for(i=0;i<(ColumneEndAddress-ColumnStartAddress+1);i++)
		{
			Write_Data(Data);
			Write_Data(Data);
		}
	}

	Set_Column_Address(ColumnStartAddress,(ColumnStartAddress+l));
	Set_Row_Address(RowStartAddress+LinewIDTH,RowEndAddress-LinewIDTH);
	Set_Write_RAM();
	for(i=0;i<(RowEndAddress-RowStartAddress+1);i++)
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == l)
			{
				switch(k)
				{
					case 0:
						Write_Data(Data);
						Write_Data(Data);
						break;
					case 1:
						Write_Data(Data&0xF0);
						Write_Data(0x00);
						break;
					case 2:
						Write_Data(Data);
						Write_Data(0x00);
						break;
					case 3:
						Write_Data(Data);
						Write_Data(Data&0xF0);
						break;
				}
			}
			else
			{
				Write_Data(Data);
				Write_Data(Data);
			}
		}
	}
}

/////////////////////////////////////////////////////////
//FILL ALL RAM OF THE CHIP
/////////////////////////////////////////////////////////
void Fill_RAM(unsigned char Data)
{
	unsigned char i,j;

	Set_Column_Address(0,63);
	Set_Row_Address(0,127);
	Set_Write_RAM();

	for(i=0;i<128;i++)
	{
		for(j=0;j<64;j++)
		{
			Write_Data(Data);
			Write_Data(Data);
		}
	}
}


/////////////////////////////////////////////////////////
//  Show Character (8x16)
/////////////////////////////////////////////////////////
void Show_Font8X16(unsigned char Front, unsigned char Ascii, 
				   unsigned char StartXAddress, unsigned char StartYAddress)
{
	const unsigned char *Pointer;
	unsigned char RowY,ZiKu_ROM;
	unsigned char DisplyRAM[4];

	switch(Front)
	{
		case 1:
			Pointer = &ASCII8X16[(Ascii-0x20)][0];
			break;
		default:
			Pointer = &ASCII8X16[(Ascii-0x20)][0];
			break;
	}
	
	Set_Column_Address(StartXAddress,StartXAddress+7);
	Set_Row_Address(StartYAddress,StartYAddress+1);	
	Set_Write_RAM();	
			
	for(RowY=0;RowY<16;RowY++)
	{
		ZiKu_ROM = *Pointer;
		Pointer +=1;

		DisplyRAM[0] = ( (ZiKu_ROM&0x80)>>7 ) * (DisplayParam.FrontDisplay<<4);
		DisplyRAM[0]|= ( (ZiKu_ROM&0x40)>>6 ) * (DisplayParam.FrontDisplay<<0);
		DisplyRAM[1] = ( (ZiKu_ROM&0x20)>>5 ) * (DisplayParam.FrontDisplay<<4);
		DisplyRAM[1]|= ( (ZiKu_ROM&0x10)>>4 ) * (DisplayParam.FrontDisplay<<0);
		DisplyRAM[2] = ( (ZiKu_ROM&0x08)>>3 ) * (DisplayParam.FrontDisplay<<4);
		DisplyRAM[2]|= ( (ZiKu_ROM&0x04)>>2 ) * (DisplayParam.FrontDisplay<<0);
		DisplyRAM[3] = ( (ZiKu_ROM&0x02)>>1 ) * (DisplayParam.FrontDisplay<<4);
		DisplyRAM[3]|= ( (ZiKu_ROM&0x01)>>0 ) * (DisplayParam.FrontDisplay<<0);
			
		Write_Data(DisplyRAM[0]);
		Write_Data(DisplyRAM[1]);
		Write_Data(DisplyRAM[2]);
		Write_Data(DisplyRAM[3]);
		
	}
	
}

/////////////////////////////////////////////////////////
unsigned long ZiKu_Word_Address_Convert( unsigned short Word_ASCII) 
			{
			unsigned long 	ZiKu_Address;
			unsigned char	ZiKu_Section,ZiKu_Byte;
			Word_ASCII     -= 0xB0A1;
			ZiKu_Section	= (Word_ASCII & 0xFF00)>>8;
			ZiKu_Byte		=  Word_ASCII & 0x00FF;	
			ZiKu_Address	= ( 94*ZiKu_Section + ZiKu_Byte )*32;
			return(ZiKu_Address);
			}
/////////////////////////////////////////////////////////

//  Show Word (16x16)
/////////////////////////////////////////////////////////
void Show_Word16X16(unsigned char Front, unsigned short Word_ASCII, 
				    unsigned char StartXAddress, unsigned char StartYAddress)
{
	const unsigned char *Pointer;
	unsigned char RowY,ColX,ZiKu_ROM;
	unsigned char DisplyRAM[4];

	switch(Front)
	{
		case 1:
//			Pointer = &ZiKu16X16[0] + ZiKu_Word_Address_Convert(Word_ASCII);
			break;
		default:
//			Pointer = &ZiKu16X16[0] + ZiKu_Word_Address_Convert(Word_ASCII);
			break;
	}

	Set_Column_Address(StartXAddress,StartXAddress+3);
	Set_Row_Address(StartYAddress,StartYAddress+15);	
	Set_Write_RAM();	
	
	for(RowY=0;RowY<16;RowY++)
	{

		for(ColX=0;ColX<2;ColX++)
		{
			ZiKu_ROM = *Pointer;
			Pointer += 1;

			DisplyRAM[0] = ( (ZiKu_ROM&0x80)>>7 ) * (DisplayParam.FrontDisplay<<4);
			DisplyRAM[0]|= ( (ZiKu_ROM&0x40)>>6 ) * (DisplayParam.FrontDisplay<<0);
			DisplyRAM[1] = ( (ZiKu_ROM&0x20)>>5 ) * (DisplayParam.FrontDisplay<<4);
			DisplyRAM[1]|= ( (ZiKu_ROM&0x10)>>4 ) * (DisplayParam.FrontDisplay<<0);
			DisplyRAM[2] = ( (ZiKu_ROM&0x08)>>3 ) * (DisplayParam.FrontDisplay<<4);
			DisplyRAM[2]|= ( (ZiKu_ROM&0x04)>>2 ) * (DisplayParam.FrontDisplay<<0);
			DisplyRAM[3] = ( (ZiKu_ROM&0x02)>>1 ) * (DisplayParam.FrontDisplay<<4);
			DisplyRAM[3]|= ( (ZiKu_ROM&0x01)>>0 ) * (DisplayParam.FrontDisplay<<0);
		
			Write_Data(DisplyRAM[0]);
			Write_Data(DisplyRAM[1]);
			Write_Data(DisplyRAM[2]);
			Write_Data(DisplyRAM[3]);
		}
		
	}
	
}

/////////////////////////////////////////////////////////
//  Show Character (16x32)
/////////////////////////////////////////////////////////
void Show_Font16X32(unsigned char Front, unsigned char Ascii, 
				    unsigned char StartXAddress, unsigned char StartYAddress)
{
	const unsigned char *Pointer;
	unsigned char RowY,ColX,ZiKu_ROM;
	unsigned char DisplyRAM[4];

	switch(Front)
	{
		case 1:
			Pointer = &ASCII16X32[(Ascii-0x20)][0];
			break;
		default:
			Pointer = &ASCII16X32[(Ascii-0x20)][0];
			break;
	}
	
	Set_Column_Address(StartXAddress,StartXAddress+3);
	Set_Row_Address(StartYAddress,StartYAddress+31);	
	Set_Write_RAM();
	
	for(RowY=0;RowY<32;RowY++)
	{	
		for(ColX=0;ColX<2;ColX++)
		{
			ZiKu_ROM = *Pointer;
			Pointer +=1;
			
			DisplyRAM[0] = ( (ZiKu_ROM&0x80)>>7 ) * (DisplayParam.FrontDisplay<<4);
			DisplyRAM[0]|= ( (ZiKu_ROM&0x40)>>6 ) * (DisplayParam.FrontDisplay<<0);
			DisplyRAM[1] = ( (ZiKu_ROM&0x20)>>5 ) * (DisplayParam.FrontDisplay<<4);
			DisplyRAM[1]|= ( (ZiKu_ROM&0x10)>>4 ) * (DisplayParam.FrontDisplay<<0);
			DisplyRAM[2] = ( (ZiKu_ROM&0x08)>>3 ) * (DisplayParam.FrontDisplay<<4);
			DisplyRAM[2]|= ( (ZiKu_ROM&0x04)>>2 ) * (DisplayParam.FrontDisplay<<0);
			DisplyRAM[3] = ( (ZiKu_ROM&0x02)>>1 ) * (DisplayParam.FrontDisplay<<4);
			DisplyRAM[3]|= ( (ZiKu_ROM&0x01)>>0 ) * (DisplayParam.FrontDisplay<<0);

			Write_Data(DisplyRAM[0]);
			Write_Data(DisplyRAM[1]);
			Write_Data(DisplyRAM[2]);
			Write_Data(DisplyRAM[3]);
		}
		
	}
	
}

/////////////////////////////////////////////////////////
//  Show String
//  Must write "0" in the end...
/////////////////////////////////////////////////////////
void Show_String8X16(unsigned char Front, char *Data_Pointer,
					 unsigned char StartXAddress, unsigned char StartYAddress)
{
	char *Pointer;

	Pointer = Data_Pointer;

	while( (*Pointer>=0x20) && (*Pointer<=0x7E) )
	{
		Show_Font8X16(Front,(unsigned char)*Pointer,StartXAddress,StartYAddress);
		Pointer +=1;
		StartXAddress += 2;
	}
}

/////////////////////////////////////////////////////////
//  Show String
//  Must write "0" in the end...
/////////////////////////////////////////////////////////
void Show_String16X32(unsigned char Front, char *Data_Pointer,
					 unsigned char StartXAddress, unsigned char StartYAddress)
{
	char *Pointer;
	Pointer = Data_Pointer;

	while( (*Pointer>=0x20) && (*Pointer<=0x7E) )
	{
		Show_Font16X32(Front,(unsigned char)*Pointer,StartXAddress,StartYAddress);
		Pointer +=1;
		StartXAddress += 4;
	}
}

/////////////////////////////////////////////////////////
//  Show words
//  Must write "0" in the end...
/////////////////////////////////////////////////////////
void Show_Words16X16(unsigned char Front, char *Data_Pointer,
					 unsigned char StartXAddress, unsigned char StartYAddress)
{
	unsigned short Word_ASCII;

	while(1)
	{
		if(*Data_Pointer<0xB0) break;

    	Word_ASCII = *Data_Pointer*256 + *(Data_Pointer+1);
		Show_Word16X16(Front,Word_ASCII,StartXAddress,StartYAddress);

        Data_Pointer +=2;
		StartXAddress += 4;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Picture (Partial or Full Screen)
/////////////////////////////////////////////////////////
void Show_Picture(const unsigned char *Data_Pointer, unsigned char ColumnStartAddress, unsigned char ColumneEndAddress, 
				  unsigned char RowStartAddress, unsigned char RowEndAddress)
{
	const unsigned char *Pointer;
	unsigned char i,j,ZiKu_ROM,DisplyRAM[4];
	
	Pointer = Data_Pointer;
	Set_Column_Address(ColumnStartAddress,ColumneEndAddress);
	Set_Row_Address(RowStartAddress,RowEndAddress);
	Set_Write_RAM();

	for(i=0;i<(RowEndAddress-RowStartAddress+1);i++)
	{
		for(j=0;j<(ColumneEndAddress-ColumnStartAddress+1);j++)
		{
			if( (j%2)==0 )
			{
				ZiKu_ROM = *Pointer;
				Pointer +=1;
												 
				DisplyRAM[0] = ( (ZiKu_ROM&0x80)>>7 ) * (DisplayParam.FrontDisplay<<4);
				DisplyRAM[0]|= ( (ZiKu_ROM&0x40)>>6 ) * (DisplayParam.FrontDisplay<<0);
				DisplyRAM[1] = ( (ZiKu_ROM&0x20)>>5 ) * (DisplayParam.FrontDisplay<<4);
				DisplyRAM[1]|= ( (ZiKu_ROM&0x10)>>4 ) * (DisplayParam.FrontDisplay<<0);
				DisplyRAM[2] = ( (ZiKu_ROM&0x08)>>3 ) * (DisplayParam.FrontDisplay<<4);
				DisplyRAM[2]|= ( (ZiKu_ROM&0x04)>>2 ) * (DisplayParam.FrontDisplay<<0);
				DisplyRAM[3] = ( (ZiKu_ROM&0x02)>>1 ) * (DisplayParam.FrontDisplay<<4);
				DisplyRAM[3]|= ( (ZiKu_ROM&0x01)>>0 ) * (DisplayParam.FrontDisplay<<0);
				
				Write_Data(DisplyRAM[0]);
				Write_Data(DisplyRAM[1]);
			}
			else
			{
				Write_Data(DisplyRAM[2]);
				Write_Data(DisplyRAM[3]);
			}
		}
	}
}



/////////////////////////////////////////////////////////
//  Vertical Fade Scrolling (Full Screen)
//
//  ScrollingDirection:"0x00" (Upward - In)  "0x01" (Downward - In)
//				 	   "0x02" (Upward - Out) "0x03" (Downward - Out)
//  NumbersPerStep	  : Set Numbers of Row Scroll per Step
//  StepTime		  : Set Time Interval between Each Scroll Step
/////////////////////////////////////////////////////////
void Vertical_FadeScroll(unsigned char ScrollingDirection, unsigned char NumbersPerStep, unsigned char StepTime)
{
	unsigned char i,j;	

	Set_Partial_Display(0x00,0x00,Max_Row);

	switch(ScrollingDirection)
	{
		case 0:
			for(i=(Max_Row+1);i<128;i+=NumbersPerStep)
			{
				Set_Start_Line(i);
				for(j=0;j<StepTime;j++)	{DelaymS(100);}
			}
			break;
		case 1:
			for(i=0;i<(Max_Row+1);i+=NumbersPerStep)
			{
				Set_Start_Line(Max_Row-i);
				for(j=0;j<StepTime;j++)	{DelaymS(100);}
			}
			break;
		case 2:
			for(i=0;i<(Max_Row+1);i+=NumbersPerStep)
			{
				Set_Start_Line(i+1);
				for(j=0;j<StepTime;j++)	{DelaymS(100);}
			}
			break;
		case 3:
			for(i=127;i>Max_Row;i-=NumbersPerStep)
			{
				Set_Start_Line(i);
				for(j=0;j<StepTime;j++)	{DelaymS(100);}
			}
			break;
	}
	Set_Partial_Display(0x01,0x00,0x00);
	Set_Start_Line(0);
}


/////////////////////////
//  Fade In (Full Screen)
/////////////////////////
void FullScreen_FadeIn(void)
{
	unsigned char i;	

	Set_Display_On_Off(0x01);
	for(i=0;i<(DisplayParam.Brightness+1);i++)
	{
		Set_Master_Current(i);
		DelaymS(200);
	}
}


//////////////////////////
//  Fade Out (Full Screen)
//////////////////////////
void FullScreen_FadOut(void)
{
	unsigned char i;	

	for(i=(DisplayParam.Brightness+1);i>0;i--)
	{
		Set_Master_Current(i-1);
		DelaymS(200);
	}
	Set_Display_On_Off(0x00);
}

//////////////////////////
//  Sleep Mode
//  "0x01" Enter Sleep Mode
//  "0x00" Exit Sleep Mode
//////////////////////////
void Sleep(unsigned char SleepMode)
{
	switch(SleepMode)
	{
		case 0:
			Set_Display_On_Off(0x00);
			Set_Display_Mode(0x01);
			break;
		case 1:
			Set_Display_Mode(0x02);
			Set_Display_On_Off(0x01);
			break;
	}
}

//////////////////////////////////////////
//  Gray Scale Table Setting (Full Screen)
//////////////////////////////////////////
void Set_Gray_Scale_Table()
{
	Write_Command(0xB8);		// Set Gray Scale Table
	Write_Data(0x0C);			//   Gray Scale Level 1
	Write_Data(0x18);			//   Gray Scale Level 2
	Write_Data(0x24);			//   Gray Scale Level 3
	Write_Data(0x30);			//   Gray Scale Level 4
	Write_Data(0x3C);			//   Gray Scale Level 5
	Write_Data(0x48);			//   Gray Scale Level 6
	Write_Data(0x54);			//   Gray Scale Level 7
	Write_Data(0x60);			//   Gray Scale Level 8
	Write_Data(0x6C);			//   Gray Scale Level 9
	Write_Data(0x78);			//   Gray Scale Level 10
	Write_Data(0x84);			//   Gray Scale Level 11
	Write_Data(0x90);			//   Gray Scale Level 12
	Write_Data(0x9C);			//   Gray Scale Level 13
	Write_Data(0xA8);			//   Gray Scale Level 14
	Write_Data(0xB4);			//   Gray Scale Level 15

	Write_Command(0x00);		// Enable Gray Scale Table
}

//////////////////////////////////////
void Set_Linear_Gray_Scale_Table(void)
{
	Write_Command(0xB9);		// Set Default Linear Gray Scale Table
}

//////////////////////////////////////
void OLEDAnalogDisplay(double DisplayValue)
{
	unsigned char AnalogPosition,DisplyData;	//range 0~50
	AnalogPosition = (unsigned char)DisplayValue;
	if(AnalogPosition>49)	{AnalogPosition = 49;}
	DisplyData = DisplayParam.FrontDisplay<<4;
	DisplyData|= DisplayParam.FrontDisplay<<0;

	Draw_Rectangle(DisplyData,1,6,57,48,63);
	
	if(AnalogPosition==0)
	{
		Fill_Block(0x00,7+1,56,49,62);
	}
	
	else if(AnalogPosition==49)
	{
		Fill_Block(0x00,7,56-1,49,62);
	}
	
	else
	{
		Fill_Block(0x00,7,7+AnalogPosition-1,49,62);
		Fill_Block(0x00,7+AnalogPosition+1,56,49,62);
	}
	
	Fill_Block(DisplyData,7+AnalogPosition,7+AnalogPosition,49,62);
}



//////////////////////////////////////
//////////////////////////////////////
void OLEDConfig(void)
{
	DisplayParam.FrontDisplay = 7;
	DisplayParam.RearDisplay  = 0;
	DisplayParam.Brightness	  = 5;
	
	Set_Command_Lock(0x12);					// Unlock Basic Commands (0x12/0x16)
	Set_Display_On_Off(0);				    // Display Off (0x00/0x01)
	Set_Display_Clock(0x91);				// Set Clock as 80 Frames/Sec
	Set_Multiplex_Ratio(0x3F);				// 1/64 Duty (0x0F~0x3F)(0-63)
	Set_Display_Offset(0);			   	    // Shift Mapping RAM Counter (0x00~0x3F)(0-63)
	Set_Start_Line(0);					    // Set Mapping RAM Display Start Line (0x00~0x7F)(0-127)
	Set_Remap_Format(0x14);	//0x06  		// Set Horizontal Address Increment
											// Column Address 0 Mapped to SEG0
											// Disable Nibble Remap
											// Scan from COM[N-1] to COM0
											// Disable COM Split Odd Even
											// Enable Dual COM Line Mode
	Set_GPIO(0x00);							// Disable GPIO Pins Input
	Set_Function_Selection(0x01);			// Enable Internal VDD Regulator
	Set_Display_Enhancement_A(0xA0,0xFD);	// Enable External VSL
											// Set Low Gray Scale Enhancement
	Set_Contrast_Current(0x80);				// Set Segment Output Current
	Set_Master_Current(DisplayParam.Brightness);		// Set Scale Factor of Segment Output Current Control
	//Iseg = Contrast_Current/256*Iref*Master_Current*2	// Iref = 10uA
	Set_Gray_Scale_Table();					// Set Pulse Width for Gray Scale Table
	Set_Phase_Length(0xE2);					// Set Phase 1 as 5 Clocks & Phase 2 as 14 Clocks
	Set_Display_Enhancement_B(0x20);		// Enhance Driving Scheme Capability (0x00/0x20)
	Set_Precharge_Voltage(0x1F);			// Set Pre-Charge Voltage Level as 0.60*VCC=7.2V
	Set_Precharge_Period(0x08);				// Set Second Pre-Charge Period as 8 Clocks
	Set_VCOMH(0x07);			    		// Set Common Pins Deselect Voltage Level as 0.86*VCC=10.0V
	Set_Display_Mode(0x02);					// Normal Display Mode (0x00(all off)/0x01(all on)/0x02(normal)/0x03(invert))
	Set_Partial_Display(0x01,0x00,0x00);	// Disable Partial Display	
}

