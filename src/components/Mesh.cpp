#include "Mesh.hpp"

#include <GL/glew.h>

//todo: implement abstract rendering primitives to allow usage of apis other than opengl

namespace Component {

    Mesh::Mesh(std::vector<glm::vec3> vertices) : m_vertices(vertices), m_numVertices((unsigned int)vertices.size())
    {
        const size_t vertexSize = 3 * sizeof(float);

        //generate buffers to hold vertex data
        //vertex array object
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        //vertex buffer object
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        //upload vertex data to GPU
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * vertexSize, vertices.data(), GL_STATIC_DRAW);

        //assign vertex data to first vertex attribute (position)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void *)0);
        glEnableVertexAttribArray(0);

        //unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Mesh::draw()
    {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    }
}