# PNG to Icon

Convert PNG to Icon
- Makes a Windows icon file (`.ico`) from PNG images with transparency:
every size is a 32 bit image with the PNG's alpha plus a 1 bit mask.
- `png-to-icon` command line tool: one PNG resized to 16, 24, 32 and 48
pixels; `--list` one PNG per size; `--list-x` any set of sizes up to 256;
`--level` mask threshold; `--background` color of transparent pixels.
- Library `png-to-icon.application`: the tool as a class,
`XYO::PNGToICON::Application`, to run it inside other programs.

Built on `xyo-pixel32` (`XYO::Pixel32::Process::pngToIcon`).

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, prepare the PNG, first icon, put it into a program
- [Command line](docs/command-line.md) - `png-to-icon` options, modes, `--level`, `--background`, exit codes, examples
- [Icon format](docs/icon-format.md) - what the `.ico` holds, sizes, resizing, the mask, pitfalls
- [Library](docs/library.md) - `pngToIcon`, custom icons, embedding the tool
- [API reference](docs/reference.md)

A Claude Code skill for this tool is in
[.claude/skills/png-to-icon](.claude/skills/png-to-icon/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
