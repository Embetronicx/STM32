/***************************************************************************//**
*   \file       main.c
*
*   \details    Blinking Onboard LED with some random delay - STM32 GPIO Tutorial
*
*   \author     EmbeTronicX
*
*   \This code is verified with STM32F103CBT6 Board
*
*******************************************************************************/

#include "stm32f10x.h"

#define DELAY_COUNT    ( 80000 )   /* delay count */

/***************************************************************************//**

  \details  Providing Delay by running empty for loop

  \return   void

  \retval   none

*******************************************************************************/
static void delay( void )
{
    uint32_t i = 0;
    for( i=0; i<=DELAY_COUNT; i++ );
}

/***************************************************************************//**

  \details  The main function.  It should not return.

  \return   void

  \retval   none

*******************************************************************************/
int main(void)
{
  /* Enable the APB clock all GPIO port C */
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);

  /* PC13 as output */
  GPIOC->CRH &= (GPIO_CRH_MODE13|GPIO_CRH_CNF13);     /* Clear MODE13 and CNF13 fields */
  GPIOC->CRH |= GPIO_CRH_MODE13_1|GPIO_CRH_MODE13_0;  /* Set MODE13 to 3 (Output) */

  /* Endless loop */
  while(1)
  {
    /* Turn ON the LED of PC13 */
    GPIOC->BSRR |= GPIO_BSRR_BS13;

    delay();

    /* Turn OFF the LED of PC13 */
    GPIOC->BSRR |= GPIO_BSRR_BR13;

    delay();
  }
}
