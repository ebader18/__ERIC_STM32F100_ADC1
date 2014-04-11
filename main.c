#include "main.h"
#include "stm32f10x.h"

#define __READ_BIT(var,pos) ((var) & (1<<(pos)))
#define __CLEAR_BIT(var,pos) ((var) &= ~(1<<(pos)))
#define __SET_BIT(var,pos) ((var) |= (1<<(pos)))
#define __TOGGLE_BIT(var,pos) ((var) ^= (1<<(pos)))

void Configure_RCC();
void Configure_LEDs();
void Configure_ADC();

int main(void)
{
  uint32_t count = 0;
  uint32_t adc_value = 0;
  
  Configure_RCC();
  Configure_LEDs();
  Configure_ADC();
  
  __CLEAR_BIT(GPIOC->ODR, 8);
  __SET_BIT(GPIOC->ODR, 9);
  
  ADC_Cmd(ADC1, ENABLE);
  while(1)
  {
    if (count++>= 1000000)
    {
      count = 0;
      
      ADC_Cmd(ADC1, ENABLE);
      while (__READ_BIT(ADC1->SR,1) == 0);
      adc_value = ADC1->DR;
      if (adc_value > 2047)
      {
        __TOGGLE_BIT(GPIOC->ODR, 8);
        __TOGGLE_BIT(GPIOC->ODR, 9);
      }
    }
  }
}

void Configure_RCC()
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;                                            // Enable GPIOC Clock
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;                                            // Enable ADC1 Clock
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;                                            // Enable GPIOA Clock
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                                            // Enable Alternate Fuctions Clock
}

void Configure_LEDs()
{
  GPIOC->CRH = 0x44444422;                                                      // PC8 and PC9 as General Purpose Push Pull Output Max Speed = 2MHz
  GPIOA->CRL = 0x44444404;                                                      // PA1 as input analog mode
}

void Configure_ADC()
{
  ADC_InitTypeDef ADC_InitStructure;
  
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
}