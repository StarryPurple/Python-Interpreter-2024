#include "InterpreterStructure.h"

// class FunctionSuite

const std::string FunctionSuite::UnassignedName = "#AnonVar";

FunctionSuite::FunctionSuite(const Initialize_List &param_list) {
  // variable_map.clear();
  // variable_space.clear();
  for(auto [var_name, init_val]: param_list)
    DefineVariable(var_name, init_val);
}


void FunctionSuite::DefineVariable(const std::string &var_name, const std::any &init_val) {
  if(variable_map.find(var_name) != variable_map.end())
    throw std::runtime_error("Re-definition of variable \"" + var_name + "\"");
  variable_map.insert({var_name, variable_map.size()});
  variable_space.push_back(init_val);
}

std::any &FunctionSuite::LocalVariable(const std::string &var_name) {
  if(variable_map.find(var_name) == variable_map.end())
    DefineVariable(var_name, Interpreter::ConstNone);
  return variable_space[variable_map[var_name]];
}

// class PythonProject

PythonProject::PythonProject() {
  function_stack.push_back(FunctionSuite()); // the placeholder for __main__
}


void PythonProject::CallFunction(const FunctionSuite &function) {
  function_stack.push_back(function);
  std::cerr << function.func_name << " called" << std::endl;
}

void PythonProject::ExitFunction() {
  std::cerr << function_stack.back().func_name << " exited" << std::endl;
  function_stack.pop_back();
}

std::any &PythonProject::Variable(const std::string &var_name) {
  int pos = function_stack.size() - 1;
  while(pos >= 0) {
    if(function_stack[pos].variable_map.count(var_name) > 0)
      return function_stack[pos].LocalVariable(var_name);
    pos--;
  }
  function_stack.back().DefineVariable(var_name, Interpreter::ConstNone);
  return function_stack.back().variable_space[function_stack.back().variable_map[var_name]];
}

void PythonProject::CallBreak() {
  function_stack.back().break_sign = true;
}

void PythonProject::EndBreak() {
  function_stack.back().break_sign = false;
}

void PythonProject::CallContinue() {
  function_stack.back().continue_sign = true;
}

void PythonProject::EndContinue() {
  function_stack.back().continue_sign = false;
}

void PythonProject::CallReturn() {
  function_stack.back().return_sign = true;
}

void PythonProject::EndReturn() {
  function_stack.back().return_sign = false;
}

void PythonProject::AddResult(const std::any &obj) {
  function_stack.back().result = obj;
}

std::any PythonProject::GiveResult() {
  return function_stack.back().result;
}


bool PythonProject::IsBreak() const {
  return function_stack.back().break_sign;
}


bool PythonProject::IsContinue() const {
  return function_stack.back().continue_sign;
}

bool PythonProject::IsReturn() const {
  return function_stack.back().return_sign;
}
