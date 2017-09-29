#ifndef MODEL_H
#define MODEL_H

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <texture.h>


class Model
{
    private:
        void loadModel(const std::string& filepath);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        void processNode(aiNode *node, const aiScene *scene);
        std::vector<Texture> loadTextures(aiMaterial *material, aiTextureType type);

        std::vector<Mesh> meshes;
        std::string meshDirectory;
    public:
        Model(const std::string &filepath);
        void draw(Material &material);
};

#endif
