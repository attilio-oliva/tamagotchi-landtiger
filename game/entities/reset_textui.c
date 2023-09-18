#include "reset_textui.h"

volatile int game_over = 0;
extern void (*on_reset)(void);
