#pragma once
#ifndef PYTHON_INTERPRETER_BASIC_OPERATION_H
#define PYTHON_INTERPRETER_BASIC_OPERATION_H

#include "Python3TypeDefinition.h"


#include <set>
#include <map>

enum Python3TypeId: int;


inline std::set<std::string> reserved_keywords = {"None", "True", "False", "def", "return", "break", "continue", "if",
"elif", "else", "while", "or", "and", "not"};
// 1-based function id map. 0 is reserved for main function / input_file.
// 1, 2, 3, 4 reserved for int(), float(), str(), bool().
inline std::map<std::string, int> function_id;

// the content of a function.
class Function {
  std::map<std::string, int> variable_list; // 1-based variable id map. 0 shouldn't be used.
  std::vector<Python3Type> variables; // the storage of variables. The initial value is undefined.
  enum function_operations: int;
  // Exception in invalid operation
  class OperationException;
  // Exception in invalid evaluation
  class InvalidVariableException;

  // supports arithmetical type cast of Python3Type objects.
  class arith_type_cast {
  public:
    // Change the Python3Type object into another type.
    // throws OperationException if cast fails.
    // std::invalid_argument, std::out_of_range may happen.
    Python3Type operator()(const Python3Type &, const Python3TypeId) const;
    // change two params to their common arithmetic type. throws OperationException when no cast is valid.
    // C++ style. Only consider integer, decimal and boolean cast-able. Make special judges for str.
    void operator()(Python3Type &, Python3Type &) const;
    // Exposes the cast result.
    operator Python3Type() const;
  };


  // The following is the basic operations list:
  // TODO: special judge the str * int operations.


  using Funcdef_arith = Python3Type (Python3Type, Python3Type) const;
  using Funcdef_arith_eval = void (Python3Type &, const Python3Type &) const;

  static Funcdef_arith_eval OperationEval;
  static Funcdef_arith OperationAdd, OperationSub, OperationMul, OperationDiv, OperationIDiv, OperationMod;
  static Funcdef_arith_eval
    OperationAddEval, OperationSubEval, OperationMulEval, OperationDivEval, OperationIDivEval, OperationModEval;
  static Funcdef_arith
    OperationEqual, OperationNEqual, OperationLesser, OperationGreater, OperationNGreater, OperationNLesser;
  static Python3Type OperationAnd(const Python3Type &, const Python3Type &);
  static Python3Type OperationOr(const Python3Type &, const Python3Type &);
  static Python3Type OperationNot(const Python3Type &);

  std::vector<int> operations;
};

inline std::vector<Function> functions;



#endif // PYTHON_INTERPRETER_BASIC_OPERATION_H