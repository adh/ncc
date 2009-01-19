#ifndef HXX__ncc__token__
#define HXX__ncc__token__

#include <string>
#include <iostream>

#include "exceptions.hxx"

namespace ncc {
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
  static const char TOKEN_RETURN = 13;
  static const char TOKEN_EQUAL = 14;
  static const char TOKEN_NOT_EQUAL = 15;
  static const char TOKEN_GT_EQUAL = 16;
  static const char TOKEN_LT_EQUAL = 17;
  static const char TOKEN_SC_AND = 18;
  static const char TOKEN_SC_OR = 19;

  class Tokenizer {
  protected:
    std::istream& stream;
    char token;
    std::string text;
    int int_value;
    double float_value;
 
    int line;
    int column;
   
    // TODO: keep track of file postion

    void parse_number();
    void skip_line_comment();
    void skip_comment();
    char read_char_escape();
    void parse_string();
    int read_char(bool eof_ok);
    void unread_char();
  public:
    Tokenizer(std::istream& stream): 
      stream(stream), line(1), column(0) {};
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
    const std::string& get_text(){
      return text;
    }
    void next_token();
    void eat_token(char expected){
      if (current_token() != expected){
        throw new ExpectedToken(expected, current_token());
      }
      next_token();
    }
    void next_token_expect(char expected){
      next_token();
      if (current_token() != expected){
        throw new ExpectedToken(expected, current_token());
      }
    }
    int get_line(){
      return line;
    }
    int get_column(){
      return column;
    }
  };
}

#endif
