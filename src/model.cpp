#include "PMGL/model.hpp"
#include "PMGL/mesh.hpp"
#include "PMGL/base.hpp"

void PMGL::model::loadModel(std::string path){
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of("/\\"));

    processNode(scene->mRootNode, scene);
}

void PMGL::model::processVertex(const aiMesh *meshe, std::vector<vertex> &vertices){
    for(unsigned int i = 0; i < meshe->mNumVertices; i++){
        vertex ver;
        glm::vec3 temp_vec;

        //Vertices
        temp_vec.x = meshe->mVertices[i].x;
        temp_vec.y = meshe->mVertices[i].y;
        temp_vec.z = meshe->mVertices[i].z;
        ver.position = temp_vec;

        //Normals
        temp_vec.x = meshe->mNormals[i].x;
        temp_vec.y = meshe->mNormals[i].y;
        temp_vec.z = meshe->mNormals[i].z;
        ver.normal = temp_vec;

        //Texture Coordinates
        if(meshe->mTextureCoords[0]){
            glm::vec2 temp_vec;
            temp_vec.x = meshe->mTextureCoords[0][i].x;
            temp_vec.y = meshe->mTextureCoords[0][i].y;
            ver.texCoord = temp_vec;
        }else{
            ver.texCoord = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(ver);
    }
}

void PMGL::model::processIndices(const aiMesh *meshe, std::vector<unsigned int> &indices){
    for(size_t i = 0; i < meshe->mNumFaces; i++){
        aiFace face = meshe->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }
}

void PMGL::model::processTextures(const aiMesh *meshe, const aiScene *scene, std::vector<texture> &textures){
    if(meshe->mMaterialIndex >= 0){
        aiMaterial *material = scene->mMaterials[meshe->mMaterialIndex];
        std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
}

void PMGL::model::processNode(aiNode *node, const aiScene *scene){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
}

PMGL::mesh PMGL::model::processMesh(aiMesh *meshe, const aiScene *scene){
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<texture> textures;

    processVertex(meshe, vertices);
    processIndices(meshe, indices);
    processTextures(meshe, scene, textures);
    return mesh(vertices, indices, textures);
}

std::vector<PMGL::texture> PMGL::model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string type_name){
    std::vector<texture> texs;
    for(size_t i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(size_t j = 0; j < loaded_textures.size(); j++){
            if(std::strcmp(loaded_textures[j].path.data(), str.C_Str()) == 0){
                texs.push_back(loaded_textures[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            texture tex;
            tex.id = load_texture(std::string(str.C_Str()), directory);
            tex.type = type_name;
            tex.path = str.C_Str();
            texs.push_back(tex);
            loaded_textures.push_back(tex);
        }
    }
    return texs;
}

PMGL::model::model(const std::string &path){
    loadModel(path);
}

void PMGL::model::draw(const shader &shad){
    for(auto s_mesh : meshes){
        s_mesh.draw(shad);
    }
}
