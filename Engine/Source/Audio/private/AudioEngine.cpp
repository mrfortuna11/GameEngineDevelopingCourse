#include "AudioEngine.h"
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>  

namespace GameEngine::Audio 
{

   AudioEngine g_AudioEngine;

   AudioEngine::AudioEngine() = default;

   AudioEngine::~AudioEngine()
   {
      Shutdown();
   }

   bool AudioEngine::Init()
   {
      if (m_initialized)
      {
         return true;
      }

      FMOD::System* sys = nullptr;
      FMOD_RESULT result = FMOD::System_Create(&sys);
      if (result != FMOD_OK)
      {
         std::cout << "FMOD error in System_Create: " << FMOD_ErrorString(result) << std::endl;
         return false;
      }

      result = sys->init(512, FMOD_INIT_NORMAL, nullptr);
      if (result != FMOD_OK)
      {
         std::cout << "FMOD error in init: " << FMOD_ErrorString(result) << std::endl;
         sys->release();
         return false;
      }

      int numDrivers = 0;
      result = sys->getNumDrivers(&numDrivers);
      if (result != FMOD_OK || numDrivers == 0)
      {
         std::cout << "No audio drivers found or error: " << FMOD_ErrorString(result) << std::endl;
         sys->close();
         sys->release();
         return false;
      }

      m_system = sys;
      m_initialized = true;
      std::cout << "AudioEngine initialized, drivers: " << numDrivers << std::endl;
      return true;
   }

   void AudioEngine::Shutdown()
   {
      if (m_system)
      {
         m_system->close();
         m_system->release();
         m_system = nullptr;
      }
      m_initialized = false;
   }

   void AudioEngine::Update()
   {
      if (m_system)
      {
         m_system->update();
      }
   }

   FMOD::Sound* AudioEngine::LoadSound(const char* path)
   {
      if (!m_system)
      {
         std::cout << "AudioEngine not initialized" << std::endl;
         return nullptr;
      }
      FMOD::Sound* sound = nullptr;
      FMOD_RESULT result = m_system->createSound(path, FMOD_DEFAULT, nullptr, &sound);
      if (result != FMOD_OK)
      {
         std::cout << "LoadSound failed for path '" << path << "': " << FMOD_ErrorString(result) << std::endl;
         return nullptr;
      }
      std::cout << "Sound loaded: " << path << std::endl;
      return sound;
   }

   void AudioEngine::PlaySound(FMOD::Sound* soundHandle)
   {
      if (!m_system || !soundHandle)
      {
         std::cout << "PlaySound skipped: invalid system or sound" << std::endl;
         return;
      }
      FMOD::Channel* channel = nullptr;
      FMOD_RESULT result = m_system->playSound(soundHandle, nullptr, false, &channel);
      if (result != FMOD_OK)
      {
         std::cout << "PlaySound failed: " << FMOD_ErrorString(result) << std::endl;
      }
      else
      {
         std::cout << "Sound played successfully" << std::endl;
      }
   }

}