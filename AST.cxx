#include "AST.hxx"
#include "exceptions.hxx"
#include <iostream>
#include <cstdlib>

#include "llvm/BasicBlock.h"

using namespace ncc;

static void print_indent(std::ostream& stream, int indent){
  while (indent){
    indent--;
    stream.put(' ');
  }
}

static std::string type_name(ValueType type){
  switch (type){
  case TYPE_INTEGER:
    return "int";
  case TYPE_DOUBLE:
    return "float";
  case TYPE_POINTER:
    return "ptr";
  case TYPE_VOID:
    return "<void>";
  default:
    return "<unknown>";
  }
}

static const llvm::Type* llvm_type(ValueType type){
  switch (type){
  case TYPE_INTEGER:
    return llvm::Type::Int32Ty;
  case TYPE_DOUBLE:
    return llvm::Type::DoubleTy;
  case TYPE_POINTER:
    return llvm::PointerType::getUnqual(llvm::OpaqueType::get());
  default:
    abort();
  }
}

static ValueType coerce_type(ValueType left, ValueType right){
  if (left == right){
    return left;
  }
  if (left == TYPE_DOUBLE && right == TYPE_INTEGER){
    return TYPE_DOUBLE;
  }
  if (right == TYPE_DOUBLE && left == TYPE_INTEGER){
    return TYPE_DOUBLE;
  }
  throw new IncompatibleTypes();
}

