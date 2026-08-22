# ggrep

A `grep`-like command-line utility written in C. Searches text for a literal substring and prints matching lines.

## Build

```bash
make
```

## Usage

```bash
./build/ggrep "pattern" file.txt
```

## ginnOS

`ggrep` is a core utility that ships with [ginnOS](https://github.com/lassedtu/ginnOS). It works as the default text search tool in the system.

## Limitations

`ggrep` performs literal substring matching and does not support regular expressions (yet).

## (Un)license

This project is part of the ginnOS ecosystem, which all shares the UNLICENSE.
