# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Set engine include path.

include_directories(../../../ThirdParty)
include_directories(../../../ThirdParty/event)
include_directories(../../../ThirdParty/gl3w)
include_directories(../../../ThirdParty/lodepng)
include_directories(../../../ThirdParty/rbtree)
include_directories(../../../ThirdParty/spirv-cross)
include_directories(../../../ThirdParty/tinyxml)
include_directories(../../../ThirdParty/zlib)
include_directories(../../../Engine)
include_directories(../../../Engine/Base/include)
include_directories(../../../Engine/Memory/include)
include_directories(../../../Engine/Renderer/Gfx/include)
include_directories(../../../Engine/Renderer/GLES3/include)
include_directories(../../../Engine/Renderer/Vulkan/include)
include_directories(../../../Engine/Renderer/Compiler/include)
include_directories(../../../Engine/Scene/include)
include_directories(../../../Engine/Engine/include)
include_directories(../../../Engine/ThirdParty)
include_directories(../../../Engine/ThirdParty/eastl)
include_directories(../../../Engine/ThirdParty/gli)
include_directories(../../../Engine/ThirdParty/glm)

# Set definitions.

add_definitions(-DHAVE_EASTL)
add_definitions(-DGLES_VER=310)
add_definitions(-DTHREAD_COUNT=4)
add_definitions(-DMEMORY_ALLOCATOR)
add_definitions(-DPLATFORM_ANDROID)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(
        libEngine
        SHARED
        IMPORTED)

add_library(
        native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        src/main/cpp/MemTest.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(
        log-lib
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

set_target_properties(
        libEngine
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libEngine.so)

target_link_libraries(
        native-lib
        ${log-lib}
        libEngine)