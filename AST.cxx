#include "AST.hxx"
#include <iostream>

using namespace NCC;

static void print_indent(std::ostream& stream, int indent){
  while (indent){
    indent--;
    stream.put(' ');
  }
}

ASTNode::~ASTNode(){}
Statement::~Statement(){}
Expression::~Expression(){}

BinaryOperation::~BinaryOperation(){
  delete left;
  delete right;
}
void BinaryOperation::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "BinaryOperation ";
  switch (op){
  case BINOP_ADD:
    stream << "+"<< std::endl;
    break;
  case BINOP_SUB:
    stream << "-"<< std::endl;
    break;
  case BINOP_MUL:
    stream << "-"<< std::endl;
    break;
  case BINOP_DIV:
    stream << "/"<< std::endl;
    break;
  }
  right->print(stream, indent+2);
  left->print(stream, indent+2);
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
