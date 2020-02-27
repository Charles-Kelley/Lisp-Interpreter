#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "test_config.hpp"

Expression run(const std::string & program){

  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parse(iss);
  if(!ok){
    std::cerr << "Failed to parse: " << program << std::endl;
  }
  REQUIRE(ok == true);

  Expression result;
  REQUIRE_NOTHROW(result = interp.eval());

  return result;
}

TEST_CASE( "Test Interpreter parser with numerical literals", "[interpreter]" ) {

  std::vector<std::string> programs = {"(1)", "(+1)", "(+1e+0)", "(1e-0)"};

  for(auto program : programs){
    std::istringstream iss(program);

    Interpreter interp;

    bool ok = interp.parse(iss);

    REQUIRE(ok == true);
  }
}

TEST_CASE( "Test Interpreter parser with expected input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == true);
}

TEST_CASE( "Test Interpreter parser with faulted input", "[interpreter]" ) {

  std::string program = ")(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with a commented input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r))) ; not included";

  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == true);
}

TEST_CASE( "Test Interpreter result with literal expressions", "[interpreter]" ) {

  { // Boolean True
    std::string program = "(True)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  { // Boolean False
    std::string program = "(False)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  { // Number
    std::string program = "(4)";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }

  { // Symbol
    std::string program = "(pi)";
    Expression result = run(program);
    REQUIRE(result == Expression(atan2(0, -1))); //failed here
  }

}

TEST_CASE( "Test Interpreter result with simple procedures (add)", "[interpreter]" ) {

  { // add, binary case
    std::string program = "(+ 1 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(3.));
  }

  { // add, 3-ary case
    std::string program = "(+ 1 2 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(6.));
  }

  { // add, 6-ary case
    std::string program = "(+ 1 2 3 4 5 6)";
    Expression result = run(program);
    REQUIRE(result == Expression(21.));
  }
}

TEST_CASE( "Test Interpreter special form: if", "[interpreter]" ) {

  {
    std::string program = "(if True (4) (-4))";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }

  {
    std::string program = "(if False (4) (-4))";
    Expression result = run(program);
    REQUIRE(result == Expression(-4.));
  }
}

TEST_CASE( "Test Interpreter special forms: begin and define", "[interpreter]" ) {

  {
    std::string program = "(define answer 42)";
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }

  {
    std::string program = "(begin (define answer 42)\n(answer))";
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }

  {
    std::string program = "(begin (define answer (+ 9 11)) (answer))";
    Expression result = run(program);
    REQUIRE(result == Expression(20.));
  }

  {
    std::string program = "(begin (define a 1) (define b 1) (+ a b))";
    Expression result = run(program);
    REQUIRE(result == Expression(2.));
  }
}

TEST_CASE( "Test a complex expression", "[interpreter]" ) {

  {
    std::string program = "(+ (+ 10 1) (+ 30 (+ 1 1)))";
    Expression result = run(program);
    REQUIRE(result == Expression(43.));
  }
}

TEST_CASE( "Test Interpreter for all procedures", "[interpreter]" ) {

  {
    std::string program = "(not True)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  {
    std::string program = "(not False)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(and True False (not True))";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  {
    std::string program = "(and True True (not False))";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(or True False)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(or False False False)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  {
    std::string program = "(< 2 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(< 3 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  {
    std::string program = "(<= 3 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(<= 2 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(<= 4 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  {
    std::string program = "(>= 3 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(>= 2 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  {
    std::string program = "(>= 4 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(> 2 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  {
    std::string program = "(> 3 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(= 3 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  {
    std::string program = "(= 3 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  {
    std::string program = "(+ 3 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(5.));
  }

  {
    std::string program = "(+ 9.1 3 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(14.1));
  }

  {
    std::string program = "(- 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(-2.));
  }

  {
    std::string program = "(- 3 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(1.));
  }

  {
    std::string program = "(* 3 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(6.));
  }

  {
    std::string program = "(* 3 2 4 5)";
    Expression result = run(program);
    REQUIRE(result == Expression(120.));
  }

  {
    std::string program = "(/ 1 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(0.5));
  }

  {
    std::string program = "(/ 10 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(5.));
  }

  {
    std::string program = "(log10 1000000)";
    Expression result = run(program);
    REQUIRE(result == Expression(6.));
  }

  {
    std::string program = "(pow 3 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(9.));
  }

  {
    std::string program = "(pow 4 0.5)";
    Expression result = run(program);
    REQUIRE(result == Expression(2.));
  }

}
