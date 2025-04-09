#ifndef ENTROPY_CONFIG_H
#define ENTROPY_CONFIG_H

// Version information
#define ENTROPY_VERSION_MAJOR 1
#define ENTROPY_VERSION_MINOR 0
#define ENTROPY_VERSION_PATCH 0

// Combined version as a single integer (e.g., 10003)
#define ENTROPY_VERSION_NUMBER                                       \
  ((PROJECT_VERSION_MAJOR * 10000) + (PROJECT_VERSION_MINOR * 100) + \
   PROJECT_VERSION_PATCH)

#define ARM64 (1)
#define X86_64 (2)

#define MACOS (1)
#define LINUX (2)
#define WINDOWS (3)
#define IOS (4)
#define IOS_SIMULATOR (5)
#define ANDROID (6)

#define ENTROPY_PLATFORM LINUX
#define ENTROPY_ARCH 

#define USE_VALIDATION_LAYERS 1

#define CONCURRENT_FRAMES_IN_FLIGHT (3)
#define MAX_INSTANCE_COUNT (1000)

#define ENTROPY_ENABLE_SCRIPTING 1
#define MONO_INCLUDE_DIR_PATH "/usr/include/mono-2.0"
#define MONO_LIBRARY_DIR_PATH "/usr/lib"
#define MONO_LIBRARY_ETC_PATH "/etc/mono"
#define MONO_LIBRARY_CONFIG "/etc/mono/config"

#if ENTROPY_PLATFORM == MACOS || ENTROPY_PLATFORM == LINUX
#define COLOR_FORMAT VK_FORMAT_B8G8R8A8_UNORM
#else
#define COLOR_FORMAT VK_FORMAT_R8G8B8A8_UNORM
#endif

#endif  // ENTROPY_CONFIG_H
