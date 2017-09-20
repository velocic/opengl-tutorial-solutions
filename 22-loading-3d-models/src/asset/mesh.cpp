#include <mesh.h>
#include <iostream>

Mesh::Mesh(
    std::vector<Vertex>&& vertices,
    std::vector<unsigned int>&& indices
    std::vector<Texture>&& textures
)
    vertices(vertices),
    indices(indices),
    textures(textures)
{
    if (vertices.size() == 0) {
        std::cout << "error: mesh created with 0 vertices" << std::endl;
    }

    if (vertices.size() == 0) {
        std::cout << "error: mesh created with 0 vertex indices" << std::endl;
    }

    bufferMeshData();
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

void Mesh::bufferMeshData(
    GLint vertexPositionAttributeLocation,
    GLint uvCoordinateAttributeLocation,
    GLint normalAttributeLocation
)
{
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

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

    //Link the vertex position attribute in the vertex shader to the glm::vec3 position
    //in each Vertex object
    glVertexAttribPointer(vertexPositionAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    //Link the normals in the vertex shader to the glm::vec3 normal in each Vertex object
    glVertexAttribPointer(normalAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

    //Link the uv coord attribute in the vertex shader to the glm::vec2 uvCoords in each Vertex object
    glVertexAttribPointer(uvCoordinateAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3 * 2));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(Material& material)
{
    //TODO: figure this all out. Unfinished/not correct
    //diffuse, specular, normal, etc textures will be named ex: diffuse0 - diffuseN
    //in the shader to allow easily setting them here
    unsigned int diffuseTextureIndex = 0;
    unsigned int specularTextureIndex = 0;
    for (unsigned int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);

        //TODO: set texture unit values here
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