static llvm::Value* coerce_value(llvm::LLVMBuilder& builder,
                                 llvm::Value* val, 
                                 ValueType vt, ValueType res){
  if (vt == res){
    return val;
  }
  if (vt == TYPE_DOUBLE && res == TYPE_INTEGER){
    return builder.CreateFPToSI(val, llvm_type(TYPE_INTEGER), "cti");
  }
  if (vt == TYPE_INTEGER && res == TYPE_DOUBLE){
    return builder.CreateSIToFP(val, llvm_type(TYPE_DOUBLE), "ctd");
  }
  throw new IncompatibleTypes();  
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
llvm::Value* BinaryOperation::generate(llvm::LLVMBuilder& builder,
                                       SymbolTable* st){
  llvm::Value* lv;
  llvm::Value* rv;
  llvm::Value* rt;
  ValueType type = coerce_type(left->get_type(st), right->get_type(st));

  if (type == TYPE_POINTER){
    throw new IncompatibleTypes();
  }

  lv = left->generate(builder, st);
  rv = right->generate(builder, st);

  if (op == BINOP_COMMA){
    return rv;
  }

  lv = coerce_value(builder, lv, left->get_type(st), type);
  rv = coerce_value(builder, rv, right->get_type(st), type);

  switch(op){
  case BINOP_ADD:
    rt = builder.CreateAdd(lv, rv, "bor");
    return rt;
  case BINOP_SUB:
    rt = builder.CreateSub(lv, rv, "bor");
    return rt;
  case BINOP_MUL:
    rt = builder.CreateMul(lv, rv, "bor");
    return rt;
  case BINOP_DIV:
    if (type == TYPE_DOUBLE){
      rt = builder.CreateFDiv(lv, rv, "bor");
    } else {
      rt = builder.CreateSDiv(lv, rv, "bor");
    }
    return rt;
  case BINOP_OR:
    if (type == TYPE_DOUBLE){
      throw new IncompatibleTypes();
    }
    rt = builder.CreateOr(lv, rv, "bor");
    return rt;
  case BINOP_AND:
    if (type == TYPE_DOUBLE){
      throw new IncompatibleTypes();
    }
    rt = builder.CreateAnd(lv, rv, "bor");
    return rt;
  case BINOP_XOR:
    if (type == TYPE_DOUBLE){
      throw new IncompatibleTypes();
    }
    rt = builder.CreateXor(lv, rv, "bor");
    return rt;
  case BINOP_EQ:
    if (type == TYPE_DOUBLE){
      rt = builder.CreateFCmpOEQ(lv, rv, "rt");
    } else {
      rt = builder.CreateICmpEQ(lv, rv, "rt");
    }
    rt = builder.CreateZExt(rt, llvm_type(TYPE_INTEGER), "bor");
    return rt;
  case BINOP_NEQ:
    if (type == TYPE_DOUBLE){
      rt = builder.CreateFCmpONE(lv, rv, "rt");
    } else {
      rt = builder.CreateICmpNE(lv, rv, "rt");
    }
    rt = builder.CreateZExt(rt, llvm_type(TYPE_INTEGER), "bor");
    return rt;
  case BINOP_GT:
    if (type == TYPE_DOUBLE){
      rt = builder.CreateFCmpOGT(lv, rv, "rt");
    } else {
      rt = builder.CreateICmpSGT(lv, rv, "rt");
    }
    rt = builder.CreateZExt(rt, llvm_type(TYPE_INTEGER), "bor");
    return rt;
  case BINOP_GTE:
    if (type == TYPE_DOUBLE){
      rt = builder.CreateFCmpOGE(lv, rv, "rt");
    } else {
      rt = builder.CreateICmpSGE(lv, rv, "rt");
    }
    rt = builder.CreateZExt(rt, llvm_type(TYPE_INTEGER), "bor");
    return rt;
  case BINOP_LT:
    if (type == TYPE_DOUBLE){
      rt = builder.CreateFCmpOLT(lv, rv, "rt");
    } else {
      rt = builder.CreateICmpSLT(lv, rv, "rt");
    }
    rt = builder.CreateZExt(rt, llvm_type(TYPE_INTEGER), "bor");
    return rt;
  case BINOP_LTE:
    if (type == TYPE_DOUBLE){
      rt = builder.CreateFCmpOLE(lv, rv, "rt");
    } else {
      rt = builder.CreateICmpSLE(lv, rv, "rt");
    }
    rt = builder.CreateZExt(rt, llvm_type(TYPE_INTEGER), "bor");
    return rt;
  case BINOP_COMMA:;/* not reached */
  }
  throw new FeatureNotImplemented("binop code generation");
}
ValueType BinaryOperation::get_type(SymbolTable* st){
  switch (op){
  case BINOP_COMMA:
    return right->get_type(st);
  case BINOP_EQ:
  case BINOP_NEQ:
  case BINOP_GT:
  case BINOP_LT:
  case BINOP_GTE:
  case BINOP_LTE:
    return TYPE_INTEGER;
  default:
    return coerce_type(left->get_type(st), right->get_type(st));
  }
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
llvm::Value* ShortCircuitOperation::generate(llvm::LLVMBuilder& builder,
                                             SymbolTable* st){
  throw new FeatureNotImplemented("scop code generation");
}
ValueType ShortCircuitOperation::get_type(SymbolTable* st){
  return TYPE_INTEGER;
}


ConditionalExpression::~ConditionalExpression(){
  delete cond;
  delete cons;
  delete alt;
}
void ConditionalExpression::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "ConditionalExpression" << std::endl;
  cond->print(stream, indent+2);
  cons->print(stream, indent+2);
  alt->print(stream, indent+2);
}
llvm::Value* ConditionalExpression::generate(llvm::LLVMBuilder& builder,
                                             SymbolTable* st){
  throw new FeatureNotImplemented("ternary code generation");
}
ValueType ConditionalExpression::get_type(SymbolTable* st){
  return TYPE_INTEGER;
}


Assignment::~Assignment(){
  delete value;
}
void Assignment::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "Assignment into " << variable << std::endl;
  value->print(stream, indent+2);
}
llvm::Value* Assignment::generate(llvm::LLVMBuilder& builder, 
                                  SymbolTable* st){
  llvm::Value* val = value->generate(builder, st);
  Variable& var = st->get_symbol(variable);
  ValueType val_type = value->get_type(st);

  if (val_type != var.get_type()){
    if (val_type == TYPE_INTEGER && var.get_type() == TYPE_DOUBLE){
      val = builder.CreateSIToFP(val, llvm_type(TYPE_DOUBLE));
    } else if (val_type == TYPE_DOUBLE && 
               var.get_type() == TYPE_INTEGER){
      val = builder.CreateFPToSI(val, llvm_type(TYPE_INTEGER));
    } else {
      throw new IncompatibleTypes();
    }
  }

  builder.CreateStore(val, var.get_address());
  return val;
}
ValueType Assignment::get_type(SymbolTable* st){
  return value->get_type(st);
}


