#pragma once

#include <flecs.h>
	
namespace FMOD 
{ 
	class Sound; 
}

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
	FMOD::Sound* soundHandle;
};

void RegisterEcsControlSystems(flecs::world& world);

