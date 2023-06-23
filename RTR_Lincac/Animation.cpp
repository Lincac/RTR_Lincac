#include"Animation.h"

Animation::Animation(const char* animationPath, std::shared_ptr<Object> model) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	m_Duration = float(animation->mDuration);
	m_TicksPerSecond = int(animation->mTicksPerSecond);
	m_RootNode = std::make_shared<AssimpNodeData>();
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	ReadHierarchyData(m_RootNode, scene->mRootNode);
	ReadMissingBones(animation, model);
}

Animation::~Animation() {}

Bone* Animation::FindBone(const std::string& name) {
	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
		[&](const Bone& Bone)
		{
			return Bone.GetBoneName() == name;
		}
	);
	if (iter == m_Bones.end()) return nullptr;
	else return &(*iter);
}

void Animation::ReadMissingBones(const aiAnimation* animation, std::shared_ptr<Object>& model) {
	int size = animation->mNumChannels;

	auto boneInfoMap = model->GetBoneInfoMap();
	int boneCount = model->GetBoneCount(); 

	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		m_Bones.push_back(Bone(channel->mNodeName.data,
			boneInfoMap[channel->mNodeName.data].id, channel));
	}

	m_BoneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(std::shared_ptr<AssimpNodeData>& dest, const aiNode* src) {
	assert(src);

	dest->name = src->mName.data;
	dest->transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
	dest->childrenCount = src->mNumChildren;

	for (unsigned int i = 0; i < src->mNumChildren; i++)
	{
		std::shared_ptr<AssimpNodeData >newData = std::make_shared<AssimpNodeData>();
		ReadHierarchyData(newData, src->mChildren[i]);
		dest->children.push_back(newData);
	}
}