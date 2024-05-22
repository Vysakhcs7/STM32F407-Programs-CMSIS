#include <stdio.h>
#include "stm32f407xx.h"
#include<stdint.h>

// Function prototypes
void uart2_gpio_config(void);
void uart2_config(void);
void uart2_single_write(int ch);
void uart2_string_write(char *str);
