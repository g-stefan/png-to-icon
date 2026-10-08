---
name: png-to-icon
description: >-
  How to use png-to-icon, the XYO command line tool and static library
  (png-to-icon.application, namespace XYO::PNGToICON) on top of
  xyo-pixel32 that converts PNG images into a Windows icon file (.ico):
  the three modes (one PNG resized to 16 / 24 / 32 / 48; --list prefix
  reading prefix-48/32/24/16.png; --list with --list-x 16,32,256 using
  each PNG at its own size, max 256); --level N (decimal 0-255, mask
  threshold, default 48; older versions read two hex digits, --level 80
  meant 128); --background RRGGBB (color of fully transparent pixels, the
  mask is unchanged); --license (xyo-pixel32 license with libpng / zlib);
  exit codes
  with silent failures; what the .ico holds (32 bit BGRA with alpha plus a
  1 bit mask per size, BITMAPINFOHEADER entries); the xyo-pixel32
  functions behind it (Process::pngToIcon three overloads, generateIcon,
  IconItem) and embedding XYO::PNGToICON::Application; putting the icon
  into an executable with "1 ICON" in Application.rc. Use when making or
  regenerating an application icon, running png-to-icon in a build or
  fabricare script, writing code that calls pngToIcon / generateIcon or
  includes <XYO/PNGToICON/Application.hpp>, depends on "png-to-icon" or
  "png-to-icon.application" in fabricare.json, or when working inside the
  png-to-icon repository.
---

# png-to-icon

Converts PNG images (RGBA with transparency) into a Windows `.ico`. Two
faces:

- the **`png-to-icon` command line tool**;
- the **static library `png-to-icon.application`**: the same tool as the
  class `XYO::PNGToICON::Application`, without `main`.

It has no conversion code of its own: it parses options and calls
`XYO::Pixel32::Process::pngToIcon` (see the `xyo-pixel32` skill; its rules
on `TPointer`, `Image`, `Bitmap` apply). In C++ prefer calling
`pngToIcon` directly.

Full documentation: `docs/` in the png-to-icon repository
(`X:\Storage\XYO\Gitea\CPP\png-to-icon\docs` on this machine): README,
getting-started, command-line, **icon-format**, library, reference. The
whole tool is `source/XYO/PNGToICON/Application.cpp`; the conversion is
`source/XYO/Pixel32/Process/png.cpp` and `icon.cpp` in xyo-pixel32.

## Pick a mode

| Need | Command |
|------|---------|
| One drawing, standard sizes | `png-to-icon app.png app.ico` → 16, 24, 32, 48 resized |
| Hand drawn small sizes | `png-to-icon --list app app.ico` → reads `app-48.png`, `app-32.png`, `app-24.png`, `app-16.png`, each resized to its slot; all four required |
| Other sizes (64, 128, 256 for high DPI / Explorer large icons) | `png-to-icon --list app --list-x 16,32,48,256 app.ico` → reads `app-16.png` ...; **not resized**, each stored at its file's size, ≤ 256, in list order |
| Hard / soft mask edge for old renderers | `--level 128` (decimal 0-255) |
| Color under fully transparent pixels | `--background FFFFFF` |

Every entry: 32 bit BGRA with the PNG's smooth alpha (what current Windows
draws) + a 1 bit mask, `mask = alpha < level` (what old renderers use).

## Hard rules

1. **Options take two dashes.** `-background` (one dash) is taken as the
   input file name and the run fails silently. Use `--background`.
2. **`--level` is decimal, 0 to 255** (the alpha scale): `0` = no
   transparency in the mask, `128` = half, `255` = all not fully opaque.
   Default `48`. Out of range or not a plain number (`256`, `-1`, `0x80`,
   `12x`) → `Error: level must be a number from 0 to 255`, exit `1`.
   Older versions read two hex digits: convert old scripts (`30` → `48`,
   `80` → `128`, `FF` → `255`). It changes only the 1 bit
   mask, never the alpha of the 32 bit image.
3. **`--background RRGGBB`** sets the RGB of alpha 0 pixels before
   resizing; the mask is **unchanged**. It reaches the icon only in images
   stored without resampling (`--list-x`, or a file already exactly its
   size); resampled images have black under transparent pixels. Invalid
   value → black, still enabled.
4. **Silent failures.** Missing / unreadable / non-PNG input, a `--list-x`
   image > 256, a missing output folder: **no message**, exit `1`. Only
   "no file names" and an option without its value print the usage, a bad
   `--level` prints an error (also exit `1`). Always check the exit code (`exitIf(Shell.system(...))`).
