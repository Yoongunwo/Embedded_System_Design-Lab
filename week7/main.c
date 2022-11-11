#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "core_cm3.h"

void RCC_Configure() {

RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}
void GPIO_Configure() {

GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_2);
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10 | GPIO_Pin_3);
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void USART1_Configure() {
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART1 ,&USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
  USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
}

void USART2_Configure() {
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART2 ,&USART_InitStructure);
  USART_Cmd(USART2, ENABLE);
  USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);
}

void NVIC_Configure() {
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
  NVIC_Init(&NVIC_InitStructure);
}


char usart1In = 'a';
char usart2In = 'a';

void delay(void){
int i = 0;
for(i=0;i<1000000;i++);
}

void delay2(void){
int i = 0;
for(i=0;i<10000;i++);
}



void USART1_IRQHandler(void){
  if ( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET ) {
    usart1In = USART_ReceiveData(USART1);
    USART_SendData(USART2, usart1In);
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
}


void USART2_IRQHandler(void){
  if ( USART_GetITStatus(USART2, USART_IT_RXNE) != RESET ) {
    usart2In = USART_ReceiveData(USART2);
    USART_SendData(USART1, usart2In);
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}



int main() {
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    USART1_Configure();
    USART2_Configure();
    NVIC_Configure();

    while(1){
    }
    return 0;
}
