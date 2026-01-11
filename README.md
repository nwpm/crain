# Crain - Terminal Rain Animation

A lightweight terminal-based rain animation written in C using ncurses.

## Features

* **Modular Architecture**: Strict separation between Physics, Rendering, and Engine logic.
* **Dynamic Resizing**: Real-time adaptation to terminal window size changes using `KEY_RESIZE`.
* **Command-Line Interface**: Advanced option parsing using `getopt_long` for colors and intensity.
* **Physics-Based Movement**: Individual raindrop properties including mass-dependent velocity.
* **Splash Effects**: Collision detection and rendering at the terminal floor.

## Project Structure

The project is organized into distinct modules:

```
crain/
├── crain.c          # Main program with argument parsing
├── physics.c        # Physics simulation for raindrops
├── render.c         # Terminal rendering with ncurses
├── utils.c          # Utility functions (random number generation)
├── include/
│   ├── physics.h    # Physics module header
│   ├── render.h     # Render module header
│   └── types.h      # Data structure definitions
└── Makefile         # Build configuration
```

## Prerequisites

* A C compiler (GCC or Clang).
* Make

* Terminal with color support
* POSIX-compatible system (Linux, macOS, WSL)
* `ncurses` library installed

## Clone and build
```bash
git clone https://github.com/nwpm/rain-drops.git
cd crain
make install
```
## Manual Compilation
```bash
gcc -o crain crain.c physics.c render.c utils.c -lncurses
```
## Color options
* g - Green
* b - Blue
* c - Cyan
* m - Magenta
* r - Red
* y - Yellow
* w - White
* h - Rainbow mode (cycles through all colors)

## Controls
* 'q': Exit the simulation safely.
* Terminal Resize: The simulation automatically recalculates drop positions when the window is resized.

## Usage
```bash
# Basic usage (default blue color, 0.5 intensity)
./crain

# With color option
./crain -cb
./crain --color=green

# With intensity option
./crain -i0.8     # 80% intensity
./crain --intensity=0.3

# Rainbow mode
./crain -ch

# Show help
./crain -h
./crain --help

# Show version
./crain -v
./crain --version
```

## License
This project is open-source and available under the MIT License.
