#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <alloca.h>
#include <iostream>
#include <string>
#include <system_error>

static unsigned int CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); /* "c_str()" ou "null-terminated string" retorna o endereço do primeiro caractere string */
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result==GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*) alloca(length * sizeof(char)); // Q: que porra é essa?
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "A compilação do " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment" ) << " shader falhou" << '\n';
        std::cout << "ERRO: " << message << '\n';

        glDeleteShader(id);
        return 0;
    }

    return id;
};

static unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader){
    unsigned int programa = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(programa, vs);
    glAttachShader(programa, fs);
    glLinkProgram(programa);
    glValidateProgram(programa);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return programa;
};

int main() {
    
    /* Inicializa a biblioteca GLFW.
     * GLFW cuida de: Criar janelas, Criar contexto OpenGL, Gerenciar input e eventos.
     * Sem isso, não existe ambiente gráfico. */
    if (!glfwInit()) {
        std::cerr << "ERRO: Falha ao inicializar GLFW\n";
        return -1;
    }
    
    int definition = 40;
    int height = 9 * definition;
    int width  = 16 * definition;

    // Cria a janela e o contexto OpenGL (O contexto é onde todo o estado do OpenGL vive)
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Experiments", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar janela\n";
        glfwTerminate();
        return -1;
    }
    
    // Torna esse contexto o atual na thread (Todas as chamadas OpenGL abaixo afetam esse contexto)
    glfwMakeContextCurrent(window);

    /* Inicializa GLEW: carrega os ponteiros das funções modernas do OpenGL.
     * Sem isso, só se teria acesso a versões antigas da API. */
    if (glewInit() != GLEW_OK) {
        std::cerr << "ERRO: Falha ao inicializar GLEW\n";
        return -1;
    }

    /* VAO (Vertex Array Object): O VAO guarda a configuração dos atributos de vértice. 
     * Fornecendo informações para a GPU de como interpretar os dados. */
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); 
    // A partir daqui, qualquer configuração de atributo pertence a esse VAO.

    // Dados
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    /* VBO (Vertex Buffer Object): O VBO é memória alocada na GPU. 
     * Aqui é onde os dados realmente ficam. */
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Enviamos os dados da CPU para a GPU.
    glBufferData(
        GL_ARRAY_BUFFER, 
        6 * sizeof(float), 
        positions, 
        GL_STATIC_DRAW // GL_STATIC_DRAW indica que os dados não vão mudar frequentemente.
    );

    /* Atributo de vértice, Dizemos ao OpenGL:
     * "O atributo de índice 0 deve ler 2 floats por vértice"
     * Esse índice 0 precisa bater com: layout(location = 0) no vertex shader. */
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,                    // índice do atributo no shader
        2,                     // quantidade de componentes (x, y)
        GL_FLOAT,              // tipo de dado
        GL_FALSE,        // não normalizar
        sizeof(float) * 2,   // stride (tamanho de 1 vértice)
        0                   // offset inicial
    );

    // Podemos desbindar o VBO, o VAO já salvou a configuração do atributo.
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    std::string vertex_shader = 
    "#version 330 core\n"
    "layout(location = 0) in vec4 position;\n"
    "void main(){"
    "   gl_Position = position;"
    "}";

    std::string fragment_shader = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){"
    "   FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
    "}";

    unsigned int shader = CreateShader(vertex_shader, fragment_shader);
    glUseProgram(shader);

    /* Loop de renderização, o programa roda até a janela ser fechada */
    while (!glfwWindowShouldClose(window)) {

        // Limpa o frame buffer atual. Se não limpar, pixels antigos permanecem.
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenha 3 vértices como um triângulo.
        // O shader ativo (não mostrado aqui) define como isso será processado.
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Troca o buffer invisível pelo visível.
        // OpenGL usa double buffering.
        glfwSwapBuffers(window);

        // Processa eventos (teclado, mouse, fechar janela).
        glfwPollEvents();
    }

    // Libera recursos.
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}