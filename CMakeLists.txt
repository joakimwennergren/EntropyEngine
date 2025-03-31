# ______ _   _ _______ _____   ____  _______     __
# |  ____| \ | |__   __|  __ \ / __ \|  __ \ \   / /
# | |__  |  \| |  | |  | |__) | |  | | |__) \ \_/ /
# |  __| | . ` |  | |  |  _  /| |  | |  ___/ \   /
# | |____| |\  |  | |  | | \ \| |__| | |      | |
# |______|_| \_|  |_|  |_|  \_\\____/|_|      |_|  v 1.0

# Set minimum cmake version
cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(ExternalProject)
include(FetchContent)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Declare project
project(Entropy VERSION 1.0.0 LANGUAGES CXX)

# Detect platform
if (APPLE)
    set(PLATFORM MACOS)
    find_package(Vulkan REQUIRED)
endif ()

if (IOS)
    set(PLATFORM IOS)
    set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
    set(CMAKE_XCODE_SCHEME_ENABLE_GPU_API_VALIDATION FALSE)
    set(CMAKE_XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE DISABLED)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

    set(CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
    find_library(UIKIT_LIB UiKit)
    find_library(COREMEDIA_LIB CoreMedia)
    find_library(METAL_LIB Metal)

    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(Vulkan REQUIRED COMPONENTS MoltenVK)
endif ()

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

FetchContent_Declare(glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG master
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw3)

# Glob source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Create the library
add_executable(${PROJECT_NAME}
        ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${Vulkan_INCLUDE_DIRS}

        ${CMAKE_BINARY_DIR}/glm
        ${CMAKE_BINARY_DIR}/quill/include
        ${CMAKE_BINARY_DIR}/flecs/include
        ${CMAKE_BINARY_DIR}/vma/include
        ${CMAKE_BINARY_DIR}/glfw3/include

        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

if (PLATFORM STREQUAL "MACOS")
    target_link_libraries(${PROJECT_NAME} PRIVATE
            ${Vulkan_LIBRARIES}
            flecs::flecs_static
            glfw
    )
endif ()

if (PLATFORM STREQUAL "IOS")
    target_link_libraries(${PROJECT_NAME} PRIVATE
            Vulkan::MoltenVK
            flecs::flecs_static
    )
endif ()

file(COPY ${CMAKE_SOURCE_DIR}/shaders/compiled/2d_shader/2d_shader_vert.spv DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/shaders/compiled/2d_shader/2d_shader_frag.spv DESTINATION ${CMAKE_BINARY_DIR})

# For testing only..
file(COPY ${CMAKE_SOURCE_DIR}/test/blank_.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/test/test.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/test/test2.png DESTINATION ${CMAKE_BINARY_DIR})

# Build config file
configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/config.h @ONLY)
