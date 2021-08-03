cmake_minimum_required(VERSION 3.9)

project(speech LANGUAGES CXX)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CC_FLAGS "-Wall -Wextra")
project(speech VERSION 1.0 DESCRIPTION "speech library")
add_library(speech SHARED
  src/parameterio.cpp
  src/audioio.cpp
  src/speech.cpp
  include/speech.hpp
  include/jsonString.hpp
  include/audioio.h
  include/parameterio.h
  )

target_include_directories(speech PUBLIC "include")
target_link_directories(speech PUBLIC "lib")
target_link_libraries(speech world world_tool)

target_sources(speech PRIVATE
  lib/libworld.a
  lib/libworld.dll
  lib/libworld_tool.a
  outDll/libc++.dll
  outDll/libspeech.dll
  outDll/libspeech.so
  outDll/libunwind.dll
 )
