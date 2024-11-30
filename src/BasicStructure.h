#pragma once
#ifndef PYTHON_INTERPRETER_BASIC_OPERATION_H
#define PYTHON_INTERPRETER_BASIC_OPERATION_H

#include "Python3TypeDefinition.h"


#include <set>
#include <map>
#include <cassert>

enum Python3TypeId: int;


inline std::set<std::string> reserved_keywords = {"None", "True", "False", "def", "return", "break", "continue", "if",
"elif", "else", "while", "or", "and", "not"};

class Function;
class CodeSuite;

// the content of a function.
class Operation {

  class OperationException; // Exception in invalid operation
  class InvalidVariableException; // Exception in invalid evaluation

  // The following is the basic operations list:
  // TODO: special judge the str * int operations.
  using Funcdef_arith = Python3Type (Python3Type, Python3Type);
  using Funcdef_arith_eval = void (Python3Type &, const Python3Type &);
  using Funcdef_two_params = Python3Type (const Python3Type &, const Python3Type &);
  using Funcdef_single_param = Python3Type (const Python3Type &);
public:
  enum class FunctionOperations: int;
  FunctionOperations OperType_;
  Function *func_; // used at OperFunc
  int goto_steps_; // used at OperIf and OperWhile. always positive.
  Python3Type &param1_;
  Python3Type &param2_;
  Python3Type &result_;
   // some may not exist/initialized
  // supports arithmetical type cast of Python3Type objects.
  struct arith_type_cast {
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
  static Funcdef_arith_eval
    OperationEval,
    OperationAddEval, OperationSubEval, OperationMulEval, OperationDivEval, OperationIDivEval, OperationModEval;
  static Funcdef_arith
    OperationAdd, OperationSub, OperationMul, OperationDiv, OperationIDiv, OperationMod,
    OperationEqual, OperationNEqual, OperationLesser, OperationGreater, OperationNGreater, OperationNLesser;
  static Funcdef_two_params
    OperationAnd, OperationOr;
  static Funcdef_single_param
    OperationNot,
    OperationInteger, OperationDecimal, OperationStr, OperationBoolean;
  static Python3Type &OperationSubscript(const Python3Type &, int);
  static void OperationPrint(const Python3Type &);
  static int OperationForward(const Python3Type &, int);
  static int OperationBackward(const Python3Type &, int);
};

void Execution(const std::vector<Operation> &);

class CodeSuite {
protected:
  CodeSuite *parent_suite_;
  std::vector<Operation> operations_; // in order
  std::map<std::string, int> variables_map_; // name -> local variable ordinal
  std::vector<Python3Type> variables_;
  std::map<std::string, const Function *> functions_;  // name -> function
public:
  CodeSuite(CodeSuite *);
  CodeSuite *parent_suite();
  const std::vector<Operation> &operations();
  const std::map<std::string, int> &variables_map();
  const std::vector<Python3Type> &variables();
  void AddVariable(const std::string &, const Python3Type &);
  void AddOperation(const Operation &);
  void AddFunction(const Function *);

};

// Function is a code suite that can be called elsewhere and extra variables added before main part
class Function {
  std::string func_name_;
  std::vector<Operation> operations_;
  std::map<std::string, Python3Type> variables_;
  std::map<std::string, const Function> functions_;
public:
  Function(const std::string &);
  const std::vector<Operation> &operations_list();
  std::string name() const;
  void AddVariable(const std::string &, const Python3Type &);
  void AddOperation(const Operation &);
};


#endif // PYTHON_INTERPRETER_BASIC_OPERATION_H