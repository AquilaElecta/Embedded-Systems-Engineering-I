#include "stm32f4xx_hal.h"

int main(void)

{
    HAL_Init();
    RCC->AHB1ENR |= 5;
    GPIOA -> MODER |= (1<<10);
    GPIOA -> PUPDR &= ~(3<<10);
    GPIOA -> OSPEEDR &= ~(3<<10);
    GPIOA -> OTYPER &= ~(1<<5);
    while (1)
    {
      if ((GPIOC ->IDR & (1<<13))==0)
      {
        while((GPIOC ->IDR & (1<<13))==0);
        GPIOA -> OTYPER ^= (1<<5);
      } 
        GPIOA -> ODR |= (1<<5);
        HAL_Delay(100);
        GPIOA -> ODR &= ~(1<<5);
        HAL_Delay(100);
    }

}

void SysTick_Handler(void) {
  HAL_IncTick();
}