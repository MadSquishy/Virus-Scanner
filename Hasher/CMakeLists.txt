# hasher
add_library(hasher hasher.cpp)

target_include_directories(hasher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hasher PUBLIC OpenSSL::Crypto)

#hasher_main
add_executable(hasher_manual_test hasher_manual_test.cpp)

target_link_libraries(hasher_manual_test PRIVATE hasher)

# gtest
add_executable(hasher_tests hasher_gtest.cpp)

target_link_libraries(hasher_tests PRIVATE
  hasher
  GTest::gtest_main
  OpenSSL::Crypto
)

include(GoogleTest)
gtest_discover_tests(hasher_tests)