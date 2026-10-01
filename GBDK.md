#todo 
- [ ] Background tiles
- [ ] Sprite pipeline
- [ ] Debugging/profiling

GBDK is a Gameboy development kit, a C-framework for building games and applications for Gameboy (and a few other platforms, NES for example). Gameboy (GB) and Gameboy Color (GBC) are supported, but native Gameboy Advanced (GBA) is not, since this uses another processor architecture, requiring another toolchain (for example [devkitPro](https://github.com/devkitPro/gba-examples) or [Butano](https://github.com/gvaliente/butano)).

The games compiled in GBDK should still run on the GBA, however.
# Getting started
Follow these steps to get started with GBDK (can also refer to [Getting started](https://gbdk.org/docs/api/docs_getting_started.html) in the docs):
* (Optional but recommended) Download [MSYS2](https://www.msys2.org/).
	* In the `MSYS2` terminal, install `make`:
```bash
pacman -S make
```
* Download GBDK itself from [here](https://github.com/gbdk-2020/gbdk-2020/releases). Put it somewhere on the `C:` drive.
* Verify that example projects can be built by opening `MSYS2` and running `make` in the `gbdk/examples/gb` folder. If you didn't download `MSYS2`, run `compile.bat` in the same folder in a terminal/PowerShell.
* Get an emulator (for example, [MesenCE](https://github.com/nesdev-org/MesenCE/releases)). Drag-and-drop any of the `.gb` files in the example project folders. Some (`sgb_`) might not work on Mesen, but I got `sgb_pong` to run on a different emulator.
* (Optional) Download the ARC Gameboy `game_template` (TODO link GitHub repo) project and put it in the `examples` folder. Verify that this builds too.
* Start experimenting!
# Links
[GBDK website](https://gbdk.org/)
[Documentation](https://gbdk.org/docs/api/)
[Links and tools](https://gbdk.org/docs/api/docs_links_and_tools.html)

[This page](https://gbdk.org/docs/api/docs_supported_consoles.html#autotoc_md154) contains information about the hardware limitations (sprites, colors etc.) of the various platforms supported by GBDK.

[GBTK graphics tool](https://github.com/gbdk-2020/GBTD_GBMB) (an older simple graphics tool for making and exporting sprites). An alternative to this is to use `png2asset`, a command line tool that is available as part of GBDK.
[Tilemap studio](https://github.com/Rangi42/tilemap-studio/) is a tool to build tile-based maps.

For music, GBDK only supports direct hardware interaction to play sounds on the four hardware channels (pulse/square wave + frequency sweep, pulse/square wave, programmable wave, noise). One approach is to use an API for this. [This blog](https://laroldsretrogameyard.com/tutorials/gb/playing-music-in-game-boy-games-with-huge-driver/) describes how to use [hUGETracker](https://github.com/SuperDisk/hUGETracker) (a DAW-like software) to generate C-code that can be used with [hUGEDriver](https://github.com/SuperDisk/hUGEDriver), an API for playing Gameboy music.

[Here](https://github.com/SuperDisk/hUGETracker/tree/hUGETracker/sample-songs) are some sample songs that can be opened in `hUGETracker` and exported to GBDK code.

If SFX is to be supported as well, it needs to use one of the four channels (possibly leading to a pause in the percussion for example). `hUGEDriver` supports muting one of the channels in order to play something else on it.

An emulator is needed to play the resulting `.gb` file. The documentation suggests using [Emulicious](https://emulicious.net/), since it supports source-level debugging. There is a `VSCode` extension that integrates with it. `romusage` is another tool. Another emulator with debugger support is [MesenCE](https://github.com/nesdev-org/MesenCE/releases).

There is a `VSCode` configuration available [here](https://gbdk.org/docs/api/docs_links_and_tools.html#code_editors_hinting).

There is a [github action](https://github.com/wujood/gbdk-2020-github-builder) for setting up CI/CD for GBDK.
# Limitations
Floating point math is not supported (can used fixed-point instead, `fixed` type). `uint32_t` and `uint16_t` are available, but `uint8_t` should be used where possible since it's more effective. Prefer unsigned, and explicit type declarations from `stdint.h`. Global and local static variables are more efficient than local non-static (stack) variables (not always the case). Avoid putting big arrays on the stack. Use `const` for read-only data to ensure it ends up in ROM rather than RAM. Multiplication, modulo and division with non-power-of-2 have no HW instruction and are costly. Do not use recursive functions, and try to avoid many parameters. When possible, use `inline`.

Use [`vsync()`](https://gbdk.org/docs/api/gb_8h.html#vsync) to cap framerate in the game loop. [`joypad()`](https://gbdk.org/docs/api/gb_8h.html#joypad) is used to fetch input (with masking defines to check if certain buttons are pressed). Build utility function to check for `is_just_pressed`. Multiplayer with more than one joystick is available through SGB, use [`joypad_init()`](https://gbdk.org/docs/api/gb_8h.html#joypad_init).

There are two hardware graphics layers, the BKG tile layer for background and static assets, and the SPRITE layer for moving assets. All tiles (including sprites) are 8x8 or 8x16 pixels in size. The tile map in the system is 8x8 tiles in a 32x32 grid, for a total work area of 256x256 pixels. Multiple sprites have to be used to build more complex images. There's also a WINDOW layer (for menus/text? 2bpp). Note that there's an overlap in memory between SPRITE tile 128-255 and BKG 128-255 on the Gameboy platforms.

[`scroll_bkg()`](https://gbdk.org/docs/api/gb_8h.html#scroll_bkg) and similar functions can be used to move the entire background layer.
# Sprite graphics pipeline
Use `Aseprite` or similar software. Use the `Sprite->Color Mode->Indexed`, and limit to using 4 colors per sprite (at least, per tile). First color is the transparent color (automatically shown by `Aseprite`). Save as indexed `.png`.

Convert the image to code by using the GBDK tool `png2asset` which resides in the bin folder. Usage:

```bash
../../../bin/png2asset.exe letters.png -sw 16 -sh 16
```
Where `sw` and `sh` are the metasprite width and height respectively. Using this command on `letters.png` will create `letters.h` and `letters.c`, which declares a bunch of variables. By default 8x16 sprites are used in HW (can be changed to 8x8 when calling `png2asset`), which requires this line in the initialization:
```c
SPRITES_8x16;
```
To use this in code, call in your graphics initialization:
```c
set_sprite_data(letters_TILE_ORIGIN, letters_TILE_COUNT, letters_tiles);
// Should set palette, not sure if it works
set_sprite_palette(S_PAL(0), letters_PALETTE_COUNT, letters_palettes);
```
And then place the sprite in the world using hardware sprites:
```c
#define LETTERS_SPRITE 0 // The first HW sprite to use
move_metasprite_ex(letters_metasprites[state.letters_tile], letters_TILE_ORIGIN, 0, LETTERS_SPRITE, x, y);
```
Note that each metasprite is actually drawn with one or more HW sprites (of which there are 40).

#todo Also need to use `set_sprite_prop` to associate the hardware sprite with a palette. The properties field can also be used to flip hardware sprites (x and/or y). The metasprite functions should handle this (see this [link](https://gbdk.org/docs/api/gb_2metasprites_8h.html#metasprite_and_sprite_properties)).
# Background layer
Same as with the sprites, the image data used for the background layer is stored in 8x8 pixel tiles. The GB/GBC has 128 dedicated tile slots and 128 additional that are shared with sprites.

To set the background, use the this code:
```c
set_bkg_data(first_bkg_tile, NUM_TILES, tiles);
set_bkg_tiles(orig_x, orig_y, map_w, map_h, map);
SHOW_BKG;
```
The first command loads sprite data into tile memory. The second command sets tile indices in the 32x32 background map. The `map` variable here is an array of 8-bit indices indicating which tiles to use for each coordinate. The last command shows the background layer.

The `tiles` and `map` arrays can be generated from a `.png` image using the `png2asset` tool. The same tileset can be reused between multiple maps.

The `move_bkg(x, y)` command can be used to set a pixel offset in order to get a scrolling map. `set_bkg_submap()` can also be useful for scrolling maps.

Note that when using multiple color palettes, `set_bgk_attributes()` (or its submap variation) must be used to identify which palette is associated with each background tile (this is not connected to the tiles set with `set_bkg_data`). Note that the sprite palettes and background palettes are separate in the hardware (8 of each).

Found [this](https://gumpyfunction.itch.io/game-boy-rpg-fantasy-tileset-free) tileset on `itch.io` that could be useful.
