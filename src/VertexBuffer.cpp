#include "VertexBuffer.hpp"
#include "Renderer.hpp"


/* VBO (Vertex Buffer Object): O VBO é memória alocada na GPU. 
* Aqui é onde os dados realmente ficam. */
VertexBuffer::VertexBuffer(const void* data, unsigned int size){
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW ); // GL_STATIC_DRAW indica que os dados não vão mudar frequentemente.

};

VertexBuffer::~VertexBuffer(){
    glDeleteBuffers(1, &m_RendererID);
};

void VertexBuffer::Bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
};

void VertexBuffer::UnBind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
};