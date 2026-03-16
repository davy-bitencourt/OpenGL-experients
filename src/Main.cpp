#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <alloca.h>
#include <iostream>
#include <string>
#include <fstream>

#include "Renderer.hpp"

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"



static std::string ShaderStringSource(const std::string& file_path){
    std::ifstream arquivo(file_path);

    if (!arquivo.is_open()){
        throw std::runtime_error("Falha ao abrir arquivo");
    }

    std::string line;
    std::string conteudo;
    while (std::getline(arquivo, line)){
        conteudo += line + "\n";
    };
    arquivo.close();

    return  conteudo;
};

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
    
    glfwMakeContextCurrent(window); // Torna esse contexto o atual na thread (Todas as chamadas OpenGL abaixo afetam esse contexto)

    glfwSwapInterval(1); // não sei oq faz

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
    float positions[] = {
        -0.5f, -0.5f,
         0.5f,  -0.5f,
         0.5f, 0.5f,
        -0.5f,  0.5f, 
    };
    
    unsigned int indices[]{
        0, 1, 2,
        2, 3, 0
    };


    VertexBuffer vb(positions,  4 * 2 * sizeof(float));

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

    IndexBuffer ib(indices, 6);

    // Podemos desbindar o VBO, o VAO já salvou a configuração do atributo.
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // TODO: Parsear do shader
    std::string vertex_shader = ShaderStringSource("./res/shaders/vertex.glsl");
    std::string fragment_shader = ShaderStringSource("./res/shaders/fragment.glsl");
    unsigned int shader = CreateShader(vertex_shader, fragment_shader);
    glUseProgram(shader);

    GLCall(int uniform_location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(uniform_location != -1); // se location for == -1, significa que não foi possível encontrá-la
    GLCall(glUniform4f(uniform_location, 0.2f, 0.3f,0.8f,1.0f));

    float red_value = 0.0f;
    float increment;
    /* Loop de renderização, o programa roda até a janela ser fechada */
    while (!glfwWindowShouldClose(window)) {

        // Limpa o frame buffer atual. Se não limpar, pixels antigos permanecem.
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glUniform4f(uniform_location, red_value, 0.3f,0.8f,1.0f));
        /* GLCall é uma ferramenta de debug */
        ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if(red_value >= 1.0f)
            increment = -0.5f;
        else if(red_value <= 0.0f)
            increment = 0.15f;
        red_value += increment;

        // Troca o buffer invisível pelo visível. OpenGL usa double buffering.
        glfwSwapBuffers(window);

        // Processa eventos (teclado, mouse, fechar janela).
        glfwPollEvents();
    }

    // Libera recursos.
    glDeleteProgram(shader); // deleta o shader da memória
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}