#include "BasicOperation.h"

#include <Exceptions.h>
#include <support/CPPUtils.h>


enum Operation::FunctionOperations {
  OprEval = 0,
  OprAdd = 1, OprSub = 2, OprMul = 3, OprDiv = 4, OprIDiv = 5, OprMod = 6,
  OprAddEval = 7, OprSubEval = 8, OprMulEval = 9, OprDivEval = 10, OprIDivEval = 11, OprModEval = 12,
  OprEqual = 13, OprNEqual = 14, OprLesser = 15, OprGreater = 16, OprNGreater = 17, OprNLesser = 18,
  OprAnd = 19, OprOr = 20, OprNot = 21,
  OprInt = 22, OprDec = 23, OprStr = 24, OprBool = 25,
  OprPrint = 26,
  OprFunc = 27
};

class Operation::OperationException: std::exception {
  std::string type1, type2, operation;
  int operand_cnt;
public:
  OperationException(const std::string &type1_, const std::string &operation_):
    type1(type1_), operation(operation_), operand_cnt(1) {}
  OperationException(const std::string &type1_, const std::string &type2_, const std::string &operation_):
    type1(type1_), type2(type2_), operation(operation_), operand_cnt(2) {}
  virtual const char* what() const override {
    if(operand_cnt == 1)
      return ("Unsupported operand type: \"" + type1 + "\" for operator " + operation).c_str();
    else if(operand_cnt == 2)
      return ("Unsupported operand types: \"" + type1 + "\" and \"" + type2 + "\" for operator " + operation).c_str();
  }
};
class Operation::InvalidVariableException: std::exception {
  std::string name;
public:
  InvalidVariableException(const std::string &name_): name(name_) {}
  virtual const char* what() const override {
    return ("Reserved name for \"" + name + "\"").c_str();
  }
};

Python3Type Operation::arith_type_cast::operator()(const Python3Type &obj, const Python3TypeId type) const {
  Python3TypeId obj_type = obj.type_id;
  // if(type == IdNoneType && obj_type == IdNoneType) return obj;
  // if(type == IdTuple && obj_type == IdTuple) return obj;
  if(type == Python3TypeId::IdInteger) {
    if(obj_type == Python3TypeId::IdInteger)      // integer -> integer
      return obj;
    else if(obj_type == Python3TypeId::IdDecimal) // decimal -> integer
      return Integer(static_cast<Integer>(std::any_cast<Decimal>(obj)));
    else if(obj_type == Python3TypeId::IdStr)     // str     -> integer // TODO: stoll -> int2048 constructor
      return Integer(std::stoll(std::any_cast<Str>(obj)));
    else if(obj_type == Python3TypeId::IdBoolean) // boolean -> integer
      return Integer(static_cast<Integer>(std::any_cast<Boolean>(obj)));

  } else if(type == Python3TypeId::IdDecimal) {
    if(obj_type == Python3TypeId::IdInteger)      // integer -> decimal
      return Decimal(static_cast<Decimal>(std::any_cast<Integer>(obj)));
    else if(obj_type == Python3TypeId::IdDecimal) // decimal -> decimal
      return obj;
    else if(obj_type == Python3TypeId::IdStr)     // str     -> decimal
      return Decimal(std::stod(std::any_cast<Str>(obj)));
    else if(obj_type == Python3TypeId::IdBoolean) // boolean -> decimal
      return Decimal(static_cast<Decimal>(std::any_cast<Boolean>(obj)));

  } else if(type == Python3TypeId::IdStr) {
    if(obj_type == Python3TypeId::IdInteger)      // integer -> str // TODO: int2048 specification
      return Str(std::to_string(std::any_cast<Integer>(obj)));
    else if(obj_type == Python3TypeId::IdDecimal) // decimal -> str
      return Str(std::to_string(std::any_cast<Decimal>(obj))); // 6 precision digits
    else if(obj_type == Python3TypeId::IdStr)     // str     -> str
      return obj;
    else if(obj_type == Python3TypeId::IdBoolean) // boolean -> str
      return Str(std::any_cast<Boolean>(obj) ? "True" : "False");

  } else if(type == Python3TypeId::IdBoolean) {
    if(obj_type == Python3TypeId::IdInteger)      // integer -> boolean
      return Boolean(static_cast<Boolean>(std::any_cast<Integer>(obj)));
    else if(obj_type == Python3TypeId::IdDecimal) // decimal -> boolean
      return Boolean(static_cast<Boolean>(std::any_cast<Decimal>(obj)));
    else if(obj_type == Python3TypeId::IdStr)     // str     -> boolean
      return Boolean(!std::any_cast<Str>(obj).empty());
    else if(obj_type == Python3TypeId::IdBoolean) // boolean -> boolean
      return obj;
  }
  // no else
  throw OperationException(
    Python3Typename[static_cast<int>(obj_type)],
    Python3Typename[static_cast<int>(type)] + "()");
}

