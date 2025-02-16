
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>

#include <cassert>
#include <iostream>

bool InitSoundEngine() {
  AkMemSettings memSettings;
  AK::MemoryMgr::GetDefaultSettings(memSettings);
  if (AK::MemoryMgr::Init(&memSettings) != AK_Success) {
    std::cout << "failed to init memory manager\n";
    return 1;
  }
  return true;
}

int main(int, char **) {
  if (!InitSoundEngine()) {
    std::cout << "failed to init wwise engine\n";
    return 1;
  }
  std::cout << "successfully init sound engine\n";
  AK::MemoryMgr::Term();
  return 0;
}
