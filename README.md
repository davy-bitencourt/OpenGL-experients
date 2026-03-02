# OpenGL-experients
Esse repositório é um conjunto de experimentos com OpenGL, focado em aprendizado de gráficos 3D e exploração de conceitos como shaders, buffers, render loop e estrutura de projetos C++ com CMake.

# Dependências

Este projeto utiliza:
- OpenGL (instalado no sistema)
- GLEW (instalado no sistema)
- GLFW (incluído localmente via add_subdirectory)
- CMake ≥ 3.16
- Compilador com suporte a C++17

Instalação das dependências (Linux):
``` Bash
    sudo apt install libglew-dev libgl1-mesa-dev
```
(O GLFW não precisa instalar porque está incluído no diretório lib/.)

# Compilação

Basta executar o script `build.sh` ele que é responsável pelos comandos do terminal.