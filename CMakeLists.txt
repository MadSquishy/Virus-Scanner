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
FetchContent_MakeAvailable(googletest)

#libmagic
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMAGIC REQUIRED libmagic)

#openssl
set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl@3)

find_package(OpenSSL REQUIRED)
add_subdirectory(Parser)
add_subdirectory(Hasher)