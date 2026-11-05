#ifndef LIGHT_HEADER
#define LIGHT_HEADER
#include "PMGL/base.hpp"
#include "PMGL/shader.hpp"
#include <span>

namespace PMGL{
    struct light_color{ 
        glm::vec3 color;
        float ambient_strength;
        float diffuse_strength;
        float specular_strength;
        light_color(const glm::vec3 &col = glm::vec3(0.0f, 0.0f, 0.0f), float amb = 0, float diff = 0, float spec = 0);
    };


    struct directional_light{
        glm::vec3 direction;
        light_color color;
        directional_light(const glm::vec3 &dir = glm::vec3(0.0f, 0.0f, 0.0f), const light_color &col = light_color());
    };

    struct point_light{
        glm::vec3 position;
        light_color color;
        float constant;
        float linear;
        float quadratic;
        point_light(const glm::vec3 &pos = glm::vec3(0.0f, 0.0f, 0.0f), const light_color &col = light_color(), float cons = 0, float line = 0, float quad = 0);
    };


    void apply_light_shader(const shader &shad, const directional_light &light);
    void apply_light_shader(const shader &shad, const point_light &light, int num);
    void apply_light_vector_shader(const shader &shad, const std::span<point_light> &lights);
};



#endif