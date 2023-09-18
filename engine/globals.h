#define MAX_SCREEN_X 240
#define MAX_SCREEN_Y 320

#define SCREEN_CENTER_X MAX_SCREEN_X * 0.5
#define SCREEN_CENTER_Y MAX_SCREEN_Y * 0.5

#define DARK_GRAY 0x4208
#define WHITE_COLOR 0xFFFF
#define BLACK_COLOR 0x0000
#define TRANSPARENT_COLOR 0x590C

#define BACKGROUND_COLOR DARK_GRAY

#define GAME_TIME_MULTIPLIER 1

#define RIT_OSCILLATOR_FREQUENCY 100000000 //100 MHz
#define RIT_INTERVAL_IN_MILLISECONDS 50
#define RIT_INTERVAL_IN_CLOCK_COUNT RIT_INTERVAL_IN_MILLISECONDS *(RIT_OSCILLATOR_FREQUENCY / 1000)

#define TIMER_OSCILLATOR_FREQUENCY 25000000 //25MHz
#define ONE_SECOND_FOR_TIMER_IN_CLOCK_COUNT 1 * TIMER_OSCILLATOR_FREQUENCY

#define FRAME_TIME_IN_MILLISECONDS 500

#define lambda(lambda$_ret, lambda$_args, lambda$_body) \
    ({                                                  \
        lambda$_ret lambda$__anon$ lambda$_args\
lambda$_body       \
            &amp;                                       \
        lambda$__anon$;                                 \
    })
