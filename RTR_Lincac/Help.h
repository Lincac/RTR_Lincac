#ifndef _HELP_H
#define _HELP_H

#include<iostream>

#include"Camera.h"
#include"Texture.h"
#include"Shader.h"
#include<unordered_map>

#define INF 1212123.0

// ������
extern bool mouseClick;
extern float lastx, lasty;
extern float currentx, currenty;

// ȫ�ִ�������
extern GLuint WIDTH;
extern GLuint HEIGHT;

// �������
extern Camera camera;
extern float _near;
extern float _far; 

// ȫ������
extern glm::mat4 projection;
extern glm::mat4 view;
extern std::shared_ptr<Shader> shader;

// ȫ����������
extern GLuint Albedo;
extern GLuint Normal;
extern GLuint Roughness;
extern GLuint Metallic;
extern GLuint Ao;

extern glm::vec3	_Color;
extern glm::vec3	_Specular;
extern glm::vec3 center;
extern float _PlaneY;
extern bool ChangeObj;
extern std::string dir;

// ȫ�ֹ�Դ����
extern glm::vec3 LightPos;
extern glm::vec3	 LightCol;
extern glm::vec3 bgColor;
extern glm::vec3 ambient;
extern float LightVol;

// ��ǰ��Ⱦģʽ
extern std::string Mode;

// ����
extern float _Gloss;
extern bool use_albedoMap;
extern bool use_normalMap;
extern bool use_roughnessMap;
extern bool use_metallicMap;
extern bool use_aoMap;
extern bool use_skybox;

extern float _Roughness;
extern float _Metallic;
extern float _Ao;

// �����
extern std::unordered_map<std::string, GLuint> textures;

// ��׽
extern glm::mat4 capturePro;
extern glm::mat4 captureViews[];

// ��������
extern GLuint quadVAO;
extern void RenderQuad();

extern GLuint cubeVAO;
extern void renderCube();

#endif // !_HELP_H
