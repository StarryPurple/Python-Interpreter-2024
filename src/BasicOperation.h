#pragma once
#ifndef PYTHON_INTERPRETER_BASIC_OPERATION_H
#define PYTHON_INTERPRETER_BASIC_OPERATION_H

#include "Python3TypeDefinition.h"


#include <set>
#include <map>

enum TypeId: int;


inline std::set<std::string> reserved_keywords = {"None", "True", "False", "def", "return", "break", "continue", "if",
"elif", "else", "while", "or", "and", "not"};
inline std::map<std::string, int> function_id; // 1-based function id map. 0 is reserved for main function / input_file.

// the content of a function.
class Function {
  std::map<std::string, int> variable_list; // 1-based variable id map. 0 shouldn't be used.
  std::vector<Python3Type> variables; // the storage of variables. The initial value is undefined.
  enum function_operations: int;
  // Exception in invalid operation
  class OperationException;
  // Exception in invalid evaluation
  class InvalidVariableException;

  using Funcdef_arith = Python3Type (const Python3Type &, const Python3Type &) const;
  using Funcdef_arith_eval = void (Python3Type &, const Python3Type &) const;

  static Funcdef_arith_eval OperationEval;
  static Funcdef_arith OperationAdd, OperationSub, OperationMul, OperationDiv, OperationIDiv, OperationMod;
  static Funcdef_arith_eval
    OperationAddEval, OperationSubEval, OperationMulEval, OperationDivEval, OperationIDivEval, OperationModEval;
  static Funcdef_arith
    OperationEqual, OperationNEqual, OperationLesser, OperationGreater, OperationNGreater, OperationNLesser;
  static Funcdef_arith OperationAnd, OperationOr, OperationNot;

  static Funcdef_arith *operation_arith[15];
  static Funcdef_arith_eval *operation_arith_eval[7];

  std::vector<int> operations;
};

inline std::vector<Function> functions;



#endif // PYTHON_INTERPRETER_BASIC_OPERATION_H