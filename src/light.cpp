#include "PMGL/light.hpp"

#include <string>
void PMGL::apply_light_shader(const shader &shad, const directional_light &light){
    shad.setVec3("dir_light.direction", light.direction);
    shad.setVec3("dir_light.color", light.color.color);
    shad.setFloat("dir_light.ambient_strength", light.color.ambient_strength);
    shad.setFloat("dir_light.diffuse_strength", light.color.diffuse_strength);
    shad.setFloat("dir_light.specular_strength", light.color.specular_strength);
}

void PMGL::apply_light_shader(const shader &shad, const point_light &light, int num){
    const std::string name = "points[" + std::to_string(num) + "]";
    shad.setVec3(name + ".position", light.position);
    shad.setVec3(name +".color", light.color.color);
    shad.setFloat(name + ".ambient_strength", light.color.ambient_strength);
    shad.setFloat(name + ".diffuse_strength", light.color.diffuse_strength);
    shad.setFloat(name + ".specular_strength", light.color.specular_strength);
    shad.setFloat(name + ".constant", light.constant);
    shad.setFloat(name + ".linear", light.linear);
    shad.setFloat(name + ".quadratic", light.quadratic);
}

void PMGL::apply_light_vector_shader(const shader &shad, const std::span<point_light> &lights){
    shad.setInt("n_point", lights.size());
    for(int i = 0; i < lights.size(); i++){
        apply_light_shader(shad, lights[i], i);
    }
}

PMGL::light_color::light_color(const glm::vec3 &col, float amb, float diff, float spec) 
                    : color(col), ambient_strength(amb), diffuse_strength(diff), specular_strength(spec){}

PMGL::directional_light::directional_light(const glm::vec3 &dir, const light_color &col)
                    : direction(dir), color(col){}

PMGL::point_light::point_light(const glm::vec3 &pos, const light_color &col, float cons, float line, float quad) 
                    : position(pos), color(col), constant(cons), linear(line), quadratic(quad){}
