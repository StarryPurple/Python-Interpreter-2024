#include "CodeInterpreter.h"

const Python3Type
  CodeInterpreter::ConstTrue = Python3Type(true),
  CodeInterpreter::ConstFalse = Python3Type(false),
  CodeInterpreter::ConstNone = Python3Type(nullptr);

CodeInterpreter::CodeInterpreter():
  funcdef_mode(false) {
  functions_.push_back(Function("__main__"));
  cur_function_ = main_function_ = &functions_.back();
}

CodeInterpreter::~CodeInterpreter() {
  // std::cout << "Thank you for using Python. See you later.\n" << std::flush;
}


// Find a variable by its name, local one other than global ones.
// return ConstNone if fails, meaning a definition.
const Python3Type& CodeInterpreter::FindVariable(Function *function, const std::string& var_name, bool &res) {
  Function *cur_function = function;
  while(cur_function != nullptr) {
    auto result = cur_function->variables_map().find(var_name);
    if(result != cur_function->variables_map().end()) {
      res = true;
      return cur_function->variables()[result->second];
    }
    cur_function = cur_function->parent_function;
  }
  res = false;
  return ConstNone;
}
void CodeInterpreter::DefineVariable(const std::string &name, const Python3Type &init_value) {
  Function *target_function;
  if(funcdef_mode)
    target_function = cur_function_def_;
  else {
    if(cur_function_ != main_function_)
      throw std::runtime_error("Don't modify existing functions, dev");
    target_function = main_function_;
  }
  target_function->AddVariable(name, init_value);
}

void CodeInterpreter::InsertOperation(const Operation &operation) {
  Function *target_function;
  if(funcdef_mode)
    target_function = cur_function_def_;
  else {
    if(cur_function_ != main_function_)
      throw std::runtime_error("Don't modify existing functions, dev");
    target_function = main_function_;
  }
  target_function->AddOperation(operation);
}
void CodeInterpreter::StartFunctionDef(const std::string &name) {
  function_map_.insert({name, functions_.size()});
  functions_.push_back(Function(name));
  cur_function_def_ = &functions_.back();
  funcdef_mode = true;
}
void CodeInterpreter::EndFunctionDef() {
  cur_function_def_ = nullptr;
  funcdef_mode = false;
}
void CodeInterpreter::RunMainFunction() {
  ExecuteOperations(main_function_->operations());
}

