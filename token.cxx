#include "token.hxx"
#include "exceptions.hxx"
#include <cctype>

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

int Tokenizer::read_char(bool eof_ok) {
  int ch = stream.get();
  if (!stream.good()){
    if (eof_ok && stream.eof()){
      return EOF;
    }
    throw new InvalidToken();
  }
  return ch;
}

void Tokenizer::skip_line_comment(){
  int ch;
        std::cerr << "skip_line_comment" << std::endl;
  do {
    ch = read_char(false);
  } while (ch != '\n' && ch != '\r');      
}
void Tokenizer::skip_comment(){
  int ch;
        std::cerr << "skip_comment" << std::endl;
  for (;;) {
    ch = read_char(false);
    if (ch == '*'){
      ch = read_char(false);
      if (ch == '/'){
        break;
      }
    }
  }
}


void Tokenizer::next_token() {
  int ch;

  do {
  next:
    ch = read_char(true);
    if (ch == EOF){
      token = TOKEN_EOF;
      return;
    }
    if (ch == '/') {
      ch = read_char(false);
      if (ch == '/') {
        skip_line_comment();
        goto next;
      } else if (ch == '*') {
        skip_comment();
        goto next;
      } else {
        stream.unget();
        ch = '/';
        break;
      }
    }
  } while (ch == ' ' | ch == '\n' | ch == '\t' | ch == '\r');

  if (isdigit(ch)){
    text = "";
    while (ch = read_char(true) && (isdigit(ch) || ch == '.')){
      if (!stream.good()){
        
      }
      text += ch;
    }
    parse_number();
    return;
  }
  if (isalpha(ch)){
    text = "";
    while (ch = read_char(true) && (isalpha(ch) || isdigit(ch) || ch == '_' || ch == '$')){
      text += ch;
    }
    token = keyword_token(text);
    return;
  }
  if (ch == '"'){
    for (;;){
      ch = read_char();
      switch (ch){
      case '\\':
      case '"':
        token = TOKEN_STRING;
      default:
      }
      return;
    }
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
    ch = read_char(true);
    if (ch == '='){
      token = TOKEN_EQUAL;
      return;
    }
    stream.unget();
    token = '=';
    return;
  default:
    throw new InvalidToken();
  }
}
