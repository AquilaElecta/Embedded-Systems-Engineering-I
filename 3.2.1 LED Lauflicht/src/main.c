#include "stm32f4xx_hal.h"
int Systick_counter = 0;int Led_Zustand = 0;

int main(void)

{
    SysTick->LOAD = 15999;//16MHz/1000Hz -1
    SysTick->VAL = 0;
    RCC->AHB1ENR |= 3;
    GPIOA -> MODER |= (1<<10) | (1<<12) | (1<<14);
    GPIOB -> MODER |= (1<<12);
    GPIOA -> PUPDR &= ~((3<<10)|(3<<12)|(3<<14));
    GPIOB -> PUPDR &= ~(3<<12);
    GPIOA -> OSPEEDR &= ~((3<<10)|(3<<12)|(3<<14));
    GPIOB -> OSPEEDR &= ~(3<<12);
    GPIOA -> OTYPER &= ~(7<<5); //～(0b111)
    GPIOB -> OTYPER &= ~(1<<6);

    GPIOA->ODR |= (1 << 5) | (1 << 6) | (1 << 7);
    GPIOB->ODR |= (1 << 6);

    SysTick ->CTRL |= 7;//0b111
    while (1);
}

void SysTick_Handler(void) {
  Systick_counter++;
  if (Systick_counter == 1000) {
    Systick_counter = 0;
    GPIOA->ODR |= (1 << 5) | (1 << 6) | (1 << 7);
    GPIOB->ODR |= (1 << 6);
    switch(Led_Zustand) {
      case 0:
        GPIOA->ODR &= ~(1 << 5);
        break;
      case 1:
        GPIOA->ODR &= ~(1 << 6);
        break;
      case 2:
        GPIOA->ODR &= ~(1 << 7);
        break;
      case 3:
        GPIOB->ODR &= ~(1 << 6);
        break;
    }
    if(Led_Zustand==3) {
      Led_Zustand = 0;
    } else {
      Led_Zustand++;
    }
  }
}