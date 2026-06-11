#include "Precompiled.h"
#include "DynamicAnimatorComponent.h"
#include "SaveUtil.h"
#include "GameObject.h"
#include "ModelComponent.h"

using namespace SabadEngine;

void DynamicAnimatorComponent::Initialize()
{
	const ModelComponent* modelComponent = GetOwner().GetComponent<ModelComponent>();
	ASSERT(modelComponent != nullptr, "AnimatorComponent: GameObject needs a ModelComponent!");
	mAnimator.Initialize(modelComponent->GetModelId());
}
void DynamicAnimatorComponent::Update(float deltaTime)
{
	mAnimator.Update(deltaTime);
}

//salmon was here 

void DynamicAnimatorComponent::DebugUI()
{
	uint32_t animCount = mAnimator.GetAnimationCount();
	if (animCount > 0)
	{
		if (ImGui::CollapsingHeader("Animations"))
		{
			if (ImGui::Button("Stop##Anim"))
			{
				Play(-1);
			}
			std::string buttonTag = "PlayAnim";
			for (uint32_t i = 0; i < animCount; ++i)
			{
				std::string buttonName = buttonTag + std::to_string(i);
				if (ImGui::Button(buttonName.c_str()))
				{
					Play(i, true);
				}
			}
		}

	}
}
bool DynamicAnimatorComponent::Play(int index, bool looping)
{
	if (index < mAnimator.GetAnimationCount())
	{
		mAnimator.PlayAnimation(index, looping);
		return true;
	}
	return false;
}

Graphics::Animator& DynamicAnimatorComponent::GetAnimator()
{
	return mAnimator;
}

const Graphics::Animator& DynamicAnimatorComponent::GetAnimator() const
{
	return mAnimator;
}