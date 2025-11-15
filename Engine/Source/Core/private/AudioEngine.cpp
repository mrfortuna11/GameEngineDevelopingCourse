
#include "AudioEngine.h"
#include <fmod.h>
#include <fmod.hpp>

namespace GameEngine::Core {

   AudioEngine g_AudioEngine;

   AudioEngine::AudioEngine() = default;

   AudioEngine::~AudioEngine()
   {
       Shutdown();
   }

   bool AudioEngine::Init()
   {
       if (m_initialized)
           return true;

       auto** sys = reinterpret_cast<FMOD::System**>(&m_system);
       FMOD_RESULT result = FMOD::System_Create(sys);
       if (result != FMOD_OK)
           return false;

       result = (*sys)->init(512, FMOD_INIT_NORMAL, nullptr);
       if (result != FMOD_OK)
       {
           (*sys)->release();
           m_system = nullptr;
           return false;
       }
       m_initialized = true;
       return true;
   }

   void AudioEngine::Shutdown()
   {
       auto* sys = reinterpret_cast<FMOD::System*>(m_system);
       if (sys)
       {
           sys->close();
           sys->release();
           m_system = nullptr;
       }
       m_initialized = false;
   }

   void AudioEngine::Update()
   {
       auto* sys = reinterpret_cast<FMOD::System*>(m_system);
       if (sys)
           sys->update();
   }

   void* AudioEngine::LoadSound(const char* path)
   {
       auto* sys = reinterpret_cast<FMOD::System*>(m_system);
       if (!sys) return nullptr;
       FMOD::Sound* sound = nullptr;
       FMOD_RESULT result = sys->createSound(path, FMOD_DEFAULT, nullptr, &sound);
       if (result != FMOD_OK) return nullptr;
       return sound;
   }

   void AudioEngine::PlaySound(void* soundHandle)
   {
       auto* sys = reinterpret_cast<FMOD::System*>(m_system);
       if (!sys || !soundHandle) return;
       sys->playSound(reinterpret_cast<FMOD::Sound*>(soundHandle), nullptr, false, nullptr);
   }

} 
