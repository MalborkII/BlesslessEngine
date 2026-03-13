cmake_minimum_required(VERSION 3.16)
project(BlesslessEngineLaunchers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Оба exe в одну папку: BEVersionSelector находит BlesslessEngine.exe рядом
set(BINARIES_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Binaries/Win64")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARIES_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARIES_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARIES_OUTPUT_DIR})

# BEVersionSelector.exe — двойной клик по project.bless → запуск BlesslessEngine.exe
add_executable(BEVersionSelector WIN32
  Engine/Source/Programs/BEVersionSelector/BEVersionSelector.cpp
)
target_compile_definitions(BEVersionSelector PRIVATE _UNICODE UNICODE)
target_link_libraries(BEVersionSelector PRIVATE Shell32)

# BlesslessEngine.exe — лаунчер Python-редактора
add_executable(BlesslessEngine WIN32
  Engine/Source/Programs/BlesslessEngine/BlesslessEngine.cpp
)
target_compile_definitions(BlesslessEngine PRIVATE _UNICODE UNICODE)
target_link_libraries(BlesslessEngine PRIVATE Shell32)
