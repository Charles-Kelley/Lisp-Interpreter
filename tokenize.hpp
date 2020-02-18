#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <istream>
#include <deque>

typedef std::deque<std::string> TokenSequenceType;

const char OPEN = '(';
const char CLOSE = ')';
const char COMMENT = ';';

void line_spacer(std::string& str, char delim);
void line_format(std::string& str);

// split string into a list of tokens where a token is one of
// OPEN or CLOSE or a space-delimited string
// ignores any whitespace and from any ";" to end-of-line
TokenSequenceType tokenize(std::istream & seq);

#endif
