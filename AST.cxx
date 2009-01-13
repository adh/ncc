#include "AST.hxx"

using namespace NCC;

ASTNode::~ASTNode(){}
Statement::~Statement(){}
Expression::~Expression(){}

BinaryOperation::~BinaryOperation(){
  delete left;
  delete right;
}
Assignment::~Assignment(){
  delete value;
}
UnaryOperation::~UnaryOperation(){
  delete expr;
}
FunCall::~FunCall(){
  for (ExpressionVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    delete *i;
  }
}
IntegerLiteral::~IntegerLiteral(){}
DoubleLiteral::~DoubleLiteral(){}
StringLiteral::~StringLiteral(){}

