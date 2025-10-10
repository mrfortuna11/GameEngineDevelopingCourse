#include <Camera.h>
#include <ecsControl.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>
#include <flecs.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <Vector.h>
#include <DefaultGeometry.h>
#include <RenderObject.h>
#include <ecsMesh.h>

using namespace GameEngine;

void RegisterEcsControlSystems(flecs::world& world)
{
	world.system<Position, CameraPtr, const ControllerPtr, LocalTimer, Player>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const ControllerPtr& controller, LocalTimer& time, Player& player)
			{
				if (time.timer.GetTotalTime() - player.lastRecharged < player.timeToRecharge) return;

				if (controller.ptr->IsPressed("Shoot") && time.timer.GetTotalTime() - player.lastShooted > player.timeToShoot)
				{
					player.lastShooted = time.timer.GetTotalTime();
					player.curAmmoCount -= 1;
					if (player.curAmmoCount <= 0)
					{
						player.curAmmoCount = player.maxAmmoCount;
						player.lastRecharged = time.timer.GetTotalTime();
					}

					Math::Vector3f bulletVelocity = camera.ptr->GetViewDir().Normalized() * 50;

					flecs::entity cubeMoving = world.entity()
						.set(Position(camera.ptr->GetPosition()))
						.set(Velocity(bulletVelocity))
						.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
						.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
						.set(Bounciness{ 1.f })
						.set(Bullet{ 5 })
						.set(LocalTimer())
						.set(GeometryPtr{ RenderCore::DefaultGeometry::littleCube() })
						.set(RenderObjectPtr{ new Render::RenderObject() });
				}
			});

	world.system<LocalTimer, Bullet, Position>()
		.each([&](flecs::entity e, LocalTimer& time, Bullet& bullet, Position& position)
			{
				if (time.timer.GetTotalTime() >= bullet.timeToDeath)
				{
					position.value = GameEngine::Math::Vector3f(10000, 10000, 10000);
					e.clear();
				}
			});

	world.system<Bullet, Position>()
		.each([&](flecs::entity e1, Bullet& bullet, Position& position1)
			{
				world.each([&](flecs::entity e2, Obstacle obsacle, Position& position2)
					{
						if (e1 != e2 && (position1.value - position2.value).GetLength() <= 1)
						{
							if (obsacle.onCollision != nullptr)
							{
								obsacle.onCollision();
							}

							position2.value = GameEngine::Math::Vector3f(10000, 10000, 10000);
						}
					});
			});

	world.system<LocalTimer>()
		.each([&](LocalTimer& time)
			{
				time.timer.Tick();
			});
}