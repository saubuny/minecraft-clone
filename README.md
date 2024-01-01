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
cmake --build ..
./minecraft
```

All dependencies (GLFW and Glad) will be automatically fetched and installed by CMake


