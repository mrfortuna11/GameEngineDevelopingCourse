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
#include <RenderEngine.h>
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
		.set(Object{})
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	flecs::entity cubeMoving = m_World.entity()
		.set(Position{ 2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 3.f, 0.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 1.f })
		.set(BulletKill{ 500 })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity camera = m_World.entity()
		.set(Position{ 0.0f, 12.0f, -10.0f })
		.set(Speed{ 10.f })
		.set(Player{})
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });
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
	ECS_META_COMPONENT(m_World, Player);
	ECS_META_COMPONENT(m_World, BulletKill);
	ECS_META_COMPONENT(m_World, ShootCD);
	ECS_META_COMPONENT(m_World, Object);
}

void GameFramework::RegisterECSGameplaySystems(flecs::world& world)
{
	world.system<Position, CameraPtr, const Speed, const ControllerPtr>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed, const ControllerPtr& controller)
			{
				if (controller.ptr->IsPressed("Fire") && !e.has<ShootCD>())
				{
					e.set(ShootCD{ 30 });
					Math::Vector3f bulletVelocity = camera.ptr->GetViewDir() * 50;
					flecs::entity Bullet = world.entity()
						.set(Position{ position.x, position.y, position.z })
						.set(Velocity{ bulletVelocity.x, bulletVelocity.y, bulletVelocity.z })
						.set(Gravity{ 0.f, -9.8065f, 0.f })
						.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
						.set(Bounciness{ 0.3f })
						.set(BulletKill{ 250 })
						.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
						.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });
				}
			});

	world.system<ShootCD>()
		.each([&](flecs::entity e, ShootCD& countdown) {
		countdown.t--;
		if (countdown.t <= 0) {
			e.remove<ShootCD>();
		}
			});

	world.system<Object, const EntitySystem::ECS::RenderObjectPtr>()
		.each([&](flecs::entity e, Object& checkout, const EntitySystem::ECS::RenderObjectPtr& controller) {
		if (checkout.setColor) {
			checkout.setColor = false;
			controller.ptr->SetAlbedo(RenderCore::Colors::Blue);
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