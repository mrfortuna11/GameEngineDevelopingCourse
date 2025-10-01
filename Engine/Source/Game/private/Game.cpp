#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <Input/InputHandler.h>
#include <TypeComponent.h>
#include <PoolAllocator.h>

namespace GameEngine
{
   constexpr size_t MaxGameObjectSize = std::max({
       sizeof(GameObject),
       sizeof(JumpingCube),
       sizeof(MovingCube),
       sizeof(PlayerCube)
      });

   constexpr size_t MaxGameObjectAlign = std::max({
       alignof(GameObject),
       alignof(JumpingCube),
       alignof(MovingCube),
       alignof(PlayerCube)
      });

   PoolAllocator<GameObject, 100, MaxGameObjectSize, MaxGameObjectAlign> g_ObjectPool;

   Game::Game(std::function<bool()> PlatformLoopFunc)
      : PlatformLoop(PlatformLoopFunc)
   {
      Core::g_MainCamera = new Core::Camera();
      Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
      Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());
      PlayerCube* g_Player = nullptr;

      m_renderThread = std::make_unique<Render::RenderThread>();
      m_Objects.reserve(100);

      std::mt19937 rng(std::random_device{}());
      std::uniform_int_distribution<int> dist(0, 2);

      for (int i = 0; i < 100; ++i)
      {
         GameObject* obj = nullptr;

         int type = dist(rng);
         if (type == 0)
            obj = g_ObjectPool.Allocate<JumpingCube>();
         else if (type == 1)
            obj = g_ObjectPool.Allocate<MovingCube>();
         else
            obj = g_ObjectPool.Allocate<PlayerCube>();


         m_Objects.push_back(obj);
         Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
         m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);


         obj->SetPosition(Math::Vector3f((float)(i % 10) * 2.0f, 0.0f, (float)(i / 10) * 2.0f),
            m_renderThread->GetMainFrame());
      }

      Core::g_InputHandler->RegisterCallback("GoForward", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
      Core::g_InputHandler->RegisterCallback("GoBack", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
      Core::g_InputHandler->RegisterCallback("GoRight", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
      Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });


      Core::g_InputHandler->RegisterCallback("Forward", [&]() {
         for (size_t i = 0; i < m_Objects.size(); ++i)
         {
            if (m_Objects[i]->IsPlayerControlled())
               static_cast<PlayerCube*>(m_Objects[i])->Move(Math::Vector3f(0, 0, 1));
         }
         });
      Core::g_InputHandler->RegisterCallback("Back", [&]() {
         for (size_t i = 0; i < m_Objects.size(); ++i)
         {
            if (m_Objects[i]->IsPlayerControlled())
               static_cast<PlayerCube*>(m_Objects[i])->Move(Math::Vector3f(0, 0, -1));
         }
         });
      Core::g_InputHandler->RegisterCallback("Right", [&]() {
         for (size_t i = 0; i < m_Objects.size(); ++i)
         {
            if (m_Objects[i]->IsPlayerControlled())
               static_cast<PlayerCube*>(m_Objects[i])->Move(Math::Vector3f(1, 0, 0));
         }
         });
      Core::g_InputHandler->RegisterCallback("Left", [&]() {
         for (size_t i = 0; i < m_Objects.size(); ++i)
         {
            if (m_Objects[i]->IsPlayerControlled())
               static_cast<PlayerCube*>(m_Objects[i])->Move(Math::Vector3f(-1, 0, 0));
         }
         });
   }

   

   void Game::Run()
   {
      assert(PlatformLoop != nullptr);

      m_GameTimer.Reset();
      bool quit = false;

      while (!quit)
      {
         m_GameTimer.Tick();
         float dt = m_GameTimer.GetDeltaTime();

         Core::g_MainWindowsApplication->Update();
         Core::g_InputHandler->Update();
         Core::g_MainCamera->Update(dt);

         Update(dt);

         m_renderThread->OnEndFrame();

         quit = !PlatformLoop();
      }
   }

   void Game::Update(float dt)
   {
      size_t frame = m_renderThread->GetMainFrame();
      for (auto* obj : m_Objects)
      {
         obj->Update(dt, frame);
      }
   }
}
