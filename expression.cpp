#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>
#include <stdexcept>

// system includes
#include <sstream>
#include <iostream>

Expression::Expression(bool tf){
  head.type = BooleanType;
  head.value.bool_value = tf;
}

Expression::Expression(double num){
  head.type = NumberType;
  head.value.num_value = num;
}

Expression::Expression(const std::string & sym){
  head.type = SymbolType;
  head.value.sym_value = sym;
}

bool Expression::operator==(const Expression & exp) const noexcept{
  bool equals = (this->head.type == exp.head.type);
  equals &= (this->tail.size() == exp.tail.size());

  if (this->head.type == NumberType)
      equals &= (this->head.value.num_value == exp.head.value.num_value);
  else if (this->head.type == BooleanType)
      equals &= (this->head.value.bool_value == exp.head.value.bool_value);
  else if (this->head.type == SymbolType)
      equals &= (this->head.value.sym_value.compare(exp.head.value.sym_value) == 0);
  return equals;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  out << "(";
  if (exp.head.type == NumberType)
      out << exp.head.value.num_value;
  else if (exp.head.type == BooleanType)
      out << exp.head.value.bool_value;
  else if (exp.head.type == SymbolType)
      out << exp.head.value.sym_value;
  out << ")" << std::endl;
  return out;
}

//  TODO: modify how function parses for a NumberType
bool token_to_atom(const std::string & token, Atom & atom){
  // TODO: implement this function
  // return true if it a token is valid. otherwise, return false.
  bool isNum = true;
  for (auto c : token) {
      isNum &= ( isdigit(c) || ispunct(c) || (c == 'e') );
  }
  if (isNum) {
    atom.type = NumberType;
    atom.value.num_value = stod(token);
    return true;
  }
  else if (isdigit(token.front())) {
    return false;
  }
  else if (token.compare("True") == 0) {
    atom.type = BooleanType;
    atom.value.bool_value = true;
    return true;
  }
  else if (token.compare("False") == 0) {
    atom.type = BooleanType;
    atom.value.bool_value = false;
    return true;
  }
  else {
    atom.type = SymbolType;
    atom.value.sym_value = token;
    return true;
  }
  return false;
}
