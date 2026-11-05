#version 330 core
#define MAX_POINT_LIGHT 10

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
    vec3 u_color;
};


struct DirLight{
    vec3 direction;

    vec3 color;
    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
};

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 color;
    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
};
//Function
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);


uniform DirLight dir_light;
uniform PointLight points[MAX_POINT_LIGHT];
uniform int n_point;
uniform Material mat;


//Output of the shader 
out vec4 FragColor;

//Input of the shader
in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

uniform float time;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{   
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(view_pos - FragPos);
    
    vec3 result = CalcDirLight(dir_light, norm, viewDir);

    for(int i = 0; i < n_point; i++){
        result += CalcPointLight(points[i], norm, viewDir);
    }

    FragColor = vec4(result,1.0);
}



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    //Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    //Specular shading
    vec3 reflecDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflecDir), 0.0), mat.shininess);
    //Combine
    vec3 ambient = (light.color * light.ambient_strength) * vec3(texture(mat.diffuse, texCoord));
    vec3 diffuse = (light.color * light.diffuse_strength) * diff * vec3(texture(mat.diffuse, texCoord));
    vec3 specular = (light.color * light.ambient_strength) * spec * vec3(texture(mat.specular, texCoord));
    return (ambient + diffuse + specular) * mat.u_color;
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);
    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    //Specular
    vec3 reflecDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflecDir), 0.0), mat.shininess);
    //attenuation
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 ambient = (light.color * light.ambient_strength) * vec3(texture(mat.diffuse, texCoord));
    vec3 diffuse = (light.color * light.diffuse_strength) * diff * vec3(texture(mat.diffuse, texCoord));
    vec3 specular = (light.color * light.specular_strength) * spec * vec3(texture(mat.specular, texCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * mat.u_color;

}