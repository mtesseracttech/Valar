cmake_minimum_required(VERSION 3.9)
project(Valcano)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_FILES
        src/ValcanoApplication.hpp
        src/ValcanoApplication.cpp
        src/gfx/window/ContextWindow.hpp
        src/gfx/window/ContextWindow.cpp
        src/aux/CallbackManager.hpp
        src/aux/CallbackManager.cpp
        src/gfx/vulkan/Instance.hpp
        src/gfx/vulkan/Instance.cpp
        src/aux/exceptions/NotImplemented.hpp
        src/aux/logging/Logger.hpp
        src/aux/logging/Logger.cpp
        src/aux/logging/LogUtils.hpp
        src/aux/time/Timer.hpp
        src/aux/time/TimeUtils.hpp src/gfx/Renderer.cpp src/gfx/Renderer.hpp)

add_executable(Valcano ${SOURCE_FILES})

#Sets a base path for the includes so that they can be used in the same way as in Visual Studio/Codeblocks
set(BASEPATH "${CMAKE_SOURCE_DIR}/src")
include_directories("${BASEPATH}")

#Setting up PkgConfig
find_package(PkgConfig REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#Finding and linking GLFW3
find_package(GLFW REQUIRED)
if(GLFW_FOUND)
    message(STATUS "Found GLFW 3.3, Including and Linking now")
    include_directories(${GLFW_INCLUDE_DIR})
    target_link_libraries(Valcano ${GLFW_LIBRARIES})
endif(GLFW_FOUND)

#Finding and linking Vulkan
find_package (Vulkan)
if (Vulkan_FOUND)
    message(STATUS "Found Vulkan, Including and Linking now")
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries (Valcano ${Vulkan_LIBRARIES})
    message(STATUS "Vulkan Include dirs: ${Vulkan_INCLUDE_DIRS} and libs: ${Vulkan_LIBRARIES}")
endif (Vulkan_FOUND)

#Finding and linking ASSIMP
find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
    message(STATUS "Found ASSIMP, Including and Linking now")
    include_directories(${ASSIMP_INCLUDE_DIR})
    target_link_libraries (Valcano ${ASSIMP_LIBRARY})
endif (ASSIMP_FOUND)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()