void Operation::arith_type_cast::operator()(Python3Type &left, Python3Type &right) const {
  Python3TypeId left_id = left.type_id, right_id = right.type_id;
  Python3TypeId target_id = Python3TypeId::IdNoneType;
  if(left_id == Python3TypeId::IdInteger) {
    if(right_id == Python3TypeId::IdInteger)      // integer && integer -> integer
      target_id = Python3TypeId::IdInteger;
    else if(right_id == Python3TypeId::IdDecimal) // decimal && integer -> decimal
      target_id = Python3TypeId::IdDecimal;
    else if(right_id == Python3TypeId::IdBoolean) // boolean && integer -> integer
      target_id = Python3TypeId::IdInteger;

  } else if(left_id == Python3TypeId::IdDecimal) {
    if(right_id == Python3TypeId::IdInteger)      // integer && decimal -> decimal
      target_id = Python3TypeId::IdDecimal;
    else if(right_id == Python3TypeId::IdDecimal) // decimal && decimal -> decimal
      target_id = Python3TypeId::IdDecimal;
    else if(right_id == Python3TypeId::IdBoolean) // boolean && decimal -> decimal
      target_id = Python3TypeId::IdDecimal;

  } else if(left_id == Python3TypeId::IdBoolean) {
    if(right_id == Python3TypeId::IdInteger)      // integer && boolean -> integer
      target_id = Python3TypeId::IdInteger;
    else if(right_id == Python3TypeId::IdDecimal) // decimal && boolean -> decimal
      target_id = Python3TypeId::IdDecimal;
    else if(right_id == Python3TypeId::IdBoolean) // boolean && boolean -> boolean
      target_id = Python3TypeId::IdBoolean;

  }/*else if(left_id == IdStr) {
    if(right_id == IdStr)
      target_id = IdStr;
  }*/
  // no else
  if(target_id == Python3TypeId::IdNoneType)
    throw OperationException(
      Python3Typename[static_cast<int>(left.type_id)],
      Python3Typename[static_cast<int>(right.type_id)],
      "arithmetic operation");
  left = arith_type_cast()(left, target_id);
  right = arith_type_cast()(right, target_id);
}



// Attention: @target must be already defined
void Operation::OperationEval(Python3Type &obj, const Python3Type &other) {
  obj = other;
}

