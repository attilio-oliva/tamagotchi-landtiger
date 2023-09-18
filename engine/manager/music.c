#include "music.h"

#include "../../timer/timer.h"
#include "lpc17xx.h"

// beat 1/4 = 1.65/4 seconds
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1

volatile int should_play_music = 0;
volatile Music const *current_music;
volatile static int currentNote = 0;
volatile static int ticks = 0;

void playNote(NOTE note)
{
	if (note.freq != pause)
	{
		reset_timer(1);
		init_timer(1, 0, 0, 3, note.freq);
		enable_timer(1);
	}
	reset_timer(2);
	init_timer(2, 0, 0, 7, note.duration);
	enable_timer(2);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM1->TCR != 0) || (LPC_TIM2->TCR != 0));
}

void reset_music()
{
	disable_timer(1);
	disable_timer(2);
	should_play_music = 0;
}
void request_music(volatile Music const *music)
{
	current_music = music;
	disable_timer(1);
	disable_timer(2);
	should_play_music = 1;
}

void play_music()
{
	if (!should_play_music)
		return;

	if (!isNotePlaying())
	{
		++ticks;
		if (ticks == UPTICKS)
		{
			ticks = 0;
			playNote(current_music->notes[currentNote++]);
		}
	}

	if (currentNote == current_music->num_notes)
	{
		currentNote = 0;
		ticks = 0;
		should_play_music = 0;
	}
}
