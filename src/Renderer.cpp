#include "Renderer.hpp"
#include <iostream>

// q porra é essa de macros???
#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(){
    while (glGetError() != GL_NO_ERROR);
};

bool GLLogCall(const char* function, const char* file, int line){
    if(GLenum error = glGetError()){
        std::cout << "[ ERRO ] " << error 
        << " [ FUNÇÃO ] " << function 
        << "\n[ LINHA ] " << line
        << " [ ARQUIVO ] " << file << '\n'; 
        return false;
    }
    return true;
};