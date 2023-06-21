#pragma once

#ifndef MESH_H
#define MESH_H

#include"Object.h"

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh {
public:
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    unsigned int VAO;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
    ~Mesh();
    void Draw();
private:
    unsigned int VBO, EBO;

    void setupMesh();
};
#endif
