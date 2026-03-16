#include "IndexBuffer.hpp"
#include "Renderer.hpp"

/* VBO (Index Buffer Object): O VBO é memória alocada na GPU. 
* Aqui é onde os dados realmente ficam. */
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count){
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW ); // GL_STATIC_DRAW indica que os dados não vão mudar frequentemente.

};

IndexBuffer::~IndexBuffer(){
    glDeleteBuffers(1, &m_RendererID);
};

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
};

void IndexBuffer::UnBind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};