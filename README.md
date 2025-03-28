# Non-Euclidean Engine
Main repo for our non-Euclidean rendering engine

## Organization Details
`tests/` contains a bunch of individual main programs that we can use for testing our engine functionality and creating cool demos

`lib/` contains external code libraries

`ne_engine/` contains all of the engine code

`shaders/` contains a bunch of useful shaders used by the engine

`res/` contains textures for use by the tests

`.vscode/` contains some of our preferred vscode configuration details

## Windows Setup Instructions
0. **Be better and use Linux**
1. Clone the repo: `git clone https://github.com/sdmay25-37/NonEuclideanEngine.git --recursive`
2. Install a compiler for Windows. I suggest [Mingw](https://sourceforge.net/projects/mingw/)
3. Install [CMake](https://cmake.org/download/)
4. Get a build system on Windows like [Ninja](https://ninja-build.org/)
5. Make sure Mingw/bin, CMake/bin, and Ninja.exe are on your path
6. Open the Non_Euclidean_Playground directory and run `cmake -S src -B build -G Ninja`
7. To build: `cmake --build build`

## Linux Installation Instructions
1. Clone the repo: `git clone https://github.com/sdmay25-37/NonEuclideanEngine.git --recursive`
2. Change directories: `cd Non_Euclidean_Playground`
3. Install the necessary packages: `sudo ./linux_setup.sh`
4. If this does not work install the necessary packages(see [Windows Setup Instructions](#windows-setup-instructions) for a list of necessary things)
5. To build: `cmake --build build`

## Catch2 (Testing) Installation

Basically, follow the instructions [here](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#installing-catch2-from-git-repository).

### Windows

For Windows, clone the Git repo, cd into it, build and compile:

```
$ git clone https://github.com/catchorg/Catch2.git
$ cd Catch2
$ cmake -B build -S . -DBUILD_TESTING=OFF -G Ninja 
$ sudo cmake --build build/ --target install
```

If you're having trouble with line 3 regarding error finding compiler, include the option:\
`-DCMAKE_CXX_COMPILER="g++"`

Replace g++ with the name of the C++ Compiler you're using (if the binary is on your include path / environment variable).

### Linux

I hate Windows.

`apt install catch2` 