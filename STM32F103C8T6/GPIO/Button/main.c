/***************************************************************************//**
*   \file       main.c
*
*   \details    Setting Port A0 as input and PORT C13 as output.
*               When we press the Port A0, we will turn on the LED in
*               PC13 - STM32 GPIO Tutorial
*
*   \author     EmbeTronicX
*
*   \This code is verified with STM32F103CBT6 Board
*
*******************************************************************************/

#include "stm32f10x.h"

/***************************************************************************//**

  \details  The main function.  It should not return.

  \return   void

  \retval   none

*******************************************************************************/
int main(void)
{
  /* Enable the APB clock all GPIO Port C and Port A */
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);

  /* PC13 as output */
  GPIOC->CRH &= ~(GPIO_CRH_MODE13|GPIO_CRH_CNF13);    /* Clear MODE13 and CNF13 fields */
  GPIOC->CRH |= GPIO_CRH_MODE13_1|GPIO_CRH_MODE13_0;  /* Set MODE13 to 3 (Output) */
  
  /* PA0 as input with pullup */
  GPIOA->CRL &= ~(GPIO_CRL_MODE0|GPIO_CRL_CNF0);     /* Clear MODE0 and CNF0 fields */
  GPIOA->CRL |= GPIO_CRL_CNF0_1;                     /* Set CNF0 to 10 (Input with pullup/pull down) */
  GPIOA->ODR |= GPIO_IDR_IDR0;                       /* pull-up PA0 */

  /* Endless loop */
  while(1)
  {
    /* Button is connected to PA0. So we need to check bit 0 of IDR register. */
    if( ( GPIOA->IDR & GPIO_IDR_IDR0 ) == GPIO_IDR_IDR0 )   
    {
      /* Turn ON the LED of PC13 */
      GPIOC->BSRR |= GPIO_BSRR_BS13;
    }
    else
    {
      /* Turn OFF the LED of PC13 */
      GPIOC->BSRR |= GPIO_BSRR_BR13;
    }
  }
}
