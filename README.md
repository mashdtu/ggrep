# ggrep

A `grep`-like command-line utility written in C. Searches text for a literal substring and prints matching lines.

## Build

```bash
make
```

## Usage

```bash
./build/ggrep [options] pattern [file...]
```

If no files are given, ggrep reads from stdin.

### Examples

```bash
./build/ggrep "hello" file.txt
./build/ggrep -i "hello" file.txt
./build/ggrep -in "hello" *.txt
cat file.txt | ./build/ggrep "hello"
```

## Options

| Option | Description |
|--------|-------------|
| `-i` | Ignore case when matching |
| `-n` | Print line numbers |
| `-v` | Invert match (print non-matching lines) |
| `-h` | Show help message |
| `--help` | Show help message |

Options can be combined, e.g. `-in` or `-inv`.

## Testing

```bash
make test
```

Runs the automated test suite in `tests/test.sh`, which covers basic matching, all flags, stdin, multiple files, exit codes, error handling, and edge cases.

## ginnOS

`ggrep` is a core utility that ships with [ginnOS](https://github.com/lassedtu/ginnOS). It serves as the default text search tool in the system.

## Limitations

`ggrep` performs literal substring matching and does not support regular expressions.

## License

This project is released under the [Unlicense](LICENSE).
