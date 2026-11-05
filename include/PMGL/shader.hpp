#ifndef SHADER_HEAD
#define SHADER_HEAD

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <span>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PMGL{
    class shader{
    private:
        /* data */
        std::vector<unsigned int> compiled_shader;

        void clear_shader();
        int check_compile_error(unsigned int shader);
        int check_link_error();
    public:
        unsigned int ID;
        shader(std::vector<std::string> shader_list);
        shader(std::string vert, std::string frag);
        ~shader();
        void use();
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3f(const std::string &name, std::span<float> data) const;
        void setMat4fv(const std::string &name, const glm::mat4 &trasformation) const;
        void setMat3f(const std::string &name, const glm::mat3 &trasformation) const;
    };
}


#endif