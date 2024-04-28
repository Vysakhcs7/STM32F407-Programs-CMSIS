#include "i2c.h"


#define DEVID_R 													(0X00)
#define DEVICE_ADDR 											(0X53)
#define DATA_FORMAT_R  										(0X31)
#define POWER_CTL_R  										  (0X2D)
#define DATA_START_ADDR										(0X32)
#define FOUR_G							              (0X01)
#define RESET					                    (0X00)
#define SET_MEASURE_B						          (0X08)



void adxl_read_address(uint8_t reg);
void adxl_write(uint8_t reg, char value);
void adxl_read_values(uint8_t reg);
void adxl_init(void);
