#pragma once

#include <Audio/export.h>

namespace FMOD { class System; class Sound; }

namespace GameEngine
{
   namespace Audio
   {

      class AUDIO_API AudioEngine final
      {
      public:
         AudioEngine();
         ~AudioEngine();

         bool Init();
         void Shutdown();
         void Update();

         FMOD::Sound* LoadSound(const char* path);
         void PlaySound(FMOD::Sound* sound);

         bool IsInitialized() const { return m_initialized; }

      private:
         FMOD::System* m_system = nullptr;
         bool m_initialized = false;
      };

      extern AUDIO_API AudioEngine g_AudioEngine;
   }
}