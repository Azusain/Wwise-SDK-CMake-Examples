# NOTES: this only works on MacOS !!!
cmake_minimum_required(VERSION 3.10.0)
project(wwise_sdk_test VERSION 0.1.0 LANGUAGES C CXX)
set(WWISE_SDK_PATH "/Applications/Audiokinetic/Wwise2024.1.2.8726/SDK")
set(WWISE_LIBRARY_PATH "${WWISE_SDK_PATH}/Mac_Xcode1600/Debug/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_STANDARD 14)
set(SAMPLE_SOUND_ENGINE_DIR "/Applications/Audiokinetic/Wwise2024.1.2.8726/SDK/samples/SoundEngine")
# remember to install the required plugins from the Wwise launcher.
set(CMAKE_BUILD_RPATH "/Applications/Audiokinetic/Wwise2024.1.2.8726/SDK/Mac_Xcode1600/Debug/bin")

# Create the executable
# ref: https://www.audiokinetic.com/zh/library/edge/?source=SDK&id=streamingdevicemanager.html
file(GLOB_RECURSE SOUND_ENGINE_COMMON_SRC
 "${SAMPLE_SOUND_ENGINE_DIR}/Common/*.cpp"
 "${SAMPLE_SOUND_ENGINE_DIR}/Common/*.inl"
 )

add_executable(wwise_sdk_test
main.cpp
"${SAMPLE_SOUND_ENGINE_DIR}/POSIX/AkDefaultIOHookDeferred.cpp"
${SOUND_ENGINE_COMMON_SRC}
)

# Include Wwise SDK headers
target_include_directories(wwise_sdk_test PUBLIC 
"${WWISE_SDK_PATH}/include"
"${SAMPLE_SOUND_ENGINE_DIR}/POSIX"
"${SAMPLE_SOUND_ENGINE_DIR}/Common"
# integration DEMO
"/Users/azusain/Documents/WwiseProjects/MyFirstWwiseProject1/GeneratedSoundBanks/"
)

# Link libraries
# ref: https://www.audiokinetic.com/zh/library/edge/?source=SDK&id=goingfurther_builds.html
target_link_libraries(wwise_sdk_test PUBLIC
  "${WWISE_LIBRARY_PATH}/libAkSoundEngine.a"
  "${WWISE_LIBRARY_PATH}/libAkMemoryMgr.a"
  "${WWISE_LIBRARY_PATH}/libAkStreamMgr.a"
  "${WWISE_LIBRARY_PATH}/libAkMusicEngine.a"
  "${WWISE_LIBRARY_PATH}/libAkSpatialAudio.a"
  "${WWISE_LIBRARY_PATH}/libCommunicationCentral.a"
  "-framework CoreAudio"
  "-framework AudioToolbox"
  "-framework AVFoundation"
  "-framework Cocoa"
)