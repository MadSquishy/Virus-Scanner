#include <gtest/gtest.h>
#include <yara.h>
#include "YARA.hpp"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST (YARAtest, EICAR) { // tests both eicar match and eicar no match
    fs::path tempDir = fs::temp_directory_path() / "YARA_RULE_TEST";
    if (fs::exists(tempDir)) fs::remove_all(tempDir);
    fs::create_directories(tempDir);

    fs::path eicarZip = fs::current_path().parent_path() / "TEST-DIRECTORY" / "eicar_com.zip";
    fs::path cleanTxt = fs::current_path().parent_path() / "TEST-DIRECTORY" / "clean.txt";
    fs::path yaraFile = tempDir / "eicarTest.yar";

    std::ofstream ruleFile(yaraFile);


    ruleFile << R"(
                    rule eicarTestDetection
                    {
                        strings:
                            $standard_eicar_string = "X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*" ascii
                        condition:
                            $standard_eicar_string
                    }
                )";

    ruleFile.close();

    yr_initialize();

    YARA compiledRules{tempDir};

    EXPECT_EQ(compiledRules.getMatch(eicarZip), 1);
    EXPECT_EQ(compiledRules.getMatch(cleanTxt), 0);

    yr_finalize();
    fs::remove_all(tempDir);

}

TEST(YARAtest, MULTIPLERules) { // tests succesfull compilation of multiple rules
    fs::path tempDir = fs::temp_directory_path() / "YARA_RULE_TEST";
    if (fs::exists(tempDir)) fs::remove_all(tempDir);   
    fs::create_directories(tempDir);

    fs::path eicarZip = fs::current_path().parent_path() / "TEST-DIRECTORY" / "eicar_com.zip";
    fs::path cleanTxt = fs::current_path().parent_path() / "TEST-DIRECTORY" / "clean.txt";
    fs::path leafFoldMP4 = fs::current_path().parent_path() / "TEST-DIRECTORY" / "howtofoldleaf.mp4";
    fs::path yaraFile = tempDir / "eicarTest.yar";
    fs::path yaraFileTwo = tempDir / "clean.yar"; // detect the clean file too!

    std::ofstream ruleFile(yaraFile);


    ruleFile << R"(
                    rule eicarTestDetection
                    {
                        strings:
                            $standard_eicar_string = "X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*" ascii
                        condition:
                            $standard_eicar_string
                    }
                )";

    ruleFile.close();

    std::ofstream clearRuleFile(yaraFileTwo);

    clearRuleFile << R"(
                        rule cleanTxtDetcetion
                        {
                            strings:
                                $clear_txt_capture = "is clean under yara ruleset!:w4t" ascii
                            condition:
                                all of them
                        }
                     )";
    clearRuleFile.close();
    yr_initialize();

    YARA compiledRules{tempDir};

    EXPECT_EQ(compiledRules.getMatch(eicarZip), 1);
    EXPECT_EQ(compiledRules.getMatch(cleanTxt), 1);
    EXPECT_EQ(compiledRules.getMatch(leafFoldMP4), 0);


    yr_finalize();
    fs::remove_all(tempDir);
}