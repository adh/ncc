#include "AST.hxx"
#include "token.hxx"

namespace NCC{
  class Parser {
  protected:
    Tokenizer& tok;
  public:
    Parser(Tokenizer& tok) : tok(tok) {};
    TopLevelForm* read_toplevel();
  }
}
