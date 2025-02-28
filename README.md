# Endless Dungeon

A simple procedural dungeon generator created using Raylib. This program generates random dungeons with rooms, corridors, and start/end points.

## Features

- Procedurally generated dungeons with rooms and corridors
- Each dungeon has a starting point (green) and end point (red)
- Generate a new dungeon by pressing the spacebar
- Simple grid-based visualization

## Requirements

- Raylib library (https://www.raylib.com/)
- C compiler (gcc or clang)

## Setup Instructions for macOS

1. **Install Raylib** (if not already installed)

   Using Homebrew:
   ```
   brew install raylib
   ```

2. **Download Project Files**
   
   Save these files to your project directory:
   - `endless_dungeon.c` (main source code)
   - `Makefile` (compilation instructions)

3. **Build the Project**

   Open Terminal, navigate to your project directory, and run:
   ```
   make
   ```

4. **Run the Game**

   After building, run:
   ```
   ./endless_dungeon
   ```
   
   Or use:
   ```
   make run
   ```

## Controls

- **SPACE**: Generate a new dungeon
- **ESC**: Exit the game

## How It Works

1. **Room Generation**: The program places random non-overlapping rooms on the grid
2. **Corridor Creation**: Rooms are connected with L-shaped corridors
3. **Start and End Points**: Placed in the first and last rooms of the dungeon

## Project Structure

- **Main Loop**: Handles input, updates, and rendering
- **Dungeon Generation**: Creates rooms, connects them with corridors
- **Drawing**: Renders the dungeon grid with different colors for different tile types

## Colors

- **Dark Gray**: Walls
- **Gray**: Room floors
- **Dark Blue**: Corridors
- **Green**: Starting position
- **Red**: Ending position

## Extending the Project

To reach the "VG" criteria, consider adding:
- Doors between rooms and corridors
- Treasures and monsters
- Different room types
- Keys and locked doors
- Player avatar with movement
- Progressive difficulty

## Troubleshooting

- If you get linker errors, ensure Raylib is properly installed
- For OpenGL related errors, check your graphics drivers
- For any other issues, verify your macOS and compiler versions