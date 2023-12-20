# Tamagotchi game on a Landtiger board
This is an optional university assignment (called **extrapoint**) for the 2022/2023 Italian course [Computer architecture](https://didattica.polito.it/pls/portal30/gap.pkg_guide.viewGap?p_cod_ins=02GOLOV&p_a_acc=2023&p_header=S&p_lang=IT&multi=N) (02GOLOV) at Polytechnic University of Turin.
The assignment has been divided in two parts and could have led to a maximum of 4 extra scores in the final exam (out of 30).

The basic idea is to create a Tamagotchi game on a Landtiger board, using the peripherals available on the board and Keil uVision5. Each part of the assignment was about the usage of a different aspect of the board. The first part focus has been the usage of the embedded LCD display, timers and joystick. The second part focus has been the usage of the embedded speaker, potentiometer and touchscreen.
<p align="center">
  <img width=500px src="https://github.com/attilio-oliva/tamagotchi-landtiger/assets/75045331/211dcd73-189f-481e-818b-12d8ad0dc61e" />
</p>


## Wait, what is a Tamagotchi?
For the not so old people, Tamagotchi is a game about a pet that you have to take care of. You have to feed it, play with it and make sure it doesn't get sick. If you don't take care of it, it will die.

## Ok, but what is a Landtiger board?
The Landtiger board is a development board based on the NXP LPC1768 microcontroller. It has a lot of peripherals, such as an LCD display, a speaker, a potentiometer, a joystick and a touchscreen. It has a 64kB SRAM, 512kB on-chip Flash memory for storage, and it can be programmed using the [Keil uVision IDE](https://www.keil.com/demo/eval/arm.htm).

**NOTE**: the board is not available for purchase anymore, but the university still has quite a lot of them.

## Does it work?
Yes, it does, but with some [limitations](#Limitations). You can see some scenarios down here.
For the audio, you can only trust me and utmost pick a random sound from your brain and play it for each action and event, it will feel the same.
| Idle       |      Snack/Meal (left, right and joystick button) | Cuddle (touchscreen) | Volume ON/OFF (potentiometer) | Game Over |
|:----------:|:-------------:|:---:|:---:|:---:|
|![idle](https://github.com/attilio-oliva/tamagotchi-landtiger/assets/75045331/3f68f3f2-9d72-4646-a166-9796fc9bfad2)| ![snack-meal](https://github.com/attilio-oliva/tamagotchi-landtiger/assets/75045331/4ff7e9c7-23f9-4855-a527-3b2c09772aca)| ![cuddle](https://github.com/attilio-oliva/tamagotchi-landtiger/assets/75045331/5e6fd63e-323d-403f-aa0e-7edff28376e4)| ![volume](https://github.com/attilio-oliva/tamagotchi-landtiger/assets/75045331/d62d2f70-00d7-4405-8cc1-dd41d1dd173e) | ![game-over](https://github.com/attilio-oliva/tamagotchi-landtiger/assets/75045331/9f9f490e-2bff-4fc3-8914-91122b08dfd1)|

## Limitations
The code is quite big, the free license won't be enough to run it. You have to get it somehow. In my case, the university provides a license for the students, I just had to follow the instructions provided by the professor.

The resulting application is quite big, so it won't fit on a 32kB SRAM board. You'll need at least 64kB RAM.

These limitations are due to the various assets used in the game for the animations. These assets are statically loaded on the RAM, so they take up a lot of space. I tried to optimize the code as much as possible, but I couldn't do much more. The only other way at this point would have been some kind of dynamic loading, or asset generation at runtime, but I didn't have nor the will nor time to implement it.

You should also make sure that you are compiling with -O0, otherwise the game may not work properly. You can do this by going to `Project -> Options for Target -> C/C++ -> Optimization` and setting the optimization level to `Level 0 (-O0)`. Otherwise, you can be brave and try to compile up to -O2, but I didn't properly test every part of the game with them.

**NOTES for my fellow students**:
- some Landtiger boards provided by the university could have some non-properly working peripherals. In this case, you can ask for a replacement.
- **BEWARE**: for some reason, the university has some old boards version with only 32kB of SRAM. This is **not enough** to run the game. In this case, the display will not render anything, but it is not broken whatsoever. You can ask for a replacement anyway.

## How does it work?
The game uses a tiny ECS framework embedded for the game logic. The framework is quite simple, but it gets the job done. This allowed the game to be quite modular and easy to extend (useful for the second part of the assignment).

The game is divided in 3 main parts:
- **Game init** ([game.c](game/game.c)): this is the initialization part of the game. It creates all the entities and systems needed for the game to run, and it starts the game loop (by initializing the timers). The main game loop is handled by the `RIT` timer.
- **Game loop** ([IRQ_RIT.c](RIT/IRQ_RIT.c)): this is the main game loop. It is called by the `RIT` timer each 50 ms (editable in `engine/globals.h` with `RIT_INTERVAL_IN_MILLISECONDS`). It handles the game logic (systems) and the input from the joystick and the touchscreen. Furthermore, it also handles the rendering of the entities on the screen. Each loop call between frame drawings are called **tick**. A tick is increased in each loop and then resets to 0 after a frame render call. In each loop call the game logic, the input and the sound are handled, meanwhile the rendering is handled only after several ticks.
- **Render** ([renderer.c](engine/renderer.c)): this is the rendering part of the game. It handles the rendering of the entities on the screen. It is periodically used by the `RIT` timer to draw a frame each 500 ms. You can tweak this value by changing the `FRAME_TIME_IN_MILLISECONDS` macro in `engine/global.h`.

### Is it just me or the code can get messy?
Yes.

#### Ok, but at least excuse yourself or something, you lazy scoundrel
Well, I had to write the code in a very short time, so I didn't have the time to properly refactor it. I tried to keep it as clean as possible, but I had to make some compromises. I also had to write the code in a way that it could be easily extended for the second part of the assignment, so I had to make some other compromises time-wise.

The most messed up part is the rendering one. I had to implement some workarounds to make it work properly on this limited hardware.

### This display animations looks so nice, how did you do it?
I implemented some workaround to avoid the **flickering** and the display slowness while avoiding previous frame pixel traces. For this purpose, I should have implemented something like a double buffering system to make it work. What I came up with is what like to call with the *“poor man's double buffering”*.
Normally, the double buffering is used to render the next frame on a different buffer, and then swap the buffers when the rendering is done. This way, the screen will always show a complete frame, and not a partial one (flickering effect). This is done by using two buffers, one for the current frame(inside the display) and one for the next frame (in RAM). When the rendering is done, the buffers are swapped. This is a very common technique used in games and other applications.

Then why didn't you just do that? Well, the RAM is not enough to save a whole frame with a resolution of 320×240 pixels and 2 Bytes per pixel (RGB 565). The available RAM is only 64 kB, and the buffer size would be 320×240×2 bytes (153 600 bytes). So I had to come up with something else.

We cannot just put every pixel info in a buffer, so we just have to compress it somehow. What I came up is to use two structures:
- **render_buffer**: an in-RAM buffer with only a portion of the screen that will be rendered.
- **dirty_areas**: preserve the information about the areas that have been modified in the current frame. This is used to avoid rendering the whole screen each frame, but only the modified areas that should be hidden by the next frame.

During the rendering phase, part of the next frame will be computed using the **render_buffer**. In this buffer will be applied a mask to cover the last frame and on its top each game sprite is saved in the correct position. This is done by using the **dirty_areas** structure. Along this dirty mask, the screen will be forced to draw the color of the background.


You can see the actual effect if you put a random color for each draw to the “cleaning mask” obtained with the “dirty areas”. You don't have to clean and write the whole display, hence the flickering is not perceived, and the time required to draw a new frame on the slow display is reduced:
<p align="center">
  <img width=200px src="https://github.com/attilio-oliva/tamagotchi-landtiger/assets/75045331/d250d6fb-792e-44b7-a619-30012ae34fcf"/>
</p>


### Wow, the actual sounds are destroying my ears, how did you do it?
I'd like to express my gratitude to [ThePseudo](https://github.com/ThePseudo) for his [example project](https://github.com/ThePseudo/Music_for_Landtiger) on how to play some music on the Landtiger board. I used his code as a base for my implementation and had to modify it a bit to make it work with my code, but it was a good starting point.

In the remaining part, I smacked some random sounds together and I called it a day.

**NOTE**: the sound system is not working properly in an emulator, so you have to test it on the actual board. Otherwise, you'll hear some weird noises.

### How did you manage to fit so many assets in the RAM?
Initially I just converted some very tiny pixel art assets to C arrays and I loaded them in the RAM using GIMP. Then, using the renderer and the game engine, I implemented the upscaling of any asset (during runtime). This worked for a while, but then I started to run out of RAM after the flickering fix. So I had to come up with something else.

What I came up with was a technique used with PNG images. Each image color is saved in a palette, and then each pixel is saved as an index to the palette. This way, the image is compressed, and it takes up less space. I used this technique to compress the assets and embed them in the code. This way we can save about 50% of the RAM used by the assets.

### Assets source
I used and modified different assets under CC0 license :
- [character animated sprite](https://arks.itch.io/dino-characters)
- [UI icons](https://piiixl.itch.io/1-bit-icons-part-2)
- [snack and meal sprites](https://piiixl.itch.io/food)
