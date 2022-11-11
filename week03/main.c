#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "touch.h"

int color[12] = {WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};
uint16_t value;

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void ADC_Configure(void);
void NVIC_Configure(void);

//---------------------------------------------------------------------------------------------------
volatile uint32_t ADC_Value[1];

void RCC_Configure(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}


void DMA_Configure(void){
  DMA_InitTypeDef DMA_Instrucure;
  
  DMA_Instrucure.DMA_PeripheralBaseAddr = ADC1->DR;
  DMA_Instrucure.DMA_MemoryBaseAddr = ADC_Value[0];
  DMA_Instrucure.DMA_BufferSize = 1;
  DMA_Instrucure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_Instrucure.DMA_M2M = DMA_M2M_Disable;
  DMA_Instrucure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_Instrucure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_Instrucure.DMA_Mode = DMA_Mode_Circular;
  DMA_Instrucure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_Instrucure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_Instrucure.DMA_Priority = DMA_Priority_VeryHigh;
  
  DMA_Channel_TypeDef DMA_Channel;
  
  DMA_Channel.CCR;
  DMA_Channel.CMAR;
  DMA_Channel.CNDTR;
  DMA_Channel.CPAR;
  
  DMA_Init(DMA1_Channel1, &DMA_Instrucure);
  DMA_Cmd(DMA1_Channel1, ENABLE);
}



void GPIO_Configure(void) // stm32f10x_gpio.h ????
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ADC_Configure() {
  ADC_InitTypeDef ADC_InitStructure;
 
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
 
  ADC_Init(ADC1, &ADC_InitStructure);
   
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1 ,ENABLE);
  ADC_ResetCalibration(ADC1);
 
  while(ADC_GetResetCalibrationStatus(ADC1)) ;
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1)) ;
  ADC_SoftwareStartConvCmd(ADC1, ENABLE) ;
 
}


void NVIC_Configure(void) { // misc.h
 
    NVIC_InitTypeDef NVIC_InitStructure;
   
    // TODO: fill the arg you want
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // ADC, ADC1_2_IRQn
    // 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(ADC1_2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // TODO
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



void Delay(void) {
int i;
for (i = 0; i < 2000000; i++) {}
}

void ADC1_2_IRQHandler() {
  if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET ) {
    value = ADC_GetConversionValue(ADC1);
   
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  }
}

int main(void)
{
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    ADC_Configure();
    NVIC_Configure();
    DMA_Configure();
    
    LCD_Init();
    Touch_Configuration();
    Touch_Adjust();
    LCD_Clear(WHITE);

    char msg[] = "WED_Team08";
    LCD_ShowString(0, 0, msg, color[11], color[0] );

    while (1) {
      ADC_Value[0] = ADC_GetConversionValue(ADC1);
      if(ADC_Value[0] < 3800) {
        LCD_Clear(GRAY);
        LCD_ShowNum(0, 32, ADC_Value[0], 10,WHITE, GRAY);
      }
      else{
        LCD_Clear(WHITE);
        LCD_ShowNum(0, 32, ADC_Value[0], 10,color[11], WHITE);
      }
          Delay();
    }
       
    return 0;
}
