# GBDK example project
Based no template_minimal

An minimal template project with a Makefile that only compiles files in the same directory

The Makefile will automatically detect and compile new source files as long as they are placed in the same directory as the Makefile

# Structure

* `main.c` is main entrypoint.
* `audio.h/c` contains code for audio engine. `music.c` contains a specific music track, exported using hUGETracker.
* `gamestate.h` contains a struct that can transfer data between game logic and graphics representation.
* `gamelogic.h/c` contains code for getting input and updating the internal state of the game (positions of objects etc.).
* `gfx.h/c` contains code for initializing and updating graphics.

* `guy.h/c` and `letters.h/c` are temporary sprites converted using `png2asset`. WIP.

The structure with many files is mainly set up to separate concerns and minimize git conflicts.

# Build

Build with `make` in `MSYS2`, or run `compile.bat` in a shell.
