#include "stm32f10x.h"

#define RCC_APB2ENR (*(volatile unsigned int *)0x40021018)
#define GPIOC_CRL (*(volatile unsigned int *)0x40011000)
#define GPIOC_IDR (*(volatile unsigned int *)0x40011008)

#define GPIOD_CRH (*(volatile unsigned int *)0x40011404)
#define GPIOD_ODR (*(volatile unsigned int *)0x4001140C)
#define GPIOD_IDR (*(volatile unsigned int *)0x40011408)

#define GPIOD_CRL (*(volatile unsigned int *)0x40011400)

#define GPIOC_BSRR (*(volatile unsigned int *)0x40011010)
#define GPIOD_BSRR (*(volatile unsigned int *)0x40011410)

void delay(){
  int i;
  for(i=0; i<10000000; i++){}
}

 int main(void){
  RCC_APB2ENR |= 0x30;

  
  GPIOC_CRL &= ~0x00F00F00;
  GPIOC_CRL |= 0x00800800;
  
  GPIOD_CRH &= ~0x000FF000;
  GPIOD_CRH |= 0x00033000;
  
  GPIOC_CRL = 0x800800;
  GPIOD_CRH = 0x00011000;

  GPIOC_BSRR |= 0x240000;
  GPIOD_BSRR |= 0x8000000;
  
  //GPIOD_BSRR |= 0x800;

   while(1){
      if((GPIOC_IDR & 0x04) == 0){ //down
                GPIOD_ODR  &= ~0x1800;
        GPIOD_ODR |= 0x800;
        delay();
      }
    
      else if((GPIOC_IDR & 0x20) == 0){ // up
                GPIOD_ODR  &= ~0x1800;
          GPIOD_ODR |= 0x1000;
          
          delay();
      }
      else if((GPIOD_IDR & 0x800) == 0){ // s1
        GPIOD_ODR  &= ~0x1800;
     }
  }
}
