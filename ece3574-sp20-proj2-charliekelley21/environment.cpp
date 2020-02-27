#include "environment.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

#include "interpreter_semantic_error.hpp"

//  This module should define the C++ types
//  and code required to implement the slisp environment mapping.

Environment::Environment(){
  clear();
}

bool Environment::isProcedure(std::string key){
    return (envmap[key].type == ProcedureType);
}

Expression Environment::getExpression(std::string key){
    return envmap[key].exp;
}

bool Environment::keyPresent(std::string key){
    return (envmap.find(key) != envmap.end());
}

bool Environment::addExpression(std::string key, Expression value){
    envmap[key].type = ExpressionType;
    envmap[key].exp = value;
}

Expression Environment::getResult(std::string key, const std::vector<Atom> & args){
    return envmap[key].proc(args);
}

void Environment::clear() {

    envmap = {};

    //adding special procedures as a check for variables
    envmap["define"].type = ProcedureType;
    envmap["begin"].type = ProcedureType;
    envmap["if"].type = ProcedureType;
    
    envmap["pi"].type = ExpressionType;
    envmap["pi"].exp = Expression( atan2(0, -1) );

    envmap["not"].type = ProcedureType;
    envmap["not"].proc = Procedure(&not_proc);

    envmap["and"].type = ProcedureType;
    envmap["and"].proc = Procedure(&and_proc);

    envmap["or"].type = ProcedureType;
    envmap["or"].proc = Procedure(&or_proc);

    envmap["<="].type = ProcedureType;
    envmap["<="].proc = Procedure(&less_than_equal_proc);

    envmap["<"].type = ProcedureType;
    envmap["<"].proc = Procedure(&less_than_proc);

    envmap[">="].type = ProcedureType;
    envmap[">="].proc = Procedure(&more_than_equal_proc);

    envmap[">"].type = ProcedureType;
    envmap[">"].proc = Procedure(&more_than_proc);

    envmap["="].type = ProcedureType;
    envmap["="].proc = Procedure(&equal_proc);

    envmap["+"].type = ProcedureType;
    envmap["+"].proc = Procedure(&addition_proc);

    envmap["-"].type = ProcedureType;
    envmap["-"].proc = Procedure(&dash_proc);

    envmap["*"].type = ProcedureType;
    envmap["*"].proc = Procedure(&multiplication_proc);

    envmap["/"].type = ProcedureType;
    envmap["/"].proc = Procedure(&slash_proc);

    envmap["log10"].type = ProcedureType;
    envmap["log10"].proc = Procedure(&log_ten_proc);

    envmap["pow"].type = ProcedureType;
    envmap["pow"].proc = Procedure(&pow_proc);
}

//  Below are all function to be used as Procedures in mapping
Expression not_proc(const std::vector<Atom> & args) {
  if (args.size() != 1)
      throw InterpreterSemanticError("Error: invalid number of arguments for not function");
  return Expression(!args[0].value.bool_value);
}

Expression and_proc(const std::vector<Atom> & args) {
  if (args.size() < 1)
      throw InterpreterSemanticError("Error: invalid number of arguments for and function");
  bool finalValue = true;
  for (auto arg: args) {
      finalValue &= arg.value.bool_value;
  }
  return Expression(finalValue);
}

Expression or_proc(const std::vector<Atom> & args) {
  if (args.size() < 1)
      throw InterpreterSemanticError("Error: invalid number of arguments for or function");
  bool finalValue = false;
  for (auto arg: args) {
      finalValue |= arg.value.bool_value;
  }
  return Expression(finalValue);
}

Expression less_than_proc(const std::vector<Atom> & args) {
  if (args.size() != 2)
      throw InterpreterSemanticError("Error: invalid number of arguments for < function");
  bool lessThan = args[0].value.num_value < args[1].value.num_value;
  return Expression(lessThan);
}

Expression less_than_equal_proc(const std::vector<Atom> & args) {
  if (args.size() != 2)
      throw InterpreterSemanticError("Error: invalid number of arguments for <= function");
  bool lessThanEq = args[0].value.num_value <= args[1].value.num_value;
  return Expression(lessThanEq);
}

Expression more_than_proc(const std::vector<Atom> & args) {
  if (args.size() != 2)
      throw InterpreterSemanticError("Error: invalid number of arguments for > function");
  bool moreThan = args[0].value.num_value > args[1].value.num_value;
  return Expression(moreThan);
}

Expression more_than_equal_proc(const std::vector<Atom> & args) {
  if (args.size() != 2)
      throw InterpreterSemanticError("Error: invalid number of arguments for >= function");
  bool moreThanEq = args[0].value.num_value >= args[1].value.num_value;
  return Expression(moreThanEq);
}

Expression equal_proc(const std::vector<Atom> & args) {
  if (args.size() != 2)
      throw InterpreterSemanticError("Error: invalid number of arguments for = function");
  bool equals = args[0].value.num_value == args[1].value.num_value;
  return Expression(equals);
}

Expression addition_proc(const std::vector<Atom> & args) {
  if (args.size() < 1)
      throw InterpreterSemanticError("Error: invalid number of arguments for + function");
  Number sum = 0.0;
  for (auto arg: args) {
      sum += arg.value.num_value;
  }
  return Expression(sum);
}

Expression dash_proc(const std::vector<Atom> & args) {
  if (args.size() > 2 || args.size() < 1)
      throw InterpreterSemanticError("Error: invalid number of arguments for - function");
  if (args.size() == 1)
    return Expression(args[0].value.num_value * -1);
  else if (args.size() == 2)
    return Expression(args[0].value.num_value - args[1].value.num_value);
}

Expression multiplication_proc(const std::vector<Atom> & args) {
  if (args.size() == 1)
      throw InterpreterSemanticError("Error: invalid number of arguments for * function");
  double product = 1;
  for (auto it = args.begin(); it != args.end(); ++it) {
      product *= it->value.num_value;
  }
  return Expression(product);
}

Expression slash_proc(const std::vector<Atom> & args) {
  if (args.size() != 2)
      throw InterpreterSemanticError("Error: invalid number of arguments for / function");
  return Expression(args[0].value.num_value / args[1].value.num_value);
}

Expression log_ten_proc(const std::vector<Atom> & args) {
  if (args.size() != 1)
      throw InterpreterSemanticError("Error: invalid number of arguments for log10 function");
  return Expression(log10(args[0].value.num_value));
}

Expression pow_proc(const std::vector<Atom> & args) {
  if (args.size() != 2)
      throw InterpreterSemanticError("Error: invalid number of arguments for pow function");
  Number power = pow(args[0].value.num_value, args[1].value.num_value);
  return Expression(power);
}
