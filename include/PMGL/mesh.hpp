#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "PMGL/shader.hpp"

namespace PMGL{
    struct vertex{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    struct texture{
        unsigned int id;
        std::string type;
        std::string path;
    };

    class mesh{
        private:
            std::vector<vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<texture> textures;
            unsigned int VAO;
            unsigned int VBO;
            unsigned int EBO;
            void setupMesh();

        public:
            mesh(std::vector<vertex> vert, std::vector<unsigned int> ind, std::vector<texture> text);
            void draw(const shader& shad);        
    };

}

#endif