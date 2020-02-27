#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"

bool Interpreter::parse(std::istream & expression) noexcept{

  //tokenize the given expression
  //std::deque<std::string> TokenSequenceType;
  //run through tokens and create ast
  TokenSequenceType tokens = tokenize(expression);
  try {
      ast = build_ast(tokens);
      return true;
  }
  catch (const InterpreterSemanticError) {
      std::cout << "Error: invalid syntax" << std::endl;
      return false;
  }
};

Expression Interpreter::evaluate(Expression exp){
    Expression evaluated;

    if (exp.head.type == NoneType)
        return Expression();

    if (exp.head.type == NumberType) {
        evaluated = Expression(exp.head.value.num_value);
    }
    else if (exp.head.type == BooleanType) {
        evaluated = Expression(exp.head.value.bool_value);
    }
    else if (exp.head.value.sym_value == "begin"){
        for(int i = 0; i < exp.tail.size(); ++i) {
            evaluated = evaluate(exp.tail.at(i));
        }
    }
    else if (exp.head.value.sym_value == "if"){
        bool cond = evaluate(exp.tail.at(0)).head.value.bool_value;
        if (cond) {
            evaluated = evaluate(exp.tail.at(1));
        }
        else {
            evaluated = evaluate(exp.tail.at(2));
        }
    }
    else if (exp.head.value.sym_value == "define"){
        std::string addKey = exp.tail.at(0).head.value.sym_value;
        if (env.keyPresent(addKey)){
            env.clear();
            throw InterpreterSemanticError("Error: symbol is already defined");
        }
        else {
            env.addExpression(addKey, evaluate(exp.tail.at(1)));
            evaluated = env.getExpression(addKey);
        }
    }
    else {
      std::string key = exp.head.value.sym_value;
      if (env.keyPresent(key)){
          if (env.isProcedure(key)) {
              std::vector<Atom> atms;
              for(int i = 0; i < exp.tail.size(); ++i) {
                  atms.push_back(evaluate(exp.tail.at(i)).head);
              }
              try {
                  evaluated = env.getResult(key, atms);
              }
              catch (InterpreterSemanticError) {
                  env.clear();
                  throw InterpreterSemanticError("Error: invlaid number of arguments");
                  return Expression();
              }
          }
          else {
              evaluated = env.getExpression(key);
          }
      }
      else{
          throw InterpreterSemanticError("Error: unknown symbol");
          return Expression();
      }
    }
    return evaluated;
}

Expression Interpreter::eval(){
    try {
        Expression exp = evaluate(ast);
        std::cout << exp << std::endl;
        return exp;
    }
    catch (InterpreterSemanticError) {
        std::cout << "Error: Semantic Error" << std::endl;
        return Expression();
    }
}



Expression Interpreter::build_ast(TokenSequenceType &tokens) {

    Expression ast;
    Atom atm;

    if (tokens.front() == "(") {

        tokens.pop_front();
        token_to_atom(tokens.front(), atm);
        tokens.pop_front();
        ast = Expression(atm);
        while (tokens.front() != ")") {
            if (tokens.front() == "(") {
                ast.tail.push_back(build_ast(tokens));
                tokens.pop_front();
            }
            else {
                token_to_atom(tokens.front(), atm);
                tokens.pop_front();
                ast.tail.push_back( Expression(atm) );
            }
        }
    }
    else if (tokens.front() == ")") {
        throw InterpreterSemanticError("Error: invalid syntax");
    }
    else {
        token_to_atom(tokens.front(), atm);
        tokens.pop_front();
        ast = Expression(atm);
    }

    return ast;
}





























//
