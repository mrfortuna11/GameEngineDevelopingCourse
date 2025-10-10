#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
	RegisterEcsPhysSystems(m_World);

	flecs::entity camera = m_World.entity()
		.set(Position{ Math::Vector3f(0.0f, 3.0f, -3.0f) })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(Player{ 3, 1, 0.2, 3, 0, 0})
		.set(LocalTimer())
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	m_World.entity()
		.set(Position{ Math::Vector3f(0.f, 0.f, 0.f) })
		.set(Obstacle(nullptr))
		.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(RenderObjectPtr{ new Render::RenderObject() });

	m_World.entity()
		.set(Position{ Math::Vector3f(-2.f, 10.f, 10.f) })
		.set(Obstacle([&]()
			{
				m_World.each([&](Player& player)
					{
						player.maxAmmoCount += 3;
					});
			}))
		.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(RenderObjectPtr{ new Render::RenderObject() });
}

void GameFramework::Update(float dt)
{

}