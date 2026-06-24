cmake_minimum_required(VERSION 3.20)
project(AV)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

#gtest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
message(STATUS "GTest source dir: ${googletest_SOURCE_DIR}")

#libgmagic
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMAGIC REQUIRED libmagic)

add_subdirectory(Parser)