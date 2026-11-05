#include "YARA.hpp"
#include <yara.h>

#include <tuple>
#include <vector>
#include <filesystem>
#include <cstdio>
#include <iostream>

YARA::YARA(std::filesystem::path rulesDir)
: rulesDir{rulesDir}, compiledRules {}
{
    this->compile();                     // add rules into compiler
}

YARA::~YARA(){
    for (YR_RULES* rules : compiledRules) { // destroy compiled rules
        yr_rules_destroy(rules);
    }
}

void YARA::compile() {
    for (const auto& ruleset : std::filesystem::directory_iterator(rulesDir)) {
        YR_COMPILER* compiler = nullptr;
        YR_RULES* rules = nullptr;

        int result = yr_compiler_create(&compiler);

        if (result != ERROR_SUCCESS || compiler == nullptr) {
            continue;
        }

        yr_compiler_set_callback(compiler, compilerCallBack, nullptr);

        FILE* file = fopen(ruleset.path().c_str(), "r");

        if (!file) {
            yr_compiler_destroy(compiler);
            continue;
        }

        int errors = yr_compiler_add_file( compiler, file, nullptr, ruleset.path().c_str() );

        fclose(file);

        if (errors == 0) { 
            int ruleCompileErrors = yr_compiler_get_rules(compiler, &rules);
            if (ruleCompileErrors == 0 && rules != nullptr) {
                compiledRules.push_back(rules);
            }
        }

        yr_compiler_destroy(compiler); // destroy compiler as its useless once we extract the rule from it
    }
}

int YARA::getMatch(std::filesystem::path targetFile) {
    if (compiledRules.empty()) {
        return -1;
    }
    bool matched = false;
    bool scanError = false;
    for (YR_RULES* rules : compiledRules) {
        int result = yr_rules_scan_file(rules, targetFile.c_str(), 0,
            scanCallBack, &matched, 10);

        if (result != ERROR_SUCCESS) {
            scanError = true;
        }
        if (matched) return 1;
    }
    return scanError ? -1 : 0;
}

bool YARA::ingest(std::filesystem::path ruleFile) {
    YR_COMPILER* compiler = nullptr;
    YR_RULES* rules = nullptr;

    int result = yr_compiler_create(&compiler);

    if (result != ERROR_SUCCESS || compiler == nullptr) {
        return false;
    }

    yr_compiler_set_callback(compiler, compilerCallBack, nullptr);

    FILE* file = fopen(ruleFile.c_str(), "r");

    if (!file) {
        yr_compiler_destroy(compiler);
        return false;
    }

    int errors = yr_compiler_add_file(compiler, file, nullptr, ruleFile.c_str());

    fclose(file);

    if (errors != 0) {
        yr_compiler_destroy(compiler);
        return false;
    }

    int compileErrors = yr_compiler_get_rules(compiler, &rules);

    if (compileErrors != 0 || rules == nullptr) {
        yr_compiler_destroy(compiler);
        return false;
    }

    compiledRules.push_back(rules);

    yr_compiler_destroy(compiler);

    return true;
}

void YARA::compilerCallBack(int error_level, const char* file_name, int line_number, const YR_RULE* rule,
const char* message, void* user_data) {
        return;
}

int YARA::scanCallBack(YR_SCAN_CONTEXT* context, int message, void* message_data, void* user_data) {
    if (message == CALLBACK_MSG_RULE_MATCHING) {
        *static_cast<bool*>(user_data) = true;
    }

    return CALLBACK_CONTINUE;
}