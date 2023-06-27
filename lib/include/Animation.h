#pragma once
#include "Bone.h"
#include "Model.h"
#include <map>

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
private:
    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    void ReadMissingBones(const aiAnimation* animation, Model& model);
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

public:
    Animation() = default;
    Animation(const std::string& animationPath, Model* model);
    Bone* FindBone(const std::string& name);
    float GetTicksPerSecond();
    float GetDuration();
    const AssimpNodeData& GetRootNode();
    const std::map<std::string, BoneInfo>& GetBoneIDMap();
};

