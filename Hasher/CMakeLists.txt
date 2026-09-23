# hasher
add_library(hasher hasher.cpp)

target_include_directories(hasher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hasher PUBLIC OpenSSL::Crypto)

#hasher_main
add_executable(hasher_main hasher_main.cpp)

target_link_libraries(hasher_main PRIVATE hasher)

# gtest
enable_testing()

add_executable(hasher_tests hasher_gtest.cpp)

target_link_libraries(hasher_tests PRIVATE
  hasher
  GTest::gtest_main
  OpenSSL::Crypto
)

include(GoogleTest)
gtest_discover_tests(hasher_tests)