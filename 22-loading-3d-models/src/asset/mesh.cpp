#include <asset/mesh.h>
#include <iostream>

Mesh::Mesh(
    std::vector<Vertex>&& vertices,
    std::vector<unsigned int>&& indices,
    std::vector<Texture>&& textures
) : vertices(vertices),
    indices(indices),
    textures(textures)
{
    if (vertices.size() == 0) {
        std::cout << "error: mesh created with 0 vertices" << std::endl;
    }

    if (vertices.size() == 0) {
        std::cout << "error: mesh created with 0 vertex indices" << std::endl;
    }
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::bufferMeshData(Material& material)
{
    glGenBuffers(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        &vertices[0],
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        &indices[0],
        GL_STATIC_DRAW
    );

    glBindVertexArray(0);

    //Link the vertex position attribute in the vertex shader to the glm::vec3 position
    //in each Vertex object
    material.setGLVertexAttribPointer(VAO, "position", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

    //Link the normals in the vertex shader to the glm::vec3 normal in each Vertex object
    material.setGLVertexAttribPointer(VAO, "normal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

    //Link the uv coord attribute in the vertex shader to the glm::vec2 uvCoords in each Vertex object
    material.setGLVertexAttribPointer(VAO, "texCoord", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(glm::vec3) * 2));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(Material& material)
{
    const auto& diffuseTexUnits = material.getDiffuseTextureUnits();
    const auto& specularTexUnits = material.getSpecularTextureUnits();
    const auto& normalTexUnits = material.getNormalTextureUnits();

    unsigned int numBoundDiffuseTextures = 0;
    unsigned int numBoundSpecularTextures = 0;
    unsigned int numBoundNormalTextures = 0;

    for (const auto& texture : textures) {
        if (texture.getType() == TextureType::Diffuse) {
            if (numBoundDiffuseTextures >= diffuseTexUnits.size()) {
                std::cout << "warning: material does not contain enough diffuse texture units for mesh" << std::endl;
                continue;
            }
            glActiveTexture(GL_TEXTURE0 + diffuseTexUnits[numBoundDiffuseTextures]);
            glBindTexture(GL_TEXTURE_2D, texture.getID());
            ++numBoundDiffuseTextures;

        } else if (texture.getType() == TextureType::Specular) {
            if (numBoundSpecularTextures >= specularTexUnits.size()) {
                std::cout << "warning: material does not contain enough specular texture units for mesh" << std::endl;
                continue;
            }
            glActiveTexture(GL_TEXTURE0 + specularTexUnits[numBoundSpecularTextures]);
            glBindTexture(GL_TEXTURE_2D, texture.getID());
            ++numBoundSpecularTextures;
        } else if (texture.getType() == TextureType::Normal) {
            if (numBoundNormalTextures >= normalTexUnits.size()) {
                std::cout << "warning: material does not contain enough normal texture units for mesh" << std::endl;
                continue;
            }
            glActiveTexture(GL_TEXTURE0 + normalTexUnits[numBoundNormalTextures]);
            glBindTexture(GL_TEXTURE_2D, texture.getID());
            ++numBoundNormalTextures;
        }
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