Python3Type Operation::OperationAdd(Python3Type left, Python3Type right) {
  if(left.type_id == Python3TypeId::IdStr && right.type_id == Python3TypeId::IdStr) // str + str
    return std::any_cast<Str>(left) + std::any_cast<Str>(right);
  if(left.type_id == Python3TypeId::IdTuple && right.type_id == Python3TypeId::IdTuple) // tuple + tuple
    return std::any_cast<Tuple>(left) + std::any_cast<Tuple>(right);

  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return std::any_cast<Integer>(left) + std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdDecimal)
    return std::any_cast<Decimal>(left) + std::any_cast<Decimal>(right);
  if(left.type_id == Python3TypeId::IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) + std::any_cast<Boolean>(right));
}
Python3Type Operation::OperationSub(Python3Type left, Python3Type right) {
  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return std::any_cast<Integer>(left) - std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdDecimal)
    return std::any_cast<Decimal>(left) - std::any_cast<Decimal>(right);
  if(left.type_id == Python3TypeId::IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) - std::any_cast<Boolean>(right));
}
Python3Type Operation::OperationMul(Python3Type left, Python3Type right) {
  if(left.type_id == Python3TypeId::IdInteger && right.type_id == Python3TypeId::IdStr) // int * str
    return std::any_cast<Integer>(left) * std::any_cast<Str>(right);
  if(left.type_id == Python3TypeId::IdStr && right.type_id == Python3TypeId::IdInteger) // str * int
    return std::any_cast<Str>(left) * std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdInteger && right.type_id == Python3TypeId::IdTuple) // int * tuple
    return std::any_cast<Integer>(left) * std::any_cast<Tuple>(right);
  if(left.type_id == Python3TypeId::IdTuple && right.type_id == Python3TypeId::IdInteger) // tuple * int
    return std::any_cast<Tuple>(left) * std::any_cast<Integer>(right);

  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return std::any_cast<Integer>(left) * std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdDecimal)
    return std::any_cast<Decimal>(left) * std::any_cast<Decimal>(right);
  if(left.type_id == Python3TypeId::IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) * std::any_cast<Boolean>(right));
}
Python3Type Operation::OperationDiv(Python3Type left, Python3Type right) {
  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return static_cast<Decimal>(std::any_cast<Integer>(left)) / static_cast<Decimal>(std::any_cast<Integer>(right));
  if(left.type_id == Python3TypeId::IdDecimal)
    return std::any_cast<Decimal>(left) / std::any_cast<Decimal>(right);
  if(left.type_id == Python3TypeId::IdBoolean)
    return static_cast<Decimal>(std::any_cast<Boolean>(left)) / static_cast<Decimal>(std::any_cast<Boolean>(right));
}
Python3Type Operation::OperationIDiv(Python3Type left, Python3Type right) {
  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return std::any_cast<Integer>(left) / std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdDecimal)
    throw OperationException("double?", "double?", "//");
  if(left.type_id == Python3TypeId::IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) / std::any_cast<Boolean>(right));
}
Python3Type Operation::OperationMod(Python3Type left, Python3Type right) {
  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return std::any_cast<Integer>(left) % std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdDecimal)
    throw OperationException("double?", "double?", "%");
  if(left.type_id == Python3TypeId::IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) % std::any_cast<Boolean>(right));
}

void Operation::OperationAddEval(Python3Type &obj, const Python3Type &other) {
  obj = OperationAdd(obj, other);
}
void Operation::OperationSubEval(Python3Type &obj, const Python3Type &other) {
  obj = OperationSub(obj, other);
}
void Operation::OperationMulEval(Python3Type &obj, const Python3Type &other) {
  obj = OperationMul(obj, other);
}
void Operation::OperationDivEval(Python3Type &obj, const Python3Type &other) {
  obj = OperationDiv(obj, other);
}
void Operation::OperationIDivEval(Python3Type &obj, const Python3Type &other) {
  obj = OperationIDiv(obj, other);
}
void Operation::OperationModEval(Python3Type &obj, const Python3Type &other) {
  obj = OperationMod(obj, other);
}

