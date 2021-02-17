#include "Mesh.hpp"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include <iostream>

namespace Loader
{
    std::shared_ptr<Component::Mesh> Mesh::load(const char* path) const
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, 0);

        if (!scene)
        {
            std::cerr << "scene import failed for " << path << ": " << importer.GetErrorString() << std::endl;
            return nullptr;
        }

        if (!scene->HasMeshes())
        {
            std::cerr << "Error: no meshes in scene" << path << std::endl;
            return nullptr;
        }

        std::vector<glm::vec3> meshData(scene->mMeshes[0]->mNumVertices);
        for (unsigned int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
        {
            auto vertex = scene->mMeshes[0]->mVertices[i];
            meshData[i] = {vertex[0], vertex[1], vertex[2]};
        }

        return std::shared_ptr<Component::Mesh>(new Component::Mesh(meshData));
    }
} // namespace Loader