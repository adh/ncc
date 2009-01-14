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
    if (f){
      f->print(std::cout, 0);
    }
  } catch (std::exception* e){
    std::cerr << "Error: " << e->what() << std::endl;
  }
}