Python3Type Operation::OperationEqual(Python3Type left, Python3Type right) {
  return left == right;
}
Python3Type Operation::OperationNEqual(Python3Type left, Python3Type right) {
  return left != right;
}
Python3Type Operation::OperationLesser(Python3Type left, Python3Type right) {
  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return std::any_cast<Integer>(left) < std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdDecimal)
    return std::any_cast<Decimal>(left) < std::any_cast<Decimal>(right);
  if(left.type_id == Python3TypeId::IdBoolean)
    return std::any_cast<Boolean>(left) < std::any_cast<Boolean>(right);
  throw OperationException(
    Python3Typename[static_cast<int>(left.type_id)],
    Python3Typename[static_cast<int>(right.type_id)],
    "<");
}
Python3Type Operation::OperationGreater(Python3Type left, Python3Type right) {
  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return std::any_cast<Integer>(left) > std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdDecimal)
    return std::any_cast<Decimal>(left) > std::any_cast<Decimal>(right);
  if(left.type_id == Python3TypeId::IdBoolean)
    return std::any_cast<Boolean>(left) > std::any_cast<Boolean>(right);
  throw OperationException(
    Python3Typename[static_cast<int>(left.type_id)],
    Python3Typename[static_cast<int>(right.type_id)],
    ">");
}
Python3Type Operation::OperationNGreater(Python3Type left, Python3Type right) {
  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return std::any_cast<Integer>(left) <= std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdDecimal)
    return std::any_cast<Decimal>(left) <= std::any_cast<Decimal>(right);
  if(left.type_id == Python3TypeId::IdBoolean)
    return std::any_cast<Boolean>(left) <= std::any_cast<Boolean>(right);
  throw OperationException(
    Python3Typename[static_cast<int>(left.type_id)],
    Python3Typename[static_cast<int>(right.type_id)],
    "<=");
}
Python3Type Operation::OperationNLesser(Python3Type left, Python3Type right) {
  arith_type_cast()(left, right);
  if(left.type_id == Python3TypeId::IdInteger)
    return std::any_cast<Integer>(left) >= std::any_cast<Integer>(right);
  if(left.type_id == Python3TypeId::IdDecimal)
    return std::any_cast<Decimal>(left) >= std::any_cast<Decimal>(right);
  if(left.type_id == Python3TypeId::IdBoolean)
    return std::any_cast<Boolean>(left) >= std::any_cast<Boolean>(right);
  throw OperationException(
    Python3Typename[static_cast<int>(left.type_id)],
    Python3Typename[static_cast<int>(right.type_id)],
    ">=");
}

Python3Type Operation::OperationAnd(const Python3Type& left, const Python3Type& right) {
  bool left_res;
  if(left.type_id == Python3TypeId::IdInteger)
    left_res = std::any_cast<Integer>(left);
  else if(left.type_id == Python3TypeId::IdDecimal)
    left_res = std::any_cast<Decimal>(left);
  else if(left.type_id == Python3TypeId::IdBoolean)
    left_res = std::any_cast<Boolean>(left);
  else throw OperationException(
    Python3Typename[static_cast<int>(left.type_id)],
    Python3Typename[static_cast<int>(right.type_id)],
    "and");
  bool right_res;
  if(left.type_id == Python3TypeId::IdInteger)
    right_res = std::any_cast<Integer>(right);
  else if(left.type_id == Python3TypeId::IdDecimal)
    right_res = std::any_cast<Decimal>(right);
  else if(left.type_id == Python3TypeId::IdBoolean)
    right_res = std::any_cast<Boolean>(right);
  else throw OperationException(
    Python3Typename[static_cast<int>(left.type_id)],
    Python3Typename[static_cast<int>(right.type_id)],
    "and");
  return left_res && right_res;
}
Python3Type Operation::OperationOr(const Python3Type& left, const Python3Type& right) {
  bool left_res;
  if(left.type_id == Python3TypeId::IdInteger)
    left_res = std::any_cast<Integer>(left);
  else if(left.type_id == Python3TypeId::IdDecimal)
    left_res = std::any_cast<Decimal>(left);
  else if(left.type_id == Python3TypeId::IdBoolean)
    left_res = std::any_cast<Boolean>(left);
  else throw OperationException(
    Python3Typename[static_cast<int>(left.type_id)],
    Python3Typename[static_cast<int>(right.type_id)],
    "or");
  bool right_res;
  if(left.type_id == Python3TypeId::IdInteger)
    right_res = std::any_cast<Integer>(right);
  else if(left.type_id == Python3TypeId::IdDecimal)
    right_res = std::any_cast<Decimal>(right);
  else if(left.type_id == Python3TypeId::IdBoolean)
    right_res = std::any_cast<Boolean>(right);
  else throw OperationException(
    Python3Typename[static_cast<int>(left.type_id)],
    Python3Typename[static_cast<int>(right.type_id)],
    "or");
  return left_res || right_res;
}
Python3Type Operation::OperationNot(const Python3Type& obj) {
  bool res;
  if(obj.type_id == Python3TypeId::IdInteger)
    res = std::any_cast<Integer>(obj);
  else if(obj.type_id == Python3TypeId::IdDecimal)
    res = std::any_cast<Decimal>(obj);
  else if(obj.type_id == Python3TypeId::IdBoolean)
    res = std::any_cast<Boolean>(obj);
  else throw OperationException(Python3Typename[static_cast<int>(obj.type_id)], "not");
  return !res;
}

