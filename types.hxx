#ifndef HXX__ncc__types__
#define HXX__ncc__types__

#include <string>

namespace ncc {
  enum ValueType {
    TYPE_VOID,
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

  std::string get_token_name(char token);

}

#endif
