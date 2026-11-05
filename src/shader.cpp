#include "PMGL/shader.hpp"
#include "PMGL/base.hpp"
#include <span>

void PMGL::shader::clear_shader(){
    for(unsigned int shader_id : compiled_shader){
        glDetachShader(ID, shader_id);
        glDeleteShader(shader_id);
    }
}

int PMGL::shader::check_compile_error(unsigned int shader){
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        char infolog[LOG_DIM];
        glGetShaderInfoLog(shader, LOG_DIM, NULL, infolog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infolog << std::endl;
    }

    return success;
}

int PMGL::shader::check_link_error(){
    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        char infolog[LOG_DIM];
        glGetProgramInfoLog(ID, LOG_DIM, NULL, infolog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
    }
    return success;
}

//Questo costruttore usa la traduzione automatica della shader in caso contrario usare un altro costuttore in arrivo
PMGL::shader::shader(std::vector<std::string> shader_list){
    ID = glCreateProgram();
    for(std::string file_name : shader_list){
        std::string text = read_file(file_name);
        unsigned int shader = glCreateShader(shader_type(file_name));
        const char* shader_code = text.c_str();
        glShaderSource(shader, 1, &shader_code, NULL);

        glCompileShader(shader);
        check_compile_error(shader);

        glAttachShader(ID, shader);
        glDeleteShader(shader);
    }
    glLinkProgram(ID);
    check_link_error();
    clear_shader();
}

PMGL::shader::shader(std::string vert, std::string frag) : shader(std::vector<std::string>{vert, frag}){}

PMGL::shader::~shader(){
    if(ID != 0){
        glDeleteProgram(ID);
    }
}

void PMGL::shader::use(){
    glUseProgram(ID);
}

void PMGL::shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void PMGL::shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void PMGL::shader::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void PMGL::shader::setVec3(const std::string &name, const glm::vec3 &value) const{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void PMGL::shader::setVec3f(const std::string &name, std::span<float> data) const{
    glUniform3fv(glGetUniformLocation(ID,name.c_str()), data.size(), data.data());
}


void PMGL::shader::setMat4fv(const std::string &name, const glm::mat4 &trasformation) const{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trasformation));
}

void PMGL::shader::setMat3f(const std::string &name, const glm::mat3 &trasformation) const{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trasformation));
}
