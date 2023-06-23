#ifndef _HELP_H
#define _HELP_H

#include<iostream>

#include"Camera.h"
#include"Texture.h"
#include"Shader.h"
#include<unordered_map>

#define INF 1212123.0

// ʱ��
extern GLfloat _time;
extern float deltaTime;
extern float lastFrame;

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
extern GLuint Specular;
extern GLuint Roughness;
extern GLuint Metallic;
extern GLuint Ao;

extern glm::vec3	_Color;
extern glm::vec3	_Specular;
extern float _Gloss;
extern float _Roughness;
extern float _Metallic;
extern float _Ao;

extern bool use_albedoMap;
extern bool use_normalMap;
extern	bool use_specularMap;
extern bool use_roughnessMap;
extern bool use_metallicMap;
extern bool use_aoMap;
extern bool use_skybox;
extern bool use_animation;

extern glm::vec3 center;
extern float _PlaneY;

extern bool ChangeObj;
extern std::string dir;
extern std::string animation_file;

extern bool show;
extern GLuint show_texture;

extern GLuint vertexNum;
extern GLuint triangleNum;

// ȫ�ֹ�Դ����
extern glm::vec3 LightPos;
extern glm::vec3	 LightCol;
extern glm::vec3 bgColor;
extern glm::vec3 ambient;
extern float LightVol;
extern float angle;

// ��ǰ��Ⱦģʽ
extern std::string preMode;
extern std::string Mode;
extern GLenum renderMode;

// ��������
extern bool display_animation;
extern std::string animation_mode;

// ��ɫ�������
extern std::unordered_map<std::string, std::shared_ptr<Shader>> hashShader;

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
