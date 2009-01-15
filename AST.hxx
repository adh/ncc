#ifndef HXX__ncc__AST__
#define HXX__ncc__AST__
#include <vector>
#include <string>

namespace NCC {
  enum ValueType {
    TYPE_INTEGER,
    TYPE_DOUBLE,
    TYPE_POINTER
  };

  enum BinaryOperator {
    BINOP_ADD,
    BINOP_MUL,
    BINOP_SUB,
    BINOP_DIV,
    BINOP_OR,
    BINOP_AND,
    BINOP_XOR,
    BINOP_EQ,
    BINOP_NEQ,
    BINOP_GT,
    BINOP_LT,
    BINOP_GTE,
    BINOP_LTE,
    BINOP_COMMA
  };

  enum UnaryOperator {
    UNOP_INV,
    UNOP_NOT,
    UNOP_LOG_NOT
  };

  enum ShortCircuitOperator {
    SCOP_OR,
    SCOP_AND
  };

  enum AssignmentOperator {
    ASOP_ASSIGN
  };

  class ASTNode {
  public:
    virtual void print(std::ostream& stream, int indent) = 0;
    virtual ~ASTNode();
  };

  class Statement : public ASTNode{
  public:
    virtual ~Statement();
  };
  typedef std::vector<Statement *> StatementVector;

  class Expression : public Statement{
  public:
    virtual ~Expression();
    //    virtual bool is_constant() = 0;
    //    virtual ValueType type() = 0;
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
  };

  class UnaryOperation : public Expression {
  protected:
    Expression* expr;
    UnaryOperator op;
  public:
    UnaryOperation(Expression* e, UnaryOperator op): expr(e), op(op) {};
    virtual ~UnaryOperation();
    virtual void print(std::ostream& stream, int indent);
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
  };

  class IntegerLiteral : public Expression {
  protected:
    int value;
  public:
    IntegerLiteral(int value):value(value){}
    virtual ~IntegerLiteral();
    virtual void print(std::ostream& stream, int indent);
  };

  class DoubleLiteral : public Expression {
  protected:
    double value;
  public:
    DoubleLiteral(double value): value(value) {}
    virtual ~DoubleLiteral();
    virtual void print(std::ostream& stream, int indent);
  };

  class StringLiteral : public Expression {
  protected:
    std::string value;
  public:
    StringLiteral(const std::string& value): value(value) {}
    virtual ~StringLiteral();
    virtual void print(std::ostream& stream, int indent);
  };
  
  class Block : public Statement {
  protected:
    StatementVector statements;
  public:
    Block(const StatementVector& statements): statements(statements) {}
    virtual ~Block();
    virtual void print(std::ostream& stream, int indent);
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
  };

  class ReturnStatement : public Statement {
  protected:
    Expression* expr;
  public:
    ReturnStatement(Expression* expr): expr(expr) {};
    virtual ~ReturnStatement();
    virtual void print(std::ostream& stream, int indent);
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
  };

  class TopLevelForm : public ASTNode{
  public:
    virtual ~TopLevelForm();
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
  }; 
}

#endif
