#include "AST.hxx"
#include "token.hxx"

namespace NCC{
  class Parser {
  protected:
    Tokenizer& tok;
    ValueType parse_type();
    FunctionDeclaration* parse_function(ValueType return_type, const std::string& name);
    VariableDefinition* parse_initializer(ValueType return_type, const std::string& name);
    Expression* parse_expression();
    Expression* parse_factor();
    Expression* parse_addend();
  public:
    Parser(Tokenizer& tok) : tok(tok) {
      tok.next_token();
    };
    TopLevelForm* read_toplevel();
  };
}
