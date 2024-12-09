#include "InterpreterStructure.h"

// class FunctionSuite

const std::string FunctionSuite::UnassignedName = "#";

FunctionSuite::FunctionSuite(const Initialize_List &param_list) { // &&list
  // variable_map.clear();
  // variable_space.clear();
  primitive_list = param_list;
}


void FunctionSuite::DefineVariable(const std::string &var_name, const std::any &init_val) {
  /*if(variable_map.find(var_name) != variable_map.end())
    throw std::runtime_error("Re-definition of variable \"" + var_name + "\"");*/
  variable_space.insert({var_name, init_val});
}

std::any &FunctionSuite::LocalVariable(const std::string &var_name) {
  if(variable_space.find(var_name) == variable_space.end())
    DefineVariable(var_name, Interpreter::ConstNone);
  return variable_space[var_name];
}

// class PythonProject

PythonProject::PythonProject() {
  function_stack.push_back(FunctionSuite()); // the placeholder for __main__
}


void PythonProject::CallFunction(const FunctionSuite &function) {
  function_stack.push_back(function);
  // std::cerr << function.func_name << " called" << std::endl;
}

void PythonProject::ExitFunction() {
  // std::cerr << function_stack.back().func_name << " exited" << std::endl;
  function_stack.pop_back();
}

std::any &PythonProject::Variable(const std::string &var_name) {
  /*int pos = function_stack.size() - 1;
  while(pos >= 0) {
    if(function_stack[pos].variable_map.count(var_name) > 0)
      return function_stack[pos].LocalVariable(var_name);
    pos--;
  }*/ // wrong grammar
  // local
  if(function_stack.back().variable_space.count(var_name) > 0)
    return function_stack.back().LocalVariable(var_name);
  // global
  if(function_stack.front().variable_space.count(var_name) > 0)
    return function_stack.front().LocalVariable(var_name);
  // new definition
  function_stack.back().DefineVariable(var_name, Interpreter::ConstNone);
  return function_stack.back().variable_space[var_name];
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
