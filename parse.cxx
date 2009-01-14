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
  ValueType a_type;
  std::string a_name;
  for (;;){
    tok.next_token();
    if (tok.current_token() == ')'){
      break;
    }
  }
  
  tok.next_token();
  switch (tok.current_token()){
  case ';':
    return NULL; //new FunctionDeclaration(return);
  case '{':
    break;
  default:
    throw new UnexpectedToken(tok.current_token());
  }
  
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
  tok.next_token();
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
    std::cerr << "Function" << std::endl;
    return parse_function(type, ident);
  case '=':
    std::cerr << "Initialized variable" << std::endl;
    break;
  }
}
