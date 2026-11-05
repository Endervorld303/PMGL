#ifndef BASE_HEAD
#define BASE_HEAD
#include <glad/glad.h>
#include <glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>

#include <string>

const glm::vec3 WORLD_FRONT = glm::vec3(0.0f, 0.0, -1.0f);
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

#define LOG_DIM 512

namespace PMGL{
   //Structures and classes
    struct vector2d{
        float x;
        float y;

        vector2d(float _x = 0, float _y = 0) : x(_x), y(_y){};
        friend vector2d operator+(vector2d, vector2d);
        friend vector2d operator-(vector2d, vector2d);
    };


    std::string read_file(std::string_view file_name);
    void init_PMGL(int minor_version, int major_version, int profile);
    void terminate_PMGL();

    GLenum shader_type(std::string_view file_name);
    unsigned int load_texture(std::string_view path);
    unsigned int load_texture(std::string_view file_reference, std::string_view directory, bool gamma = false);
}


#endif