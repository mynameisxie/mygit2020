#include "stm32f10x_gpio.h"
#include  "Bsp_Gpio.h"

#define DS2431_GPIO     GPIOB
#define DS2431_GPIO_Pin GPIO_Pin_12

#define DS2431_Write_1()        GPIO_SetBits( DS2431_GPIO, DS2431_GPIO_Pin )        //?1
#define DS2431_Write_0()    GPIO_ResetBits( DS2431_GPIO, DS2431_GPIO_Pin )        //?0
#define DS2431_ReadBit()    GPIO_ReadInputDataBit( DS2431_GPIO, DS2431_GPIO_Pin )        //?DS2431???

/*DS2431 ROMÖ¸Áî*/
#define Rom_Read_Cmd                0x33    //Read ROM
#define Rom_Match_Cmd               0x55    //Match ROM
#define Rom_Skip_Cmd                0xCC    //Skip ROM
#define Rom_Search_Cmd              0xF0    //Search ROM

/*DS2431 ´æ´¢Æ÷ÃüÁî*/
#define Memory_Read_Cmd             0xF0    //Read Memory
#define Scratchpad_Read_Cmd         0xAA    //Read Scratchpad
#define Scratchpad_Write_Cmd        0x0F    //Write Scratchpad
#define Scratchpad_Copy_Cmd         0x55    //Copy Scratchpad


void Delay_3us()
{
	uint8_t i =17;
	while(i--)
	{
		__ASM ("nop");
	}
}
void Delay_Nus(uint32_t Nus)
{
	while(Nus--)
	{
		Delay_3us();
	}
}

/******************************************
????:GPIO_DS2431_Out_Mode
?    ?:??DS2431?????????
?    ?:?
???  :?
*******************************************/
static void GPIO_DS2431_Out_Mode( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DS2431_GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;        //????
    GPIO_Init( DS2431_GPIO, &GPIO_InitStructure );
}
/******************************************
????:GPIO_DS2431_Input_Mode
?    ?:??DS2431?????????
?    ?:?
???  :?
*******************************************/
static void GPIO_DS2431_Input_Mode( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DS2431_GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //????
    GPIO_Init( DS2431_GPIO, &GPIO_InitStructure );
}

/******************************************
1-Wire????, ?????;?????0, ?????1
*******************************************/
static unsigned char Reset( void )
{
    GPIO_DS2431_Out_Mode();
    DS2431_Write_1();
    DS2431_Write_0();
    Delay_Nus( 237 );                //???????

    DS2431_Write_1();                //?????70us???
    Delay_Nus( 35 );
    GPIO_DS2431_Input_Mode();
         if( 0 == DS2431_ReadBit() )
    {
        Delay_Nus( 200 );
        return ( 0 );
     }
    else
    {
       return ( 1 );
     }  
}
/******************************************
               ????1
*******************************************/
static void WriteBit_1( void )
{
    GPIO_DS2431_Out_Mode();
    DS2431_Write_1();
    DS2431_Write_0();        //??????6us
    Delay_Nus( 2 );

    DS2431_Write_1();        //??????65us
    Delay_Nus( 30 );
}
/******************************************
                ????0
*******************************************/
static void WriteBit_0( void )
{
    GPIO_DS2431_Out_Mode();
    DS2431_Write_1();
    DS2431_Write_0();        //??????65us
    Delay_Nus( 30 );

    DS2431_Write_1();        //??????6us
    Delay_Nus( 2 );
}
/******************************************
                ????
*******************************************/
static unsigned char Read_Bit( void )
{
    unsigned char value;

    GPIO_DS2431_Out_Mode();
    DS2431_Write_1();
    DS2431_Write_0();                //??????6us
    Delay_Nus( 2 );
    DS2431_Write_1();                //??????9us
        Delay_Nus( 3 );
    GPIO_DS2431_Input_Mode();
    value = DS2431_ReadBit();   //????????55us 
    Delay_Nus( 24 );
    return value;   
}
/******************************************
                ???
*******************************************/
static void Write_Byte( unsigned char value )
{
    unsigned char i;

    for( i = 0; i < 8; i++ )
    {
        if( value & 0x01 )
        {
            WriteBit_1();
        }
        else
        {
            WriteBit_0();
        }
        value = value >> 1;
    }
}
/******************************************
                ???
*******************************************/
static unsigned char Read_Byte( void )
{
    unsigned char i, value;

    value = 0;
    for( i = 0; i < 8; i++ )
    {
        value >>= 1;
        if( Read_Bit() ) 
            value |= 0x80;        
    }
    return value;
}
/******************************************
¶Á8Î»¼Ò×åÂë
*******************************************/
unsigned char DS2431_ReadRom( unsigned char *id )
{
        unsigned char i;

        if( Reset() != 0 )
                return ( 1 );

        Write_Byte( Rom_Read_Cmd );        //???

        for( i = 0; i < 8; i++ )
        {
                  *id++ =        Read_Byte();
        }
        return ( 0 );
}
