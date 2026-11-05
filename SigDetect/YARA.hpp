#ifndef YARA_HPP
#define YARA_HPP

#include <yara.h>
#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

class YARA{
    std::filesystem::path rulesDir;
    std::vector<YR_RULES*> compiledRules{};

    void compile(); // compile yara rules within the rules_dir
public:
    YARA(std::filesystem::path rulesDir); // store rules_dir of yara rules
    ~YARA();
    int getMatch(std::filesystem::path targetFile); // return1 if the given file is a match to the compiled yara rules 0 if not, -1 if incomplete
    bool ingest(std::filesystem::path ruleFile); // return true if the given rulefile is succesfully added 

    static void compilerCallBack(int error_level, const char* file_name, int line_number, const YR_RULE* rule,
    const char* message, void* user_data); // callback for compiled rules. 
    static int scanCallBack(YR_SCAN_CONTEXT* context, int message, void* message_data, void* user_data);
};
#endif