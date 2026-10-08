# Library

There are two ways to make icons from C++:

| Way | Depend on | Use it when |
|-----|-----------|-------------|
| call `XYO::Pixel32::Process::pngToIcon` | `xyo-pixel32` | the usual case: one function call, no option parsing |
| run the tool class `XYO::PNGToICON::Application` | `png-to-icon.application` | you have command line style arguments (a build tool, a script host) and want exactly the `png-to-icon` behavior |

`png-to-icon` has no conversion code of its own; both ways end in the
same three `xyo-pixel32` functions.

## pngToIcon (xyo-pixel32)

```cpp
#include <XYO/Pixel32.hpp>

namespace XYO::Pixel32::Process {
	// 16, 24, 32, 48 resized from one PNG
	bool pngToIcon(const char *pngFile, const char *iconFile,
	               int level, bool useBackgroundAlpha, uint32_t background);
	// 16, 24, 32, 48, one PNG per size, each resized to its slot
	bool pngToIcon(const char *png48, const char *png32, const char *png24, const char *png16,
	               const char *iconFile, int level, bool useBackgroundAlpha, uint32_t background);
	// one image per PNG, at the PNG's own size (<= 256), in list order
	bool pngToIcon(TDynamicArray<String> &pngFile, const char *iconFile,
	               int level, bool useBackgroundAlpha, uint32_t background);
};
```

| Parameter | Meaning | Tool option |
|-----------|---------|-------------|
| `level` | mask: alpha `< level` is transparent; the tool's default is `48` | `--level` |
| `useBackgroundAlpha` | `true` to apply `background` | `--background` given |
| `background` | a `Pixel`, `XYO_PIXEL32_PIXEL(r, g, b, 0xFF)`; fully transparent pixels get its RGB | `--background RRGGBB` |

All three return `true` on success and `false` on any failure (a PNG
missing or unreadable, an image larger than 256, the output can not be
written); they print nothing and throw nothing. The output is
overwritten, its folder must exist. Details of what is written are in
[Icon format](icon-format.md).

```cpp
using namespace XYO::Pixel32;

// png-to-icon app.png app.ico
Process::pngToIcon("app.png", "app.ico", 48, false, 0);

// png-to-icon --list app app.ico   (note the order: 48, 32, 24, 16)
Process::pngToIcon("app-48.png", "app-32.png", "app-24.png", "app-16.png", "app.ico", 48, false, 0);

// png-to-icon --background FFFFFF --list app --list-x 16,32,256 app.ico
TDynamicArray<String> fileList;
fileList.push("app-16.png");
fileList.push("app-32.png");
fileList.push("app-256.png");
Process::pngToIcon(fileList, "app.ico", 48, true, XYO_PIXEL32_PIXEL(0xFF, 0xFF, 0xFF, 0xFF));
```

In `fabricare.json`:

```json
"dependency": [ "xyo-pixel32" ]
```

## Building a custom icon

For other sizes from one image, other bit depths or a different mask,
build the `IconItem` list yourself and call `generateIcon` (all
`xyo-pixel32`):

```cpp
#include <XYO/Pixel32.hpp>

using namespace XYO::Pixel32;

bool makeIcon(const char *pngFile, const char *iconFile) {
	TPointer<Image> source = Process::pngLoad(pngFile);
	if (!source) {
		return false;
	};

	TDoubleEndedQueue<TPointer<IconItem>> iconList;
	static const long int size[] = {16, 24, 32, 48, 64, 256};
	for (int k = 0; k < 6; ++k) {
		TPointer<Image> image = Process::resize(source, size[k], size[k]);
		if (!image) {
			return false;
		};
		TPointer<IconItem> item;
		item.newMemory();
		item->bmp = Process::toBitmap32(image);            // smooth alpha
		item->mask = Process::alphaToBitmap1(image, 48); // 1 bit mask, alpha < 48 transparent
		if (!item->bmp || !item->mask) {
			return false;
		};
		iconList.push(item);
	};

	// written in push order; false for an empty list, an item > 256, a bad mask, a write error
	return Process::generateIcon(iconList, iconFile);
};
```

`Image`, `Bitmap` and `IconItem` are managed objects: hold them in
`TPointer`, never copy them. The full image API (`resize`, `cut`, `wrap`,
`kernel3X3`, palettes with `bitmap32To8Alpha1`, ...) is documented in the
`xyo-pixel32` repository, `docs/`.

## Embedding the command line tool

Depend on `png-to-icon.application`, the tool compiled as a static
library without `main` (`XYO_PNGTOICON_LIBRARY` is set when it is built,
the consumer does not need it):

```json
"dependency": [ "png-to-icon.application" ]
```

```cpp
#include <XYO/PNGToICON/Application.hpp>

int runPngToIcon(TDynamicArray<String> &arguments) {
	int cmdN = (int)arguments.length() + 1;
	char **cmdS = new char *[cmdN];
	cmdS[0] = const_cast<char *>("png-to-icon");
	for (int k = 1; k < cmdN; ++k) {
		cmdS[k] = const_cast<char *>(arguments[k - 1].value());
	};
	int retV;
	{
		XYO::PNGToICON::Application application;
		retV = application.main(cmdN, cmdS);
	};
	delete[] cmdS;
	return retV;
};
```

`cmdS[0]` is the program name and is skipped. `TDynamicArray` is not
contiguous memory, so build a plain `char *` array. The tool prints the
usage / license to `stdout` and returns the exit code; it never calls
`exit`. The options are in [Command line](command-line.md).

`xyo-pixel32` and the layers below come in as transitive dependencies.
The program still needs its own entry point (`XYO_APPLICATION_MAIN` of
`xyo-system`), which initializes the managed memory registry.

## Tool metadata

```cpp
#include <XYO/PNGToICON/Version.hpp>
#include <XYO/PNGToICON/Copyright.hpp>
#include <XYO/PNGToICON/License.hpp>

XYO::PNGToICON::Version::version();            // "5.9.0"
XYO::PNGToICON::Version::build();
XYO::PNGToICON::Version::versionWithBuild();
XYO::PNGToICON::Version::datetime();
XYO::PNGToICON::Copyright::copyright();
XYO::PNGToICON::License::license();            // std::string
```
