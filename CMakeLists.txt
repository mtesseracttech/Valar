cmake_minimum_required(VERSION 3.9)
project(Valcano)

#Setting C++17 as the standard and disabling platform/compiler specific extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
endif()

#Sets a base path for the includes so that they can be used in the same way as in Visual Studio/Codeblocks
set(BASEPATH "${CMAKE_SOURCE_DIR}/src")
include_directories("${BASEPATH}")

#Adding GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(libs/glfw)

#Adding ASSIMP
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(libs/assimp)

#Adding SPIRV-Cross
add_subdirectory(libs/spirv-cross)

#Adding shaderc
option(SHADERC_ENABLE_TESTS OFF)
option(EFFCEE_BUILD_TESTING OFF)
option(BUILD_TESTING OFF)
option(BUILD_GMOCK OFF)
option(ENABLE_CTEST OFF)
option(EEFCEE_BUILD_SAMPLES OFF)
option(EEFCEE_BUILD_TESTING OFF)
option(INSTALL_GTEST OFF)

add_subdirectory(libs/shaderc)


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
endif()


include_directories(src/
                    libs/assimp/include/
                    libs/glfw/include/
                    libs/shaderc/libshaderc/include/
                    libs/shaderc/libshaderc_spvc/include/
                    libs/spirv-cross/)

file(GLOB_RECURSE PROJECT_HEADERS src/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)
file(GLOB_RECURSE PROJECT_SHADERS
        res/shaders/*.comp
        res/shaders/*.frag
        res/shaders/*.geom
        res/shaders/*.vert
        res/shaders/*.tese
        res/shaders/*.tesc)

file(GLOB PROJECT_CONFIGS CMakeLists.txt
        README.md
        .gitattributes
        .gitignore
        .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Shaders" FILES ${PROJECT_SHADERS})


add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
               ${PROJECT_SHADERS} ${PROJECT_CONFIGS})

target_link_libraries(${PROJECT_NAME} assimp glfw shaderc ${GLFW_LIBRARIES})

#Finding and linking Vulkan
find_package (Vulkan)
if (Vulkan_FOUND)
    message(STATUS "Found Vulkan, Including and Linking now")
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${Vulkan_LIBRARIES})
    message(STATUS "Vulkan Include dirs: ${Vulkan_INCLUDE_DIRS} and libs: ${Vulkan_LIBRARIES}")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>
        DEPENDS ${PROJECT_SHADERS})