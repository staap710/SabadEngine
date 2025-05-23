#pragma once
#include "GameState.h"

using namespace SabadEngine;
using namespace SabadEngine::Graphics;

void Initialize() override;
void Terminate() override;
void Update(float deltaTime) override;
void Render() override;
void DebugUI() override;
void UpdateCamera(float deltaTime);
