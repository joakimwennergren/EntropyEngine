#  ______       _                                            _
#  |  ____|     | |                                          (_)
#  | |__   _ __ | |_ _ __ ___  _ __  _   _    ___ _ __   __ _ _ _ __   ___
#  |  __| | '_ \| __| '__/ _ \| '_ \| | | |  / _ \ '_ \ / _` | | '_ \ / _ \
#  | |____| | | | |_| | | (_) | |_) | |_| | |  __/ | | | (_| | | | | |  __/
#  |______|_| |_|\__|_|  \___/| .__/ \__, |  \___|_| |_|\__, |_|_| |_|\___|
#                             | |     __/ |              __/ |
#                             |_|    |___/              |___/               v 1.0.0

cmake_minimum_required(VERSION 3.28.3)

include(FetchContent)
include(cmake/entropy.cmake)
project(Entropy VERSION 1.0.0 LANGUAGES CXX)

# Define an option to choose between executable or library
option(BUILD_ENTROPY_AS_EXECUTABLE "Build Entropy as executable" ON)
option(ENTROPY_LIBRARY_TYPE SHARED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ENTROPY_VERSION_MAJOR 1)
set(ENTROPY_VERSION_MINOR 0)
set(ENTROPY_VERSION_PATCH 0)

if(IOS)
    message("BUILDING FOR IOS!!!")
    set(ENTROPY_PLATFORM IOS)
    set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
    set(CMAKE_XCODE_SCHEME_ENABLE_GPU_API_VALIDATION FALSE)
    set(CMAKE_XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE DISABLED)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

    # Find vulkan
    find_package(Vulkan REQUIRED COMPONENTS MoltenVK)

    # Find mono libraries
    #set(MONO_INCLUDE_DIR_PATH "/opt/homebrew/Cellar/mono/6.14.0/include/mono-2.0")
    #set(MONO_LIBRARY_DIR_PATH "/Library/Frameworks/Xamarin.iOS.framework/Versions/Current/bin")
    #set(MONO_LIBRARY_ETC_PATH "/opt/homebrew/Cellar/mono/6.14.0/etc")
    #set(MONO_LIBRARY_CONFIG "/opt/homebrew/Cellar/mono/6.14.0/etc/mono/config")
    # Locate the Mono headers
    #find_path(MONO_INCLUDE_DIR
    #        NAMES mono/jit/jit.h
    #        PATHS ${MONO_INCLUDE_DIR_PATH}
    #)
    # Locate the Mono library
    #find_library(MONO_LIBRARY
    #        NAMES arm64-darwin-mono-sgen
    #        PATHS "/Library/Frameworks/Xamarin.iOS.framework/Versions/Current/bin"
    #)

    # Libraries to link with
    set(ENTROPY_LINK_LIBS
            Vulkan::MoltenVK
            flecs::flecs_static
            GPUOpen::VulkanMemoryAllocator
            glm::glm
            quill
    )
    if(${CMAKE_OSX_SYSROOT} MATCHES "iphonesimulator")
        set(ENTROPY_PLATFORM IOS_SIMULATOR)
        if (${CMAKE_OSX_ARCHITECTURES} MATCHES "arm64")
            message("-- iOS Build for simulator arm64")
            set(ENTROPY_ARCH ARM64)
        elseif(${CMAKE_OSX_ARCHITECTURES} MATCHES "x86_64")
            message("-- iOS Build for simulator x86_64")
            set(ENTROPY_ARCH X86_64)
        endif()
    elseif(${CMAKE_OSX_SYSROOT} MATCHES "iphoneos")
        message("-- iOS Build for iphone")
        set(ENTROPY_PLATFORM IOS)
    else()
        #message(FATAL_ERROR"-- iOS undefined build")
    endif()
elseif(APPLE AND NOT IOS)
    message("BUILDING FOR MACOS!!")
    find_package(Vulkan REQUIRED)
    FetchContent_Declare(glfw3
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG master
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw3)
    set(MONO_INCLUDE_DIR_PATH "/opt/homebrew/Cellar/mono/6.14.0/include/mono-2.0")
    set(MONO_LIBRARY_DIR_PATH "/opt/homebrew/Cellar/mono/6.14.0/lib")
    set(MONO_LIBRARY_ETC_PATH "/opt/homebrew/Cellar/mono/6.14.0/etc")
    set(MONO_LIBRARY_CONFIG "/opt/homebrew/Cellar/mono/6.14.0/etc/mono/config")
    set(ENTROPY_PLATFORM MACOS)
    # Locate the Mono headers
    find_path(MONO_INCLUDE_DIR
            NAMES mono/jit/jit.h
            PATHS ${MONO_INCLUDE_DIR_PATH}
    )

    # Locate the Mono library
    find_library(MONO_LIBRARY
            NAMES mono-2.0
            PATHS ${MONO_LIBRARY_DIR_PATH}
    )
    set(ENTROPY_LINK_LIBS
            ${Vulkan_LIBRARIES}
            ${MONO_LIBRARY}
            flecs::flecs_static
            GPUOpen::VulkanMemoryAllocator
            glfw
            glm::glm
            quill
    )
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        message("-- MacOS Build for x86_64")
        set(ENTROPY_ARCH X86_64)
    elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
        message("-- MacOS Build for arm64")
        set(ENTROPY_ARCH ARM64)
    else()
        message(FATAL_ERROR "-- MacOS undefined build")
    endif()
