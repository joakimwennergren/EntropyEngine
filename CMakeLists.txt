cmake_minimum_required(VERSION 3.28.3)

include(FetchContent)
project(Entropy VERSION 1.0.0 LANGUAGES CXX)

set(ENTROPY_VERSION_MAJOR 1)
set(ENTROPY_VERSION_MINOR 0)
set(ENTROPY_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENTROPY_TESTS "Build Entropy Tests" ON)

macro(fetch_dependencies platform)
    if(${platform} STREQUAL "MACOS" OR ${platform} STREQUAL "LINUX" OR ${platform} STREQUAL "WINDOWS")
        # Find Vulkan
        find_package(Vulkan REQUIRED)

        # GLFW3
        FetchContent_Declare(glfw3
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG master
        )
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(glfw3)
    endif()

    # Quill
    FetchContent_Declare(quill
        GIT_REPOSITORY https://github.com/odygrd/quill.git
        GIT_TAG master
    )

    # Flecs
    FetchContent_MakeAvailable(quill)
    FetchContent_Declare(flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
        GIT_TAG master
    )
    set(FLECS_SHARED OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(flecs)

    # GLM
    FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(glm)

    # Vulkan Memory Allocator
    FetchContent_Declare(vma
        GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
        GIT_TAG master
    )
    set(VMA_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
    set(VMA_BUILD_DOCUMENTATION OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(vma)
endmacro()

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# IOS
if(IOS)
    find_package(Vulkan REQUIRED COMPONENTS MoltenVK)

    set(ENTROPY_PLATFORM IOS)
    fetch_dependencies("IOS")

    add_library(${PROJECT_NAME} SHARED ${SOURCES})

    target_link_libraries(${PROJECT_NAME} PUBLIC
        "-framework foundation"
        "-framework metal"
        "-framework metalkit"
        "-framework uikit"
        "-framework iosurface"
        "-framework coregraphics"
        "-framework quartzcore"
        "-framework coreaudio"
        "-framework audiotoolbox"
        "-framework avfoundation"
        "-framework coretext"
    )

    # Include directories
    target_include_directories(${PROJECT_NAME} INTERFACE
        ${vma_SOURCE_DIR}/include
        ${quill_SOURCE_DIR}/include
        ${glfw3_SOURCE_DIR}/include
        ${flecs_SOURCE_DIR}/include
        ${glm_SOURCE_DIR}
    )

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${Vulkan_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        Vulkan::MoltenVK flecs::flecs_static GPUOpen::VulkanMemoryAllocator glm::glm quill
    )
    # MacOS
elseif(APPLE AND NOT IOS)
    set(ENTROPY_PLATFORM MACOS)
    fetch_dependencies("MACOS")
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${Vulkan_LIBRARIES} flecs::flecs_static GPUOpen::VulkanMemoryAllocator glfw glm::glm quill
    )
    # Linux
elseif(UNIX AND NOT APPLE)
    set(ENTROPY_PLATFORM LINUX)
    fetch_dependencies("LINUX")
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${Vulkan_LIBRARIES} flecs::flecs_static GPUOpen::VulkanMemoryAllocator glfw glm::glm quill
    )
    # Windows
elseif(WIN32)
    set(ENTROPY_PLATFORM WINDOWS)
    fetch_dependencies("WINDOWS")
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${Vulkan_LIBRARIES} flecs::flecs_static GPUOpen::VulkanMemoryAllocator glfw glm::glm quill
    )
    # Android
elseif(ANDROID)
    # @TODO: Add Android support
    message(FATAL_ERROR "-- Android build not supported yet")
else()
    message(FATAL_ERROR "-- Platform not supported")
endif()

if(ENTROPY_TESTS)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/unit/*.cpp)

    add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

    target_link_libraries(${PROJECT_NAME}_test PRIVATE
        GTest::gtest_main
        ${PROJECT_NAME}
    )

    include(GoogleTest)
    gtest_add_tests(TARGET ${PROJECT_NAME}_test AUTO)
endif()

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

target_include_directories(${PROJECT_NAME} INTERFACE
    ${vma_SOURCE_DIR}/include
    ${quill_SOURCE_DIR}/include
    ${glfw3_SOURCE_DIR}/include
    ${flecs_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

file(CREATE_LINK
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/compiled/2dshader/2dshader_vert.spv"
    "${CMAKE_BINARY_DIR}/2dshader_vert.spv"
    SYMBOLIC
)

file(CREATE_LINK
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/compiled/2dshader/2dshader_frag.spv"
    "${CMAKE_BINARY_DIR}/2dshader_frag.spv"
    SYMBOLIC
)

# Build config file
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h @ONLY)
