#include <stdio.h>
#include "stm32f4xx.h"


#define HSEON	16
#define HSERDY  17
#define PLLON   24
#define PLLRDY  25

#define PLLSRC 22
#define PLLP   16
#define PLLN   6
#define PLLM   0

/*coNFIGURE GPIO TO OTPUT COMPARE PA8 PWM 1*/
/*CONFIGURE GPIO TO ALTERNATE FUNCITON PA9 PWM2*/

int main(){


	HAL_Init();
	SystemClockConfig();
	uint32_t tempreg;

	/*Enable HSE and wait for it to be statble, later it will be used by PLL*/
	RCC->CR |= (1<<HSEON);
	while((RCC->CR & (1<<HSERDY))== 0x00);
/**********************************************************************************************/
	/*Configure PLL to 84 Mhz */

		/*1.Select source for PLL
		 * 2.Confgiure paramters to generate 84Mhz
		 * 3.Enable PLL
		 * Wait for PLL to get stable*/

	/*1.1  Select source for PLL selecting HSE*/
	RCC->PLLCFGR |= (1<<PLLSRC) ;

	/*1.2 Configure PLL parameters
	 * TO get 84Mhz
	 * Input frequency must be less than 2Mhz, hence M = 12
	 * The VCO output frequency must be between 192 and 432 Mhz setting N=252
	 * To make PLL output less than 84 mhz then P =3
	 * */
	tempreg=RCC->PLLCFGR;
	tempreg &= ~((0x3<<PLLP)|(0x1FF<<PLLN)|(0x3F<<PLLM));
	RCC->PLLCFGR |= ((12<<PLLM)|(252<<PLLN)|(3<<PLLP));

	/*1.3 Enable PLL*/
		RCC->CR |= (1<<PLLON);
		while((RCC->CR & (1<<PLLRDY))==0);

/**********************************************************************************************/
	/*Connect Core to PLL*/
		RCC->CFGR &= ~(0x3<<0);
		RCC->CFGR |=(1<<1);
		while(((RCC->CFGR >>2) & 0X3)!= 0x2);   //waiting it to be stable

	/*Configure the root clock to APB domain
	 * Select clock source for AHB
	 * Configre APB prescaler
	 * */
	RCC->CFGR &= ~(0xF<<4);  //84Mhz
	RCC->CFGR &= ~((7<<10)|(7<<13));

	RCC->CFGR |= (3<<10);    //AP1 div4

	/*Enable CLock to timer peripheral*/
	RCC->APB2ENR |=(1<<0);

	/*Enable clock to GPIO peripheral*/
	RCC->AHB1ENR|= (1<<0);
	GPIOA->MODER &= ~(3<<10);
	GPIOA->MODER |= (1<<10);

	/*COnfgure clock to timer peripheral
	 * Timer would be recieving a clock of 6Mhz,setting the presclar to
	 * i.e setting prescaler to 239
	 * */
	TIM1->PSC = 6000;


	DBGMCU->APB2FZ |= (1<<0);   //timer will freeze when under debug mode

	/*Timer specific configuration*/
	TIM1->ARR =1000; 				 /*Number upto which the counter must count*/
	TIM1->CR1 |= ((1<<7)|(1<<2)|(1<<4)); 		//ENABLE AUTO PRELOAD and update event on overflow

	TIM1->DIER |= (1<<0);

	/**pwm spECIFIC ONFIGURATION*/

	/*coNFIGURE GPIO TO OTPUT COMPARE PA8*/
	GPIOA->MODER &= ~(0X3<<16);
	GPIOA->MODER |= (0X2<<16);

	GPIOA->AFR[1] &= ~(0X3<<0);
	GPIOA->AFR[1] |= (0X1<<0);

	/*CONFIGURE PWM MODE IN CCMR*/

	TIM1->CCMR1 &= ~(0X3<<0);   //SETTING CHANNEL1 AS OUTPUT COMPARE

	TIM1->CCMR1 &= ~(0X7<<4);
	TIM1->CCMR1 |= (0X6<<4);   //CONFIGURING MODE OF PWM

	TIM1->CCMR1 |= (1<<3);  //ENABLING PRELOAD

	TIM1->CCER |= (1<<0);  /*hAVING OC1 WITH LOW POLARITY*/
	/*CONFIGURE POLARITY*/
	TIM1->CCER &= ~(1<<1);

	/*cONFGURING PWM2*/

	/*coNFIGURE GPIO TO OTPUT COMPARE PA8*/
		GPIOA->MODER &= ~(0X3<<18);
		GPIOA->MODER |= (0X2<<18);

		GPIOA->AFR[1] &= ~(0X3<<4);
		GPIOA->AFR[1] |= (0X1<<4);

		/*CONFIGURE PWM MODE IN CCMR*/

		TIM1->CCMR1 &= ~(0X3<<8);   //SETTING CHANNEL1 AS OUTPUT COMPARE

		TIM1->CCMR1 &= ~(0X7<<12);
		TIM1->CCMR1 |= (0X6<<12);   //CONFIGURING MODE OF PWM

		TIM1->CCMR1 |= (1<113);  //ENABLING PRELOAD

		TIM1->CCER |= (1<<4);  /*hAVING OC2 WITH LOW POLARITY*/
		/*CONFIGURE POLARITY*/
		TIM1->CCER &= ~(1<<5);


	/*UPDATE DUTY CYCLE AND ENABLE PWM*/

	TIM1->CCR1 = 500;
	TIM1->CCR2 =600;

	/*Interrupt configurations*/
		//Enable NVIC
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);


	TIM1->BDTR |= (1<<15);  //ENABL OUTPUT COMPARE
	TIM1->CR1|= (1<<0);  //ENABLE THE TIMER
	while(1){


	}

}



void TIM1_UP_TIM10_IRQHandler(void){

	static int32_t i;
	if((TIM1->SR &0x01)==1){


		GPIOA->ODR ^= (1<<5);
		TIM1->SR&= ~(1);
	}

}
void SystemClockConfig(){



}
