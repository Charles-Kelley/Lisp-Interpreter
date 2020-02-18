#include "tokenize.hpp"
#include <cctype>

#include <sstream>

TokenSequenceType tokenize(std::istream & seq){
    TokenSequenceType tokens;

    std::string line, token;

    while (getline(seq, line)){
        line_format(line);
        std::istringstream tokenizer(line);

        while(tokenizer >> token){
            tokens.push_back(token);
        }
    }
    return tokens;
}

void line_format(std::string& str)
{
    line_spacer(str, OPEN);
    line_spacer(str, CLOSE);
    int index = str.find(COMMENT, 0);
    if (index != std::string::npos)
        str.replace(index, std::string::npos, "");
}

void line_spacer(std::string& str, char delim) {
    int index = 0;
    std::string s_replace = " ";
    s_replace.push_back(delim);
    s_replace.append(" ");
    while (true) {
        index = str.find(delim, index);
        if (index == std::string::npos)
          break;
        str.replace(index, 1, s_replace);
        index += 3;
    }
}
