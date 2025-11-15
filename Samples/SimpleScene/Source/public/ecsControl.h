#pragma once

#include <flecs.h>

namespace GameEngine::Core
{
	class Camera;
	class Controller;
}

struct ControllerPtr
{
	GameEngine::Core::Controller* ptr;
};

struct JumpSpeed
{
	float value;
};

struct CameraPtr
{
	GameEngine::Core::Camera* ptr;
};

struct JumpSoundPtr
{
	void* soundHandle = nullptr;
};

void RegisterEcsControlSystems(flecs::world& world);

