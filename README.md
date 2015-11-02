# Checkers AI for ECE469
Caleb Zulawski


## Requirements
This software was developed with ``gcc`` 4.9.2+ and ``make`` on Linux and Windows (using Cygwin).  Other C++11 compatible compilers and older versions of ``gcc`` may work but no guarantees.  Building in the Cygwin environment requires the ``mingw-pthreads`` package.  To play the game, you must be using a terminal emulator that supports ANSI color codes.

## Installing and running
To build and run:
```sh
git clone https://github.com/calebzulawski/checkers-ai.git
cd checkers-ai
make
./checkers-ai
```

To load a board state:
```sh
./checkers-ai /path/to/board/file
```