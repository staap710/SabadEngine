#pragma once

#include "CustomTypeIds.h"
#include <SabadEngine/Inc/SabadEngine.h>

class CustomDebugDrawComponent : public SabadEngine::Component
{
public:
	SET_TYPE_ID(CustomComponentId::CustomDebugDraw);

	void Initialize() override;
	void Terminate() override;
	void DebugUI() override;
	void Deserialize(const rapidjson::Value& value) override;

	void AddDebugDraw() const;
private:
	const SabadEngine::TransformComponent* mTransformComponent = nullptr;
	SabadEngine::Math::Vector3 mPosition = SabadEngine::Math::Vector3::Zero;
	SabadEngine::Graphics::Color mColor = SabadEngine::Graphics::Colors::White;

	uint32_t mSlices = 0;
	uint32_t mRings = 0;
	float mRadius = 0;
};