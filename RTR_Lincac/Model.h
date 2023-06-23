#pragma once

#ifndef _MODEL_H
#define _MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<map>

#include "Mesh.h"

using namespace std;

class Model : public Object
{
public:
    Model(const char* path);
    ~Model();

    virtual void render() override;

    virtual std::map<string, BoneInfo> GetBoneInfoMap() override { return m_BoneInfoMap; }
    virtual int GetBoneCount() override { return m_BoneCounter; }
private:
    glm::vec3 maxVec;
    glm::vec3 minVec;

    vector<Mesh>    meshes;
    vector<Mesh>    animation_meshes;
    vector<Texture> textures_loaded;
    string directory;

    std::map<string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;

    void loadModel(const char* path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif
