#ifndef HASHER_HPP
#define HASHER_HPP
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <filesystem>
#include <fstream>

#include <openssl/evp.h>
std::string hash(const std::filesystem::path& path, EVP_MD_CTX * ctx);

#endif