#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 projection_view;
uniform mat3 normal_matrix;

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

void main(){
    gl_Position = projection_view * model * vec4(aPos, 1.0);
    texCoord = aTexCoord;
    Normal =  normal_matrix * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
}