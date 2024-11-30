#include "CodeInterpreter.h"

const Python3Type
  CodeInterpreter::ConstTrue = Python3Type(true),
  CodeInterpreter::ConstFalse = Python3Type(false),
  CodeInterpreter::ConstNone = Python3Type(nullptr);

CodeInterpreter::CodeInterpreter():
  MainSuite(nullptr), funcdef_mode(false) {}
void CodeInterpreter::InsertOperation(const Operation &) {
  CodeSuite &code_suite = code_suites.back();
}
const Python3Type& CodeInterpreter::FindVariable(CodeSuite *code_suite, const std::string& var_name, bool &res) {
  CodeSuite *cur_suite = code_suite;
  while(cur_suite != nullptr) {
    auto result = cur_suite->variables_map().find(var_name);
    if(result != cur_suite->variables_map().end()) {
      res = true;
      return cur_suite->variables()[result->second];
    }
    else
      cur_suite = cur_suite->parent_suite();
  }
  res = false;
  return Python3Type(nullptr);
}

