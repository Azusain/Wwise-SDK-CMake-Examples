

#include "AK/Tools/Common/AkMonitorError.h"
#include "Wwise_IDs.h"
#include <chrono>
#include <cstddef>

#include "AK/SoundEngine/Common/AkSoundEngine.h"
#include "AK/SoundEngine/Common/AkStreamMgrModule.h"
#include "AK/SoundEngine/Common/AkTypes.h"
#include "AK/SoundEngine/Platforms/Mac/AkMacSoundEngine.h"
#include "AK/SoundEngine/Platforms/POSIX/AkTypes.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AkFilePackageLowLevelIO.h>
#include <AkFilePackageLowLevelIODeferred.h>
#include <cassert>
#include <iostream>
#include <thread>

CAkFilePackageLowLevelIODeferred g_lowLevelIO;

bool InitSoundEngine() {
  // memory manager.
  AkMemSettings memSettings;
  AK::MemoryMgr::GetDefaultSettings(memSettings);
  if (AK::MemoryMgr::Init(&memSettings) != AK_Success) {
    std::cout << "failed to init memory manager\n";
    return false;
  }

  // stream manager.
  AkStreamMgrSettings streamSettings;
  AK::StreamMgr::GetDefaultSettings(streamSettings);
  if (!AK::StreamMgr::Create(streamSettings)) {
    std::cout << "failed to create stream manager\n";
    return false;
  }
  AkDeviceSettings deviceSettings;
  AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

  if (g_lowLevelIO.Init(deviceSettings) != AK_Success) {
    std::cout << "failed to init low level I/O system\n";
    return false;
  }

  // sound engine.
  AkInitSettings initSettings;
  AkPlatformInitSettings platformInitSettings;
  AK::SoundEngine::GetDefaultInitSettings(initSettings);
  AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
  if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) !=
      AK_Success) {
    std::cout << "failed to init sound engine\n";
    return false;
  }

  return true;
}

// ref:
// https://www.audiokinetic.com/zh/library/edge/?source=SDK&id=workingwithsdks_termination.html
void TermSoundEngine() {
  AK::SoundEngine::Term();
  g_lowLevelIO.Term();
  auto streamManager = AK::IAkStreamMgr::Get();
  if (streamManager) {
    streamManager->Destroy();
  }
  AK::MemoryMgr::Term();
}

// call it only after initiating the sound engine.
void PlaySomething() {
  AkGameObjectID emitter = 1;
  AkGameObjectID listener = 2;
  AK::SoundEngine::RegisterGameObj(emitter);
  AK::SoundEngine::RegisterGameObj(listener);
  AkSoundPosition sound_pos;
  // ref(xyz):
  // https://www.audiokinetic.com/zh/library/edge/?source=SDK&id=soundengine_3dpositions.html.
  sound_pos.SetPosition(0.5, 0, 0.5);
  sound_pos.SetOrientation(-1, 0, 0, 0, 1, 0);
  AK::SoundEngine::SetPosition(emitter, sound_pos);
  AK::SoundEngine::SetDefaultListeners(&listener, 1);
  AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_KICK, emitter);
}

int main(int, char **) {
  if (!InitSoundEngine()) {
    std::cout << "failed to init wwise engine\n";
    return 1;
  }

  // init soundbanks.
  g_lowLevelIO.SetBasePath(
      AKTEXT("/Users/azusain/Documents/WwiseProjects/MyFirstWwiseProject1/"
             "GeneratedSoundBanks/Mac"));
  AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));
  AkBankID bankID;
  AKRESULT eResult = AK::SoundEngine::LoadBank(L"Init.bnk", bankID);
  assert(eResult == AK_Success);
  eResult = AK::SoundEngine::LoadBank("Main.bnk", bankID);
  assert(eResult == AK_Success);

  // render the audio on a per-frame basis.
  while (true) {
    PlaySomething();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    AK::SoundEngine::RenderAudio();
  }

  TermSoundEngine();
  return 0;
}
