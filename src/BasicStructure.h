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
// 1-based function id map. 0 is reserved for main function / input_file.
// 1, 2, 3, 4 reserved for int(), float(), str(), bool().
inline std::map<std::string, int> function_id;

class Function;
class CodeSuite;

// the content of a function.
class Operation {
  enum class FunctionOperations: int;

  class OperationException; // Exception in invalid operation
  class InvalidVariableException; // Exception in invalid evaluation

  // The following is the basic operations list:
  // TODO: special judge the str * int operations.
  using Funcdef_arith = Python3Type (Python3Type, Python3Type);
  using Funcdef_arith_eval = void (Python3Type &, const Python3Type &);
  using Funcdef_two_params = Python3Type (const Python3Type &, const Python3Type &);
  using Funcdef_single_param = Python3Type (const Python3Type &);
  struct OperationDetail {
    FunctionOperations OperType;
    std::string func_name; // used when OperType = OperFunc
    int goto_line; // indicates which line next to go
    Python3Type &param1;
    Python3Type &param2; // unused when OperType =
    Python3Type &target;
  }; // some may not exist/initialized
public:
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
  static int OperationDecide(const Python3Type &);
};

class CodeSuite {
protected:
  CodeSuite *parent_suite = nullptr;
  std::vector<Operation> operations; // in order
  std::map<std::string, Python3Type> variables; // name -> local variable
  std::map<std::string, const Function *> functions;  // name -> function
public:
  explicit CodeSuite(CodeSuite *);
  // Should not be used in OperationEval.
  virtual Python3Type& FindVariable(const std::string &);
  virtual void AddVariable(const std::string &, const Python3Type &);
  virtual void AddOperation(const Operation &);
  virtual void AddFunction(const Function *);

  virtual void Execution();
};

// Function is a code suite that can be called elsewhere and extra variables added before main part
class Function: CodeSuite {
  std::string func_name;
public:
  Function(const std::string &);
  virtual void AddFunction(const Function *) override = delete;
  std::string name() const;
  virtual void Execution() override;
};


// The structure:
// Function 1:



#endif // PYTHON_INTERPRETER_BASIC_OPERATION_H