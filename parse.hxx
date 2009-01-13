#include "AST.hxx"
#include "token.hxx"

namespace NCC{
  class Parser {
  protected:
    Tokenizer& tok;
    ValueType parse_type();
    FunctionDeclaration* parse_function(ValueType return_type, const std::string& name);
  public:
    Parser(Tokenizer& tok) : tok(tok) {};
    TopLevelForm* read_toplevel();
  };
}
