#pragma once
#ifndef CODE_INTERPRETER_H
#define CODE_INTERPRETER_H

#include "BasicStructure.h"

// Every CodeInterpreter object holds the capacity of interpreting ONE code file.
class CodeInterpreter {
  static const Python3Type ConstTrue, ConstFalse, ConstNone;

  std::vector<Function> functions; // the global function storage
  std::map<std::string, int> function_list; // name -> function ordinal

  std::vector<CodeSuite> code_suites; // the current code suite stack.
  std::map<std::string, int> code_suite_list;

  CodeSuite MainSuite; // __main__ in Shell-like form. It's parent_suit is nullptr;
  Python3Type TempVar; // the global, shared storage for variables.

  bool funcdef_mode;

public:
  CodeInterpreter();
  static const Python3Type& FindVariable(CodeSuite *, const std::string &, bool &);
  void InsertOperation(const Operation &);
  void StartCodeSuite();
  void EndCodeSuite();
  void StartFunctionDef(const std::string &);
  void EndFunctionDef();
  void StartFunction(const Function *);
  void EndFunction();
};

inline CodeInterpreter interpreter;


#endif // CODE_INTERPRETER_H