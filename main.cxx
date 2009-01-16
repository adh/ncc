#include "token.hxx"
#include "parse.hxx"
#include "AST.hxx"
#include "symbol.hxx"
#include <iostream>

int main(){
  ncc::Tokenizer t(std::cin);
  ncc::Parser p(t);
  ncc::TopLevelForm *f;
  ncc::SymbolTable global_symbols;
  llvm::Module module("");

  try {
    f = p.read_toplevel();
  } catch (std::exception* e){
    std::cerr << "Parse Error: " << t.get_line() << ":" << t.get_column() << ": " << e->what() << std::endl;
    return 1;
  }

  if (f){
    f->print(std::cout, 0);
  }

  try {
    f->generate(&module, &global_symbols);
  } catch (std::exception* e){
    std::cerr << "Error: " << e->what() << std::endl;
    return 1;
  }

  module.dump();
}
