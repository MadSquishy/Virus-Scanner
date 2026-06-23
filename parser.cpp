#include "parser.hpp"
#include <iostream>
#include <filesystem>

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
}

bool is_script(const fs::path& filepath) {
    
}
