# GatoCLI 0.631.0

This is the first README I'm writing mostly by myself, and I hardly know markdown, so bear with me.

## Objective:
- A small project to learn C
- Something to help me learn the terminal more
- ENTIRELY written in vim/nvim with minimal vibecoding!

## Build
To build the project, simply run:
```bash
make
```
The compiled binary will then appear in the bin/ directory.

To clean build artifacts, run:
```bash
make clean
```
## Optional Installation
To install the CLI system wide (to /usr/local/bin), just run:
```bash
sudo make install
```

After installing, you'll be able to run the program from anywhere.

To uninstall:
```bash
sudo make uninstall
```

## Usage

Run the compiled binary with a command and optional arguments:

```
gatocli <command> [options...]
```

Most commands share a simple convention: the first argument is the command name and the rest are positional values such as a username or repeat count. Use `--help` or `-h` to see this message at any time, and `--version`/`-v` to check the bundled version string.

## Commands

| Command | Description | Notes |
| --- | --- | --- |
| `greet [name]` | Prints a friendly greeting. | Defaults to “there” when no name is supplied. |
| `repeat <count>` | Prints numbered notifications. | Requires a positive integer count. |
| `hello <count>` | Prints “Hello!” with a running number. | Requires a positive integer count. |
| `add <num1> <num2>` | Adds two integers and prints the sum. | Requires two positive integers. |
| `echo [sentence]` | Repeats the provided words verbatim. | Needs at least one word to repeat. |
| `test` | Runs a simple developer test. | |
| `count [sentence] [character]` | Counts the occurences of a given character in a sentence | |
| `coinflip [times]` | Flips a coin one or more times.|Defaults to 1 time when no 'times' is supplied. |
| `banner [file]` | Dumps the contents of a banner file. | Defaults to `banner.txt` in the current directory. |
| `--help`, `-h` | Show the usage summary. | |
| `--version`, `-v` | Print the program version (`0.631.0`). | |

### `count <sentence> <character>`

Count reports how often a single character appears in the provided sentence. Wrap the sentence in quotes when it contains spaces and follow it with the character you want to count (only the first character of that argument is considered). Example:

```bash
gatocli count "Hello, world" o
```

This prints a message like `There are 2 occurrences of 'o'` and exits with `0`.

If you invoke a command without the required arguments, the program explains what is missing and exits with a non-zero status.

## Notes:
- This is the first proper project I'm making public!
- I don't know any C, so this project is helping me learn (though I do know Java and other languages which should hopefully make this experience easier for me)





<small><sub>6310</sub></small>
