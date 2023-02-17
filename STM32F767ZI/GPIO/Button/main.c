/***************************************************************************//**
*   \file       main.c
*
*   \details    Setting Port B as output and PORT C13 as input.
*               When we press the Port C13, we will turn on the PB0, PB7, PB14 - STM32 GPIO Tutorial
*
*   \author     EmbeTronicX
*
*   \This code is verified with STM32F767Zi Nucleo Board
*
*******************************************************************************/

#include "stm32f7xx.h" 


/***************************************************************************//**

  \details  The main function.  It should not return.

  \return   void

  \retval   none

*******************************************************************************/
int main(void)
{
  /* Enable the AHB clock all GPIO Port B and Port C */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

  /* set Port B0, B7, B14 as output */
  GPIOB->MODER &= ~( GPIO_MODER_MODER0 | 
                     GPIO_MODER_MODER7 | 
                     GPIO_MODER_MODER14 );    /* Clear MODER0, MODER7, MODER14 fields */
                     
  GPIOB->MODER |=  ( GPIO_MODER_MODER0_0 | 
                     GPIO_MODER_MODER7_0 | 
                     GPIO_MODER_MODER14_0 );  /* Set MODER0, MODER7, MODER14 to 1 (Output) */
  
  /* set Port C13 as input */
  GPIOC->MODER &= ~( GPIO_MODER_MODER13 );    /* Clear MODER13 fields to 0 (input) */
  
  /* Disable Pullup/Pulldown on PC13 */
  GPIOC->PUPDR &= ~( GPIO_PUPDR_PUPDR13 );    /* Clear PUPDR13 fields (No Pullup, No Pull down) */
  
  /* Set Port B as Push Pull */
  GPIOB->OTYPER = 0x00000000;
  
  /* Set Low Speed */
  GPIOB->OSPEEDR = 0x00000000;

  /* Endless loop */
  while(1)
  {
    /* Button is connected to PC13. So we need to check bit 13 of IDR register. */
    if( ( GPIOC->IDR & GPIO_IDR_ID13) == GPIO_IDR_ID13 )   
    {
      /* Turn ON the PB0 (Green LED), PB7 (Blue LED), PB14 (Red LED) */
      GPIOB->BSRR |= GPIO_BSRR_BS0 | GPIO_BSRR_BS7 | GPIO_BSRR_BS14;
    }
    else
    {
      /* Turn OFF the PB0 (Green LED), PB7 (Blue LED), PB14 (Red LED) */
      GPIOB->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BR7 | GPIO_BSRR_BR14;
    }
  }
}
