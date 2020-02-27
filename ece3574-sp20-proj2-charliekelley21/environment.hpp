#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <map>

// module includes
#include "expression.hpp"

class Environment{
public:
  Environment();
  void clear();
  bool keyPresent(std::string key);
  Expression getExpression(std::string key);
  bool isProcedure(std::string key);
  Expression getResult(std::string key, const std::vector<Atom> & args);
  bool addExpression(std::string key, Expression value);

private:

  // Environment is a mapping from symbols to expressions or procedures
  enum EnvResultType {ExpressionType, ProcedureType};
  struct EnvResult{
    EnvResultType type;
    Expression exp;
    Procedure proc;
  };

  std::map<Symbol,EnvResult> envmap;
};

Expression not_proc(const std::vector<Atom> & args);
Expression and_proc(const std::vector<Atom> & args);
Expression or_proc(const std::vector<Atom> & args);
Expression less_than_proc(const std::vector<Atom> & args);
Expression less_than_equal_proc(const std::vector<Atom> & args);
Expression more_than_proc(const std::vector<Atom> & args);
Expression more_than_equal_proc(const std::vector<Atom> & args);
Expression equal_proc(const std::vector<Atom> & args);
Expression addition_proc(const std::vector<Atom> & args);
Expression dash_proc(const std::vector<Atom> & args);
Expression multiplication_proc(const std::vector<Atom> & args);
Expression slash_proc(const std::vector<Atom> & args);
Expression log_ten_proc(const std::vector<Atom> & args);
Expression pow_proc(const std::vector<Atom> & args);

#endif
