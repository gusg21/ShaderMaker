//
// Created by angus.goucher on 4/5/2024.
//

#ifndef SHADERMAKER_MESH_H
#define SHADERMAKER_MESH_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace sm {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    struct MeshData {
        std::vector <Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    enum class DrawMode {
        TRIANGLES = 0,
        POINTS = 1
    };

    class Mesh {
    public:
        Mesh() {};

        Mesh(const MeshData &meshData);

        void load(const MeshData &meshData);

        void draw(DrawMode drawMode = DrawMode::TRIANGLES) const;

        inline int getNumVertices() const { return m_numVertices; }

        inline int getNumIndices() const { return m_numIndices; }

    private:
        bool m_initialized = false;
        unsigned int m_vao = 0;
        unsigned int m_vbo = 0;
        unsigned int m_ebo = 0;
        unsigned int m_numVertices = 0;
        unsigned int m_numIndices = 0;
    };
}


#endif //SHADERMAKER_MESH_H
