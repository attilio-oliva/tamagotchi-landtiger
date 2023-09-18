#include "time.h"

#include "lpc17xx.h"

#define CLOCK_INTERVAL_MILLISECONDS 0.0004 // F: 25MHz -> T: 1/25 us = 0.4 us

volatile uint64_t clock_milliseconds = 0;

//fast division with floor (e.g. 0.9 equals to 0)
int floor_div(int a, int b)
{
	uint64_t d = a / b;

	return d * b == a ? d : d - ((a < 0) ^ (b < 0));
}

void reset_clock()
{
	clock_milliseconds = 0;
}
void update_clock(unsigned int elapsed_ms)
{
	clock_milliseconds += elapsed_ms;
}

uint32_t get_clock_seconds()
{
	return floor_div(clock_milliseconds, 1000);
}

uint64_t get_clock_milliseconds()
{
	return clock_milliseconds % 1001;
}

uint64_t get_clock_time_in_milliseconds()
{
	return clock_milliseconds;
}
