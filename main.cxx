#include "token.hxx"
#include "parse.hxx"
#include <iostream>

int main(){
  NCC::Tokenizer t(std::cin);
  NCC::Parser p(t);
  
  p.read_toplevel();
}
