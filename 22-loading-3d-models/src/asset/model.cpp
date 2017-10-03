#include <asset/model.h>
#include <iostream>

Model::Model(const std::string& filepath)
{
    loadModel(filepath);
}

void Model::draw(Material &material)
{
    for (auto& mesh : meshes) {
        mesh.draw(material);
    }
}

void Model::loadModel(const std::string& filepath)
{
    Assimp::Importer importer;
    auto scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
        std::cout << "error: " << importer.GetErrorString() << std::endl;
        return;
    }

    meshDirectory = filepath.substr(0, filepath.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // process the top-level meshes
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // process each child node recursively
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
}

std::vector<Texture> Model::loadTextures(aiMaterial *material, aiTextureType type)
{
}
