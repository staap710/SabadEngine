#pragma once
#include <SabadEngine/Inc/SabadEngine.h>
#include "CustomTypeIds.h"

struct DialogueChoice
{
    std::string text;
    int nextNodeId = -1;
};

struct DialogueNode
{
    int id = -1;
    std::string speaker;
    std::string text;
    std::string background;
    std::string characterImage;
    int animationIndex = -1;
    std::string soundEvent;
    int nextNodeId = -1;
    std::vector<DialogueChoice> choices;
};

class DialogueComponent final : public SabadEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::Dialogue);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Deserialize(const rapidjson::Value& value) override;
    void DebugUI() override;

private:
    void LoadDialogueFile(const std::filesystem::path& path);
    void SetNode(int nodeId);
    void AdvanceText();
    void MakeChoice(int choiceIndex);

    std::filesystem::path mDialogueFile;
    std::unordered_map<int, DialogueNode> mNodes;
    int mCurrentNodeId = -1;
    int mStartNodeId = 0;
    
    // Typing effect state
    bool mIsTyping = false;
    float mTypeTimer = 0.0f;
    float mTypeSpeed = 0.05f; // Seconds per character
    size_t mCurrentCharIndex = 0;
    std::string mCurrentFullText;
    
    // Cached references
    SabadEngine::UITextComponent* mTextComponent = nullptr;
    SabadEngine::UISpriteComponent* mSpriteComponent = nullptr; // Used for Background
    SabadEngine::AnimatorComponent* mAnimatorComponent = nullptr;
    SabadEngine::SoundEventComponent* mSoundEventComponent = nullptr;

    SabadEngine::GameObject* mCharacterObject = nullptr;
    SabadEngine::UISpriteComponent* mCharacterSpriteComponent = nullptr;
};
