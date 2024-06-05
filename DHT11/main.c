#include "stm32f4xx.h"
#include "core_cm4.h"
#include <stdio.h>

// GPIO macros for STM32F4
#define GPIO_PIN_1  (1U << 1) // PA1
#define GPIOA_BASE 0x40020000UL
#define GPIOA       ((GPIO_TypeDef *) GPIOA_BASE)

#define DHT11_PIN   GPIO_PIN_1
#define DHT11_PORT  GPIOA
#define DHT11_TIMEOUT 10000

uint8_t DHT11_Data[5]; // Declare DHT11_Data array

// Function Prototypes
void GPIO_Config(void);
void DHT11_SetPinOutput(void);
void DHT11_SetPinInput(void);
void Delay_us(uint32_t us);
void DHT11_StartSignal(void);
uint8_t DHT11_ReadByte(void);
uint8_t DHT11_ReadData(void);

void GPIO_Config(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable clock for GPIOA

    GPIOA->MODER &= ~(GPIO_MODER_MODER1); // Clear mode for PA1
    GPIOA->MODER |= GPIO_MODER_MODER1_0;  // Set PA1 as output
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0;  // Pull-up for PA1
}

void DHT11_SetPinOutput(void) {
    GPIOA->MODER |= GPIO_MODER_MODER1_0;  // Set PA1 as output
}

void DHT11_SetPinInput(void) {
    GPIOA->MODER &= ~(GPIO_MODER_MODER1); // Set PA1 as input
}

void Delay_us(uint32_t us) {
    uint32_t delay = us * (SystemCoreClock / 1000000) / 3;
    while (delay--);
}

void DHT11_StartSignal(void) {
    DHT11_SetPinOutput();
    GPIOA->ODR &= ~DHT11_PIN;   // Pull the pin low
    Delay_us(18000);            // Wait for 18ms
    GPIOA->ODR |= DHT11_PIN;    // Pull the pin high
    Delay_us(20);               // Wait for 20-40us
    DHT11_SetPinInput();        // Set pin to input
}

uint8_t DHT11_ReadByte(void) {
    uint8_t i, byte = 0;
    for (i = 0; i < 8; i++) {
        while (!(GPIOA->IDR & DHT11_PIN)); // Wait for the pin to go high
        Delay_us(40);
        if (GPIOA->IDR & DHT11_PIN) {
            byte |= (1 << (7 - i));
        }
        while (GPIOA->IDR & DHT11_PIN); // Wait for the pin to go low
    }
    return byte;
}

uint8_t DHT11_ReadData(void) {
    uint8_t i;
    DHT11_StartSignal();
    
    if (!(GPIOA->IDR & DHT11_PIN)) {
        Delay_us(80); // Wait for response
        if ((GPIOA->IDR & DHT11_PIN)) {
            Delay_us(80);
            for (i = 0; i < 5; i++) {
                DHT11_Data[i] = DHT11_ReadByte();
            }
            if ((DHT11_Data[0] + DHT11_Data[1] + DHT11_Data[2] + DHT11_Data[3]) == DHT11_Data[4]) {
                return 1; // Checksum OK
            }
        }
    }
    return 0; // Checksum Error
}

int main(void) {
    GPIO_Config();

    while (1) {
        if (DHT11_ReadData()) {
            // Successfully read the data
            uint8_t humidity_int = DHT11_Data[0];
            uint8_t humidity_dec = DHT11_Data[1];
            uint8_t temperature_int = DHT11_Data[2];
            uint8_t temperature_dec = DHT11_Data[3];

            printf("Humidity: %d.%d%%\n", humidity_int, humidity_dec);
            printf("Temperature: %d.%d°C\n", temperature_int, temperature_dec);
        } else {
            printf("Failed to read from DHT11\n");
        }
        Delay_us(2000000); // 2 seconds delay between readings
    }
}
