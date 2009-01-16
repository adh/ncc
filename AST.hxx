#ifndef HXX__ncc__AST__
#define HXX__ncc__AST__

#include "symbol.hxx"

#include "llvm/DerivedTypes.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/LLVMBuilder.h"

#include <vector>
#include <string>

namespace ncc {
  class ASTNode {
  public:
    virtual void print(std::ostream& stream, int indent) = 0;
    virtual ~ASTNode();
  };

  class Statement : public ASTNode{
  public:
    virtual ~Statement();
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st) = 0;
  };
  typedef std::vector<Statement *> StatementVector;

  class Expression : public Statement{
  public:
    virtual ~Expression();
    virtual ValueType get_type(SymbolTable* st) = 0;
  };
  typedef std::vector<Expression *> ExpressionVector;
    
  class BinaryOperation : public Expression {
  protected:
    Expression* left;
    Expression* right;
    BinaryOperator op;
  public:
    BinaryOperation(Expression* left, Expression* right, BinaryOperator op):
      left(left), right(right), op(op) {};
    virtual ~BinaryOperation();
    virtual void print(std::ostream& stream, int indent);

    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };

  class ShortCircuitOperation : public Expression {
  protected:
    Expression* left;
    Expression* right;
    ShortCircuitOperator op;
  public:
    ShortCircuitOperation(Expression* left, Expression* right, 
                          ShortCircuitOperator op):
      left(left), right(right), op(op) {};
    virtual ~ShortCircuitOperation();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };

  class ConditionalExpression : public Expression {
  protected:
    Expression* cond;
    Expression* cons;
    Expression* alt;
  public:
    ConditionalExpression(Expression* cond, Expression* cons, Expression* alt):
      cond(cond), cons(cons), alt(alt) {}
    virtual ~ConditionalExpression();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };

  class Assignment : public Expression {
  protected:
    std::string variable;
    AssignmentOperator op;
    Expression* value;
  public:
    Assignment(const std::string& variable, 
               AssignmentOperator op, 
               Expression* value) : variable(variable), op(op), value(value) {}
    virtual ~Assignment();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };

  class UnaryOperation : public Expression {
  protected:
    Expression* expr;
    UnaryOperator op;
  public:
    UnaryOperation(Expression* e, UnaryOperator op): expr(e), op(op) {};
    virtual ~UnaryOperation();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };

  class FunCall : public Expression {
  protected:
    std::string function;
    ExpressionVector arguments;
  public:
    FunCall(const std::string& function,     
            const ExpressionVector& arguments):
      function(function), arguments(arguments) {}

    virtual ~FunCall();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };

  class VariableReference : public Expression {
  protected:
    std::string name;
  public:
    VariableReference(const std::string& name) : name(name){}
    virtual ~VariableReference();
    virtual void print(std::ostream& stream, int indent);
    const std::string& get_name(){
      return name;
    }
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };

  class IntegerLiteral : public Expression {
  protected:
    int value;
  public:
    IntegerLiteral(int value):value(value){}
    virtual ~IntegerLiteral();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };

  class DoubleLiteral : public Expression {
  protected:
    double value;
  public:
    DoubleLiteral(double value): value(value) {}
    virtual ~DoubleLiteral();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };

  class StringLiteral : public Expression {
  protected:
    std::string value;
  public:
    StringLiteral(const std::string& value): value(value) {}
    virtual ~StringLiteral();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
    virtual ValueType get_type(SymbolTable* st);
  };
  
  class Block : public Statement {
  protected:
    StatementVector statements;
  public:
    Block(const StatementVector& statements): statements(statements) {}
    virtual ~Block();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
  };

  class ConditionalStatement : public Statement {
  protected:
    Expression* cond;
    Statement* cons;
    Statement* alt;
  public:
    ConditionalStatement(Expression* cond, Statement* cons, Statement* alt):
      cond(cond), cons(cons), alt(alt) {}
    virtual ~ConditionalStatement();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
  };

  class ReturnStatement : public Statement {
  protected:
    Expression* expr;
  public:
    ReturnStatement(Expression* expr): expr(expr) {};
    virtual ~ReturnStatement();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
  };
  class WhileStatement : public Statement {
  protected:
    Expression* cond;
    Statement* body;
  public:
    WhileStatement(Expression* cond, Statement* body):
      cond(cond), body(body) {}
    virtual ~WhileStatement();
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
  };

  class TopLevelForm : public ASTNode{
  public:
    virtual ~TopLevelForm();
    virtual void generate(llvm::Module* module,
                          SymbolTable* st) = 0;
  };
  class LocalVariable : public Statement {
  protected:
    ValueType type;
    std::string name;
    Expression* value;
  public:
    LocalVariable(ValueType type, const std::string& name, 
                  Expression* value): 
      type(type), name(name), value(value) {}
    virtual ~LocalVariable();    
    virtual void print(std::ostream& stream, int indent);
    virtual llvm::Value* generate(llvm::LLVMBuilder& builder,
                                  SymbolTable* st);
  };

  class GlobalVariable : public TopLevelForm {
  protected:
    ValueType type;
    std::string name;
    Expression* value;
  public:
    GlobalVariable(ValueType type, const std::string& name, 
                       Expression* value): 
      type(type), name(name), value(value) {}
    virtual ~GlobalVariable();    
    virtual void print(std::ostream& stream, int indent);
    virtual void generate(llvm::Module* module,
                          SymbolTable* st);
  };

  class Argument : public ASTNode {
  protected:
    ValueType type;
    std::string name;
  public:
    Argument(ValueType type, const std::string& name): 
      type(type), name(name) {}
    //virtual ~Argument();    
    virtual void print(std::ostream& stream, int indent);
    const std::string get_name(){
      return name;
    }
    ValueType get_type(){
      return type;
    }
  };
  typedef std::vector<Argument*> ArgumentVector;

  class FunctionDeclaration : public TopLevelForm {
  protected:
    ValueType type;
    std::string name;
    ArgumentVector arguments;
  public:
    FunctionDeclaration(ValueType type, std::string name, ArgumentVector arguments):
      type(type), name(name), arguments(arguments) {};
    virtual ~FunctionDeclaration();
    virtual void print(std::ostream& stream, int indent);
    virtual void generate(llvm::Module* module,
                          SymbolTable* st);
  };
  class FunctionDefinition : public FunctionDeclaration {
  protected:
    Block* contents;
  public:
    FunctionDefinition(ValueType type, std::string name, ArgumentVector arguments,
                       Block* contents): 
      FunctionDeclaration(type, name, arguments), contents(contents) {};
    virtual ~FunctionDefinition();
    virtual void print(std::ostream& stream, int indent);
    virtual void generate(llvm::Module* module,
                          SymbolTable* st);
  }; 
}

#endif