UnaryOperation::~UnaryOperation(){
  delete expr;
}
void UnaryOperation::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "UnaryOperation -" << std::endl;
  expr->print(stream, indent+2);
}
llvm::Value* UnaryOperation::generate(llvm::LLVMBuilder& builder, 
                                      SymbolTable* st){
  throw new FeatureNotImplemented("unop code generation");
}
ValueType UnaryOperation::get_type(SymbolTable* st){
  return expr->get_type(st);
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
llvm::Value* FunCall::generate(llvm::LLVMBuilder& builder, 
                               SymbolTable* st){
  throw new FeatureNotImplemented("funcall code generation");
}
ValueType FunCall::get_type(SymbolTable* st){
  return TYPE_INTEGER;
}


VariableReference::~VariableReference(){}
void VariableReference::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "VariableReference " << name << std::endl;
}
llvm::Value* VariableReference::generate(llvm::LLVMBuilder& builder, 
                                         SymbolTable* st){
  Variable& v = st->get_symbol(name);

  return builder.CreateLoad(v.get_address(), name.c_str());
}
ValueType VariableReference::get_type(SymbolTable* st){
  return st->get_symbol(name).get_type();
}


IntegerLiteral::~IntegerLiteral(){}
void IntegerLiteral::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "IntegerLiteral " << value << std::endl;
}
llvm::Value* IntegerLiteral::generate(llvm::LLVMBuilder& builder, 
                                         SymbolTable* st){
  return llvm::ConstantInt::get(llvm::APInt(32, value, true));
}
ValueType IntegerLiteral::get_type(SymbolTable* st){
  return TYPE_INTEGER;
}
DoubleLiteral::~DoubleLiteral(){}
void DoubleLiteral::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "DoubleLiteral " << value << std::endl;
}
llvm::Value* DoubleLiteral::generate(llvm::LLVMBuilder& builder, 
                                         SymbolTable* st){
  return llvm::ConstantFP::get(llvm::Type::DoubleTy, 
                               llvm::APFloat(value));
}
ValueType DoubleLiteral::get_type(SymbolTable* st){
  return TYPE_DOUBLE;
}

StringLiteral::~StringLiteral(){}
void StringLiteral::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "StringLiteral " << value << std::endl;
}
llvm::Value* StringLiteral::generate(llvm::LLVMBuilder& builder, 
                                     SymbolTable* st){
  return NULL;
}
ValueType StringLiteral::get_type(SymbolTable* st){
  return TYPE_POINTER;
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
llvm::Value* Block::generate(llvm::LLVMBuilder& builder, 
                             SymbolTable* st){
  for (StatementVector::iterator i = statements.begin();
       i != statements.end(); i++){
    (*i)->generate(builder, st);
  }
  return NULL;
}

ConditionalStatement::~ConditionalStatement(){
  delete cond;
  delete cons;
  if (alt) {
    delete alt;
  }
}
void ConditionalStatement::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "ConditionalStatement" << std::endl;
  cond->print(stream, indent+2);
  cons->print(stream, indent+2);
  if (alt) {
    alt->print(stream, indent+2);
  }
}
llvm::Value* ConditionalStatement::generate(llvm::LLVMBuilder& builder, 
                                            SymbolTable* st){
  throw new FeatureNotImplemented("cond code generation");
}


ReturnStatement::~ReturnStatement(){
  delete expr;
}
llvm::Value* ReturnStatement::generate(llvm::LLVMBuilder& builder, 
                                       SymbolTable* st){
  llvm::Value* ret = expr->generate(builder, st);
  ret = coerce_value(builder, ret, expr->get_type(st), st->get_lex_rtype());
  builder.CreateRet(ret);
  return NULL;
}
void ReturnStatement::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "ReturnStatement" << std::endl;
  expr->print(stream, indent+2);
}

WhileStatement::~WhileStatement(){
  delete cond;
  delete body;
}
void WhileStatement::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "WhileStatement" << std::endl;
  cond->print(stream, indent+2);
  body->print(stream, indent+2);
}
llvm::Value* WhileStatement::generate(llvm::LLVMBuilder& builder, 
                                       SymbolTable* st){
  throw new FeatureNotImplemented("while code generation");
}


