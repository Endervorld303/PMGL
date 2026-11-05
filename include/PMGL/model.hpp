#ifndef MODEL_H
#define MODEL_H
#include "mesh.hpp"
#include "shader.hpp"
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace PMGL{
    class model{
        private:
            std::vector<mesh> meshes;
            std::string directory;
            std::vector<texture> loaded_textures;

            void loadModel(std::string path);
            void processVertex(const aiMesh *meshe, std::vector<vertex> &vertices);
            void processIndices(const aiMesh *meshe, std::vector<unsigned int> &indices);
            void processTextures(const aiMesh *meshe, const aiScene *scene, std::vector<texture> &textures);
            void processNode(aiNode *node, const aiScene *scene);
            mesh processMesh(aiMesh *meshe, const aiScene *scene);
            std::vector<texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string type_name); 
        
        public:
            model(const std::string& path);
            void draw(const shader& shad);
    };
}





#endif