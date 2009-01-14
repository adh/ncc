#include "token.hxx"
#include "exceptions.hxx"
#include <cctype>
#include <cstdlib>

using namespace NCC;

struct TokenName {
  char token;
  const char* name;
};

#define TOKEN_NAME(x) {TOKEN_ ## x, #x}
static TokenName token_names[] = {
  TOKEN_NAME(EOF),
  TOKEN_NAME(INT_VALUE),
  TOKEN_NAME(FLOAT_VALUE),
  TOKEN_NAME(STRING),
  TOKEN_NAME(IDENT),
  TOKEN_NAME(IF),
  TOKEN_NAME(ELSE),
  TOKEN_NAME(WHILE),
  TOKEN_NAME(FOR),
  TOKEN_NAME(DO),
  TOKEN_NAME(INT),
  TOKEN_NAME(FLOAT),
  TOKEN_NAME(PTR),
  TOKEN_NAME(RETURN),
  TOKEN_NAME(EQUAL),
};

std::string NCC::get_token_name(char token){
  int i;
  for (i = 0; i < sizeof(token_names) / sizeof(TokenName); i++){
    if (token_names[i].token == token){
      return token_names[i].name;
    }
  }
  std::string s;
  s = token;
  return s;
}

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
  {"return", TOKEN_RETURN},
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
  char* str;
  str = (char*)text.c_str();
  if (text.find('.') != std::string::npos){
    token = TOKEN_FLOAT_VALUE;
    float_value = strtod(str, &str);
  } else {
    token = TOKEN_INT_VALUE;
    int_value = strtol(str, &str, 10);
  }
  if (*str != '\0'){
    //    throw new InvalidToken();
  }
}

int Tokenizer::read_char(bool eof_ok) {
  int ch = stream.get();

  if (ch == '\n') {
    line++;
    column = 0;
  } else {
    column++;
  }

  if (!stream.good()){
    if (eof_ok && stream.eof()){
      return EOF;
    }
    throw new InvalidToken();
  }

  return ch;
}

void Tokenizer::unread_char(){
  if (column == 0){
    line--;
  } else {
    column--;
  }
  stream.unget();
}

void Tokenizer::skip_line_comment(){
  int ch;
  do {
    ch = read_char(false);
  } while (ch != '\n' && ch != '\r');      
}
void Tokenizer::skip_comment(){
  int ch;
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

char Tokenizer::read_char_escape(){
  int ch = read_char(false);
  switch (ch){
  case 'n':
    return '\n';
  case 'r':
    return '\r';
  case 't':
    return '\t';
  case 'a':
    return '\a';
  case 'b':
    return '\b';
  case 'v':
    return '\v';
  case 'f':
    return '\f';
    // TODO: hex and octal
  default:
    return ch;
  }
}

void Tokenizer::parse_string(){
  int ch;
  text = "";
  for (;;){
    token = TOKEN_STRING;
    ch = read_char(false);
    switch (ch){
    case '"':
      return;
    case '\\':
      text += read_char_escape();
      break;
    default:
      text += ch;
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
        unread_char();
        ch = '/';
        break;
      }
    }
  } while (ch == ' ' | ch == '\n' | ch == '\t' | ch == '\r');

  if (isdigit(ch)){
    text = ch;
    while (ch = read_char(true), (isdigit(ch) || ch == '.')){
      text += ch;
    }
    unread_char();
    parse_number();
    return;
  }
  if (isalpha(ch)){
    text = ch;
    while (ch = read_char(true), (isalpha(ch) || isdigit(ch) || ch == '_' || ch == '$')){
      text += ch;
    }
    unread_char();
    token = keyword_token(text);
    return;
  }
  if (ch == '"'){
    parse_string();
    return;
  }
  if (ch == '\''){
    ch = read_char(false);
    if (ch == '\\'){
      ch = read_char_escape();
    }
    int_value = ch;
    token = TOKEN_INT_VALUE;
    ch = read_char(false);
    if (ch != '\''){
      throw new InvalidToken();
    }
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
  case ';':
  case ',':
    token = ch;
    return;
  case '=':
    ch = read_char(true);
    if (ch == '='){
      token = TOKEN_EQUAL;
      return;
    }
    unread_char();
    token = '=';
    return;
  default:
    throw new InvalidToken();
  }
}
