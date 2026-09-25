#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    fs::path output {"/Users/akhildoddapaneni/Code/AV/AV/Parser/output.txt"};
    std::ofstream output_file {output};
    bool finished = false;


    while (!finished) {
        std::string pathInput;

        std::getline(std::cin, pathInput);

        if ( pathInput == "x" || pathInput == "X" ) { finished = true; }
        else {
            fs::path directory {pathInput};

            if ( fs::exists(directory) && fs::is_directory(directory) ) {
                parse(output_file, directory);
                finished = true;
            }
            else { std::cout << "given path does not exist or is not a directory, try again or enter x to exit" << std::endl; }
        }
    }
    

    return 0;
}