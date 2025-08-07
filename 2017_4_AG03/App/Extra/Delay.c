#include	"Config.h"

/****************************************************************************
* 名称：void  Delay1us(u16 s)
* 功能：延时1us
* 入口参数：u16 s
* 出口参数：无  
****************************************************************************/
void  Delay1us(u16 us)
{
	u8 u;
	for(;us>0;us--)
	{
		for(u=12;u>0;)
		{
			u--;
		}
	}
}

/****************************************************************************
* 名称：void  Delay1ms(u16 s)
* 功能：延时1ms
* 入口参数：u16 s
* 出口参数：无  
*	调用函数：void  Delay1us(u16 s)；
****************************************************************************/
void  Delay1ms(u16 ms)
{
	for(;ms>0;ms--)
	{
		Delay1us(1000);
	}
}



/*********************************************************************
嘀嗒延时
*********************************************************************/

// couter 减1的时间 等于 1/systick_clk
// 当counter 从 reload 的值减小到0的时候，为一个循环，如果开启了中断则执行中断服务程序，
// 同时 CTRL 的 countflag 位会置1
// 这一个循环的时间为 reload * (1/systick_clk)

void SysTick_Delay_5Us( __IO uint32_t us)
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/200000);
	//禁止进入中断
	SysTick->CTRL &= ~ SysTick_CTRL_TICKINT_Msk;
	
	for(i=0;i<us;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}


void SysTick_Delay_1Ms( __IO uint32_t ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	//禁止进入中断
	SysTick->CTRL &= ~ SysTick_CTRL_TICKINT_Msk; 
	for(i=0;i<ms;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		// 当置1时，读取该位会清0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}


