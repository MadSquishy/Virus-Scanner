#include "parser.hpp"
#include <iostream>
#include <filesystem>
#include <magic.h>
#include <unordered_set>

namespace fs = std::filesystem;

// AI generated placehodler whitelist
// modify to read from a file instead
static const std::unordered_set<std::string> whitelist = {
    // audio (pure data, no execution capability)
    "audio/mpeg",
    "audio/wav",
    "audio/ogg",
    "audio/flac",
    "audio/aac",
    "audio/midi",
    "audio/x-m4a",

    // video (pure data, no execution capability)
    "video/mp4",
    "video/quicktime",
    "video/x-msvideo",
    "video/x-matroska",
    "video/webm",
    "video/mpeg",
    "video/x-flv",

    // fonts (pure data)
    "font/ttf",
    "font/otf",
    "font/woff",
    "font/woff2",

    // 3d formats (pure data)
    "model/obj",
    "model/stl",
    "model/gltf+json",
    "model/gltf-binary",
};


bool has_executable(const fs::path& filepath) {
    auto perms = fs::status(filepath).permissions();
    bool owner_exec = (perms & fs::perms::owner_exec) != fs::perms::none;
    bool group_exec = (perms & fs::perms::group_exec) != fs::perms::none;
    bool others_exec = (perms & fs::perms::others_exec) != fs::perms::none;

    return owner_exec || group_exec || others_exec;
}

bool is_scan_target(const std::filesystem::path& filepath, magic_t cookie) {
    if (has_executable(filepath)) {return true;}


    const char * result = magic_file(cookie, filepath.c_str());
    if(!result) { return true; }
    else {
        std::string type(result);
        return whitelist.count(type) == 0;
    }
    
    return true;
}

void parse(std::ostream& out, const fs::path& filepath) {
    // CREATE THE COOKIE AND LOAD IT SO IT CAN BE PASSED INTO IS_SCAN_TARGET
    magic_t cookie = magic_open(MAGIC_MIME_TYPE);
    magic_load(cookie, nullptr);
    for (const auto& file : fs::recursive_directory_iterator(filepath)) {
        if (fs::is_regular_file(file)) {
            if (is_scan_target(file.path(), cookie)) { 
                out << file.path().string() << '\n'; 
            }

        }
    }
    magic_close(cookie);
}
