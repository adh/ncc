#include "parse.hxx"
#include "exceptions.hxx"
#include <iostream>

using namespace NCC;

/* type ::= 'int' | 'float' | 'ptr' */
ValueType Parser::parse_type(){
  switch(tok.current_token()){
  case TOKEN_INT:
    return TYPE_INTEGER;
  case TOKEN_FLOAT:
    return TYPE_DOUBLE;
  case TOKEN_PTR:
    return TYPE_POINTER;
  default:
    throw new UnexpectedToken(tok.current_token());
  }
}

/*
 * function-definition ::= function-prototype block
 * function-declaration ::= function-prototype ';'
 * function-prototype ::= type IDENTIFIER '(' ( type IDENTIFIER ( ',' type IDENTIFIER)* )? ')'
 */
FunctionDeclaration* Parser::parse_function(ValueType return_type, const std::string& name){
  VariableVector arguments;
  ValueType a_type;
  std::string a_name;

  tok.next_token();
  if (tok.current_token() != ')'){
    for(;;) {
      a_type = parse_type();
      tok.next_token_expect(TOKEN_IDENT);
      a_name = tok.get_text();
      arguments.push_back(new VariableDeclaration(a_type, a_name));
      tok.next_token();
      if (tok.current_token() == ')'){
        break;
      }
      tok.eat_token(',');
    }
  }
  
  tok.next_token();
  switch (tok.current_token()){
  case ';':
    return new FunctionDeclaration(return_type, name, arguments);
  case '{':
    throw "unimplemented";
    break;
  default:
    throw new UnexpectedToken(tok.current_token());
  }
  
}

VariableDefinition* Parser::parse_initializer(ValueType return_type, const std::string& name){
  Expression* e;
  tok.next_token();
  e = parse_expression();
  tok.eat_token(';');
  return new VariableDefinition(return_type, name, e);
}

/*
 * expression ::= expression '*' factor | expression '/' factor | factor
 * factor ::= factor '+' addend | factor '-' addend | addend | '-' addend
 * addend ::= '(' expression ') | IDENTIFIER | FLOAT | INT
 */

Expression* Parser::parse_expression(){
  Expression* e;
  e = parse_factor();
  return e;
}
Expression* Parser::parse_factor(){
  Expression* e;
  e = parse_addend();
  return e;
}
Expression* Parser::parse_addend(){
  Expression* e;

  switch (tok.current_token()){
  case '(':
    tok.next_token();
    e = parse_expression();
    tok.eat_token(')');
    return e;
  case TOKEN_IDENT:
    e = new VariableReference(tok.get_text());
    break;
  case TOKEN_FLOAT_VALUE:
    e = new DoubleLiteral(tok.get_float_value());
    break;
  case TOKEN_INT_VALUE:
    e = new IntegerLiteral(tok.get_int_value());
    break;
  case TOKEN_STRING:
    e = new StringLiteral(tok.get_text());
    break;
  }
  tok.next_token();
  return e;
}



/*
 * compilation-unit ::= ( global-variable | function-definition | function-declaration )*
 * global-variable ::= type IDENTIFIER ( '=' expression )?
 *
 * function-definition ::= function-prototype block
 * function-declaration ::= function-prototype ';'
 * function-prototype ::= type IDENTIFIER '(' ( type IDENTIFIER ( ',' type IDENTIFIER)* )? ')'
 */
TopLevelForm* Parser::read_toplevel(){
  ValueType type;
  std::string ident;
  if (tok.current_token() == TOKEN_EOF){
    return NULL;
  }
  type = parse_type();
  tok.next_token_expect(TOKEN_IDENT);
  ident = tok.get_text();
  tok.next_token();
  switch (tok.current_token()){
  case ';':
    return new VariableDefinition(type, ident, NULL);
  case '(':
    return parse_function(type, ident);
  case '=':
    return parse_initializer(type, ident);
  }
}
