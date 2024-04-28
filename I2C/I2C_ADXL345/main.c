#include "adxl345.h"

int16_t x,y,z;
float xg,yg,zg;
extern uint8_t data_rec[6];

volatile int i =5;

int main(void)
{
	i++;
	adxl_init();
	
	
	while(1)
	{
		
		adxl_read_values(DATA_START_ADDR);
		x = ((data_rec[1]<<8)|data_rec[0]);
		y = ((data_rec[3]<<8)|data_rec[2]);
		z = ((data_rec[5]<<8)|data_rec[4]);
		
		xg = (x * 0.0078);
		yg = (y * 0.0078);
		zg = (z * 0.0078);
		
	}
	
}