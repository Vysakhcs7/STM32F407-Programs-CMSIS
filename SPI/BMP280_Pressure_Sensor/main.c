/*
Procedure
1) Enable the SPI clock at RCC_APBxENR register
2) Enable the GPIOx clock at RCC_AHB1ENR register
3) Configure the port pins for SPI by GPIOx_MODER and GPIOx_AFRy
registers
4) Configure a port pin for Slave Select by GPIOx_MODER register
5) Configure SPI_CR1 register to select data bit ordering, clock polarity,
clock phase, Baud rate, SPI master mode, data frame size, and others.
6) Clear or leave SPI_CR2 in default
7) Enable the SPI by setting SPE (SPI enable) bit of SPI_CR1 register
8) Monitor the TXE (Transfer buffer Empty) flag bit of the SPI_SR (status)
register until the transmit data register is empty
9) Assert slave select pin
10) Write data to SPI_DR (data) register to be transmitted
11) Wait until BSY flag is cleared in SPI_SR then deassert slave select
10) To transmit another character, go to step 8


PA15 - SPI1_NSS - Pins - 31,30
PA5 - SPI1_SCK -  Pins - 11,10
PA6 - SPI1_MISO - Pins - 13,12
PA7 - SPI1_MOSI - Pins - 15,14

AF5 - SPI1 (10: Alternate function mode) - 0101: Pins - 23 22 21 20 - GPIOA->AFRL[0]
*/
//---------------------------------------------------------------------------------------------------
#include "stm32f407xx.h"
//---------------------------------------------------------------------------------------------------
#define BMP280_TEMP_XLSB                0XFC
#define BMP280_TEMP_LSB                 0XFB
#define BMP280_TEMP_MSB                 0XFA
#define BMP280_PRESS_XLSB               0XF9
#define BMP280_PRESS_LSB                0XF8
#define BMP280_PRESS_MSB                0XF7
#define BMP280_CONFIG                   0XF5
#define BMP280_CTRL_MEAS                0XF4 				/* Data acquisition register */
#define BMP280_STATUS                   0XF3
#define BMP280_RESET                    0XEO
#define BMP280_ID                       0XD0        /* Chip identification number = 0x58 */
//---------------------------------------------------------------------------------------------------
void clock_config(void);
void spi_gpio_conifg(void);
void spi_config(void);
void spi_tx(uint8_t tx_data);
void spi_rx_1(void);
void spi_rx(uint8_t *data, uint32_t size);
void cs_enable(void);
void cs_disable(void);
uint8_t data1;
unsigned char bit_position = 7;
//---------------------------------------------------------------------------------------------------
int main(void)
{
  clock_config();
  spi_gpio_conifg();
  spi_config();
  while(1)
  {
		//START
    cs_enable();
		
		spi_tx( (BMP280_CTRL_MEAS & ~(1 << bit_position) ));
    spi_tx(0x57);  //(osrs_t = 010 - oversampling * 2, osrs_p = 101 - oversampling * 16, 11 - Normal Mode)
		
		
		spi_tx( (BMP280_CONFIG & ~(1 << bit_position) ));
		spi_tx(0x); //t_sb - 000 -.5ms standby time
		
		spi_rx_1();
    cs_disable(); 
  }
}
//----------------------------------------------------------------------------------------------------
void clock_config(void)
{
  //Bit 12 SPI1EN: SPI1 clock enable
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  //Bit 0 GPIOAEN: IO port A clock enable
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}
//-----------------------------------------------------------------------------------------------------
void spi_gpio_conifg(void)
{
  /* PA15 - SPI1_NSS -  Pins - 31,30
     PA5 -  SPI1_SCK -  Pins - 11,10
     PA6 -  SPI1_MISO - Pins - 13,12
     PA7 -  SPI1_MOSI - Pins - 15,14 
     (10: Alternate function mode) */
    GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
     // Configure PA15 as output for CS
    GPIOA->MODER &= ~GPIO_MODER_MODE15;
    GPIOA->MODER |= GPIO_MODER_MODE15_0;
  
  /* AF5 - SPI1 - 0101: Pins -  23 22 21 20 - GPIOA->AFRL[0]
     AF6 - SPI1 - 0101: Pins -  27 26 25 24 - GPIOA->AFRL[0]
     AF7 - SPI1 - 0101: Pins -  31 30 29 28 - GPIOA->AFRL[0] */
  GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_2 | GPIO_AFRL_AFRL5_0;
  GPIOA->AFR[0] |= GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL6_0;
  GPIOA->AFR[0] |= GPIO_AFRL_AFRL7_2 | GPIO_AFRL_AFRL7_0;
}
//-----------------------------------------------------------------------------------------------------
void spi_config(void)
{
  //Bit 6 SPE: SPI Disable
  SPI1->CR1 &= ~SPI_CR1_SPE;
  
  /*Bits 5:3 BR[2:0]: Baud rate control
  000: fPCLK/2 */
  SPI1->CR1 &= ~SPI_CR1_BR;
  
  //Bit1 CPOL: Clock polarity - 0 when idle
  SPI1->CR1 &= ~SPI_CR1_CPOL;
  
  /* Bit 0 CPHA: Clock phase
    0: The first clock transition is the first data capture edge */
  SPI1->CR1 &= ~SPI_CR1_CPHA;
  
  /* Bit 11 DFF: Data frame format
  0: 8-bit data frame format is selected for transmission/reception */
  SPI1->CR1 &= ~SPI_CR1_DFF;
  
  /*Bit 7 LSBFIRST: Frame format
  0: MSB transmitted first*/
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
  
  /*Bit 2 MSTR: Master selection
  1: Master configuration */
  SPI1->CR1 |= SPI_CR1_MSTR;
  
  /* Bit 7 TXEIE: Tx buffer empty interrupt enable
  1: TXE interrupt not masked. */
  //SPI1->CR2 |= SPI_CR2_TXEIE;
  
  /* Bit 9 SSM: Software slave management
  When the SSM bit is set, the NSS pin input is replaced with the value from the SSI bit.
  1: Software slave management enabled */
  SPI1->CR1 |= SPI_CR1_SSM;
  
  /* Bit 8 SSI: Internal slave select
  This bit has an effect only when the SSM bit is set. The value of this bit is forced onto the 
  NSS pin and the IO value of the NSS pin is ignored. */
  SPI1->CR1 |= SPI_CR1_SSI;
  
  //Bit 6 SPE: SPI Enable
  SPI1->CR1 |= SPI_CR1_SPE;
}
//-----------------------------------------------------------------------------------------------------
void spi_tx(uint8_t tx_data)
{
  while(!( SPI1->SR & SPI_SR_TXE )){}
  SPI1->DR = tx_data;
  
    // Wait until TXE flag is set and BSY flag is reset
   // while (!(SPI1->SR & SPI_SR_TXE) || (SPI1->SR & SPI_SR_BSY)) {}
}
//-----------------------------------------------------------------------------------------------------
void spi_rx(uint8_t *data, uint32_t size)
{
    while (size)
    {

        /* Wait for RXNE flag to be set */
        while (!(SPI1->SR & SPI_SR_RXNE )) {}

        /* Read data from data register */
        *data++ = SPI1->DR;
        size--;
    }
}
//-----------------------------------------------------------------------------------------------------
void spi_rx_1(void)
{
    // Send dummy data to generate clock
    SPI1->DR = 0x00;
  
   /* Wait for RXNE flag to be set */
        while (!(SPI1->SR & SPI_SR_RXNE )) {}
        data1 = SPI1->DR;  
}
//-----------------------------------------------------------------------------------------------------
void cs_enable(void)
{
    GPIOA->ODR &= ~GPIO_ODR_OD15;
}
//-----------------------------------------------------------------------------------------------------
void cs_disable(void)
{
    GPIOA->ODR |= GPIO_ODR_OD15;
}
//-----------------------------------------------------------------------------------------------------
