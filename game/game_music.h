#include "../engine/manager/music.h"

volatile const Music music_menu_left = {
    .notes = {
        {f7, time_semicroma},
        {c7, time_biscroma}},
    .num_notes = 2};
volatile const Music music_menu_right = {
    .notes = {
        {c7, time_semicroma},
        {f7, time_biscroma}},
    .num_notes = 2};

volatile const Music music_snack_selected = {
    .notes = {
        {c5, time_semiminima},
        {b4, time_semiminima},
        {c7, time_biscroma}},
    .num_notes = 3};

volatile const Music music_meal_selected = {
    .notes = {
        {d3, time_semiminima},
        {c5, time_semiminima},
        {c7, time_biscroma}},
    .num_notes = 3};

volatile const Music music_cuddles = {
    .notes = {
        {f7, time_semicroma},
        {f7, time_semicroma},
        {c7, time_semicroma},
        {c7, time_semicroma},
    },
    .num_notes = 4};

volatile const Music music_runaway = {
    .notes = {
        {f7, time_semicroma},
        {c7, time_semicroma},
        {f7, time_semicroma},
        {c5, time_semibreve},
    },
    .num_notes = 4};
