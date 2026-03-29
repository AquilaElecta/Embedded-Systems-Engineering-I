#include "stm32f4xx_hal.h"
    //0xC0 // 0
    //0xF9 // 1
    //0xA4 // 2
    //0xB0 // 3
    //0x99 // 4
    //0x92 // 5
    //0x82 // 6
    //0xF8 // 7
    //0x80 // 8
    //0x90 // 9
    //0x88 // A (10)
    //0x83 // B (11)
    //0xC6 // C (12)
    //0xA1 // D (13)
    //0x86 // E (14)
    //0x8E // F (15)
    //0xFF // All Off (16)
    //0x01 // Digit 1   - 0001
    //0x02 // Digit 2   - 0010
    //0x04 // Digit 3   - 0100
    //0x08 // Digit 4   - 1000
void Schreiben_74HC595(uint16_t num);
uint8_t Rechnen_Segment_Code(int num); 
void Hexadecimal_Anzeigen(int zahl);
int main(void)
{
    int zahl=0; uint16_t zahlcode=0;
    HAL_Init();
    RCC->AHB1ENR |= 3;//GPIOA und GPIOB aktivieren
    GPIOA -> MODER &= ~((3 << 16) | (3 << 18) | (3 << 8) | (3 << 2));//Pin 8,9,4,1 auf input
    GPIOB -> MODER &= ~((3<<10)|(3));
    GPIOA -> MODER |= (1<<16) | (1<<18);//Pin 8 und Pin 9 auf output
    GPIOB -> MODER |= (1<<10);//pin 5 auf output
    GPIOA -> PUPDR &= ~((3 << 16) | (3 << 18) | (3 << 8) | (3 << 2)); //Pin 8,9,4,1 auf no pull
    GPIOB -> PUPDR &= ~((3<<10)|(3));
    GPIOA -> OSPEEDR &= ~((3<<16)|(3<<18)); //Pin 8 und Pin 9 auf low speed
    GPIOB -> OSPEEDR &= ~(3<<10);//pin 5 auf low speed
    GPIOA -> OTYPER &= ~((1<<8)|(1<<9)|(1<<4)|(1<<1)); //Pin 8,9,4,1 auf push pull
    GPIOB -> OTYPER &= ~((1<<5)|(1));//pin 5,1 auf push pull
    GPIOA->ODR &= ~(1 << 8);//CK=0
    GPIOB->ODR |= (1 << 5);//CS=1
    while (1) {
        if ((GPIOB ->IDR & (1))==0)
      {
        while((GPIOB ->IDR & (1))==0);
        zahl=0;
        //zahlcode=0xc008;
      } 
        if ((GPIOA ->IDR & (1<<1))==0)
      {
        while((GPIOA ->IDR & (1<<1))==0);
        zahl++;
      } 
        if ((GPIOA ->IDR & (1<<4))==0)
      {
        while((GPIOA ->IDR & (1<<4))==0);
        zahl--;
      } 
        if(zahl<0) zahl=65535;
        if(zahl>65535) zahl=0;
        Hexadecimal_Anzeigen(zahl);
        //Schreiben_74HC595(0x8208);
        //HAL_Delay(500);
        //Schreiben_74HC595(0x9904);
        //HAL_Delay(500);
    }
}

void Schreiben_74HC595(uint16_t num) {
    GPIOB->ODR &= ~(1 << 5); //CS=0
    for (int i = 0; i < 16; i++) {
        if(num&(1<<(15-i))) {
            GPIOA->ODR |= (1 << 9); //DO=1
        } else {
            GPIOA->ODR &= ~(1 << 9); //DO=0
        }
        GPIOA->ODR |= (1 << 8); //CK=1
        volatile int m = 0;
        for (volatile int k=0;k<10;k++) { m+=k;}
        GPIOA->ODR &= ~(1 << 8); //CK=0
    }
    GPIOB->ODR |= (1 << 5); //CS=1
}

uint8_t Rechnen_Segment_Code(int num) {
    switch (num) {
        case 0: return 0xC0;
        case 1: return 0xF9;
        case 2: return 0xA4;
        case 3: return 0xB0;
        case 4: return 0x99;
        case 5: return 0x92;
        case 6: return 0x82;
        case 7: return 0xF8;
        case 8: return 0x80;
        case 9: return 0x90;
        case 10: return 0x88; // A
        case 11: return 0x83; // B
        case 12: return 0xC6; // C
        case 13: return 0xA1; // D
        case 14: return 0x86; // E
        case 15: return 0x8E; // F
        default: return 0xFF; // All Off
    }
}

void SysTick_Handler(void) {
    HAL_IncTick();
}

void Hexadecimal_Anzeigen(int zahl){
    uint16_t zahlcode=0;
    zahlcode= (Rechnen_Segment_Code(zahl%16)<<8|0x08);
    Schreiben_74HC595(zahlcode);
    HAL_Delay(2);
    zahlcode= (Rechnen_Segment_Code((zahl/16)%16)<<8|0x04);
    Schreiben_74HC595(zahlcode);
    HAL_Delay(2);
    zahlcode= (Rechnen_Segment_Code((zahl/256)%16)<<8|0x02);
    Schreiben_74HC595(zahlcode);
    HAL_Delay(2);
    zahlcode= (Rechnen_Segment_Code((zahl/4096)%16)<<8|0x01);
    Schreiben_74HC595(zahlcode);
    HAL_Delay(2);
}