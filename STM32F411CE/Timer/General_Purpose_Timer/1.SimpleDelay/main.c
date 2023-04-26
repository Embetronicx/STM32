/***************************************************************************//**
*   \file       main.c
*
*   \details    Blinking Onboard LED with 1 second delay using Timer
*               - STM32 Timer Tutorial
*
*   \author     EmbeTronicX
*
*   \This code is verified with STM32411CE Board
*
*******************************************************************************/

#include "stm32f4xx.h" 

/***************************************************************************//**

  \details  Providing milli seconds Delay by running Timer

  \return   void

  \retval   none

*******************************************************************************/
static void delay( uint32_t ms )
{
  uint32_t i;
  for( i = 0; i <= ms; i++ )
  {
    /* Clear the count */
    TIM3->CNT = 0;
    
    /* Wait UIF to be set */
    while((TIM3->SR & TIM_SR_UIF) == 0);    /* This will generate 1ms delay */
    
    /* Reset UIF */
    TIM3->SR &= ~TIM_SR_UIF;
  }
}

/***************************************************************************//**

  \details  This function enables the HSI clock as a system clock and generate 
            the 16MHz. The Internal HSI Clock is 16MHz. So, we are not using PLL
            and not dividing, Multiplying. So, we will get the 16MHz as it is.

  \return   void

  \retval   none

*******************************************************************************/
static void SetSystemClockTo16Mhz(void)
{
  /* Enabling the HSI clock - If not enabled and ready */
  if( (RCC->CR & RCC_CR_HSIRDY) == 0) 
  {
    RCC->CR |= RCC_CR_HSION;  /* HSION=1 */
  
    /* Waiting until HSI clock is ready */
    while( (RCC->CR & RCC_CR_HSIRDY) == 0);
  }
  
  /* Select AHB prescaler to 1 */
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  
  /* APB1 prescaler to 1 */
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
  
  /* APB2 prescaler to 1 */
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
  
  /* Select the HSI as system clock source */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= RCC_CFGR_SW_HSI;
  
  /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
  FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_3WS;
  
  /* Disabling HSE Clock*/
  RCC->CR &= ~RCC_CR_HSEON;
}

/***************************************************************************//**

  \details  This function configures the timer 3 to generate the 1ms delay. 

  \return   void

  \retval   none

*******************************************************************************/
static void ConfigureTimer3(void)
{
  /* Enable the APB clock FOR TIM3  */
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
  
  /* fCK_PSC / (PSC[15:0] + 1)
     (16 MHz / (15+1)) = 1 MHz timer clock speed */
  TIM3->PSC = 15;
  
  /* (1 MHz / 1000) = 1KHz = 1ms */
  /* So, this will generate the 1ms delay */
  TIM3->ARR = 999;
  
  /* Finally enable TIM3 module */
  TIM3->CR1 = (1 << 0);
}

/***************************************************************************//**

  \details  The main function.  It should not return.

  \return   void

  \retval   none

*******************************************************************************/
int main(void)
{

  /* Set System clock to 16 MHz using HSI */
  SetSystemClockTo16Mhz();
  
  /* Configure the Timer 3 */
  ConfigureTimer3();
  
  /* Enable the AHB clock all GPIO port C */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

  /* set all Port C as output */
  GPIOC->MODER = 0x55555555;

  /* Endless loop */
  while(1)
  {
    /* Turn ON the LED of PC13 */
    GPIOC->BSRR |= GPIO_BSRR_BS13;

    delay(1000);

    /* Turn OFF the LED of PC13 */
    GPIOC->BSRR |= GPIO_BSRR_BR13;

    delay(1000);
  }
}
