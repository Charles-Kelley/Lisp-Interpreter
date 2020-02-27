#include "argumentparser.hpp"
#include <iostream>
#include <string>

ArgumentParser::ArgumentParser(int argc, char **argv){
    read_arguments(argc, argv);
}

bool ArgumentParser::file_present() {
    return !filename.empty();
}

bool ArgumentParser::short_program() {
    return !program.empty();
}

std::string ArgumentParser::getProgram() {
    return program;
}

std::string ArgumentParser::getFilename() {
    return filename;
}

bool ArgumentParser::read_arguments(int argc, char **argv) {
    if (argc == 3){
        std::string str = argv[1];
        if (str == "-e"){
            program = argv[2];
        }
    }
    else if (argc == 2)
        filename = argv[1];
}
