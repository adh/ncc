#include "token.hxx"
#include "parse.hxx"
#include "AST.hxx"
#include <iostream>

int main(){
  NCC::Tokenizer t(std::cin);
  NCC::Parser p(t);
  NCC::TopLevelForm *f;

  try {
    f = p.read_toplevel();
  } catch (std::exception* e){
    std::cerr << "Error: " << t.get_line() << ":" << t.get_column() << ": " << e->what() << std::endl;
    return 1;
  }

  if (f){
    f->print(std::cout, 0);
  }
}
