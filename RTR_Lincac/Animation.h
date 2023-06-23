#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include"Bone.h"
#include <functional>
#include"AnimationData.h"
#include"Model.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<std::shared_ptr<AssimpNodeData>> children;
};

class Animation
{
public:
	Animation() = default;

	Animation(const char* animationPath, std::shared_ptr<Object> obj);
	~Animation();

	Bone* FindBone(const std::string& name);
	inline int GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration; }
	inline const std::shared_ptr< AssimpNodeData> GetRootNode() { return m_RootNode; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap(){return m_BoneInfoMap;}

private:
	void ReadMissingBones(const aiAnimation* animation, std::shared_ptr<Object>& model);
	void ReadHierarchyData(std::shared_ptr<AssimpNodeData>& dest, const aiNode* src);

	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	std::shared_ptr< AssimpNodeData> m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
};
