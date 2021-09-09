#include "Mesh.hpp"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include <iostream>

namespace Loader
{
    std::shared_ptr<Component::Mesh> Mesh::load(const char* path) const
    {
        std::cout << "Loading scene: " << path << std::endl;
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, 0);

        //recycle this as needed to read out filepaths and names
        aiString property;

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

        std::cout << "Number of meshes in scene: " << scene->mNumMeshes << std::endl;

        if (scene->HasTextures())
        {
            std::cout << "Scene has " << scene->mNumTextures << " textures" << std::endl;
        }


        if (scene->HasMaterials())
        {
            std::cout << "Scene has " << scene->mNumMaterials << " materials" << std::endl;
            for (unsigned int i = 0; i < scene->mNumMaterials; i++)
            {
                auto& material = scene->mMaterials[i];
                
                material->Get(AI_MATKEY_NAME, property);
                std::cout << "Processing material " << i << ": " << property.C_Str() << std::endl;

                if (unsigned int textureCount = material->GetTextureCount(aiTextureType_DIFFUSE); textureCount > 0)
                {
                    std::cout << "Number of diffuse textures: " << textureCount << std::endl;
                    for (unsigned int i = 0; i < textureCount; i++)
                    {
                        material->GetTexture(aiTextureType_DIFFUSE, i, &property);
                        const char* path = property.C_Str();

                        if (path[0] == '*')
                        {   //embedded texture
                            //skip the '*' which signifies that the texture is embedded
                            unsigned int index = atoi(++path);
                            std::cout << "embedded texture index: " << index << std::endl;

                            //TODO: Load embedded texture
                        }
                        else
                        {   //external texture
                            std::cout << "external texture path: " << path << std::endl;
                            
                            //TODO: Load external texture file
                        }
                    }
                }
            }
        }

        if (scene->HasCameras())
        {
            std::cout << "Scene has " << scene->mNumCameras << " cameras" << std::endl;
        }

        if (scene->HasAnimations())
        {
            std::cout << "Scene has " << scene->mNumAnimations << " animations" << std::endl;
        }

        std::vector<glm::vec3> meshData(scene->mMeshes[0]->mNumVertices);
        for (unsigned int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
        {
            auto vertex = scene->mMeshes[0]->mVertices[i];
            meshData[i] = {vertex[0], vertex[1], vertex[2]};
        }
        
        importer.FreeScene();
        return std::shared_ptr<Component::Mesh>(new Component::Mesh(meshData));
    }
} // namespace Loader