#include "adxl345.h"

char data;

uint8_t data_rec[6];

void adxl_read_address(uint8_t reg) {
    i2c_byte_read(DEVICE_ADDR, reg, &data);
}

void adxl_write(uint8_t reg, char value) {
    char data[1];
    data[0] = value;
    i2c_burst_write(DEVICE_ADDR, reg, 1, data);
}

void adxl_read_values(uint8_t reg) {
    i2c_burst_read(DEVICE_ADDR, reg, 6, (char *)data_rec);
}

void adxl_init(void) {
	//enable i2c
	i2c_init();
    // Read the DEVID, this should return 0xE5
    adxl_read_address(DEVID_R);

    /*set data format range to +-4g*/
	adxl_write(DATA_FORMAT_R,FOUR_G);
    /*Reset all bits*/
	adxl_write(POWER_CTL_R,RESET);
    //CONFIGURE power control measure bit
	adxl_write(POWER_CTL_R,SET_MEASURE_B);
}
