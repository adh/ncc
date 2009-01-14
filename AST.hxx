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
    BinaryOperator op;
    Expression* right;
  public:
    BinaryOperation(Expression* left, Expression* right, BinaryOperator op):
      left(left), right(right), op(op) {};
    virtual ~BinaryOperation();
    virtual void print(std::ostream& stream, int indent);
  };

  class ShortCircuitOperation : public Expression {
  protected:
    Expression* left;
    ShortCircuitOperator op;
    Expression* right;
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
    Expression* value;
    AssignmentOperator op;
    std::string variable;
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

  class TopLevelForm : public ASTNode{
  public:
    virtual ~TopLevelForm();
  };
  class VariableDeclaration : public TopLevelForm {
  protected:
    ValueType type;
    std::string name;
  public:
    VariableDeclaration(ValueType type, const std::string& name):
      type(type), name(name) {}
    virtual ~VariableDeclaration();
    virtual void print(std::ostream& stream, int indent);
  };
  class VariableDefinition : public VariableDeclaration {
  protected:
    Expression* value;
  public:
    VariableDefinition(ValueType type, const std::string& name, 
                       Expression* value): 
      VariableDeclaration(type, name), value(value) {}
    virtual ~VariableDefinition();    
    virtual void print(std::ostream& stream, int indent);
  };
  typedef std::vector<VariableDeclaration*> VariableVector;
  class FunctionDeclaration : public TopLevelForm {
  protected:
    ValueType type;
    std::string name;
    VariableVector arguments;
  public:
    FunctionDeclaration(ValueType type, std::string name, VariableVector arguments):
      type(type), name(name), arguments(arguments) {};
    virtual ~FunctionDeclaration();
    virtual void print(std::ostream& stream, int indent);
  };
  class FunctionDefinition : public FunctionDeclaration {
  protected:
    Block* contents;
  public:
    virtual ~FunctionDefinition();
    virtual void print(std::ostream& stream, int indent);
  }; 
}

#endif