Python3Type Operation::OperationInteger(const Python3Type &obj) {
  return arith_type_cast()(obj, Python3TypeId::IdInteger);
}
Python3Type Operation::OperationDecimal(const Python3Type &obj) {
  return arith_type_cast()(obj, Python3TypeId::IdDecimal);
}
Python3Type Operation::OperationBoolean(const Python3Type &obj) {
  return arith_type_cast()(obj, Python3TypeId::IdBoolean);
}
Python3Type Operation::OperationStr(const Python3Type &obj) {
  return arith_type_cast()(obj, Python3TypeId::IdStr);
}

Python3Type &Operation::OperationSubscript(const Python3Type &obj, int idx) {
  if(obj.type_id != Python3TypeId::IdTuple)
    throw OperationException(Python3Typename[static_cast<int>(obj.type_id)], "operator[]");
  // TODO: segmentation fault
  return std::any_cast<Tuple>(obj)[idx];
}

void Operation::OperationPrint(const Python3Type &obj) {
  if(obj.type_id == Python3TypeId::IdNoneType)
    std::cout << "None";
  else if(obj.type_id == Python3TypeId::IdInteger)
    std::cout << std::any_cast<Integer>(obj);
  else if(obj.type_id == Python3TypeId::IdDecimal)
    std::cout << std::any_cast<Decimal>(obj);
  else if(obj.type_id == Python3TypeId::IdBoolean)
    std::cout << (std::any_cast<Boolean>(obj) ? "True" : "False");
  else if(obj.type_id == Python3TypeId::IdStr)
    std::cout << std::any_cast<Str>(obj);
  else if(obj.type_id == Python3TypeId::IdTuple) {
    auto ret = std::any_cast<Tuple>(obj);
    int len = ret.size();
    std::cout << "[";
    for(const auto &stuff: ret) {
      len--;
      OperationPrint(stuff);
      if(len != 0)
        std::cout << ", ";
    }
    std::cout << "]";
  }
}

CodeSuite::CodeSuite(CodeSuite *parent) {
  parent_suite = parent;
}

Python3Type& CodeSuite::FindVariable(const std::string& var_name) {
  CodeSuite *cur_suite = this;
  while(cur_suite != nullptr) {
    auto result = cur_suite->variables.find(var_name);
    if(result != variables.end())
      return result->second;
    else
      cur_suite = cur_suite->parent_suite;
  }
  // failure
  throw std::runtime_error("Undefined variable");
}
void CodeSuite::AddOperation(const Operation &oper) {
  operations.push_back(oper);
}
void CodeSuite::AddVariable(const std::string &name, const Python3Type &var) {
  variables.insert({name, var});
}
void CodeSuite::AddFunction(const Function *func) {
  functions.insert({func->name(), func});
}

std::string Function::name() const {
  return func_name;
}
Function::Function(const std::string &funcname) {
  func_name = funcname;
}


