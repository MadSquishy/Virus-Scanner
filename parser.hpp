#ifndef PARSER_HPP
#define PARSER_HPP

#include <iosfwd>
#include <filesystem>

void parse(std::ostream& out, const std::filesystem::path& filepath);

bool is_executable(const std::filesystem::path& filepath);

bool is_script(const std::filesystem::path& filepath);

bool is_scan_target(const std::filesystem::path& filepath) {
    return is_executable(filepath) || is_script(filepath);
}

#endif PARSER_HPP