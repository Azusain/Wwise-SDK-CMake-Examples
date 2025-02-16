
#include "AK/SoundEngine/Common/AkSoundEngine.h"
#include "AK/SoundEngine/Common/AkStreamMgrModule.h"
#include "AK/SoundEngine/Common/AkTypes.h"
#include "AK/SoundEngine/Platforms/Mac/AkMacSoundEngine.h"
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AkFilePackageLowLevelIO.h>
#include <AkFilePackageLowLevelIODeferred.h>
#include <cassert>
#include <iostream>

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

int main(int, char **) {
  if (!InitSoundEngine()) {
    std::cout << "failed to init wwise engine\n";
    return 1;
  }

  TermSoundEngine();

  return 0;
}
