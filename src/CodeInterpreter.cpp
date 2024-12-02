#include "CodeInterpreter.h"

const Python3Type
  CodeInterpreter::ConstTrue = Python3Type(true),
  CodeInterpreter::ConstFalse = Python3Type(false),
  CodeInterpreter::ConstNone = Python3Type(nullptr);

CodeInterpreter::CodeInterpreter():
  funcdef_mode(false) {
  main_suite_ = cur_suite_ = new CodeSuite(nullptr);
  cur_function_def_ = nullptr;
}

CodeInterpreter::~CodeInterpreter() {
  delete main_suite_;
  std::cout << "Thank you for using Python Interpreter. See you later.\n" << std::flush;
}

void CodeInterpreter::ExecuteSingleOperation(const Operation &operation) {
  switch(operation.OperType_) {
    case Operation::FunctionOperations::OprEval:
      Operation::OperationEval(*operation.param1_, *operation.param2_);
    break;

    case Operation::FunctionOperations::OprAdd:
      *operation.result_ = Operation::OperationAdd(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprSub:
      *operation.result_ = Operation::OperationSub(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprMul:
      *operation.result_ = Operation::OperationMul(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprDiv:
      *operation.result_ = Operation::OperationDiv(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprIDiv:
      *operation.result_ = Operation::OperationIDiv(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprMod:
      *operation.result_ = Operation::OperationMod(*operation.param1_, *operation.param2_);
    break;

    case Operation::FunctionOperations::OprEqual:
      *operation.result_ = Operation::OperationEqual(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprNEqual:
      *operation.result_ = Operation::OperationNEqual(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprLesser:
      *operation.result_ = Operation::OperationLesser(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprGreater:
      *operation.result_ = Operation::OperationGreater(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprNGreater:
      *operation.result_ = Operation::OperationNGreater(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprNLesser:
      *operation.result_ = Operation::OperationNLesser(*operation.param1_, *operation.param2_);
    break;

    case Operation::FunctionOperations::OprAnd:
      *operation.result_ = Operation::OperationAnd(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprOr:
      *operation.result_ = Operation::OperationOr(*operation.param1_, *operation.param2_);
    break;
    case Operation::FunctionOperations::OprNot:
      *operation.result_ = Operation::OperationNot(*operation.param1_);
    break;
    case Operation::FunctionOperations::OprNeg:
      *operation.result_ = Operation::OperationNegative(*operation.param1_);
    break;

    case Operation::FunctionOperations::OprInt:
      *operation.result_ = Operation::OperationInteger(*operation.param1_);
    break;
    case Operation::FunctionOperations::OprDec:
      *operation.result_ = Operation::OperationDecimal(*operation.param1_);
    break;
    case Operation::FunctionOperations::OprStr:
      *operation.result_ = Operation::OperationStr(*operation.param1_);
    break;
    case Operation::FunctionOperations::OprBool:
      *operation.result_ = Operation::OperationBoolean(*operation.param1_);
    break;

    case Operation::FunctionOperations::OprPrint:
      Operation::OperationPrint(*operation.param1_);
    break;
    case Operation::FunctionOperations::OprFunc:{
      CodeSuite *new_suite = new CodeSuite(cur_suite_, operation.func_);
      cur_suite_ = new_suite;
      ExecuteOperations(cur_suite_->operations()); // TODO: logic may change in future
      cur_suite_ = cur_suite_->parent_suite();
      delete new_suite;
    }
    break;
    case Operation::FunctionOperations::OprForward:
      throw std::runtime_error("No goto in single operation, dev.");
    break;
    case Operation::FunctionOperations::OprBackward:
      throw std::runtime_error("No goto in single operation, dev.");
    break;
    case Operation::FunctionOperations::OprReturn:
      return;
    break;
    case Operation::FunctionOperations::OprPopStack:
      shared_res_.pop_back();
    break;
    case Operation::FunctionOperations::OprPushStack:
      shared_res_.push_back(ConstNone);
    default:
      throw std::runtime_error("What? How did you get here?");
  }
}
// sequentially conduct an operation sequence.
void CodeInterpreter::ExecuteOperations(const std::vector<Operation> &operations) {
  for(int step = 0; step < operations.size(); ++step) {
    const Operation &operation = operations[step];
    switch(operation.OperType_) {
      case Operation::FunctionOperations::OprEval:
        Operation::OperationEval(*operation.param1_, *operation.param2_);
      break;

      case Operation::FunctionOperations::OprAdd:
        *operation.result_ = Operation::OperationAdd(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprSub:
        *operation.result_ = Operation::OperationSub(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprMul:
        *operation.result_ = Operation::OperationMul(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprDiv:
        *operation.result_ = Operation::OperationDiv(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprIDiv:
        *operation.result_ = Operation::OperationIDiv(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprMod:
        *operation.result_ = Operation::OperationMod(*operation.param1_, *operation.param2_);
      break;

      case Operation::FunctionOperations::OprEqual:
        *operation.result_ = Operation::OperationEqual(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprNEqual:
        *operation.result_ = Operation::OperationNEqual(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprLesser:
        *operation.result_ = Operation::OperationLesser(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprGreater:
        *operation.result_ = Operation::OperationGreater(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprNGreater:
        *operation.result_ = Operation::OperationNGreater(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprNLesser:
        *operation.result_ = Operation::OperationNLesser(*operation.param1_, *operation.param2_);
      break;

      case Operation::FunctionOperations::OprAnd:
        *operation.result_ = Operation::OperationAnd(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprOr:
        *operation.result_ = Operation::OperationOr(*operation.param1_, *operation.param2_);
      break;
      case Operation::FunctionOperations::OprNot:
        *operation.result_ = Operation::OperationNot(*operation.param1_);
      break;
      case Operation::FunctionOperations::OprNeg:
        *operation.result_ = Operation::OperationNegative(*operation.param1_);
      break;

      case Operation::FunctionOperations::OprInt:
        *operation.result_ = Operation::OperationInteger(*operation.param1_);
      break;
      case Operation::FunctionOperations::OprDec:
        *operation.result_ = Operation::OperationDecimal(*operation.param1_);
      break;
      case Operation::FunctionOperations::OprStr:
        *operation.result_ = Operation::OperationStr(*operation.param1_);
      break;
      case Operation::FunctionOperations::OprBool:
        *operation.result_ = Operation::OperationBoolean(*operation.param1_);
      break;

      case Operation::FunctionOperations::OprPrint:
        Operation::OperationPrint(*operation.param1_);
      break;
      case Operation::FunctionOperations::OprFunc:{
        CodeSuite *new_suite = new CodeSuite(cur_suite_, operation.func_);
        cur_suite_ = new_suite;
        ExecuteOperations(cur_suite_->operations()); // TODO: logic may change in future
        cur_suite_ = cur_suite_->parent_suite();
        delete new_suite;
      }
      break;
      case Operation::FunctionOperations::OprForward:
        step += Operation::OperationForward(*operation.param1_, operation.goto_steps_);
      break;
      case Operation::FunctionOperations::OprBackward:
        step -= Operation::OperationBackward(*operation.param1_, operation.goto_steps_);
      break;
      case Operation::FunctionOperations::OprReturn:
        return;
      break;
      case Operation::FunctionOperations::OprPopStack:
        shared_res_.pop_back();
      break;
      case Operation::FunctionOperations::OprPushStack:
        shared_res_.push_back(ConstNone);
      break;
      default:
        throw std::runtime_error("What? How did you get here?");
    }
  }
}

// Find a variable by its name, local one other than global ones.
// return ConstNone if fails, meaning a definition.
Python3Type& CodeInterpreter::FindVariable(CodeSuite *function, const std::string& var_name, bool &res) {
  CodeSuite *cur_suite = function;
  while(cur_suite != nullptr) {
    auto result = cur_suite->variables_map().find(var_name);
    if(result != cur_suite->variables_map().end()) {
      res = true;
      return cur_suite->variables()[result->second];
    }
    cur_suite = cur_suite->parent_suite();
  }
  res = false;
  return main_suite_->variables()[0]; // only a placeholder.
}
Function *CodeInterpreter::FindFunction(const std::string &name) {
  if(function_map_.count(name) == 0)
    throw std::runtime_error("Don't try to call an undefined function, user.");
  int ordinal = function_map_.find(name)->second;
  return &functions_[ordinal];
}

void CodeInterpreter::DefineVariable(const std::string &name, const Python3Type &init_value) {
  if(funcdef_mode)
    cur_function_def_->DefineVariable(name, init_value);
  else
    cur_suite_->DefineVariable(name, init_value);
}

void CodeInterpreter::AppendOperation(const Operation &operation) {
  if(funcdef_mode)
    cur_function_def_->AppendOperation(operation);
  else {
    cur_suite_->AppendOperation(operation);
    assert(cur_suite_ == main_suite_);
    if(cur_suite_ == main_suite_)
      RunNewOperations(main_suite_);
  }
  if(oper_cnt_mode)
    operation_cnt++;
}
void CodeInterpreter::StartFunctionDef(const std::string &name) {
  if(function_map_.count(name) != 0)
    throw std::runtime_error("Don't define function with a used function name, dev.");
  function_map_.insert({name, functions_.size()});
  functions_.push_back(Function(name));
  cur_function_def_ = &functions_.back();
  funcdef_mode = true;
}
void CodeInterpreter::EndFunctionDef() {
  cur_function_def_ = nullptr;
  funcdef_mode = false;
}
void CodeInterpreter::RunNewOperations(CodeSuite *suite) {
  ExecuteOperations(suite->operations());
  suite->EraseExistingOperations();
}
void CodeInterpreter::RunNewOperations() {
  RunNewOperations(cur_suite_);
}
void CodeInterpreter::Load(Python3Type obj) {
  shared_res_.push_back(obj);
}
Python3Type CodeInterpreter::PopResult() {
  Python3Type res = shared_res_.back();
  shared_res_.pop_back();
  return res;
}
// Should be always followed by a StackPop.
Operation CodeInterpreter::ArithOperation(Operation::FunctionOperations oper_type) {
  Python3Type *param1 = &shared_res_[shared_res_.size() - 1];
  Python3Type *param2 = &shared_res_[shared_res_.size() - 2];
  Operation operation(oper_type, param1, param2, param2);
  return operation;
}
Operation CodeInterpreter::SelfOperation(Operation::FunctionOperations oper_type) {
  Python3Type *param = &shared_res_[shared_res_.size() - 1];
  Operation operation(oper_type, param, nullptr, param);
  return operation;
}

Operation CodeInterpreter::StackPushNoneOperation() {
  Operation operation(Operation::FunctionOperations::OprPushStack, nullptr, nullptr, nullptr);
  return operation;
}

Operation CodeInterpreter::StackPopOperation() {
  Operation operation(Operation::FunctionOperations::OprPopStack, nullptr, nullptr, nullptr);
  return operation;
}

Operation CodeInterpreter::LoadOperation(Python3Type &var_loc) {
  Operation operation(Operation::FunctionOperations::OprEval,
    &shared_res_[shared_res_.size() - 1], &var_loc, nullptr);
  return operation;
}
// Should always follow a StackPushNone.
Operation CodeInterpreter::WriteOperation(Python3Type &var_loc) {
  Operation operation(Operation::FunctionOperations::OprEval,
    &var_loc, &shared_res_[shared_res_.size() - 1], nullptr);
  return operation;
}

void CodeInterpreter::StartOperationCount() {
  operation_cnt = 0;
  oper_cnt_mode = true;
}
int CodeInterpreter::EndOperationCount() {
  oper_cnt_mode = false;
  return operation_cnt;
}
