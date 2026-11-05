#include "PMGL/base.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>

namespace PMGL{
    const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    void init_PMGL(int minor_version, int major_version, int profile){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

    }
    
    void terminate_PMGL(){
        glfwTerminate();
    }

    vector2d operator+(vector2d v1, vector2d v2){
        return vector2d((v1.x + v2.x), (v1.y + v2.y));
    }

    vector2d operator-(vector2d v1, vector2d v2){
        return vector2d((v1.x - v2.x), (v1.y - v2.y));
    }

    std::string read_file(std::string_view file_name)
    {
        std::ifstream file_s{std::string(file_name)};
        std::stringstream buffer;
        buffer << file_s.rdbuf();
        return buffer.str();
    }

    GLenum shader_type(std::string_view file_name){
        size_t cut = file_name.rfind(".");
        
        if(cut == std::string::npos){
            return GL_FALSE;
        }

        std::string_view pref = file_name.substr(cut);

        if(pref == ".vert" || pref == ".vs"){
            return GL_VERTEX_SHADER;
        }else if(pref == ".frag" || pref == ".fs"){
            return GL_FRAGMENT_SHADER;
        }else if(pref == ".geom"){
            return GL_GEOMETRY_SHADER;
        }

        return GL_FALSE;
    }

    unsigned int load_texture(std::string_view _path){
        unsigned int textureID; 
        glGenTextures(1, &textureID);
        
        int width, height, nrChannels;
        std::string path = std::string(_path);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if(data){
            GLenum format;
            switch(nrChannels){
                case 1:
                    format = GL_RED;
                    break;
                case 3:
                    format = GL_RGB;
                    break;
                case 4:
                    format = GL_RGBA;
                    break;
            }
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }else{
            std::cerr << "Failed to load texture" << path <<std::endl;
        }
        stbi_image_free(data);

        return textureID;
    }

    unsigned int load_texture(std::string_view file_reference, std::string_view directory, bool gamma){
        std::string file = (std::filesystem::path(directory) / file_reference).string();
        
        unsigned int textureID; 
        glGenTextures(1, &textureID);
        
        int width, height, nrChannels;
        unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
        if(data){
            GLenum format;
            switch(nrChannels){
                case 1:
                    format = GL_RED;
                    break;
                case 3:
                    format = GL_RGB;
                    break;
                case 4:
                    format = GL_RGBA;
                    break;
            }
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }else{
            std::cerr << "Failed to load texture: " << file << std::endl;
        }
        stbi_image_free(data);

        return textureID;
    }
}