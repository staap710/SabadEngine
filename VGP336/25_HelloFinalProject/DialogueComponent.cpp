#include "DialogueComponent.h"

using namespace SabadEngine;
using namespace SabadEngine::Input;

void DialogueComponent::Initialize()
{
    mTextComponent = GetOwner().GetComponent<UITextComponent>();
    // The background sprite component is expected to be on the same GameObject as DialogueComponent
    mSpriteComponent = GetOwner().GetComponent<UISpriteComponent>(); 
    mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
    mSoundEventComponent = GetOwner().GetComponent<SoundEventComponent>();

    // Dynamically create a child GameObject for the Character Sprite so it renders on top of the Background
    mCharacterObject = GetOwner().GetWorld().CreateGameObject("DialogueCharacter");
    if (mCharacterObject)
    {
        mCharacterSpriteComponent = mCharacterObject->AddComponent<UISpriteComponent>();
        
        // Provide a valid dummy texture so it doesn't assert on Initialize.
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value texVal("../../Assets/Textures/misc/basketball.jpg", doc.GetAllocator());
        doc.AddMember("Texture", texVal, doc.GetAllocator());

        // Set Scale for massive 4K textures so they fit on 1080p
        rapidjson::Value scaleArray(rapidjson::kArrayType);
        scaleArray.PushBack(0.125f, doc.GetAllocator());
        scaleArray.PushBack(0.125f, doc.GetAllocator());
        doc.AddMember("Scale", scaleArray, doc.GetAllocator());

        // Set Pivot to Bottom so we can center it easily
        rapidjson::Value pivotVal("Bottom", doc.GetAllocator());
        doc.AddMember("Pivot", pivotVal, doc.GetAllocator());

        mCharacterSpriteComponent->Deserialize(doc);
        
        mCharacterObject->Initialize();
        mCharacterSpriteComponent->SetActive(false); // Hide until needed
        
        // Let's set a default position for the character (centered horizontally, anchored to bottom)
        mCharacterSpriteComponent->SetPosition({960.0f, 700.0f});
        
        GetOwner().AddChild(mCharacterObject);
    }

    if (!mDialogueFile.empty())
    {
        LoadDialogueFile(mDialogueFile);
        SetNode(mStartNodeId);
    }

    // Force Text to render on top of newly spawned Character Sprite
    if (mTextComponent != nullptr)
    {
        mTextComponent->Terminate();
        mTextComponent->Initialize();
    }
}

void DialogueComponent::Terminate()
{
}

void DialogueComponent::Update(float deltaTime)
{
    if (mIsTyping && mTextComponent != nullptr)
    {
        mTypeTimer += deltaTime;
        if (mTypeTimer >= mTypeSpeed)
        {
            mTypeTimer = 0.0f;
            mCurrentCharIndex++;
            if (mCurrentCharIndex > mCurrentFullText.length())
            {
                mIsTyping = false;
                mCurrentCharIndex = mCurrentFullText.length();
                // When typing finishes, play animation if specified
                if (mNodes.count(mCurrentNodeId) > 0)
                {
                    const auto& node = mNodes[mCurrentNodeId];
                    if (node.animationIndex >= 0 && mAnimatorComponent != nullptr)
                    {
                        mAnimatorComponent->Play(node.animationIndex);
                    }
                }
            }
            std::string displayText = mCurrentFullText.substr(0, mCurrentCharIndex);
            if (mNodes.count(mCurrentNodeId) > 0 && !mNodes[mCurrentNodeId].speaker.empty())
            {
                displayText = mNodes[mCurrentNodeId].speaker + ": " + displayText;
            }
            mTextComponent->SetText(displayText);
        }
    }

    auto inputSystem = InputSystem::Get();
    if (inputSystem->IsKeyPressed(KeyCode::SPACE) || inputSystem->IsMousePressed(MouseButton::LBUTTON))
    {
        AdvanceText();
    }
}

void DialogueComponent::AdvanceText()
{
    if (mIsTyping)
    {
        // Skip typing
        mIsTyping = false;
        mCurrentCharIndex = mCurrentFullText.length();
        std::string displayText = mCurrentFullText;
        if (mNodes.count(mCurrentNodeId) > 0 && !mNodes[mCurrentNodeId].speaker.empty())
        {
            displayText = mNodes[mCurrentNodeId].speaker + ": " + displayText;
        }
        if (mTextComponent != nullptr)
        {
            mTextComponent->SetText(displayText);
        }

        // Play animation when skipped
        if (mNodes.count(mCurrentNodeId) > 0)
        {
            const auto& node = mNodes[mCurrentNodeId];
            if (node.animationIndex >= 0 && mAnimatorComponent != nullptr)
            {
                mAnimatorComponent->Play(node.animationIndex);
            }
        }
    }
    else
    {
        // Advance to next node if no choices
        if (mNodes.count(mCurrentNodeId) > 0)
        {
            const auto& node = mNodes[mCurrentNodeId];
            if (node.choices.empty() && node.nextNodeId != -1)
            {
                SetNode(node.nextNodeId);
            }
        }
    }
}

