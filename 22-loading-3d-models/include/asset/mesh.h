#ifndef MESH_H
#define MESH_H

#include <GL/gl3w.h>
#include <texture.h>

#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uvCoords;
};

class Mesh
{
    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        GLuint VBO = 0;
        GLuint IBO = 0;

        void bufferMeshData();
    public:
        Mesh(
            std::vector<Vertex>&& vertices,
            std::vector<unsigned int>&& indices
            std::vector<Texture>&& textures
        );
        ~Mesh();
        void draw(Material& material);
}

#endif
