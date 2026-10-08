# API reference

## Headers

| Header | Contents |
|--------|----------|
| `<XYO/PNGToICON/Application.hpp>` | `XYO::PNGToICON::Application`, the command line tool class |
| `<XYO/PNGToICON/Dependency.hpp>` | `<XYO/Pixel32.hpp>`, `namespace XYO::PNGToICON { using namespace XYO::Pixel32; }` |
| `<XYO/PNGToICON/Copyright.hpp>`, `License.hpp`, `Version.hpp` | tool metadata |
| `<XYO/Pixel32.hpp>` (xyo-pixel32) | `Process::pngToIcon`, `Process::generateIcon`, `IconItem`, the image API |

## Macros

| Macro | Meaning |
|-------|---------|
| `XYO_PNGTOICON_LIBRARY` | build `Application.cpp` without `main` (set by the `png-to-icon.application` project) |
| `XYO_PNGTOICON_VERSION_*` (`Version.rh`) | version for the Windows resource, generated from `Version.Template.rh` by xyo-version |
| `XYO_PNGTOICON_COPYRIGHT`, `_PUBLISHER`, `_COMPANY`, `_CONTACT` (`Copyright.rh`) | copyright strings |

## class XYO::PNGToICON::Application

```cpp
class Application : public virtual IApplication {
	public:
		void showUsage();
		void showLicense();
		void showVersion();
		int main(int cmdN, char *cmdS[]);
		static void initMemory();
};
```

| Member | Does |
|--------|------|
| `main` | the `png-to-icon` command: parses `cmdS[1..cmdN-1]`, converts, returns the exit code |
| `showUsage` | prints name, version, copyright and the usage to `stdout` |
| `showLicense` | prints the xyo-pixel32 license (with the libpng / zlib licenses it carries) to `stdout` |
| `showVersion` | prints `version X build Y [date time]` |
| `initMemory` | initializes the managed memory of `String` and `TDynamicArray<String>` |

Not copyable, not movable. See [Command line](command-line.md) and
[Library](library.md#embedding-the-command-line-tool).

## namespace XYO::PNGToICON::Version / Copyright / License

```cpp
const char *XYO::PNGToICON::Version::version();
const char *XYO::PNGToICON::Version::build();
const char *XYO::PNGToICON::Version::versionWithBuild();
const char *XYO::PNGToICON::Version::datetime();

const char *XYO::PNGToICON::Copyright::copyright();
const char *XYO::PNGToICON::Copyright::publisher();
const char *XYO::PNGToICON::Copyright::company();
const char *XYO::PNGToICON::Copyright::contact();

std::string XYO::PNGToICON::License::license();
std::string XYO::PNGToICON::License::shortLicense();
```

## xyo-pixel32 functions used by the tool

| Function | Tool mode |
|----------|-----------|
| `bool Process::pngToIcon(const char *pngFile, const char *iconFile, int level, bool useBackgroundAlpha, uint32_t background)` | `in.png out.ico` |
| `bool Process::pngToIcon(const char *png48, const char *png32, const char *png24, const char *png16, const char *iconFile, int level, bool useBackgroundAlpha, uint32_t background)` | `--list in out.ico` |
| `bool Process::pngToIcon(TDynamicArray<String> &pngFile, const char *iconFile, int level, bool useBackgroundAlpha, uint32_t background)` | `--list in --list-x ... out.ico` |
| `bool Process::generateIcon(TDoubleEndedQueue<TPointer<IconItem>> &itemList, const char *fileName)` | writes the `.ico` (called by `pngToIcon`) |

Details: [Library](library.md), [Icon format](icon-format.md).

## Command line options

`--license`, `--level N` (decimal 0 to 255, default `48`),
`--background RRGGBB`, `--list`, `--list-x a,b,...`, then `in` and
`out.ico`.

| Exit code | Meaning |
|-----------|---------|
| `0` | icon written, or `--license` alone |
| `1` | no file names, option without value, `--level` out of range, conversion failed (no message) |

## fabricare projects

| Project | Kind | Notes |
|---------|------|-------|
| `png-to-icon.application` | static library, tool without `main` | defines `XYO_PNGTOICON_LIBRARY` for its own build; version from `png-to-icon` (`linkVersion`) |
| `png-to-icon` | executable | the `png-to-icon` command |

Both depend on `xyo-pixel32`.
