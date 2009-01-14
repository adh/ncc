#include "AST.hxx"
#include <iostream>

using namespace NCC;

static void print_indent(std::ostream& stream, int indent){
  while (indent){
    indent--;
    stream.put(' ');
  }
}

#define NAME(x) #x
static const char* binop_names[] = {
  NAME(BINOP_ADD),
  NAME(BINOP_MUL),
  NAME(BINOP_SUB),
  NAME(BINOP_DIV),
  NAME(BINOP_OR),
  NAME(BINOP_AND),
  NAME(BINOP_XOR),
  NAME(BINOP_EQ),
  NAME(BINOP_NEQ),
  NAME(BINOP_GT),
  NAME(BINOP_LT),
  NAME(BINOP_GTE),
  NAME(BINOP_LTE),
  NAME(BINOP_COMMA)
};


ASTNode::~ASTNode(){}
Statement::~Statement(){}
Expression::~Expression(){}

BinaryOperation::~BinaryOperation(){
  delete left;
  delete right;
}
void BinaryOperation::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "BinaryOperation " << binop_names[op] << std::endl;
  left->print(stream, indent+2);
  right->print(stream, indent+2);
}
ShortCircuitOperation::~ShortCircuitOperation(){
  delete left;
  delete right;
}
void ShortCircuitOperation::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "ShortCircuitOperation ";
  switch (op){
  case SCOP_AND:
    stream << "&&"<< std::endl;
    break;
  case SCOP_OR:
    stream << "||"<< std::endl;
    break;
  }
  left->print(stream, indent+2);
  right->print(stream, indent+2);
}

ConditionalExpression::~ConditionalExpression(){
  delete cond;
  delete cons;
  delete alt;
}
void ConditionalExpression::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "ConditionalExpression";
  cond->print(stream, indent+2);
  cons->print(stream, indent+2);
  alt->print(stream, indent+2);
}


Assignment::~Assignment(){
  delete value;
}
void Assignment::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "Assignment into " << variable << std::endl;
  value->print(stream, indent+2);
}
UnaryOperation::~UnaryOperation(){
  delete expr;
}
void UnaryOperation::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "UnaryOperation -" << std::endl;
  expr->print(stream, indent+2);
}
FunCall::~FunCall(){
  for (ExpressionVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    delete *i;
  }
}
void FunCall::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "FunCall " << function << std::endl;
  for (ExpressionVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    (*i)->print(stream, indent+2);
  }
}
VariableReference::~VariableReference(){}
void VariableReference::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "VariableReference " << name << std::endl;
}
IntegerLiteral::~IntegerLiteral(){}
void IntegerLiteral::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "IntegerLiteral " << value << std::endl;
}
DoubleLiteral::~DoubleLiteral(){}
void DoubleLiteral::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "IntegerLiteral " << value << std::endl;
}
StringLiteral::~StringLiteral(){}
void StringLiteral::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "IntegerLiteral " << value << std::endl;
}

Block::~Block() {
  for (StatementVector::iterator i = statements.begin();
       i != statements.end(); i++){
    delete *i;
  }
}
void Block::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "Block" << std::endl;
  for (StatementVector::iterator i = statements.begin();
       i != statements.end(); i++){
    (*i)->print(stream, indent+2);
  }
}

TopLevelForm::~TopLevelForm(){}

VariableDeclaration::~VariableDeclaration(){}
void VariableDeclaration::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "VariableDeclaration " << name << std::endl; 
}

VariableDefinition::~VariableDefinition(){}
void VariableDefinition::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "VariableDefinition " << name << std::endl; 
  if (value){
    value->print(stream, indent+2);
  } else {
    print_indent(stream, indent+2);
    stream << "Void" << std::endl;     
  }
}

FunctionDeclaration::~FunctionDeclaration(){
  for (VariableVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    delete *i;
  }
}
void FunctionDeclaration::print(std::ostream& stream, int indent){
  stream << "FunctionDeclaration " << name << std::endl; 
  for (VariableVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    (*i)->print(stream, indent+2);
  }
}
FunctionDefinition::~FunctionDefinition(){
  delete contents;
}
void FunctionDefinition::print(std::ostream& stream, int indent){
  stream << "FunctionDefinition " << name << std::endl; 
  stream << "  Arguments" << std::endl; 
  for (VariableVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    (*i)->print(stream, indent+4);
  }
  contents->print(stream, indent+2);
}
