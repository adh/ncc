#include <string>
#include <iostream>

namespace NCC {
  static const char TOKEN_EOF = 0;
  static const char TOKEN_INT_VALUE = 1;
  static const char TOKEN_FLOAT_VALUE = 2;
  static const char TOKEN_STRING = 3;
  static const char TOKEN_IDENT = 4;
  static const char TOKEN_IF = 5;
  static const char TOKEN_ELSE = 6;
  static const char TOKEN_WHILE = 7;
  static const char TOKEN_FOR = 8;
  static const char TOKEN_DO = 9;
  static const char TOKEN_INT = 10;
  static const char TOKEN_FLOAT = 11;
  static const char TOKEN_PTR = 12;
  static const char TOKEN_EQUAL = 13;

  class Tokenizer {
  protected:
    std::istream& stream;
    char token;
    std::string text;
    int int_value;
    double float_value;
    
    // TODO: keep track of file postion

    void parse_number();
    void skip_line_comment();
    void skip_comment();
    int read_char(bool eof_ok);
  public:
    Tokenizer(std::istream& stream): stream(stream) {};
    char current_token(){
      return token;
    }
    const std::string& get_string_value(){
      return text;
    }
    int get_int_value(){
      return int_value;
    }
    double get_float_value() {
      return float_value;
    }
    void next_token();
    void eat_token(char token){
      if (current_token() != token){
        throw "Unexpected token";
      }
      next_token();
    }
  };
}
