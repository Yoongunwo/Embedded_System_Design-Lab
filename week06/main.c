
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void EXTI_Configure(void);
void USART1_Init(void);
void NVIC_Configure(void);

void EXTI15_10_IRQHandler(void);
void EXIT2_IRQHandler(void);
void EXIT9_5_IRQHandler(void);
void Delay(void);

void sendDataUART1(uint16_t data);

//---------------------------------------------------------------------------------------------------

void RCC_Configure(void)
{
	// TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'
	
	/* UART TX/RX port clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* JoyStick Up/Down port clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/* JoyStick Selection port clock enable */

	/* LED port clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	/* USART1 clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	// TODO: Initialize the GPIO pins using the structure 'GPIO_InitTypeDef' and the function 'GPIO_Init'
	
    /* JoyStick up, down pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5;
    //GPIO_InitStructure.GPIO_Speed;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* JoyStick selection pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    
    /* button pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* LED pin setting*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
    /* UART pin setting */
    //TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXTI_Configure(void)
{
    EXTI_InitTypeDef EXTI_InitStructure, EXTI_InitStructure2, EXTI_InitStructure3;

	// TODO: Select the GPIO pin (Joystick, button) used as EXTI Line using function 'GPIO_EXTILineConfig'
	// TODO: Initialize the EXTI using the structure 'EXTI_InitTypeDef' and the function 'EXTI_Init'
	
    /* Joystick Down */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Joystick Up */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
    EXTI_InitStructure2.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure2.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure2.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure2.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure2);
    
    /* Joystick Selection */

    /* Button */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11);
    EXTI_InitStructure3.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure3.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure3.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure3.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure3);
    // NOTE: do not select the UART GPIO pin used as EXTI Line here
    
}

void USART1_Init(void)
{
	USART_InitTypeDef USART1_InitStructure;

	// Enable the USART1 peripheral
	USART_Cmd(USART1, ENABLE);
	
	// TODO: Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
      USART1_InitStructure.USART_BaudRate = 28800; 
      USART1_InitStructure.USART_WordLength = USART_WordLength_8b;        
      USART1_InitStructure.USART_StopBits = USART_StopBits_1;          
      USART1_InitStructure.USART_Parity = USART_Parity_No;     
      USART1_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;           
      USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
      USART_Init(USART1, &USART1_InitStructure);
        
	// TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
      USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure, NVIC_InitStructure2, NVIC_InitStructure3, NVIC_InitStructure4;
    
    // TODO: fill the arg you want
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // TODO: Initialize the NVIC using the structure 'NVIC_InitTypeDef' and the function 'NVIC_Init'

    // Joystick Down
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
    NVIC_Init(&NVIC_InitStructure);
    
    // Joystick Up
    NVIC_InitStructure2.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure2.NVIC_IRQChannelSubPriority =0;
    NVIC_Init(&NVIC_InitStructure2);
    
    // User S1 Button
    NVIC_InitStructure3.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure3.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure3.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure3.NVIC_IRQChannelSubPriority =0;
    NVIC_Init(&NVIC_InitStructure3);
    
    // UART1
	// 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure4.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure4.NVIC_IRQChannelPreemptionPriority = 0; // TODO
    NVIC_InitStructure4.NVIC_IRQChannelSubPriority = 0; // TODO
    NVIC_InitStructure4.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure4);
}

int myflag = 0;

void USART1_IRQHandler() {
	uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    	// the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);

        // TODO implement
        if(word == 'a'){
           myflag = 0;
        }else if(word == 'b') myflag =1;
        
        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

int print = 0;
void EXTI15_10_IRQHandler(void) { // when the button is pressed

	if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == Bit_RESET) {
			// TODO implement
                    print = 1;
                  }
        EXTI_ClearITPendingBit(EXTI_Line11);
	}
}


// TODO: Create Joystick interrupt handler functions
void EXTI9_5_IRQHandler(void){  // up
  if(EXTI_GetITStatus(EXTI_Line5) != RESET){
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == Bit_RESET ) {
        myflag = 0;

    }
    EXTI_ClearITPendingBit(EXTI_Line5);
  }
}
                         
void EXTI2_IRQHandler(void){  // down
   if(EXTI_GetITStatus(EXTI_Line2) != RESET){
     if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) == Bit_RESET ) {
        myflag = 1;
    }
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}

void Delay(void) {
	int i;

	for (i = 0; i < 2000000; i++) {}
}

void sendDataUART1(uint16_t data) {
	/* Wait till TC is set */
	while ((USART1->SR & USART_SR_TC) == 0);
	USART_SendData(USART1, data);
}

int main(void)
{

    SystemInit();

    RCC_Configure();

    GPIO_Configure();

    EXTI_Configure();

    USART1_Init();

    NVIC_Configure();
    
    int index = 0;
    
    char msg[] = "Hello Team08\r\n";
    
    while (1) {
    	// TODO: implement 
      if(myflag == 0){
        index++;
        if(index>3) index = 0;
      }
      else if(myflag == 1){
        index--;
        if(index <0) index =3;
      }
      if(print == 1){
        for(int i = 0; i < 16; ++i) {
          sendDataUART1((uint16_t)msg[i]);
        }
        print = 0;
      }
      if(index==0)
        (*(volatile unsigned int *)0x40011410) |= GPIO_BSRR_BS2;
      else if(index==1){
        (*(volatile unsigned int *)0x40011410) |= GPIO_BSRR_BS3;
      }
      else if(index==2){
        (*(volatile unsigned int *)0x40011410) |= GPIO_BSRR_BS4;
      }
      else if(index==3){
        (*(volatile unsigned int *)0x40011410) |= GPIO_BSRR_BS7;
      }
      
      Delay();
      
      (*(volatile unsigned int *)0x40011410) |= 0x009C0000;
      
    }
    return 0;
}
