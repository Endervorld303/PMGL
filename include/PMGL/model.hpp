#ifndef MODEL_H
#define MODEL_H
#include "mesh.hpp"
#include "shader.hpp"
#include <vector>
#include <string>


namespace PMGL{
    class model{
        private:
            std::vector<mesh> meshes;
            std::string directory;
            std::vector<texture> loaded_textures;

            void loadModel(std::string path);
        public:
            model(const std::string& path);
            void draw(const shader& shad) const;
    };
}





#endif