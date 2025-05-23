#include <SabadEngine/Inc/SabadEngine.h>


class GameState : public SabadEngine::Core::AppState
{
public:
	void Initialize() {

	}
	void Terminate() {

	}
	void Update(float deltaTime) {

	}
	void Render() {

	}

	bool gCheckValue = false;
	float gFloatVal = 0.0f;
	Math::Vector3 gVectorVal = Math::Vector3::Zero;
	Color gColor= Colors::White
	void DebugUI() {
		ImGui::Begin("Debug", nullptr, ImGuiWindowsFlags_AlwaysAutoResize);
		ImGui::Text("Hello Debug");
		ImGui::Checkbox("IsChecked", &gCheckValue);
		ImGui::DragFloat("FloatVal", &gFloatVal);
		ImGui::DragFloat3("VectorVal", &gVectorVal.x);
		ImGui::End();

	}
	void UpdateCamera(float deltaTime) {
		SabadEngine::Graphics::Camera mCamera;

	}

}