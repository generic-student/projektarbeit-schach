# Projektarbeit - Schwachmatt

Das Thema der Projektarbeit ist die erstellung einer Schach-Engine + Webfrontend

- [Projektarbeit - Schwachmatt](#projektarbeit---schwachmatt)
- [About](#about)
- [Build Status](#build-status)
- [Installation](#installation)
  - [Requirements](#requirements)
  - [Setting up the repository](#setting-up-the-repository)
  - [Generate the makefile or Visual Studio Solution with Premake](#generate-the-makefile-or-visual-studio-solution-with-premake)
  - [Compile the Engine](#compile-the-engine)
    - [Visual Studio](#visual-studio)
    - [Makefile](#makefile)
- [Miscellaneous](#miscellaneous)

# About
> WIP

# Build Status
> WIP

# Installation
## Requirements
1. Git
2. [Premake](https://premake.github.io/)
3. CMAKE, MAKE, Visual Studio or any other platform supported by premake
4. [boost](https://www.boost.org/users/history/version_1_77_0.html) C++ library (version 1.77.0)
5. [spdlog](https://github.com/gabime/spdlog) C++ library

## Setting up the repository
1. clone the repository
2. create a folder "external" in the cloned repo
   - projektarbeit-schach/external 
3. copy the boost library into the external folder
   - projektarbeit-schach/external/boost_1_77_0/boost 
4. copy the spdlog library into the external folder
   - projektarbeit-schach/external/spdlog/spdlog

## Generate the makefile or Visual Studio Solution with Premake
1. Navigate to the repository using any terminal
2. Run Premake
   - `./premake5 vs2022` (Visual Studio 2022)
   - `./premake5 vs2019` (Visual Studio 2019)
   - `./premake5 gmake2` (Makefile)

## Compile the Engine
### Visual Studio
- Open the generated solution file
- Visual Studio will open and display the project
- Either click on the green start-button to compile and start the engine
- Or hit `ctrl-shift-b` to just compile the engine

### Makefile
- Run Make
- `make SchwachMatt` Or
- `make config=release SchwachMatt`
- the executable will be saved under "/build/server/Debug/" Or "/build/server/Release"
  

# Miscellaneous
premake Downloaden

mit dem Befehl in der Console wird das Visual Studio Projekt erstellt

Visual Studio 2022  : "premake5.exe vs2022"
Visual Studio 2019  : "premake5.exe vs2019"

