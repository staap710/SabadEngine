#pragma once

#include "UIComponent.h"

namespace SabadEngine
{
	class UISpriteComponent : public UIComponent
	{
	public:
		SET_TYPE_ID(ComponentId::UISprite);

		void Initialize() override;
		void Terminate() override;
		void Render() override;
		void Deserialize(const rapidjson::Value& value) override;

		void SetTexture(const std::filesystem::path& texturePath);
		void SetPosition(const Math::Vector2& pos);
		void SetActive(bool active);

		Math::Vector2 GetPosition(bool includeOrigin = true);

	private:
		bool mActive = true;
		std::filesystem::path mTexturePath;
		Math::Vector2 mPosition;
		RECT mRect = { 0, 0, 0, 0 };
		Graphics::UISprite mUISprite;

	};
}