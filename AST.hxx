#include <vector>
#include <string>

namespace NCC {
  enum ValueType {
    INTEGER,
    DOUBLE,
    POINTER
  };

  enum BinaryOperator {
    ADD,
    MUL,
    SUB,
    DIV    
  };

  enum UnaryOperator {
    INV
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
    virtual bool is_constant() = 0;
    virtual ValueType type() = 0;
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

  class Assignment : public Expression {
  protected:
    Expression* value;
    std::string variable;
  public:
    Assignment(const std::string& variable, Expression* value) : variable(variable), value(value) {}
    virtual ~Assignment();
  };

  class UnaryOperation : public Expression {
  protected:
    Expression* expr;
    UnaryOperator op;
  public:
    UnaryOperation(Expression* e, UnaryOperator op): expr(e), op(op) {};
    virtual ~UnaryOperation();
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
  };

  class IntegerLiteral : public Expression {
  protected:
    int value;
  public:
    IntegerLiteral(int value):value(value){}
    virtual ~IntegerLiteral();
  };

  class DoubleLiteral : public Expression {
  protected:
    double value;
  public:
    DoubleLiteral(double value): value(value) {}
    virtual ~DoubleLiteral();
  };

  class StringLiteral : public Expression {
  protected:
    std::string value;
  public:
    StringLiteral(const std::string& value): value(value) {}
    virtual ~StringLiteral();
  };
  
  class Block : public Statement {
  protected:
    StatementVector statements;
  public:
    Block(const StatementVector& statements): statements(statements) {}
    virtual ~Block();
  };

  class TopLevelForm {
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
  };
  typedef std::vector<VariableDeclaration*> VariableVector;
  class FunctionDeclaration : public TopLevelForm {
  protected:
    VariableVector arguments;
    std::string name;
  public:
    virtual ~FunctionDeclaration();
  };
  class FunctionDefinition : public FunctionDeclaration {
  protected:
    Block* contents;
  public:
    virtual ~FunctionDefinition();
  }; 
}

