#include <GLFW/glfw3.h>

// q porra é essa de macros???
#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError();

static bool GLLogCall(const char* function, const char* file, int line);