#include "catch.hpp"

#include <string>

#include "expression.hpp"

TEST_CASE( "Test Type Inference", "[types]" ) {

  Atom a;

  std::string token = "True";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == true);

  token = "False";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == false);

  token = "1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 1);

  token = "1.25";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 1.25);

  token = "1e2";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 100);

  token = "1e-2";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 0.01);

  token = "-1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == -1);

  token = "var";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == "var");

  token = "1abc";
  REQUIRE(!token_to_atom(token, a));

  token = "9help";
  REQUIRE(!token_to_atom(token, a));

  token = "0test";
  REQUIRE(!token_to_atom(token, a));

  token = "var1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == token);

  token = "var18394802389839";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == token);

}
