#pragma once

#include <Core/export.h>

namespace FMOD { class System; }

namespace GameEngine::Core 
{


   class CORE_API AudioEngine
   {
   public:
       AudioEngine();
       ~AudioEngine();

       bool Init();
       void Shutdown();
       void Update();


       void* LoadSound(const char* path);

       void PlaySound(void* soundHandle);

       bool IsInitialized() const { return m_initialized; }

   private:
       void* m_system = nullptr;
       bool m_initialized = false;
   };

       extern CORE_API AudioEngine g_AudioEngine;
}
