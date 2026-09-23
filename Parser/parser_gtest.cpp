#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>
#include <string>
#include <iterator>
#include <magic.h>

#include "parser.hpp"

namespace fs = std::filesystem;



TEST(ParserTest, Parser) {
    // fs::temp_directory_path() gives a /tmp path returns path object
    // fs::create_directories(path) creates a directory and all missing parent directories
    // std::ofstream(path)
    // /, /= work as + and += for files/paths and strings
    // use std::fstream(path, std::ios::binary) to create temporary files
    // 0 use std::ios::binary as the second param since its a flag that makes it so that no implicit things are added when writing bytes
    // 0 i.e windows will no longer add the \r character implicitly
    // .write(x, y) takes x as a string of bytes i.e "\x7f\x45\x4c\x46" and y is the number of bytes, in this case 4.
    // 0 directly writes bytes to file
    // fs::remove_all(path) deletes a directory and everything in it
    // fs::remove(path) deletes one file given path doesnt work if directory has stuff in it.


    fs::path tempDir = fs::temp_directory_path() / "AV_PARSER_TEST";
    fs::create_directories(tempDir);
    std::ofstream output {tempDir / "output", std::ios::binary};
  

    std::ofstream whitelisted_one {tempDir / "wl_one.exe", std::ios::binary}; // mp3
    whitelisted_one.write("\xff\xfb\x90\x00", 4);
    whitelisted_one.close();


    fs::path deepDir = tempDir / "DEEPER_TEST";
    fs::create_directories(deepDir);

    std::ofstream whitelisted_two {deepDir / "wl_two", std::ios::binary}; // mp4
    whitelisted_two.write("\x00\x00\x00\x20\x66\x74\x79\x70\x6d\x70\x34\x32", 12);
    whitelisted_two.close();

    std::ofstream whitelisted_three {tempDir / "wl_three", std::ios::binary}; // ttf
    whitelisted_three.write("ttcf\x00\x02\x00\x00", 8);
    whitelisted_three.close();


    std::ofstream whitelisted_four {tempDir / "wl_four", std::ios::binary}; // gltf
    whitelisted_four.write("glTF\x02\x00\x00\x00\x0c\xdd\x06\x00", 12);
    whitelisted_four.close();


    std::ofstream whitelisted_exec {tempDir / "wl_exec", std::ios::binary};
    whitelisted_exec.write("\xff\xfb\x90\x00", 4);
    whitelisted_exec.close();
    fs::permissions(tempDir / "wl_exec", fs::perms::owner_exec, fs::perm_options::add);


    std::ofstream bad_mac {tempDir / "bad_mac", std::ios::binary};
    bad_mac.write("\xfe\xed\xfa\xcf", 4); 
    bad_mac.close();

    std::ofstream bad_win {tempDir / "bad_win", std::ios::binary};
    bad_win.write("MZ", 2);
    bad_win.close();


    // run parse
    parse(output, tempDir);
    output.close();

    std::ifstream result {tempDir/"output"};
    std::string content((std::istreambuf_iterator<char>(result)), std::istreambuf_iterator<char>());

    EXPECT_FALSE(content.find("wl_one") != std::string::npos);
    EXPECT_FALSE(content.find("wl_two") != std::string::npos);
    EXPECT_FALSE(content.find("wl_three") != std::string::npos);
    EXPECT_FALSE(content.find("wl_four") != std::string::npos);

    EXPECT_FALSE(content.find("wl_exec") == std::string::npos);
    EXPECT_FALSE(content.find("bad_mac") == std::string::npos);
    EXPECT_FALSE(content.find("bad_win") == std::string::npos);

    fs::remove_all(tempDir);
   
}


TEST(ParserTest, HasExecutable) {
    fs::path tempDir = fs::temp_directory_path() / "AV_PARSER_EXEC_TEST";
    fs::create_directories(tempDir);
    std::ofstream whitelisted_exec {tempDir / "wl_exec", std::ios::binary};
    whitelisted_exec.write("\xff\xfb\x90\x00", 4);
    whitelisted_exec.close();
    fs::permissions(tempDir / "wl_exec", fs::perms::owner_exec, fs::perm_options::add);

    std::ofstream whitelisted {tempDir / "wl", std::ios::binary}; // ttf
    whitelisted.write("ttcf\x00\x02\x00\x00", 8);
    whitelisted.close(); 

    EXPECT_TRUE(has_executable(tempDir / "wl_exec"));
    EXPECT_FALSE(has_executable(tempDir / "wl"));

    fs::remove_all(tempDir);
}

TEST(ParserTest, IsScanTarget) {
     fs::path tempDir = fs::temp_directory_path() / "AV_PARSER_TEST";
    fs::create_directories(tempDir);
    std::ofstream output {tempDir / "output", std::ios::binary};
  

    std::ofstream whitelisted_one {tempDir / "wl_one.exe", std::ios::binary}; // mp3
    whitelisted_one.write("\xff\xfb\x90\x00", 4);
    whitelisted_one.close();



    std::ofstream whitelisted_two {tempDir / "wl_two", std::ios::binary}; // mp4
    whitelisted_two.write("\x00\x00\x00\x20\x66\x74\x79\x70\x6d\x70\x34\x32", 12);
    whitelisted_two.close();

    std::ofstream whitelisted_three {tempDir / "wl_three", std::ios::binary}; // ttf
    whitelisted_three.write("ttcf\x00\x02\x00\x00", 8);
    whitelisted_three.close();


    std::ofstream whitelisted_four {tempDir / "wl_four", std::ios::binary}; // gltf
    whitelisted_four.write("glTF\x02\x00\x00\x00\x0c\xdd\x06\x00", 12);
    whitelisted_four.close();


    std::ofstream whitelisted_exec {tempDir / "wl_exec", std::ios::binary};
    whitelisted_exec.write("\xff\xfb\x90\x00", 4);
    whitelisted_exec.close();
    fs::permissions(tempDir / "wl_exec", fs::perms::owner_exec, fs::perm_options::add);


    std::ofstream bad_mac {tempDir / "bad_mac", std::ios::binary};
    bad_mac.write("\xfe\xed\xfa\xcf", 4); 
    bad_mac.close();

    std::ofstream bad_win {tempDir / "bad_win", std::ios::binary};
    bad_win.write("MZ", 2);
    bad_win.close();

    magic_t cookie = magic_open(MAGIC_MIME_TYPE);
    magic_load(cookie, nullptr);
    EXPECT_FALSE(is_scan_target(tempDir / "wl_one.exe", cookie));
    EXPECT_FALSE(is_scan_target(tempDir / "wl_two", cookie));
    EXPECT_FALSE(is_scan_target(tempDir / "wl_three", cookie)); 
    EXPECT_FALSE(is_scan_target(tempDir / "wl_four", cookie));

    EXPECT_TRUE(is_scan_target(tempDir / "wl_exec", cookie));
    EXPECT_TRUE(is_scan_target(tempDir / "bad_mac", cookie));
    EXPECT_TRUE(is_scan_target(tempDir / "bad_win", cookie));

    magic_close(cookie);


}