5. **File names**: first non option argument = input (or `--list`
   prefix), second = output, later ones ignored, unknown options count as
   file names. `--list` builds `prefix + "-" + size + ".png"`; spaces in
   `--list-x` become part of the name. `--list-x` without `--list` is
   ignored.
6. **Square input.** Resizing stretches to square, no padding. Downscale
   from ≥ 48 px (256 is common); hand draw 16 / 24 for crispness.
7. **The output is overwritten**, its folder is never created. Paths are
   relative to the current directory.
8. `--license` prints the xyo-pixel32 license on purpose (it carries the
   libpng / zlib licenses the PNG code needs); with no files exit `0`,
   with files it also converts.

## Put it into an executable

XYO applications keep `Application.ico` beside `Application.rc` in the
source folder:

```rc
1  ICON "Application.ico"
```

The lowest icon id is the one Explorer shows. Regenerate only when the PNG
changed (`fabricare/make.prepare.js`; there is no `Shell.isChanged` in
scripts):

```js
runInPath("source/XYO/MyTool", function() {
	if (!Shell.fileExists("Application.ico") || (Shell.compareLastWriteTime("Application.png", "Application.ico") > 0)) {
		exitIf(Shell.system("png-to-icon Application.png Application.ico"));
		Shell.touchIfExists("Application.rc");
	};
});
```

Touching the `.rc` makes the incremental build recompile the resources.
Usually the `.ico` is committed and regenerated by hand.

## From C++

```cpp
#include <XYO/Pixel32.hpp>
using namespace XYO::Pixel32;

Process::pngToIcon("app.png", "app.ico", 48, false, 0);                                    // one PNG
Process::pngToIcon("app-48.png", "app-32.png", "app-24.png", "app-16.png", "app.ico", 48, false, 0); // 48, 32, 24, 16 order!
TDynamicArray<String> list; list.push("app-16.png"); list.push("app-256.png");
Process::pngToIcon(list, "app.ico", 48, true, XYO_PIXEL32_PIXEL(0xFF, 0xFF, 0xFF, 0xFF));  // --list-x, --background
```

All return `bool`, print nothing, throw nothing. Depend on
`"xyo-pixel32"`. For other sizes from one image or another mask, build
`TDoubleEndedQueue<TPointer<IconItem>>` (`bmp = toBitmap32(image)`,
`mask = alphaToBitmap1(image, level)`, each ≤ 256) and call
`Process::generateIcon(list, file)`; items are written in push order.

Embedding the tool with its option parsing: depend on
`"png-to-icon.application"`, include `<XYO/PNGToICON/Application.hpp>`,
build a plain `char *` array (`cmdS[0]` = program name; `TDynamicArray` is
not contiguous) and call `XYO::PNGToICON::Application application;
application.main(cmdN, cmdS)` — it returns the exit code, prints to
stdout, never calls `exit`.

Metadata: `XYO::PNGToICON::Version::version()` etc. from
`<XYO/PNGToICON/Version.hpp>`, `Copyright.hpp`, `License.hpp`.

## Debugging an icon

Read the directory of an `.ico` (count, then 16 bytes per entry: width,
height (0 = 256), colors, reserved, planes, bit count, size, offset):

```bash
python -c "import struct,sys;d=open(sys.argv[1],'rb').read();n=struct.unpack('<H',d[4:6])[0];[print(struct.unpack('<BBBBHHII',d[6+16*i:22+16*i])) for i in range(n)]" app.ico
```

Each entry's data is a 40 byte `BITMAPINFOHEADER` (height doubled), then
`w*h*4` BGRA bytes bottom-up, then the mask, rows padded to 4 bytes.

## Working inside this repository

- Two projects in `fabricare.json`: `png-to-icon.application` (static
  lib, `XYO_PNGTOICON_LIBRARY`, version via `linkVersion`) and
  `png-to-icon` (exe). One version key, `png-to-icon`, in `version.json`.
- `fabricare make`, `fabricare install` (needs `xyo-pixel32` installed).
  There is no test project: try changes with `output/bin/png-to-icon` on
  sample PNGs (`xyo-pixel32/test/*.png`) in all three modes and dump the
  result as above. In a plain shell `cl.exe` is missing: run
  `fabricare make` after `vcvars64.bat`.
- Behavior changes to the conversion belong in xyo-pixel32
  (`Process/png.cpp`); this repository only parses options.
- Keep `docs/` and this skill in step with `Application.cpp` (the option
  behavior above was checked against the built tool).
- Licensing follows REUSE: `source/`, `docs/` and `README.md` are MIT
  (source files also carry SPDX headers); config, `fabricare.json`,
  `version.json` and `.claude/` are Unlicense. Every new top level file or
  folder needs a `Files:` entry in `.reuse/dep5` (check with `reuse lint`).
