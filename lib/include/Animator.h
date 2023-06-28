#pragma once
#include "Animation.h"
#include "Component.h"
#include <memory>

class TimeManager;
class Aniamtion;

enum PlayerAnimType {
	Standing,
	Running,
	TileAction,
	Punching,
	Punched
};


class Animator : public Component
{
private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	std::shared_ptr<Animation> m_CurrentAnimation = nullptr;
	float m_CurrentTime = 0;
	float m_DeltaTime = 0;
	std::vector<std::shared_ptr<Animation>> animations = std::vector<std::shared_ptr<Animation>>();
public:
	Animator(Model* playerModel);
	void UpdateAnimation(float dt);
	void PlayAnimation(std::shared_ptr<Animation> pAnimation);
	void PlayAnimation(PlayerAnimType animType);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
	std::vector<glm::mat4> GetFinalBoneMatrices();
	void update() override;
};

