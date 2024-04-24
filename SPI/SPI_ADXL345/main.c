/*
Configuring the SPI in master mode
In the master configuration, the serial clock is generated on the SCK pin.
Procedure
1. Select the BR[2:0] bits to define the serial clock baud rate (see SPI_CR1 register).
2. Select the CPOL and CPHA bits to define one of the four relationships between the 
data transfer and the serial clock (see Figure 248). This step is not required when the 
TI mode is selected.
3. Set the DFF bit to define 8- or 16-bit data frame format
4. Configure the LSBFIRST bit in the SPI_CR1 register to define the frame format. This 
step is not required when the TI mode is selected.
5. If the NSS pin is required in input mode, in hardware mode, connect the NSS pin to a 
high-level signal during the complete byte transmit sequence. In NSS software mode, 
set the SSM and SSI bits in the SPI_CR1 register. If the NSS pin is required in output 
mode, the SSOE bit only should be set. This step is not required when the TI mode is 
selected.
6. Set the FRF bit in SPI_CR2 to select the TI protocol for serial communications.
7. The MSTR and SPE bits must be set (they remain set only if the NSS pin is connected 
to a high-level signal).
In this configuration the MOSI pin is a data output and the MISO pin is a data input.

PB3  - SPI1_SCK
PB4  - SPI1_MISO
PB5  - SPI1_MOSI
PA15 - SPI1_NSS

AF5

*/
/*----------------------------------------------------------------------------------------------------------------*/
#include "stm32f407xx.h"
void GPIO_Clock_Config(void);
void SPI_Clock_Config(void);
void GPIO_Config(void);
void SPI_Config(void);
void SPI_Transmit(uint8_t *data, uint32_t size);
void SPI_Receive(uint8_t *data, uint32_t size);
void CS_Enable(void);
void CS_Disable(void);

/*----------------------------------------------------------------------------------------------------------------*/

int main(void)
{
  
  while(1)
  {
    
  }
}

/*----------------------------------------------------------------------------------------------------------------*/
void GPIO_Clock_Config(void)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN; 
}
/*----------------------------------------------------------------------------------------------------------------*/
void SPI_Clock_Config(void)
{
  /* Bit 12 SPI1EN: SPI1 clock enable */
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}
/*----------------------------------------------------------------------------------------------------------------*/
void GPIO_Config(void)
{
  /* 10: Alternate function mode
     PB3  - SPI1_SCK  - MODER3  - 7,6
     PB4  - SPI1_MISO - MODER4  - 9,8
     PB5  - SPI1_MOSI - MODER5  - 11,10
     PA15 - SPI1_NSS  - MODER15 - 31,30
  */
  GPIOB->MODER |= GPIO_MODER_MODE3_1 | GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1;
  GPIOB->MODER &= ~(GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0);
  GPIOA->MODER |= GPIO_MODER_MODE15_1;
  GPIOA->MODER &= ~GPIO_MODER_MODE15_0;   
  
  /*
   BIT POSITION        3  2  1  0
            AF5  -     0  1  0  1
     AFRL3[3:0]  -    15 14 13 12
     AFRL4[3:0]  -    19 18 17 16
     AFRL5[3:0]  -    23 22 21 20
     AFRH15[3:0] -    31 30 29 28
  */
  GPIOB->AFR[0] |= GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_0;
  GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL3_3 | GPIO_AFRL_AFRL3_1);
  
  GPIOB->AFR[0] |= GPIO_AFRL_AFRL4_2 | GPIO_AFRL_AFRL4_0;
  GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL4_3 | GPIO_AFRL_AFRL4_1);
  
  GPIOB->AFR[0] |= GPIO_AFRL_AFRL5_2 | GPIO_AFRL_AFRL5_0;
  GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL5_3 | GPIO_AFRL_AFRL5_1);
  
  GPIOA->AFR[1] |= GPIO_AFRH_AFRH7_2 | GPIO_AFRH_AFRH7_0;
  GPIOA->AFR[1] &= ~(GPIO_AFRH_AFRH7_3 | GPIO_AFRH_AFRH7_1);
}
/*----------------------------------------------------------------------------------------------------------------*/

void SPI_Config(void)
{
    /*Bit 6 SPE: SPI enable
      0: Peripheral disabled*/ ///////
      //SPI1->CR1 &= ~SPI_CR1_SPE;
  
    /*Bits 5:3 BR[2:0]: Baud rate control
      001: fPCLK/4 */
      SPI1->CR1 |= SPI_CR1_BR_0;
      SPI1->CR1 &= ~(SPI_CR1_BR_2 | SPI_CR1_BR_1);
  
    /*Bit 0 CPHA: Clock phase */
      SPI1->CR1 |= SPI_CR1_CPHA;
    
    /*Bit1 CPOL: Clock polarity */
      SPI1->CR1 |= SPI_CR1_CPOL;
    
      /*Bit 2 MSTR: Master selection
      1: Master configuration*/
      SPI1->CR1 |= SPI_CR1_MSTR;
    
    /*Bit 11 DFF: Data frame format
      0: 8-bit data frame format is selected for transmission/reception*/
      SPI1->CR1 &= ~SPI_CR1_DFF;
    
   /*Bit 7 LSBFIRST: Frame format
     0: MSB transmitted first*/
     SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
     
   /*Bit 9 SSM: Software slave management
     1: Software slave management enabled*/
     SPI1->CR1 |= SPI_CR1_SSM;
     
    /*Bit 8 SSI: Internal slave select*/
     SPI1->CR1 |= SPI_CR1_SSI;
     
    /*Bit 2 SSOE: SS output enable*/  //////////
      SPI1->CR2 |= SPI_CR2_SSOE;
      
    /*Bit 6 SPE: SPI enable
      1: Peripheral enabled*/
      SPI1->CR1 |= SPI_CR1_SPE;
}
/*----------------------------------------------------------------------------------------------------------------*/
void SPI_Transmit(uint8_t *data, uint32_t size)
{
  uint32_t i = 0;
  uint32_t temp;
  while(i <size)
  {
  /* Bit 1 TXE: Transmit buffer empty */
  while(!(SPI1->SR & SPI_SR_TXE)){}
  SPI1->DR = data[i];
    i++;
  }
  while(!(SPI1->SR & SPI_SR_TXE)){}
  while((SPI1->SR & (SPI_SR_BSY))){}
temp = SPI1->DR;
    temp = SPI1->DR;
}
/*----------------------------------------------------------------------------------------------------------------*/
void SPI_Receive(uint8_t *data, uint32_t size)
{
  while(size)
  {
    SPI1->DR = 0;
      while(!(SPI1->SR & SPI_SR_RXNE)){}
        *data++ = (SPI1->DR);
        size--;
      }
    }
/*----------------------------------------------------------------------------------------------------------------*/
    void CS_Enable(void)
    {
      GPIOA->ODR &= ~GPIO_ODR_OD15;
    }

/*----------------------------------------------------------------------------------------------------------------*/
     void CS_Disable(void)
    {
      GPIOA->ODR |= GPIO_ODR_OD15; 
    }
/*----------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------*/