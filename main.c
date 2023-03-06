#include "stm32l4xx.h"                  // Device header

void GPIO_Config (void);
void initClockHSI (void);
void delay (uint32_t time);
void SysTick_Initialize (uint32_t ticks);
void Systick_Handler (void);

void initClockHSI (void)
{                           
	RCC->CR |= RCC_CR_HSION; 
	while(!(RCC->CR & RCC_CR_HSIRDY)){}
	
	RCC->CFGR &= ~RCC_CFGR_SW;
			
	RCC->CFGR |= RCC_CFGR_SW_HSI;	
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI){}	
}

void GPIO_Config (void)
{

	/* 1. Enable the GPIO CLOCK 
		 2. Set the Pin as OUT
	   3. Configure the OUTPUT MODE */
		
	// 1. Enable the GPIO CLOCK
	
	RCC->AHB2ENR |= (1<<2);
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // RCC_AHB2ENR_GPIOBEN; (1<<1);

	/*LED*/
	GPIOB->MODER &= ~GPIO_MODER_MODE13;
	GPIOB->MODER |= 0x4000000;
	
	// 3. Configure the OUTPUT MODE
	
	GPIOB->OTYPER &= ~GPIO_OTYPER_ODR_13;
	GPIOB->OSPEEDR = (1<<27);
}

void SysTick_Initialize (uint32_t ticks)
{
	
	SysTick->CTRL &= ~(0xffffff);
	
	SysTick->CTRL |= (1<<2);
	
	SysTick->CTRL |= (1<<1);
	
	SysTick->LOAD = ticks-1;
	
	SysTick->VAL &= ~(0xffffff);

	SysTick->CTRL |= (1<<0);

	NVIC_SetPriority(SysTick_IRQn, 1);
}


void Systick_Handler (void)
{
		GPIOB->ODR ^= (1<<13);
}

void delay (uint32_t time)
{

	while(time--){
	//while((SysTick->CTRL & (1 << 16)) != (1<<16)); // Wait until COUNTFLAG is 1		
	}
}
int main()
{

	initClockHSI();
	GPIO_Config();
	


while(1)
{
	SysTick_Initialize(10000000);
	delay(99999);
	GPIOB->ODR ^= (1<<13);
	
}
return 0;
}
