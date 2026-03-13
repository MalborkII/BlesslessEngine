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
  Engine/Source/EngineCore/EngineLoop.cpp
  Engine/Source/EngineCore/Scene.cpp
  Engine/Source/EngineCore/GameObject.cpp
  Engine/Source/EngineCore/Transform.cpp
  Engine/Source/EngineCore/ModuleManager.cpp
)
target_include_directories(EngineCore PUBLIC
  ${CMAKE_SOURCE_DIR}/Engine/Source/EngineCore
  ${CMAKE_SOURCE_DIR}/Engine/Source/ManagedBridge
  ${CMAKE_SOURCE_DIR}/Engine/Source/EngineEditorBridge
)

# BlesslessEngine.exe — exe Python-редактора и точка входа движка
add_executable(BlesslessEngine WIN32
  Editor/BlesslessEngine.cpp
)
target_compile_definitions(BlesslessEngine PRIVATE _UNICODE UNICODE)
target_link_libraries(BlesslessEngine PRIVATE EngineCore Shell32)