void DialogueComponent::SetNode(int nodeId)
{
    if (mNodes.find(nodeId) == mNodes.end())
        return;

    mCurrentNodeId = nodeId;
    const auto& node = mNodes[nodeId];

    mCurrentFullText = node.text;
    mCurrentCharIndex = 0;
    mIsTyping = true;
    mTypeTimer = 0.0f;

    if (mTextComponent != nullptr)
    {
        std::string displayText = "";
        if (!node.speaker.empty())
            displayText = node.speaker + ": ";
        mTextComponent->SetText(displayText);
    }

    // Update Background
    if (mSpriteComponent != nullptr && !node.background.empty())
    {
        mSpriteComponent->SetTexture(node.background);
    }

    // Update Character Sprite
    if (mCharacterSpriteComponent != nullptr)
    {
        if (!node.characterImage.empty())
        {
            mCharacterSpriteComponent->SetTexture(node.characterImage);
            mCharacterSpriteComponent->SetActive(true);
        }
        else
        {
            mCharacterSpriteComponent->SetActive(false);
        }
    }

    if (mSoundEventComponent != nullptr && !node.soundEvent.empty())
    {
        mSoundEventComponent->Play();
    }
}

void DialogueComponent::MakeChoice(int choiceIndex)
{
    if (mNodes.count(mCurrentNodeId) > 0)
    {
        const auto& node = mNodes[mCurrentNodeId];
        if (choiceIndex >= 0 && choiceIndex < node.choices.size())
        {
            int nextId = node.choices[choiceIndex].nextNodeId;
            if (nextId != -1)
            {
                SetNode(nextId);
            }
        }
    }
}

void DialogueComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("DialogueFile"))
    {
        mDialogueFile = value["DialogueFile"].GetString();
    }
    if (value.HasMember("TypeSpeed"))
    {
        mTypeSpeed = value["TypeSpeed"].GetFloat();
    }
}

void DialogueComponent::LoadDialogueFile(const std::filesystem::path& path)
{
    FILE* file = nullptr;
    auto err = fopen_s(&file, path.u8string().c_str(), "r");
    if (err != 0 || file == nullptr)
        return;

    char readBuffer[65536];
    rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));

    rapidjson::Document doc;
    doc.ParseStream(readStream);
    
    fclose(file);

    if (doc.HasMember("StartNodeId"))
    {
        mStartNodeId = doc["StartNodeId"].GetInt();
    }

    if (doc.HasMember("Nodes"))
    {
        const auto& nodesArray = doc["Nodes"].GetArray();
        for (const auto& nodeVal : nodesArray)
        {
            DialogueNode node;
            if (nodeVal.HasMember("Id")) node.id = nodeVal["Id"].GetInt();
            if (nodeVal.HasMember("Speaker")) node.speaker = nodeVal["Speaker"].GetString();
            if (nodeVal.HasMember("Text")) node.text = nodeVal["Text"].GetString();
            if (nodeVal.HasMember("Background")) node.background = nodeVal["Background"].GetString();
            if (nodeVal.HasMember("CharacterImage")) node.characterImage = nodeVal["CharacterImage"].GetString();
            if (nodeVal.HasMember("Animation")) node.animationIndex = nodeVal["Animation"].GetInt();
            if (nodeVal.HasMember("Sound")) node.soundEvent = nodeVal["Sound"].GetString();
            if (nodeVal.HasMember("NextNodeId")) node.nextNodeId = nodeVal["NextNodeId"].GetInt();

            if (nodeVal.HasMember("Choices"))
            {
                const auto& choicesArray = nodeVal["Choices"].GetArray();
                for (const auto& choiceVal : choicesArray)
                {
                    DialogueChoice choice;
                    if (choiceVal.HasMember("Text")) choice.text = choiceVal["Text"].GetString();
                    if (choiceVal.HasMember("NextNodeId")) choice.nextNodeId = choiceVal["NextNodeId"].GetInt();
                    node.choices.push_back(choice);
                }
            }

            mNodes[node.id] = node;
        }
    }
}

void DialogueComponent::DebugUI()
{
    ImGui::Text("Dialogue System");
    if (mNodes.count(mCurrentNodeId) > 0)
    {
        const auto& node = mNodes[mCurrentNodeId];
        ImGui::Text("Current Node ID: %d", mCurrentNodeId);
        ImGui::Text("Speaker: %s", node.speaker.c_str());
        
        if (!mIsTyping && !node.choices.empty())
        {
            ImGui::Separator();
            ImGui::Text("Choices:");
            for (size_t i = 0; i < node.choices.size(); ++i)
            {
                if (ImGui::Button(node.choices[i].text.c_str()))
                {
                    MakeChoice(i);
                }
            }
        }
    }
}
