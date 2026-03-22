cmake_minimum_required(VERSION 3.20)

project(SecureBuild VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_TESTS "Build the SecureBuild test suite" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

add_subdirectory(cpp)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
