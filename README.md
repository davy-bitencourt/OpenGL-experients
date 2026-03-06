# OpenGL-experiments

This repository is a collection of experiments with OpenGL, focused on learning 3D graphics and exploring concepts such as shaders, buffers, the render loop, and C++ project structure using CMake.

# Dependencies

This project uses:

- OpenGL (installed on the system)
- GLEW (installed on the system)
- GLFW (included locally via `add_subdirectory`)
- CMake ≥ 3.16
- A compiler with C++17 support

Dependency installation (Linux):

```bash
sudo apt install libglew-dev libgl1-mesa-dev
````

(GLFW does not need to be installed because it is included in the `lib/` directory.)

# Compilation

Simply run the `build.sh` script, which is responsible for executing the required terminal commands.