TopLevelForm::~TopLevelForm(){}


LocalVariable::~LocalVariable(){
  if (value){
    delete value;
  }
}
void LocalVariable::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "LocalVariable " << type_name(type) << " " << name <<  std::endl; 
  if (value){
    value->print(stream, indent+2);
  }
}
llvm::Value* LocalVariable::generate(llvm::LLVMBuilder& builder, 
                                     SymbolTable* st){
  throw new FeatureNotImplemented("lvar");
}


GlobalVariable::~GlobalVariable(){
  if (value){
    delete value;
  }
}
void GlobalVariable::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "GlobalVariable " << type_name(type) << " " << name << std::endl; 
  if (value){
    value->print(stream, indent+2);
  }
}
void GlobalVariable::generate(llvm::Module* module,
                              SymbolTable* st){
  llvm::GlobalVariable* gv;
  if (value){
    throw new FeatureNotImplemented("global initializers");
  }
  
  gv = new llvm::GlobalVariable(llvm_type(type),
                                false,
                                llvm::GlobalValue::ExternalLinkage,
                                llvm::UndefValue::get(llvm_type(type)),
                                name,
                                module);

  st->put_symbol(name, Variable(gv, type));
}

void Argument::print(std::ostream& stream, int indent){
  print_indent(stream, indent);
  stream << "Argument " << type_name(type) << " " << name << std::endl; 
}

FunctionDeclaration::~FunctionDeclaration(){
  for (ArgumentVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    delete *i;
  }
}
void FunctionDeclaration::print(std::ostream& stream, int indent){
  stream << "FunctionDeclaration " << name << std::endl; 
  for (ArgumentVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    (*i)->print(stream, indent+2);
  }
}
void FunctionDeclaration::generate(llvm::Module* module,
                                   SymbolTable* st){
  std::vector<const llvm::Type*> arg_types;
  for (ArgumentVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    arg_types.push_back(llvm_type((*i)->get_type()));
  }

  llvm::FunctionType* t = llvm::FunctionType::get(llvm_type(type), 
                                                  arg_types, 
                                                  false);
  llvm::Function* f = 
    new llvm::Function(t, 
                       llvm::GlobalValue::ExternalLinkage,
                       name,
                       module);

  llvm::Function::arg_iterator j = f->arg_begin();
  for (ArgumentVector::iterator i = arguments.begin();
       i != arguments.end(); i++, j++){
    j->setName((*i)->get_name());
  }
}


FunctionDefinition::~FunctionDefinition(){
  delete contents;
}
void FunctionDefinition::print(std::ostream& stream, int indent){
  stream << "FunctionDefinition " << name << std::endl; 
  stream << "  Arguments" << std::endl; 
  for (ArgumentVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    (*i)->print(stream, indent+4);
  }
  contents->print(stream, indent+2);
}
void FunctionDefinition::generate(llvm::Module* module,
                                  SymbolTable* st){
  std::vector<const llvm::Type*> arg_types;
  for (ArgumentVector::iterator i = arguments.begin();
       i != arguments.end(); i++){
    arg_types.push_back(llvm_type((*i)->get_type()));
  }

  llvm::FunctionType* t = llvm::FunctionType::get(llvm_type(type), 
                                                  arg_types, 
                                                  false);
  llvm::Function* f = 
    new llvm::Function(t, 
                       llvm::GlobalValue::ExternalLinkage,
                       name,
                       module);
  SymbolTable fst(st, type);
  llvm::BasicBlock* entry = new llvm::BasicBlock("entry", f);
  llvm::LLVMBuilder builder(entry);

  llvm::Function::arg_iterator j = f->arg_begin();
  for (ArgumentVector::iterator i = arguments.begin();
       i != arguments.end(); i++, j++){
    llvm::Value* ptr;
    j->setName((*i)->get_name());
    ptr = builder.CreateAlloca(llvm_type((*i)->get_type()), 
                               0, 
                               (*i)->get_name().c_str());
    builder.CreateStore(j, ptr);
    fst.put_symbol((*i)->get_name(), Variable(ptr, (*i)->get_type()));
  }


  contents->generate(builder, &fst);

  builder.CreateRet(llvm::UndefValue::get(llvm_type(type)));
}
