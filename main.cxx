#include "token.hxx"
#include <iostream>

int main(){
  NCC::Tokenizer t(std::cin);
  while (std::cin.good()){
    t.next_token();
    std::cout << (int)t.current_token() << "(" << t.current_token() << ") i=" << t.get_int_value() << ", f=" << t.get_float_value() << ", t=" << t.get_text() << std::endl;
  }
}
