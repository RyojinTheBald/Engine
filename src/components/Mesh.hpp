#pragma once

#include <vector>
#include <glm/vec3.hpp>

namespace Component
{
    class Mesh
    {
    public:
        Mesh() = default;
        Mesh(std::vector<glm::vec3> vertices);

        void draw();

        std::vector<glm::vec3> m_vertices;

        //Vertex Array Object
        unsigned int m_VAO;

        //Vertex Buffer Object
        unsigned int m_VBO;

        unsigned int m_numVertices;
    };
}