#include "AST.hxx"

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
  print_indent(indent);
  stream << "BinaryOperation" << std::endl;
  left->print(stream, indent+2);
  print_indent(indent);
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

Block::~Block() {
  for (StatementVector::iterator i = statements.begin();
       i != statements.end(); i++){
    delete *i;
  }
}

TopLevelForm::~TopLevelForm(){}
VariableDeclaration::~VariableDeclaration(){}
FunctionDeclaration::~FunctionDeclaration(){
  for (VariableVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    delete *i;
  }
}
FunctionDefinition::~FunctionDefinition(){
  delete contents;
}
