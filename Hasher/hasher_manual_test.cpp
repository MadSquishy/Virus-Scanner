#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

#include <openssl/evp.h>

#include "hasher.hpp"

namespace fs = std::filesystem;

int main() {
    fs::path path{"/Users/akhildoddapaneni/Code/AV/AV/Parser/output.txt"};
    std::ifstream iPath {path};

    fs::path output {"/Users/akhildoddapaneni/Code/AV/AV/Hasher/output.txt"};
    std::ofstream output_file {output};

    std::string targetLine;
    EVP_MD_CTX * ctx = EVP_MD_CTX_new();

    while(std::getline(iPath, targetLine)) {
        fs::path scanFile {targetLine};
        output_file << hash(scanFile, ctx) << '\n';
    }
    EVP_MD_CTX_free(ctx);
    return 0;
}