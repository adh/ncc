#include "token.hxx"
#include <ctype.h>

using namespace NCC;

struct Keyword {
  std::string name;
  char token;
};

static Keyword keywords[] = {
  {"if", TOKEN_IF},
  {"else", TOKEN_ELSE},
  {"while", TOKEN_WHILE},
  {"for", TOKEN_FOR},
  {"do", TOKEN_DO},
  {"int", TOKEN_INT},
  {"float", TOKEN_FLOAT},
  {"ptr", TOKEN_PTR},
};

static char keyword_token(const std::string& token){
  int i;
  for (i = 0; i < sizeof(keywords) / sizeof(Keyword); i++){
    if (keywords[i].name == token){
      return keywords[i].token;
    }
  }
  return TOKEN_IDENT;
}

void Tokenizer::parse_number(){
  // float constant contains '.'
}

void Tokenizer::next_token() {
  int ch = stream.get();

  if (ch.eof()){
    token = TOKEN_EOF;
    return;
  }

  // TODO: skip whitespace

  if (isdigit(ch)){
    text = "";
    while (ch = stream.get && (isdigit(ch) || ch == '.')){
      text += ch;
    }
    parse_number();
    return;
  }
  if (isalpha(ch)){
    text = "";
    while (ch = stream.get && (isalpha(ch) || isdigit(ch) || ch == '_' || ch == '$')){
      text += ch;
    }
    token = keyword_token(text);
    return;
  }
  if (ch == '"'){
    // string literal
    return;
  }
  if (ch == '\''){
    // char literal
    return;
  }

  switch (ch){
  case '+':
  case '-':
  case '*':
  case '/':
  case '(':
  case ')':
  case '{':
  case '}':
    token = ch;
    return;
  case '=':
    ch = stream.get();
    if (ch == '='){
      token = TOKEN_EQUAL;
      return;
    }
    stream.unget();
    token = '=';
    return;
  }
}
