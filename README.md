# Window Resizer

## Overview

Window Resizer is a GUI application designed to manage and manipulate window sizes and positions for Windows 7/8/10/11.

## Features

- Set window positions and sizes.
- Resize any window, even those with fixed sizes!

## Download

Download the latest builds from the releases section

## Usage

1. Unpack the zip file
2. Launch the `WindowResizer.exe` application
3. Select the application you wish to resize, and set the desired width and height
4. Click the "Resize" button

## CLI - PowerShell

A version of this program is also available in CLI form via a PowerShell script.
The script however, requires passing the PID directly. You can find the PID either via the GUI, or through Task Manager -> Details tab.

### Usage

```
.\resize_window.ps1 -ProcessId <PID> -Width <width> -Height <height>
```

## Development

### Requirements

- CMake 3.23
- Ninja
- C++ 17
- Qt5
- Doxygen (optional, for generating documentation)
- graphviz (optional, for generating dependency graph)
- vcpkg (optional, will be installed within the project directory if not found on the system)

### Building

To build the project, follow these steps:

1. Clone the repository:

```
git clone <repository-url>
cd windowresizer
```

2. Build

```
cmake --preset x64-release
cd out/build/x64-release
ninja
ninja install
```

CMake options available for build:
- `WindowResizer_Generate_Docs`. Default: enabled
- `WindowResizer_Generate_Graph`. Default: enabled
- `WindowResizer_Build_Unit_Tests`. Default: enabled

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
