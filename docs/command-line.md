# Command line

```
png-to-icon --license
png-to-icon [--level 0-255] [--background RRGGBB] in.png out.ico
png-to-icon [--level 0-255] [--background RRGGBB] --list in out.ico
png-to-icon [--level 0-255] [--background RRGGBB] --list in --list-x 16,24,32... out.ico
```

## Options

| Option | Effect |
|--------|--------|
| `--license` | print the license (the xyo-pixel32 license, which includes the libpng and zlib licenses of the PNG code); exit `0` if no files are given, otherwise convert as well |
| `--level N` | mask threshold, decimal `0` to `255`: alpha below `N` is transparent in the 1 bit mask; default `48` |
| `--background RRGGBB` | color, six hex digits, given to the fully transparent pixels of the color bitmap (see below) |
| `--list` | `in` is a name prefix, one PNG per size (see [Modes](#modes)) |
| `--list-x a,b,...` | with `--list`: the sizes to read, comma separated, no spaces |
| first other argument | input: a PNG file, or the prefix with `--list` |
| second other argument | output `.ico` file; overwritten; its folder must exist |

Options can come in any order, before or after the file names. Every
argument that is not one of the options above is a file name; the third
and later ones are ignored. That includes unknown options and options
with one dash (`-background`): they are taken as file names.

Running with no file names prints the usage and exits `1`.

## Modes

### One PNG

```
png-to-icon app.png app.ico
```

`app.png` is resized to 16, 24, 32 and 48 pixels (square, aspect ratio not
kept) and the four images are written in that order.

### --list: one PNG per size

```
png-to-icon --list app app.ico
```

reads `app-48.png`, `app-32.png`, `app-24.png` and `app-16.png` (the
prefix, `-`, the size, `.png`). All four must exist. Each one is still
resized to its slot, so a file that is not exactly its size is scaled.
The icon holds 16, 24, 32, 48 in that order.

### --list with --list-x: any sizes

```
png-to-icon --list app --list-x 16,32,48,256 app.ico
```

reads `app-16.png`, `app-32.png`, `app-48.png` and `app-256.png`. Here the
images are **not** resized: each is stored at the size of its file (the
number in the name is only used to build the name), at most 256 × 256,
in the order of the list. A file that is missing, unreadable or bigger
than 256 pixels fails the whole run. A space in the list becomes part of
the name (`--list-x "16, 32"` looks for `app- 32.png`).

`--list-x` without `--list` is ignored.

## --level

The 1 bit mask (the "AND" mask that Windows XP and older renderers use)
marks a pixel transparent when its alpha is **below** the level. The 32
bit color bitmap keeps the PNG's full alpha whatever the level, which is
what current Windows draws.

The value is a **decimal** number from `0` to `255`, the same scale as
the alpha channel.

| Argument | Effect |
|----------|--------|
| (none) | `48`, the default: almost transparent pixels are cut |
| `--level 0` | nothing is transparent in the mask |
| `--level 128` | half transparent and below is cut |
| `--level 255` | everything not fully opaque is cut |
| `--level 256`, `--level -1`, `--level 0x80`, `--level 12x` | `Error: level must be a number from 0 to 255`, exit `1` |

A missing value (`--level` as the last argument) prints the usage and
exits `1`.

Older versions read the value as two hex digits
(`--level 80` was 128). Convert old build scripts: `--level 30` → `48`,
`--level 80` → `128`, `--level FF` → `255`.

## --background

```
png-to-icon --background FFFFFF app.png app.ico
```

The fully transparent pixels (alpha 0) of every loaded PNG get the color
`RRGGBB`, still with alpha 0. Programs that ignore alpha and the mask then
show that color instead of black. The mask is **not** changed: it still
follows `--level`.

The color reaches the icon only for images that are stored without
resampling: every image in `--list-x` mode, and in the other modes an
image whose file is already exactly its size (e.g. the 48 pixel entry made
from a 48 × 48 PNG). Resampled images have black (`000000`) under their
fully transparent pixels.

An invalid value (not six hex digits) means black, still enabled. A
missing value prints the usage and exits `1`.

## Exit codes

| Code | Meaning |
|------|---------|
| `0` | icon written; also `--license` with no files |
| `1` | no file names (usage printed), `--level` / `--background` without a value, `--level` not a number from 0 to 255, an input missing or not a PNG, a `--list-x` image larger than 256 pixels, the output can not be written |

An invalid `--level` prints an error; conversion errors print **nothing**: only the exit code says it failed.
Always check it (`exitIf(Shell.system(...))` in fabricare scripts).

## Examples

Default icon from one drawing:

```
png-to-icon Application.png Application.ico
```

Crisp small sizes drawn by hand:

```
png-to-icon --list Application Application.ico
```

Modern set with a 256 pixel image, white under transparent pixels:

```
png-to-icon --background FFFFFF --list Application --list-x 16,24,32,48,64,256 Application.ico
```

Hard mask edge (only nearly opaque pixels are kept by old renderers):

```
png-to-icon --level 192 Application.png Application.ico
```

From a fabricare script:

```js
runInPath("source/XYO/MyTool", function() {
	exitIf(Shell.system("png-to-icon Application.png Application.ico"));
});
```
