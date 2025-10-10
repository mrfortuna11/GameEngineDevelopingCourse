#pragma once

#include <flecs.h>
#include <Timer.h>

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


struct Player
{
	int maxAmmoCount;
	float timeToRecharge;
	float timeToShoot;
	int curAmmoCount;
	float lastRecharged;
	float lastShooted;
};

struct LocalTimer
{
	GameEngine::Core::Timer timer;
};

struct Bullet
{
	float timeToDeath;
};

struct Obstacle
{
	std::function<void()> onCollision;
};

void RegisterEcsControlSystems(flecs::world& world);