elseif(UNIX AND NOT APPLE)
    message("-- Linux Build")
    find_package(Vulkan REQUIRED)
    FetchContent_Declare(glfw3
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG master
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw3)
    set(MONO_INCLUDE_DIR_PATH "/usr/include/mono-2.0")
    set(MONO_LIBRARY_DIR_PATH "/usr/lib")
    set(MONO_LIBRARY_ETC_PATH "/etc/mono")
    set(MONO_LIBRARY_CONFIG "/etc/mono/config")
    set(ENTROPY_PLATFORM LINUX)
    find_path(MONO_INCLUDE_DIR
            NAMES mono/jit/jit.h
            PATHS ${MONO_INCLUDE_DIR_PATH}
    )
    # Locate the Mono library
    find_library(MONO_LIBRARY
            NAMES mono-2.0
            PATHS ${MONO_LIBRARY_DIR_PATH}
    )
    set(ENTROPY_LINK_LIBS
            ${Vulkan_LIBRARIES}
            ${MONO_LIBRARY}
            flecs::flecs_static
            GPUOpen::VulkanMemoryAllocator
            glfw
            glm::glm
            quill
    )
elseif(ANDROID)
    if(${ANDROID_ABI} STREQUAL "x86_64")
        message("-- Android Build for x86_64")
    elseif(${ANDROID_ABI} STREQUAL "x86")
        message("-- Android Build for x86")
    elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
        message("-- Android Build for armeabi-v7a")
    elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
        message("-- Android Build for arm64-v8a")
    else()
        message(FATAL_ERROR "-- Android undefined build")
    endif()
else()
    message(FATAL_ERROR "-- Undefined build")
endif()

FetchContent_Declare(quill
    GIT_REPOSITORY https://github.com/odygrd/quill.git
    GIT_TAG master
)
FetchContent_MakeAvailable(quill)

FetchContent_Declare(flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG master
)
set(FLECS_SHARED OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(flecs)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(vma
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG master
)
set(VMA_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(VMA_BUILD_DOCUMENTATION OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(vma)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(CS_SOURCES_ENTROPY
    ${PROJECT_SOURCE_DIR}/scripting/entropy/component.cs
    ${PROJECT_SOURCE_DIR}/scripting/entropy/entity.cs
    ${PROJECT_SOURCE_DIR}/scripting/entropy/entropy_script.cs
)
file(GLOB CS_SOURCES ${PROJECT_SOURCE_DIR}/scripting/*.cs)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_LIST_DIR)

else()
    set(ENTROPY_INCLUDE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/include/metal-cpp
            PARENT_SCOPE
    )
endif()

# Path to C# source file
set(CS_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/GameScripts.dll)
set(CS_AOT_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/GameScripts_AOT.dll)

find_program(CSC_EXECUTABLE NAMES csc mcs REQUIRED)

# Custom command to build the DLL
add_custom_command(
        OUTPUT ${CS_OUTPUT}
        COMMAND ${CSC_EXECUTABLE} /target:library /out:${CS_OUTPUT} ${CS_SOURCES_ENTROPY} ${CS_SOURCES}
        DEPENDS ${CS_SOURCES_ENTROPY} ${CS_SOURCES}
        COMMENT "Compiling GameScripts.dll"
)

# Custom command to build the DLL
add_custom_command(
        OUTPUT ${CS_AOT_OUTPUT}
        COMMAND mono --aot=full ${CS_OUTPUT}
        DEPENDS ${CS_OUTPUT}
        COMMENT "Compiling GameScripts_AOT.dll"
)

# Define a custom target for C# scripts
if(NOT IOS)
    add_custom_target(GameScripts ALL
            DEPENDS ${CS_OUTPUT}
    )

    add_custom_target(GameScriptsAOT ALL
            DEPENDS ${CS_AOT_OUTPUT}
    )
endif()

if(BUILD_ENTROPY_AS_EXECUTABLE)
    add_executable(${PROJECT_NAME} ${SOURCES} ${PROJECT_SOURCE_DIR}/test/main.cpp)
else()
    add_library(${PROJECT_NAME} ${ENTROPY_LIBRARY_TYPE} ${SOURCES})
endif()

# Make your main target (Entropy) depend on GameScripts
if(NOT IOS)
    add_dependencies(${PROJECT_NAME} GameScripts)
    add_dependencies(${PROJECT_NAME} GameScriptsAOT)
endif()

target_include_directories(${PROJECT_NAME} INTERFACE
        ${vma_SOURCE_DIR}/include
        ${quill_SOURCE_DIR}/include
        ${glfw3_SOURCE_DIR}/include
        ${flecs_SOURCE_DIR}/include
        ${glm_SOURCE_DIR}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${MONO_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${Vulkan_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${ENTROPY_LINK_LIBS}
)

target_compile_options(${PROJECT_NAME} PRIVATE
        -Wno-undef
        -Wno-unused-parameter
        -Wno-nested-anon-types
        -Wno-nullability-completeness
        -Wno-sign-conversion
        -Wno-unused-variable
        -Wno-missing-field-initializers
        -Wno-nullability-extension
        -Wdeprecated-declarations
        -Wno-deprecated-declarations
)

file(COPY shaders/compiled/2d_shader/2d_shader_vert.spv DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shaders/compiled/2d_shader/2d_shader_frag.spv DESTINATION ${CMAKE_BINARY_DIR})

# For testing only..
file(COPY test/test.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY test/test2.png DESTINATION ${CMAKE_BINARY_DIR})

# Build config file
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)
