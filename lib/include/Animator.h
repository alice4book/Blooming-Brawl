#pragma once
#include "Animation.h"
#include <memory>

class TimeManager;

class Animator : public Component
{
private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	std::shared_ptr<Animation> m_CurrentAnimation = nullptr;
	float m_CurrentTime = 0;
	float m_DeltaTime = 0;
public:
	Animator(std::shared_ptr<Animation> animation);
	void UpdateAnimation(float dt);
	void PlayAnimation(std::shared_ptr<Animation> pAnimation);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
	std::vector<glm::mat4> GetFinalBoneMatrices();
	void update() override;
};

