# Getting started

## 1. Build and install

The tool is built with [fabricare](https://github.com/g-stefan/fabricare).
`xyo-platform`, `xyo-managed-memory`, `xyo-data-structures`,
`xyo-multithreading`, `xyo-encoding`, `xyo-system`, `xyo-cryptography` and
`xyo-pixel32` (with `vendor-libpng` and `vendor-zlib`) must be installed to
the SDK first. From the repository root:

```bash
fabricare make       # build into output/
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

After `install`, `png-to-icon` is in `~/.fabricare/<platform>/bin`, which
is on the `PATH` of a fabricare build, so the scripts of other projects can
call it.

Two projects are produced (`fabricare.json`):

| Project | Kind | Use it when |
|---------|------|-------------|
| `png-to-icon.application` | static library with the command line tool, no `main` (`XYO_PNGTOICON_LIBRARY`) | run `png-to-icon` inside another program |
| `png-to-icon` | executable | the `png-to-icon` command |

The library reuses the executable's version (`"linkVersion"`), so
`version.json` has a single key, `png-to-icon`.

The tool builds on Windows and Linux; the `.ico` it writes is a Windows
format, but it can be produced on any platform (for cross builds).

## 2. Prepare the PNG

- **RGBA with transparency.** Any PNG libpng reads works (gray, palette,
  RGB, 16 bit, with or without alpha); it is converted to 8 bit RGBA. A
  PNG without alpha gives an icon with no transparent pixels.
- **Square.** The single file mode resizes to 16 × 16, 24 × 24, 32 × 32
  and 48 × 48 without keeping the aspect ratio; a 256 × 64 image is
  squashed. Pad the drawing to a square first.
- **Big enough.** Downscaling looks better than upscaling: draw at 48
  pixels or more (256 is common). For crisp small sizes draw them by hand
  and use `--list`.
- **At most 256 × 256** for `--list-x`, where every file is stored at its
  own size.

## 3. First icon

```
png-to-icon app.png app.ico
```

Exit code `0` and an `app.ico` with four images: 16, 24, 32 and 48
pixels, each a 32 bit color bitmap with the smooth alpha of the PNG plus a
1 bit mask where alpha is below 48.

Hand drawn small sizes:

```
app-16.png  app-24.png  app-32.png  app-48.png
png-to-icon --list app app.ico
```

Bigger sizes, for high DPI and the Explorer "large icons" view:

```
app-16.png  app-32.png  app-48.png  app-256.png
png-to-icon --list app --list-x 16,32,48,256 app.ico
```

All the options are in [Command line](command-line.md).

## 4. Put the icon into a program

XYO applications keep `Application.ico` in their source folder and name
it in `Application.rc`:

```rc
#include <XYO/MyTool/Application.rh>
#include <XYO/MyTool/Copyright.rh>
#include <XYO/MyTool/Version.rh>

1  ICON "Application.ico"

XYO_PLATFORM_VERSION_INFO(XYO_MYTOOL,"my-tool.exe","My Tool")
XYO_PLATFORM_MANIFEST("Application.manifest")
```

The icon with the lowest id (here `1`) is the one Explorer shows for the
`.exe`. To regenerate it in the build, in the project's
`fabricare/make.prepare.js`:

```js
messageAction("make.prepare");

runInPath("source/XYO/MyTool", function() {
	if (!Shell.fileExists("Application.ico") || (Shell.compareLastWriteTime("Application.png", "Application.ico") > 0)) {
		exitIf(Shell.system("png-to-icon Application.png Application.ico"));
		Shell.touchIfExists("Application.rc");
	};
});
```

`png-to-icon` always writes; the check keeps the build incremental, and
touching `Application.rc` makes the resource compiler pick up the new
icon. Most projects commit the `.ico` and run the tool only by hand when
the artwork changes.

## 5. From C++

Without a process, call the `xyo-pixel32` function the tool uses:

```cpp
#include <XYO/Pixel32.hpp>

using namespace XYO::Pixel32;

// 16, 24, 32 and 48 pixels, alpha below 48 is transparent in the mask
if (!Process::pngToIcon("app.png", "app.ico", 48, false, 0)) {
	printf("* Error: app.png\n");
	return 1;
};
```

Or run the tool itself, with its option parsing, through
`png-to-icon.application`. Both are in [Library](library.md).

## 6. Conventions

- **Exit code is the only error report.** A missing or unreadable PNG,
  a PNG larger than 256 pixels in `--list-x`, or an output that can not be
  written all end with exit code `1` and **no message**. Check it.
- **Paths are used as given**, relative to the current directory. The
  output folder must exist; it is not created.
- **The output is overwritten.**
- **`--level` is decimal, 0 to 255**, the alpha scale; anything else is
  an error. See [Command line](command-line.md#--level).
- **Two dashes for every option**: `--background`. A one dash option is
  taken as a file name.
