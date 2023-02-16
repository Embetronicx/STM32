/***************************************************************************//**
*   \file       main.c
*
*   \details    Blinking Onboard LED with some random delay - STM32 GPIO Tutorial
*
*   \author     EmbeTronicX
*
*   \This code is verified with STM32F767Zi Nucleo Board
*
*******************************************************************************/

#include "stm32f7xx.h" 

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
  /* Enable the AHB clock all GPIO port B */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);

  /* set Port B as output */
  GPIOB->MODER |= 0x55555555;
  
  /* Set Port B as Push Pull */
  GPIOB->OTYPER = 0x00000000;
  
  /* Set Low Speed */
  GPIOB->OSPEEDR = 0x00000000;

  /* Endless loop */
  while(1)
  {
    /* Turn ON the PB0 (Green LED), PB7 (Blue LED), PB14 (Red LED) */
    GPIOB->BSRR |= GPIO_BSRR_BS0 | GPIO_BSRR_BS7 | GPIO_BSRR_BS14;

    delay();

    /* Turn OFF the PB0 (Green LED), PB7 (Blue LED), PB14 (Red LED) */
    GPIOB->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BR7 | GPIO_BSRR_BR14;

    delay();
  }
}
