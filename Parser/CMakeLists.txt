# parser 
add_library(parser parser.cpp)

target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${LIBMAGIC_INCLUDE_DIRS})

target_link_directories(parser PUBLIC ${LIBMAGIC_LIBRARY_DIRS})
target_link_libraries(parser PUBLIC ${LIBMAGIC_LIBRARIES})

#parser_main
add_executable(parser_main parser_main.cpp)
target_link_libraries(parser_main PRIVATE parser)

# gtest
enable_testing()

add_executable(parser_tests parser_gtest.cpp)

target_include_directories(parser_tests PRIVATE ${LIBMAGIC_INCLUDE_DIRS})

target_link_directories(parser_tests PRIVATE ${LIBMAGIC_LIBRARY_DIRS})
target_link_libraries(parser_tests PRIVATE
  parser
  GTest::gtest_main
  ${LIBMAGIC_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(parser_tests)