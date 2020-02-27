#include <cstdlib>
#include "argumentparser.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "interpreter_semantic_error.hpp"

#include <sstream>
#include <fstream>

//system includes
#include <iostream>

bool file_exists(std::string& fileName);

int main(int argc, char **argv)
{

  ArgumentParser commandLine = ArgumentParser(argc, argv);
  Interpreter interp;
  Expression result;
  bool ok;

  if (commandLine.short_program()){
      std::istringstream iss(commandLine.getProgram());
      ok = interp.parse(iss);
      if (!ok)
          return EXIT_FAILURE;
      result = interp.eval();
      if (result.head.type == NoneType)
          return EXIT_FAILURE;
      return EXIT_SUCCESS;
  }
  else if (commandLine.file_present()){
      std::string fileName = commandLine.getFilename();
      if (file_exists(fileName)){
          std::ifstream programFile;
          programFile.open(fileName);
          ok = interp.parse(programFile);
          programFile.close();
          if (!ok)
              return EXIT_FAILURE;
          result = interp.eval();
          if (result.head.type == NoneType)
              return EXIT_FAILURE;
          return EXIT_SUCCESS;
      }
      else
          std::cout << "Error: file does not exsist" << std::endl;
          return EXIT_FAILURE;
  }
  else{
    std::string interactive;
    std::cout << "slisp> ";
      while (getline(std::cin, interactive)){
          std::istringstream isss(interactive);
          ok = interp.parse(isss);
          Expression result = interp.eval();
          std::cout << "slisp> ";
      }
  }

  return EXIT_FAILURE;
}

bool file_exists(std::string& fileName)
{
    std::ifstream exists(fileName.c_str());
    return (bool)exists;
}
