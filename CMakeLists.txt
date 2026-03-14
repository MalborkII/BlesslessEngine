cmake_minimum_required(VERSION 3.16)
project(BlesslessEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Все бинарники (ядро + редактор) кладём в Binaries/
set(BINARIES_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Binaries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARIES_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARIES_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARIES_OUTPUT_DIR})

# Статическая библиотека ядра движка (EngineCore)
add_library(EngineCore STATIC
  Engine/Source/Engine/Core/Private/Engine.cpp
  Engine/Source/Engine/ModuleManager/Private/ModuleManager.cpp
  Engine/Source/Engine/Scene/Private/Scene.cpp
  Engine/Source/Engine/Scene/Private/GameObject.cpp
  Engine/Source/Engine/Scene/Private/Transform.cpp
  
)
target_include_directories(EngineCore PUBLIC
  ${CMAKE_SOURCE_DIR}/Engine/Source/Engine/Core/Public
  ${CMAKE_SOURCE_DIR}/Engine/Source/Engine/ModuleManager/Public
  ${CMAKE_SOURCE_DIR}/Engine/Source/Engine/Scene/Public
  ${CMAKE_SOURCE_DIR}/Engine/Source/Engine/Renderer
  ${CMAKE_SOURCE_DIR}/Engine/Source/EngineBridge
)

# BlesslessEngine.exe — exe Python-редактора и точка входа движка
add_executable(BlesslessEngine WIN32
  Editor/Source/BlesslessEngine.cpp
)
target_compile_definitions(BlesslessEngine PRIVATE _UNICODE UNICODE)
target_link_libraries(BlesslessEngine PRIVATE EngineCore Shell32)
