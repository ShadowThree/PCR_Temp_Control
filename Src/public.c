#include "public.h"

void delay_us(uint32_t us)
{
	us = (us<<2) + (us<<1) + us;
	while(us--);
}
