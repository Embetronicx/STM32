/***************************************************************************//**
*   \file       main.c
*
*   \details    Blinking Onboard LED with some random delay - STM32 GPIO Tutorial             
*
*   \author     EmbeTronicX
*
*   \This code is verified with STM32411CE Board
*
*******************************************************************************/

#include "stm32f4xx.h" 

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
  /* Enable the AHB clock all GPIO port C */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

  /* set all Port C as output */
  GPIOC->MODER = 0x55555555;

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
