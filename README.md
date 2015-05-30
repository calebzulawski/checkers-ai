# checkers-ai
Checkers AI for ECE469

## Requirements
This requires GCC 4.7 or later with C++11.  Other C++11 compatible compilers may work but no guarantees.  It appears to work in a Cygwin environment with the ``mingw-pthreads`` package.  To play the game, you must be using a terminal emulator that supports ANSI color codes.

To build and run with make:
```bash
cd /path/to/checkers-ai
make
./checkers-ai
```

in Cygwin:

```bash
cd /path/to/checkers-ai
make
./checkers-ai.exe
```