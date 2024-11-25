#include "BasicOperation.h"


enum Function::function_operations {
  OprEval = 0,
  OprAdd = 1, OprSub = 2, OprMul = 3, OprDiv = 4, OprIDiv = 5, OprMod = 6,
  OprAddEval = 7, OprSubEval = 8, OprMulEval = 9, OprDivEval = 10, OprIDivEval = 11, OprModEval = 12,
  OprEqual = 13, OprNEqual = 14, OprLesser = 15, OprGreater = 16, OprNGreater = 17, OprNLesser = 18,
  OprAnd = 19, OprOr = 20, OprNot = 21
};

class Function::OperationException {
  std::string type1, type2, operation;
  int operand_cnt;
  OperationException(const std::string &type1_, const std::string &operation_):
    type1(type1_), operation(operation_), operand_cnt(1) {}
  OperationException(const std::string &type1_, const std::string &type2_, const std::string &operation_):
    type1(type1_), type2(type2_), operation(operation_), operand_cnt(2) {}
  void what() const {
    if(operand_cnt == 1)
      std::cerr << "Unsupported operand type: \"" << type1 << "\" for operator " << operation << std::endl;
    else if(operand_cnt == 2)
      std::cerr << "Unsupported operand types: \"" << type1 << "\" and \"" << type2 << "\" for operator "
        << operation << std::endl;
  }
};
class Function::InvalidVariableException {
  std::string name;
  InvalidVariableException(const std::string &name_): name(name_) {}
  void what() const {
    std::cerr << "Reserved name for \"" << name << "\"" << std::endl;
  }
};

Function::Funcdef_arith_eval *Function::operation_arith_eval[7] = {
  OperationEval,
  OperationAddEval, OperationSubEval, OperationMulEval, OperationDivEval, OperationIDivEval, OperationModEval};

Function::Funcdef_arith *Function::operation_arith[15] = {
  OperationAdd, OperationSub, OperationMul, OperationDiv, OperationIDiv, OperationMod,
  OperationEqual, OperationNEqual, OperationLesser, OperationGreater, OperationNGreater, OperationNLesser,
  OperationAnd, OperationOr, OperationNot};

// Attention: @target must be already defined
void Function::OperationEval(Python3Type &target, const Python3Type &other) const {
  target = other;
}