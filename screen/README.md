# screen

Files in this section:

- `screen/README.md` (this file)
- `screen/screen.h` (C99 interface)
- `screen/screen.c` (fixed pixel buffer and draw primitives)
- `screen/screen_map.c` (standalone harness: draw, move figure, eyes mono round-trip)

## What this is

Shakti's owned **screen**: one fixed RGBA pixel surface inside her shell.

- Draw writes pixels onto the buffer.
- Eyes read the same pixels (`eyes_pull_mono` / reconstruct / `eyes_diff`).
- Error rule matches eyes: a pixel is a pixel; error is any pixel that cannot rebuild.
- No window system, no GPU, no JS, no camera, no threads, no dynamic allocation.
- Not the Swift phone display. A later host may mirror this buffer only.
- Not awareness, not freedom language, not a second core. Pixels only.

## Defaults (this build)

| Item | Choice |
|------|--------|
| Size | 64 x 64 |
| Storage | RGBA (white paper, black ink) |
| Truth path | mono pull through eyes |
| Self figure | abstract 8 x 8 ink sprite |
| Output | `screen/output/` only |

## Build and run

From the repository root:

```sh
make screen
```

Builds `screen/screen_map`, runs the harness, writes frames under `screen/output/`,
prints frame size, pixels drawn, and eyes mono drift (expect 0).

## API summary

- `screen_init` / `screen_clear` — blank white paper at 64 x 64
- `screen_set_pixel` / `screen_get_pixel` — single pixel
- `screen_fill_rect` — solid ink or paper rectangle
- `screen_blit_glyph` — stamp one clean 8 x 8 eyes-font glyph
- `screen_blit_sprite` — stamp an 8 x 8 bit pattern (self figure)
- `screen_rgba` / `screen_width` / `screen_height` — present buffer to eyes

## Harness proof

1. Clear screen.
2. Draw self sprite at (x, y).
3. Still frame: mono pull → reconstruct → diff (drift must be 0).
4. Move sprite across a few frames (harness loop, not a daemon).
5. Write `.tan` recon artifacts under `screen/output/`.

## Out of scope here

- `src/**`, `include/**`, MCP menu, Swift host
- Camera, School, curriculum
- Animation engine beyond harness frame steps
