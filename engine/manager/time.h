#include "stdint.h"
extern void reset_clock(void);
extern void update_clock(unsigned int elapsed_ms);
extern uint32_t get_clock_seconds(void);
extern uint64_t get_clock_milliseconds(void);
extern uint64_t get_clock_time_in_milliseconds(void);
