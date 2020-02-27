#ifndef ARGUMENTPARSER
#define ARGUMENTPARSER

#include <string>


class ArgumentParser {
public:
    ArgumentParser() {
        filename = "";
        program = "";
    };
    ArgumentParser(int argc, char **argv);

    //returns true if arguments read, false if reader error
    bool read_arguments(int argc, char **argv);

    //get functions
    std::string getProgram();
    std::string getFilename();

    //check optional arguments
    bool file_present();
    bool short_program();

private:
    std::string filename;
    std::string program;
};

#endif
