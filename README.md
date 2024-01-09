# Minecraft Clone

A Minecraft clone written in C99 and OpenGL

## How to compile

MacOS & Linux

```bash
mkdir build
cd build
cmake ..
make
./minecraft
```

Windows (Have MINGW installed in the default location)

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
./minecraft
```

Run the `install.py` script to move the executable and assets to the directory `minecraft`

All dependencies will be automatically fetched and installed by CMake


