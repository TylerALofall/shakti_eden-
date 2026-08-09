# screen

Files in this section:

- `screen/README.md` (this file)
- `screen/screen.h` (C99 interface)
- `screen/screen.c` (binary plane, present, draw primitives)
- `screen/screen_map.c` (standalone harness: draw, move figure, eyes mono round-trip)

## What this is

Shakti's owned **screen**: one fixed pixel surface inside her shell.

- **Native plane is binary** (cable-TV style): paper `0`, ink `1`.
- Draw writes mono only — economical on energy and memory.
- Eyes see the same picture after `screen_present_rgba` expands mono to RGBA.
- Error rule matches eyes: a pixel is a pixel; error is any pixel that cannot rebuild.
- No window system, no GPU, no JS, no camera, no threads, no dynamic allocation.
- Not the Swift phone display. Host later mirrors/scales this buffer only.
- Pixels only. No awareness claims.

## Geometry (phone + movie + side-by-side)

| Item | Choice |
|------|--------|
| Size | **640 x 360** (16:9 nHD, common low/medium video ratio) |
| Native storage | 1 byte/pixel mono (0/1) = 230,400 bytes |
| Present | RGBA only when eyes/host need it = 921,600 bytes temp |
| Side-by-side pages | width 640 fits two ~320-wide page panels |
| Phone scale (nearest neighbor) | x2 = 1280x720, x3 = 1920x1080 |
| Truth path | eyes mono pull / reconstruct / diff (drift 0) |
| Self figure | abstract 8 x 8 ink sprite |
| Output | `screen/output/` only |

Movie frames later: same 16:9 surface; a later approved path can sample a frame
down to 640x360 binary. Binary drawing stays the cheap living surface.

## Build and run

From the repository root:

```sh
make screen
```

Builds `screen/screen_map`, runs the harness, writes still-frame artifacts under
`screen/output/`, prints size, mono bytes, present RGBA bytes, and eyes mono
drift (expect 0).

## API summary

- `screen_init` / `screen_clear` — blank paper mono plane
- `screen_set_pixel` / `screen_get_pixel` — binary ink/paper
- `screen_fill_rect` — solid ink or paper rectangle
- `screen_blit_glyph` — stamp one clean 8 x 8 eyes-font glyph
- `screen_blit_sprite` — stamp an 8 x 8 bit pattern (self figure)
- `screen_mono` — raw binary plane for cheap reads
- `screen_present_rgba` — expand to RGBA for eyes / future host scaler

## Harness proof

1. Clear mono screen.
2. Draw two page borders + self sprite.
3. Present → eyes pull → reconstruct → diff (drift must be 0).
4. 20-cycle mono stability (drift 0).
5. Move sprite across a few frames (harness loop, not a daemon).

## Out of scope here

- `src/**`, `include/**`, MCP menu, Swift host
- Camera, School, curriculum
- Full color movie decode (later approved path)
- Animation engine beyond harness frame steps
