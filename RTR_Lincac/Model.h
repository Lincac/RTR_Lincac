#pragma once

#ifndef _MODEL_H
#define _MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

using namespace std;

class Model : public Object
{
public:
    Model(const char* path);
    ~Model();

    virtual void render() override;
private:
    glm::vec3 maxVec;
    glm::vec3 minVec;
    vector<Mesh>    meshes;

    void loadModel(const char* path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

#endif
