/***************************************************************************//**
*   \file       main.c
*
*   \details    Setting Port A0 as input and PORT C13 as output.
*               When we press the Port A0, we will turn on the LED in
*               PC13 - STM32 GPIO Tutorial
*
*   \author     EmbeTronicX
*
*   \This code is verified with proteus STM32411CE Board
*
*******************************************************************************/

#include "stm32f4xx.h" 

/***************************************************************************//**

  \details  The main function.  It should not return.

  \return   void

  \retval   none

*******************************************************************************/
int main(void)
{
  /* Enable the AHB clock all GPIO Port C and Port A */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  
  /* set Port C13 as output */
  GPIOC->MODER &= ~( GPIO_MODER_MODER13 );    /* Clear MODER13 fields */
  GPIOC->MODER |=  ( GPIO_MODER_MODER13_0 );  /* Set MODER13 to 1 (Output) */
  
  /* set Port A0 as input */
  GPIOA->MODER &= ~( GPIO_MODER_MODER0 );    /* Clear MODER0 fields to 0 (input) */
  
  /* Enable Pullup on PA0 */
  GPIOA->PUPDR &= ~( GPIO_PUPDR_PUPDR0 );    /* Clear PUPDR0 fields (No Pullup, No Pull down) */
  GPIOA->PUPDR |=  GPIO_PUPDR_PUPDR0_0;      /* Set PUPDR0 fields to 0x01 (Pullup) */
  
  /* Endless loop */
  while(1)
  {
    /* Button is connected to PA0. So we need to check bit 0 of IDR register. */
    if( ( GPIOA->IDR & GPIO_IDR_ID0) == GPIO_IDR_ID0 )   
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
