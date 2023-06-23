#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include"Animation.h"

class Animator
{
public:
	Animator(std::shared_ptr<Animation> animation);
	~Animator();

	void UpdateAnimation(float dt);
	void PlayAnimation(std::shared_ptr<Animation> pAnimation);
	void CalculateBoneTransform(const std::shared_ptr<AssimpNodeData> node, glm::mat4 parentTransform);
	std::vector<glm::mat4> GetFinalBoneMatrices();
private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	std::shared_ptr<Animation> m_CurrentAnimation;
	float m_CurrentTime;
	float m_DeltaTime;
};