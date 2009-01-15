#include "parse.hxx"
#include "exceptions.hxx"
#include <iostream>

using namespace NCC;

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

FunctionDeclaration* Parser::parse_function(ValueType return_type, const std::string& name){
  VariableVector arguments;
  ValueType a_type;
  Block* b;
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
    b = parse_block();
    return new FunctionDefinition(return_type, name, arguments, b);
  default:
    throw new UnexpectedToken(tok.current_token());
  }
  
}

VariableDefinition* Parser::parse_initializer(ValueType return_type, const std::string& name){
  Expression* e;
  tok.next_token();
  e = parse_ternary();
  tok.eat_token(';');
  return new VariableDefinition(return_type, name, e);
}

Expression* Parser::parse_funcall(const std::string& ident){
  ExpressionVector arguments;

  tok.eat_token('(');
  if (tok.current_token() != ')'){
    for(;;) {
      arguments.push_back(parse_assign());
      if (tok.current_token() == ')'){
        break;
      }
      tok.eat_token(',');
    }
  }
  tok.next_token();

  return new FunCall(ident, arguments);
}

Expression* Parser::parse_value(){
  Expression* e;
  std::string ident;

  switch (tok.current_token()){
  case '(':
    tok.next_token();
    e = parse_comma();
    tok.eat_token(')');
    return e;
  case TOKEN_IDENT:
    ident = tok.get_text();
    tok.next_token();
    if (tok.current_token() == '('){
      return parse_funcall(ident);
    }
    return new VariableReference(ident);
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
Expression* Parser::parse_unary(){
  switch (tok.current_token()){
  case '-':
    tok.next_token();
    return new UnaryOperation(parse_unary(), UNOP_INV);
  case '~':
    tok.next_token();
    return new UnaryOperation(parse_unary(), UNOP_NOT);
  case '!':
    tok.next_token();
    return new UnaryOperation(parse_unary(), UNOP_LOG_NOT);
  default:
    return parse_value();
  }
}
Expression* Parser::parse_multiplicative(){
  Expression* e;
  Expression* r;
  r = parse_unary();    
  for(;;){
    switch(tok.current_token()){
    case '*':
      tok.next_token();
      e = parse_unary();
      r = new BinaryOperation(r, e, BINOP_MUL);
      break;
    case '/':
      tok.next_token();
      e = parse_unary();
      r = new BinaryOperation(r, e, BINOP_DIV);
      break;
    default:
      return r;
    }
  }
}
Expression* Parser::parse_additive(){
  Expression* e;
  Expression* r;
  r = parse_multiplicative();    
  for(;;){
    switch(tok.current_token()){
    case '+':
      tok.next_token();
      e = parse_multiplicative();
      r = new BinaryOperation(r, e, BINOP_ADD);
      break;
    case '-':
      tok.next_token();
      e = parse_multiplicative();
      r = new BinaryOperation(r, e, BINOP_SUB);
      break;
    default:
      return r;
    }
  }
}
Expression* Parser::parse_comparison(){
  Expression* e;
  Expression* r;
  r = parse_additive();    
  for(;;){
    switch(tok.current_token()){
    case TOKEN_EQUAL:
      tok.next_token();
      e = parse_additive();
      r = new BinaryOperation(r, e, BINOP_EQ);
      break;
    case TOKEN_NOT_EQUAL:
      tok.next_token();
      e = parse_additive();
      r = new BinaryOperation(r, e, BINOP_NEQ);
      break;
    default:
      return r;
    }
  }
}
Expression* Parser::parse_bitwise(){
  Expression* e;
  Expression* r;
  r = parse_comparison();    
  for(;;){
    switch(tok.current_token()){
    case '&':
      tok.next_token();
      e = parse_comparison();
      r = new BinaryOperation(r, e, BINOP_AND);
      break;
    case '|':
      tok.next_token();
      e = parse_comparison();
      r = new BinaryOperation(r, e, BINOP_OR);
      break;
    case '^':
      tok.next_token();
      e = parse_comparison();
      r = new BinaryOperation(r, e, BINOP_XOR);
      break;
    default:
      return r;
    }
  }
}
Expression* Parser::parse_logic(){
  Expression* e;
  Expression* r;
  r = parse_bitwise();    
  for(;;){
    switch(tok.current_token()){
    case TOKEN_SC_AND:
      tok.next_token();
      e = parse_bitwise();
      r = new ShortCircuitOperation(r, e, SCOP_AND);
      break;
    case TOKEN_SC_OR:
      tok.next_token();
      e = parse_bitwise();
      r = new ShortCircuitOperation(r, e, SCOP_OR);
      break;
    default:
      return r;
    }
  }
}
Expression* Parser::parse_ternary(){
  Expression* c;
  Expression* a;
  Expression* r;
  r = parse_logic();    
  if (tok.current_token() == '?'){
    tok.next_token();
    c = parse_ternary();
    tok.eat_token(':');
    a = parse_ternary();
    return new ConditionalExpression(r, c, a);
  }
  return r;
}
Expression* Parser::parse_assign(){
  Expression* r;
  Expression* v;
  r = parse_ternary();
  if (tok.current_token() == '='){
    VariableReference* n = dynamic_cast<VariableReference*>(r);
    if (!n){
      throw "l-value expected";
    }
    tok.next_token();
    v = parse_assign();
    r = new Assignment(n->get_name(), ASOP_ASSIGN, v);
    delete n;
  }
  return r;
}
Expression* Parser::parse_comma(){
  Expression* e;
  Expression* r;
  r = parse_assign();    
  for(;;){
    switch(tok.current_token()){
    case ',':
      tok.next_token();
      e = parse_assign();
      r = new BinaryOperation(r, e, BINOP_COMMA);
      break;
    default:
      return r;
    }
  }
}
Block* Parser::parse_block(){
  StatementVector v;
  Statement* s;
  tok.eat_token('{');
  while (tok.current_token() != '}'){
    if (tok.current_token() == ';'){
      tok.next_token();
      continue;
    }
    s = parse_statement();
    v.push_back(s);
  }
  return new Block(v); 
}
Statement* Parser::parse_statement(){
  Statement* s;

  switch (tok.current_token()){
  default:
    s = parse_comma();
    tok.eat_token(';');
    return s;
  }
}

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
  default:
    throw new UnexpectedToken(tok.current_token());
  }
}
