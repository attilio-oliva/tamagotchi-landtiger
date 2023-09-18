#include "../ecs/data_types.h"
#define N_BTN 3

extern int btn_down_counter[N_BTN];

extern void debouncer_setup(void);

extern Input get_debounced_input(void);
