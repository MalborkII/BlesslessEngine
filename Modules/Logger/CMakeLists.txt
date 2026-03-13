cmake_minimum_required(VERSION 3.16)
project(LoggerModule LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Кладём бинарники модуля в локальную папку Binaries внутри модуля
set(MODULE_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Binaries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MODULE_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${MODULE_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MODULE_BIN_DIR})

add_library(LoggerModule SHARED
  Engine/LoggerModule.cpp
)
target_include_directories(LoggerModule PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/Engine
)
set_target_properties(LoggerModule PROPERTIES OUTPUT_NAME "BE-Logger")

