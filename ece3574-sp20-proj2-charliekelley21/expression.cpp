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
  else if (exp.head.type == BooleanType) {
      if (exp.head.value.bool_value)
          out << "True";
      else
          out << "False";
  }
  else if (exp.head.type == SymbolType)
      out << exp.head.value.sym_value;
  out << ")";
  return out;
}

bool token_to_atom(const std::string & token, Atom & atom){
    try {
        size_t end = 0;
        atom.type = NumberType;
        atom.value.num_value = stod(token, &end);
        return (end == token.length());
      }
    catch (std::invalid_argument) {
        if (isdigit(token.front())) {
          return false;
        }
        else if (token == "True") {
          atom.type = BooleanType;
          atom.value.bool_value = true;
          return true;
        }
        else if (token == "False") {
          atom.type = BooleanType;
          atom.value.bool_value = false;
          return true;
        }
        else {
          atom.type = SymbolType;
          atom.value.sym_value = token;
          return true;
        }
    }
    return false;
}
