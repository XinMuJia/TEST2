#include	"BEEP_LED.h"

/*********************
1ทไร๙ฦ๗ ตฮษ๙
********************/
void	BEEP_Di(void)
{
	u16 i;
	for(i=0;i<200;i++)
	{
		BEEP(Bit_SET);
		Delay1us(150);
		BEEP(Bit_RESET);
		Delay1us(150);
		nLED_Link(Bit_RESET);
	}
		for(i=0;i<150;i++)
	{
		BEEP(Bit_SET);
		Delay1us(400);
		BEEP(Bit_RESET);
		Delay1us(400);
	}
	 nLED_Link(Bit_SET);
	
}

/*********************
2ทไร๙ฦ๗ ณคร๙
********************/
void	BEEP_Error(void)
{
	u16 i;
	for(i=0;i<2000;i++)
	{
		BEEP(Bit_SET);
		Delay1us(100);
		BEEP(Bit_RESET);
		Delay1us(100);
	}
	//BEEP(Bit_SET);
	nLED_ERROR(Bit_RESET);
}


u8	Key1(void)
{
		if(Start_Key()	!=	Bit_RESET)	
		{
			Delay1ms(10);
			if(Start_Key()	!=	Bit_RESET)	
			{
				Delay1ms(10);
				if(Start_Key()	!=	Bit_RESET)	
				{
					return	Bit_SET;
				}
			}
		}
		return	Bit_RESET;
}

