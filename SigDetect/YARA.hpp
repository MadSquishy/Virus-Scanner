#ifndef YARA_HPP
#define YARA_HPP

#include <yara.h>
#include <filesystem>
#include <string>
#include <tuple>
#include <vector>


// YARA object is a object of compiled yara rules. Serialized versions provided to consumer by provider.
// User of virus scanner not expected have to compile a YARA object. Engine should default to loading when properly in use
class YARA{
    std::filesystem::path rulesDir;
    std::vector<YR_RULES*> compiledRules{};

    void compile(); // compile yara rules within the rules_dir
    bool save();    // Save compiled yara rules
    bool load();    // load compiled yara rules
public:
    YARA(std::filesystem::path rulesDir); // store rules_dir of yara rules
    YARA(); // YARA obj constructor if a load object is present
    ~YARA();
    int getMatch(std::filesystem::path targetFile); // return1 if the given file is a match to the compiled yara rules 0 if not, -1 if incomplete
    bool ingest(std::filesystem::path ruleFile); // return true if the given rulefile is succesfully added 

    static void compilerCallBack(int error_level, const char* file_name, int line_number, const YR_RULE* rule,
    const char* message, void* user_data); // callback for compiled rules. 
    static int scanCallBack(YR_SCAN_CONTEXT* context, int message, void* message_data, void* user_data);
};
#endif