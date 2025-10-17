// This define is essential for the scripts to expose the ECS components to lua syntax
// There is a task to rework this behavior
#define GAME_FRAMEWORK

#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ECS/ecsSystems.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>
#include <flecs.h>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterComponents();
	RegisterSystems();

	flecs::entity cubeControl = m_World.entity()
		.set(Position{ -2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Speed{ 10.f })
		.set(FrictionAmount{ 0.9f })
		.set(JumpSpeed{ 10.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 0.3f })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	flecs::entity cubeMoving = m_World.entity()
		.set(Position{ 2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 3.f, 0.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 1.f })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity camera = m_World.entity()
		.set(Position{ 0.0f, 12.0f, -10.0f })
		.set(Speed{ 10.f })
		.set(Player{ 0.2, 0 })
		.set(Timer(0))
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	flecs::entity destoyable = m_World.entity()
		.set(Position{ 6.0f, 6.f, 0.f })
		.set(Timer({ 0 }))
		.set(ToDestroy({ 5 }))
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity ent1 = m_World.entity()
		.set(Position{ 10.0f, 6.f, 10.0f })
		.set(Velocity{ 0.0f, 0.0f, 0.5f })
		.set(Collider())
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity ent2 = m_World.entity()
		.set(Position{ 10.0f, 6.f, 15.0f })
		.set(Velocity{ 0.0f, 0.0f, -0.5f })
		.set(Collider())
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity target = m_World.entity()
		.set(Position{ -6.f, 6.0f, 0.f })
		.set(Collider())
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });
}

void GameFramework::RegisterComponents()
{
	// Exposing these components for the lua system
	ECS_META_COMPONENT(m_World, Position);
	ECS_META_COMPONENT(m_World, Velocity);
	ECS_META_COMPONENT(m_World, Gravity);
	ECS_META_COMPONENT(m_World, BouncePlane);
	ECS_META_COMPONENT(m_World, Bounciness);
	ECS_META_COMPONENT(m_World, ShiverAmount);
	ECS_META_COMPONENT(m_World, FrictionAmount);
	ECS_META_COMPONENT(m_World, Speed);
	ECS_META_COMPONENT(m_World, Timer);
	ECS_META_COMPONENT(m_World, Collider);
	ECS_META_COMPONENT(m_World, ToDestroy);
}

void RegisterECSGameplaySystems(flecs::world& world)
{
	world.system<Position, CameraPtr, const ControllerPtr, Timer, Player>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const ControllerPtr& controller, Timer& timer, Player& player)
			{
				if (controller.ptr->IsPressed("Fire") && timer.time - player.lastShoot > player.timeShoot)
				{
					player.lastShoot = timer.time;

					Math::Vector3f bulletVelocity = camera.ptr->GetViewDir().Normalized() * 10;
					Math::Vector3f spawnPoint = camera.ptr->GetPosition();

					flecs::entity bullet = world.entity()
						.set(Position({ spawnPoint.x, spawnPoint.y, spawnPoint.z }))
						.set(Velocity({ bulletVelocity.x, bulletVelocity.y, bulletVelocity.z }))
						.set(Timer({ 0 }))
						.set(ToDestroy({ 3 }))
						.set(Collider({ true, false, false }))
						.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::littleCube() })
						.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });
				}
			});
}

void GameFramework::RegisterSystems()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
	RegisterECSGameplaySystems(m_World);
}

void GameFramework::Update(float dt)
{

}