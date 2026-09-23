#ifndef PARSER_HPP
#define PARSER_HPP

#include <iosfwd>
#include <filesystem>
#include <unordered_set>
#include <magic.h>

void parse(std::ostream& out, const std::filesystem::path& filepath);

bool has_executable(const std::filesystem::path& filepath);

bool is_scan_target(const std::filesystem::path& filepath, magic_t cookie); 

#endif // PARSER_HPP