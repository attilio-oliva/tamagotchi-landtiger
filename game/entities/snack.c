#include "snack.h"

volatile int snack_requested = 0;

void snack_on_update(Entity* e) {
	if(snack_requested) {
		e->is_visible = 1;
	} else {
		e->is_visible = 0;
	}
}
