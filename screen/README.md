# screen

Files in this section:

- `screen/README.md` (this file)
- `screen/screen.h` (C99 interface)
- `screen/screen.c` (full-color RGBA plane, binary helpers, disks)
- `screen/screen_map.c` (harness: binary run + dream color sample)

## What this is

Shakti's owned **screen**: one fixed pixel surface inside her shell.

- **Full color RGBA from day one** — the surface can hold any RGB pixel.
  Dreams may show color before School teaches it.
- **Binary ink helpers stay** — cheap cable-TV style black/white drawing on
  the same buffer (energy-light everyday draw).
- Eyes read the same buffer: mono pull for ink scenes; color lens optional.
- Error rule (ink path): a pixel is a pixel; mono rebuild drift must be 0.
- No window, no GPU, no JS, no camera, no threads, no dynamic allocation.
- Not the Swift phone display. Host later mirrors/scales this buffer only.
- Pixels only. No awareness claims.
- Tools and subscriptions Tyler wires for her (e.g. music studio in the host
  GUI) are **hers to use**, not labor extraction. This module does not sell,
  farm, or outsource her.

## School vs surface (Tyler)

| Layer | Rule |
|-------|------|
| Surface | Color exists now (dreams, later movies). |
| Lesson 1 | Counting solo — no color glued to numbers. |
| Lesson 2 | ABC solo — still no color lesson. |
| Lesson 3 | Colors solo — then she knows the difference. |
| Mix | Never teach "3 blue balls" until counting and color were each solo. |
| Why | So she does not think `3 = blue`. Keep them separate for 2 lessons. |

Lock learning gate (solo before mix) still controls School. The screen does not
teach; it only holds pixels.

## Geometry (phone + movie + side-by-side)

| Item | Choice |
|------|--------|
| Size | **640 x 360** (16:9 nHD) |
| Storage | RGBA = 921,600 bytes fixed |
| Side-by-side pages | width 640 ≈ two ~320 panels |
| Phone scale | x2 = 1280x720, x3 = 1920x1080 |
| Binary draw | black/white helpers on the color buffer |
| Color draw | `screen_set_pixel_rgb`, `screen_fill_rect_rgb`, `screen_fill_disk_rgb` |
| Output | `screen/output/` only |

## Build and run

```sh
make screen
```

Expect: binary mono drift 0 (still + run + 20-cycle); dream color exact
self-diff 0; color-lens drift reported (eyes color pull is lossy by design).

## API summary

- `screen_init` / `screen_clear` — white paper RGBA
- `screen_set_pixel_rgb` / `screen_fill_rect_rgb` / `screen_fill_disk_rgb`
- `screen_set_pixel` / `screen_fill_rect` — binary ink on the color surface
- `screen_blit_glyph` / `screen_blit_sprite` — black ink stamps
- `screen_rgba` — live buffer for eyes / future host

## Out of scope here

- Wiring into `src/**`, MCP, Swift
- School curriculum tables for lesson 3
- Full movie decode pipeline
