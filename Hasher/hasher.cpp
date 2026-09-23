#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <filesystem>
#include <fstream>

#include "hasher.hpp"

//#include <openssl/evp.h>

namespace fs = std::filesystem;

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