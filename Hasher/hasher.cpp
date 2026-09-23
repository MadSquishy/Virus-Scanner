#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <filesystem>
#include <fstream>

#include "hasher.hpp"

//#include <openssl/evp.h>

namespace fs = std::filesystem;
/*
EVP_MD_CTX * ctx = EVP_MD_CTX_new();          // makes a new context, take this is a param
EVP_MD_CTX_free(ctx);                         // deletes from heap, do so outside of function call
EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) // resets the context

load file into buffer and build hash bit by bit
file.gcount() tracks how much from the file object was read in. (make sure file is a ifstream with std::ios::binary flag)
std::vector<char> buffer(8192);
    while (file.read(buffer.data(), buffer.size()) || file.gcount()) {
        EVP_DigestUpdate(ctx, buffer.data(), file.gcount());
    }



EVP_DigestFinal_ex(ctx, hash, &hashlen)
- takes a hash array (which is expected to be of type unsigned char)
- takes a hashlen var (which is expected to be of type unsigned int)


pulled from stackoverflow
stringstream ss;
    for(int i = 0; i < hash_length; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();

*/

std::string hash(const std::filesystem::path& path, EVP_MD_CTX * ctx) {
    std::ifstream file {path};
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);

    std::vector<char> buffer(8192);
    while (file.read(buffer.data(), buffer.size()) || file.gcount()) {
        EVP_DigestUpdate(ctx, buffer.data(), file.gcount());
    } 

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;
    EVP_DigestFinal_ex(ctx, hash, &hash_len);

    std::stringstream ss;
    for(int i = 0; i < hash_len; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}