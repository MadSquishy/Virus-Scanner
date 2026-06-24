#include "parser.hpp"
#include <iostream>
#include <filesystem>
// #include <magic.h> is the main directive to include libmagic, but before downloading, figure out compilers, and how to use them
namespace fs = std::filesystem;

void parse(std::ostream& out, const fs::path& filepath) {
    for (const auto& file : fs::recursive_directory_iterator(filepath)) {
        if (fs::is_regular_file(file)) {
            if (is_scan_target(file)) {
                out << file.path().string() << '\n';                
            }

        }
    }
    
}

bool is_executable(const fs::path& filepath) {
    if (fs::is_regular_file(filepath)) {}
    return true;
}

bool is_script(const fs::path& filepath) {
    if (fs::is_regular_file(filepath)) {}
    return true;
}

bool is_scan_target(const std::filesystem::path& filepath) {
    return is_script(filepath) || is_executable(filepath);
}