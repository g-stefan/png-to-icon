# Icon format

What `png-to-icon` writes, so you can predict how the icon looks and what
to change in the PNG when it does not.

## The file

```
png-to-icon app.png app.ico      # app.png: 256 x 64 RGBA
```

```
ICONDIR       reserved 0, type 1 (icon), count 4
ICONDIRENTRY  16 x 16,  32 bit, 1128 bytes
ICONDIRENTRY  24 x 24,  32 bit, 2440 bytes
ICONDIRENTRY  32 x 32,  32 bit, 4264 bytes
ICONDIRENTRY  48 x 48,  32 bit, 9640 bytes
image 1       BITMAPINFOHEADER (height x 2), BGRA pixels bottom-up, 1 bit mask
...
```

- Every image is a `BITMAPINFOHEADER` bitmap (no PNG compressed entries),
  **32 bit BGRA** with the PNG's alpha, followed by a **1 bit mask** of the
  same size (rows padded to 4 bytes). The header height is twice the
  image height, as the format requires.
- A 256 pixel side is stored as `0` in the directory entry, as the format
  requires; Windows reads it as 256.
- The images are written in the order the tool builds them: 16, 24, 32, 48
  in the default and `--list` modes; the `--list-x` order otherwise.
  Windows picks the best size itself, the order does not matter to it.

Size of one entry: `40 + w × h × 4 + h × (((w + 31) / 32) × 4)` bytes, so a
256 × 256 image is about 264 KB uncompressed; a 16, 24, 32, 48, 256 set is
about 280 KB.

## Sizes

| Mode | Sizes | Source of each image |
|------|-------|----------------------|
| `in.png` | 16, 24, 32, 48 | `in.png` resized |
| `--list in` | 16, 24, 32, 48 | `in-16.png`, ... `in-48.png`, each resized to its slot |
| `--list in --list-x a,b,...` | the size of each file, ≤ 256 | `in-a.png`, `in-b.png`, ... as they are |

Where Windows uses them:

| Size | Typical use (100 % scaling) |
|------|------------------------------|
| 16 | title bar, small icons view, notification area |
| 24 | taskbar with small buttons |
| 32 | taskbar, `Alt+Tab`, desktop at small sizes |
| 48 | Explorer medium icons, desktop |
| 64, 96, 128 | high DPI scaling of the sizes above |
| 256 | Explorer large / extra large icons |

Missing sizes are scaled by Windows from the nearest one; with only 16 to
48, large views and high DPI screens show a blurry upscale. Add a 256
image with `--list-x` for those.

## Resizing

The default and `--list` modes resize with `XYO::Pixel32::Process::resize`
(scale up or down per axis, with alpha). Notes:

- **Square output.** A non square input is stretched, not padded.
- **Fully transparent pixels become black** (`RGB 0, alpha 0`) in a
  resized image. Invisible on current Windows, but a renderer that ignores
  the alpha and uses only the mask shows them black where the mask is
  opaque, that is with `--level 0`.
- **A file already at its size is copied** unchanged (no resampling).
- Fine detail at 16 and 24 pixels is lost by any scaler. Draw those by
  hand and use `--list`.

## The mask

The 1 bit mask is what old renderers (Windows XP and older, some
toolkits) use instead of the alpha channel: a mask bit `1` means
"transparent, show the screen", `0` means "draw the color pixel".

```
mask bit = (alpha < level) ? 1 : 0       level = --level, 0 to 255, default 48
```

| Alpha of the pixel | Current Windows | Old renderer, level `48` | Old renderer, level `128` |
|--------------------|-----------------|----------------------------|----------------------------|
| 0 | invisible | invisible | invisible |
| 32 | 12 % visible | invisible | invisible |
| 96 | 37 % visible | drawn, as the full color | invisible |
| 255 | opaque | drawn | drawn |

A low level keeps soft edges and shadows as solid pixels on old
renderers; a high level cuts them. It has no effect on how current Windows
draws the icon.

## --background

Fully transparent pixels of the loaded PNG get the `--background` color
before resizing; the mask is unchanged. So the color shows only in
images stored without resampling (`--list-x`, or a file already at its
size) and only in renderers that ignore alpha, under pixels the mask
leaves visible. In practice it matters with `--level 0`, or for a
toolchain that converts the icon to a format without alpha.

## Pitfalls

| Situation | What happens | Do this |
|-----------|--------------|---------|
| exit code `1`, no message | input missing / not a PNG, `--list-x` image > 256, output folder missing | check the names (`--list` adds `-NN.png`), the sizes, the folder |
| `-background FF0000` (one dash) | taken as the input file name; the run fails | `--background` |
| `--level 80` from an old script (hex) | now 80 decimal, a harder edge than intended | `--level 128` (hex `80`) |
| `--level 256`, `--level 0x80` | `Error: level must be a number from 0 to 255`, exit `1` | decimal 0 to 255 |
| non square PNG | stretched | pad it to a square |
| `--list` with a missing `in-24.png` | the whole run fails | the plain `--list` mode needs all four of 16, 24, 32, 48; use `--list-x` for another set |
| `--list-x "16, 32"` | looks for `in- 32.png` | no spaces |
| `--list-x` without `--list` | ignored, `in` is read as one PNG | add `--list` |
| blurry icon in Explorer large view | no image above 48 | add 256 with `--list-x` |
| smudged 16 pixel icon | downscaled from a big drawing | hand drawn `in-16.png` with `--list` |
| PNG without alpha | no transparency at all | export with an alpha channel |
| the `.ico` changed but the `.exe` still shows the old icon | the `.rc` was not recompiled, or Explorer's icon cache | touch the `.rc` (rebuild); the cache refreshes on its own or after `ie4uinit.exe -show` |
