#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>
#include <string>
#include <iterator>

#include "hasher.hpp"
#include <openssl/evp.h>

namespace fs = std::filesystem;


TEST(HasherTest, EmptyFileHash) {
    EVP_MD_CTX * ctx = EVP_MD_CTX_new();

    fs::path temp = fs::temp_directory_path() / "AV_HASHER_TEST";
    fs::create_directories(temp);

    { std::ofstream file {temp/"empty.tmp"}; }

    std::string expected {"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"}; 
    EXPECT_EQ(hash(temp/"empty.tmp", ctx), expected);
    EVP_MD_CTX_free(ctx);
    fs::remove_all(temp);
}

TEST(HasherTest, HelloNHash) {
    EVP_MD_CTX * ctx = EVP_MD_CTX_new();

    fs::path temp = fs::temp_directory_path() / "AV_HASHER_TEST";
    fs::create_directories(temp);
    
    std::ofstream file {temp/"hello.txt"};
    file << "hello\n";
    file.close();

    std::string expected {"5891b5b522d5df086d0ff0b110fbd9d21bb4fc7163af34d08286a2e846f6be03"};
    EXPECT_EQ(hash(temp/"hello.txt", ctx), expected);
    EVP_MD_CTX_free(ctx);
    fs::remove_all(temp);
}
