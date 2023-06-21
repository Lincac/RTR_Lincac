#ifndef _HELP_H
#define _HELP_H

#include<iostream>

#include"Camera.h"
#include"Texture.h"
#include"Shader.h"
#include<unordered_map>

#define INF 1212123.0

// 鼠标控制
extern bool mouseClick;
extern float lastx, lasty;
extern float currentx, currenty;

// 全局窗口设置
extern GLuint WIDTH;
extern GLuint HEIGHT;

// 相机设置
extern Camera camera;
extern float _near;
extern float _far; 

// 全局设置
extern glm::mat4 projection;
extern glm::mat4 view;
extern std::shared_ptr<Shader> shader;

// 全局物体设置
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

// 全局光源设置
extern glm::vec3 LightPos;
extern glm::vec3	 LightCol;
extern glm::vec3 bgColor;
extern glm::vec3 ambient;
extern float LightVol;

// 当前渲染模式
extern std::string Mode;

// 参数
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

// 纹理池
extern std::unordered_map<std::string, GLuint> textures;

// 捕捉
extern glm::mat4 capturePro;
extern glm::mat4 captureViews[];

// 帮助函数
extern GLuint quadVAO;
extern void RenderQuad();

extern GLuint cubeVAO;
extern void renderCube();

#endif // !_HELP_H
