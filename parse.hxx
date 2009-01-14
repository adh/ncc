#include "AST.hxx"
#include "token.hxx"

namespace NCC{
  class Parser {
  protected:
    Tokenizer& tok;
    ValueType parse_type();
    FunctionDeclaration* parse_function(ValueType return_type, const std::string& name);
    VariableDefinition* parse_initializer(ValueType return_type, const std::string& name);
    Expression* parse_value();
    Expression* parse_unary();
    Expression* parse_multiplicative();
    Expression* parse_additive();
    Expression* parse_comparison();
    Expression* parse_bitwise();
    Expression* parse_logic();
    Expression* parse_ternary();
    Expression* parse_assign();
    Expression* parse_comma();

  public:
    Parser(Tokenizer& tok) : tok(tok) {
      tok.next_token();
    };
    TopLevelForm* read_toplevel();
  };
}
