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

  class TreeNode {
  public:
    virtual ~TreeNode();
  };

  class Expression {
  public:
    virtual ~Expression();
    virtual bool is_constant();
    virtual ValueType type();
  };
    
  class BinaryOperation : public Expression {
  protected:
    Expression* left;
    BinaryOperator op;
    Expression* right;
  public:
    BinaryOperation(Expression* left, Expression* right, BinaryOperator op):
      left(left), right(right), op(op) {};
    virtual ~BinaryOperation();
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
    Expression* function;
    std::vector<Expression *> arguments;
  public:
    virtual ~FunCall();
  };

  class IntegerLiteral : public Expression {
  protected:
    int value;
  public:

  };

  class DoubleLiteral : public Expression {
  protected:
    double value;
  public:
    
  };

  class StringLiteral : public Expression {
  protected:
    std::string value;
  public:
    
  };

  class Statement : public TreeNode {
    
  };

  class ExpressionStatement : public Statement {

  };
  
  class VariableDeclaration : public TreeNode {
  protected:
    ValueType type;
    
  };

  class Block : public Statement {
  protected:
    std::vector<Statement *> statements;
  public:
  };

}

