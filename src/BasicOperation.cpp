#include "BasicOperation.h"

#include <Exceptions.h>
#include <support/CPPUtils.h>


enum Function::function_operations {
  OprEval = 0,
  OprAdd = 1, OprSub = 2, OprMul = 3, OprDiv = 4, OprIDiv = 5, OprMod = 6,
  OprAddEval = 7, OprSubEval = 8, OprMulEval = 9, OprDivEval = 10, OprIDivEval = 11, OprModEval = 12,
  OprEqual = 13, OprNEqual = 14, OprLesser = 15, OprGreater = 16, OprNGreater = 17, OprNLesser = 18,
  OprAnd = 19, OprOr = 20, OprNot = 21
};

class Function::OperationException: std::exception {
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
class Function::InvalidVariableException: std::exception {
  std::string name;
public:
  InvalidVariableException(const std::string &name_): name(name_) {}
  virtual const char* what() const override {
    return ("Reserved name for \"" + name + "\"").c_str();
  }
};

Python3Type Function::arith_type_cast::operator()(const Python3Type &obj, const Python3TypeId type) const {
  Python3TypeId obj_type = obj.type_id;
  // if(type == IdNoneType && obj_type == IdNoneType) return obj;
  // if(type == IdTuple && obj_type == IdTuple) return obj;
  if(type == IdInteger) {
    if(obj_type == IdInteger)      // integer -> integer
      return obj;
    else if(obj_type == IdDecimal) // decimal -> integer
      return Integer(static_cast<Integer>(std::any_cast<Decimal>(obj)));
    else if(obj_type == IdStr)     // str     -> integer // TODO: stoll -> int2048 constructor
      return Integer(std::stoll(std::any_cast<Str>(obj)));
    else if(obj_type == IdBoolean) // boolean -> integer
      return Integer(static_cast<Integer>(std::any_cast<Boolean>(obj)));

  } else if(type == IdDecimal) {
    if(obj_type == IdInteger)      // integer -> decimal
      return Decimal(static_cast<Decimal>(std::any_cast<Integer>(obj)));
    else if(obj_type == IdDecimal) // decimal -> decimal
      return obj;
    else if(obj_type == IdStr)     // str     -> decimal
      return Decimal(std::stod(std::any_cast<Str>(obj)));
    else if(obj_type == IdBoolean) // boolean -> decimal
      return Decimal(static_cast<Decimal>(std::any_cast<Boolean>(obj)));

  } else if(type == IdStr) {
    if(obj_type == IdInteger)      // integer -> str // TODO: int2048 specification
      return Str(std::to_string(std::any_cast<Integer>(obj)));
    else if(obj_type == IdDecimal) // decimal -> str
      return Str(std::to_string(std::any_cast<Decimal>(obj))); // 6 precision digits
    else if(obj_type == IdStr)     // str     -> str
      return obj;
    else if(obj_type == IdBoolean) // boolean -> str
      return Str(std::any_cast<Boolean>(obj) ? "True" : "False");

  } else if(type == IdBoolean) {
    if(obj_type == IdInteger)      // integer -> boolean
      return Boolean(static_cast<Boolean>(std::any_cast<Integer>(obj)));
    else if(obj_type == IdDecimal) // decimal -> boolean
      return Boolean(static_cast<Boolean>(std::any_cast<Decimal>(obj)));
    else if(obj_type == IdStr)     // str     -> boolean
      return Boolean(!std::any_cast<Str>(obj).empty());
    else if(obj_type == IdBoolean) // boolean -> boolean
      return obj;
  }
  // no else
  throw OperationException(Python3Typename[obj_type], Python3Typename[type] + "()");
}

void Function::arith_type_cast::operator()(Python3Type &left, Python3Type &right) const {
  Python3TypeId left_id = left.type_id, right_id = right.type_id;
  Python3TypeId target_id = IdNoneType;
  if(left_id == IdInteger) {
    if(right_id == IdInteger)      // integer && integer -> integer
      target_id = IdInteger;
    else if(right_id == IdDecimal) // decimal && integer -> decimal
      target_id = IdDecimal;
    else if(right_id == IdBoolean) // boolean && integer -> integer
      target_id = IdInteger;

  } else if(left_id == IdDecimal) {
    if(right_id == IdInteger)      // integer && decimal -> decimal
      target_id = IdDecimal;
    else if(right_id == IdDecimal) // decimal && decimal -> decimal
      target_id = IdDecimal;
    else if(right_id == IdBoolean) // boolean && decimal -> decimal
      target_id = IdDecimal;

  } else if(left_id == IdBoolean) {
    if(right_id == IdInteger)      // integer && boolean -> integer
      target_id = IdInteger;
    else if(right_id == IdDecimal) // decimal && boolean -> decimal
      target_id = IdDecimal;
    else if(right_id == IdBoolean) // boolean && boolean -> boolean
      target_id = IdBoolean;

  }/*else if(left_id == IdStr) {
    if(right_id == IdStr)
      target_id = IdStr;
  }*/
  // no else
  if(target_id == IdNoneType)
    throw OperationException(Python3Typename[left_id], Python3Typename[right_id], "arithmetic operation");
  left = arith_type_cast()(left, target_id);
  right = arith_type_cast()(right, target_id);
}



// Attention: @target must be already defined
void Function::OperationEval(Python3Type &obj, const Python3Type &other) const {
  obj = other;
}

Python3Type Function::OperationAdd(Python3Type left, Python3Type right) const {
  if(left.type_id == IdStr && right.type_id == IdStr) // str + str
    return std::any_cast<Str>(left) + std::any_cast<Str>(right);
  if(left.type_id == IdTuple && right.type_id == IdTuple) // tuple + tuple
    return std::any_cast<Tuple>(left) + std::any_cast<Tuple>(right);

  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return std::any_cast<Integer>(left) + std::any_cast<Integer>(right);
  if(left.type_id == IdDecimal)
    return std::any_cast<Decimal>(left) + std::any_cast<Decimal>(right);
  if(left.type_id == IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) + std::any_cast<Boolean>(right));
}
Python3Type Function::OperationSub(Python3Type left, Python3Type right) const {
  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return std::any_cast<Integer>(left) - std::any_cast<Integer>(right);
  if(left.type_id == IdDecimal)
    return std::any_cast<Decimal>(left) - std::any_cast<Decimal>(right);
  if(left.type_id == IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) - std::any_cast<Boolean>(right));
}
Python3Type Function::OperationMul(Python3Type left, Python3Type right) const {
  if(left.type_id == IdInteger && right.type_id == IdStr) // int * str
    return std::any_cast<Integer>(left) * std::any_cast<Str>(right);
  if(left.type_id == IdStr && right.type_id == IdInteger) // str * int
    return std::any_cast<Str>(left) * std::any_cast<Integer>(right);
  if(left.type_id == IdInteger && right.type_id == IdTuple) // int * tuple
    return std::any_cast<Integer>(left) * std::any_cast<Tuple>(right);
  if(left.type_id == IdTuple && right.type_id == IdInteger) // tuple * int
    return std::any_cast<Tuple>(left) * std::any_cast<Integer>(right);

  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return std::any_cast<Integer>(left) * std::any_cast<Integer>(right);
  if(left.type_id == IdDecimal)
    return std::any_cast<Decimal>(left) * std::any_cast<Decimal>(right);
  if(left.type_id == IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) * std::any_cast<Boolean>(right));
}
Python3Type Function::OperationDiv(Python3Type left, Python3Type right) const {
  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return static_cast<Decimal>(std::any_cast<Integer>(left)) / static_cast<Decimal>(std::any_cast<Integer>(right));
  if(left.type_id == IdDecimal)
    return std::any_cast<Decimal>(left) / std::any_cast<Decimal>(right);
  if(left.type_id == IdBoolean)
    return static_cast<Decimal>(std::any_cast<Boolean>(left)) / static_cast<Decimal>(std::any_cast<Boolean>(right));
}
Python3Type Function::OperationIDiv(Python3Type left, Python3Type right) const {
  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return std::any_cast<Integer>(left) / std::any_cast<Integer>(right);
  if(left.type_id == IdDecimal)
    throw OperationException("double?", "double?", "//");
  if(left.type_id == IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) / std::any_cast<Boolean>(right));
}
Python3Type Function::OperationMod(Python3Type left, Python3Type right) const {
  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return std::any_cast<Integer>(left) % std::any_cast<Integer>(right);
  if(left.type_id == IdDecimal)
    throw OperationException("double?", "double?", "%");
  if(left.type_id == IdBoolean)
    return static_cast<Integer>(std::any_cast<Boolean>(left) % std::any_cast<Boolean>(right));
}

void Function::OperationAddEval(Python3Type &obj, const Python3Type &other) const {
  obj = OperationAdd(obj, other);
}
void Function::OperationSubEval(Python3Type &obj, const Python3Type &other) const {
  obj = OperationSub(obj, other);
}
void Function::OperationMulEval(Python3Type &obj, const Python3Type &other) const {
  obj = OperationMul(obj, other);
}
void Function::OperationDivEval(Python3Type &obj, const Python3Type &other) const {
  obj = OperationDiv(obj, other);
}
void Function::OperationIDivEval(Python3Type &obj, const Python3Type &other) const {
  obj = OperationIDiv(obj, other);
}
void Function::OperationModEval(Python3Type &obj, const Python3Type &other) const {
  obj = OperationMod(obj, other);
}

Python3Type Function::OperationEqual(Python3Type left, Python3Type right) const {
  return left == right;
}
Python3Type Function::OperationNEqual(Python3Type left, Python3Type right) const {
  return left != right;
}
Python3Type Function::OperationLesser(Python3Type left, Python3Type right) const {
  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return std::any_cast<Integer>(left) < std::any_cast<Integer>(right);
  if(left.type_id == IdDecimal)
    return std::any_cast<Decimal>(left) < std::any_cast<Decimal>(right);
  if(left.type_id == IdBoolean)
    return std::any_cast<Boolean>(left) < std::any_cast<Boolean>(right);
  throw OperationException(Python3Typename[left.type_id], Python3Typename[right.type_id], "<");
}
Python3Type Function::OperationGreater(Python3Type left, Python3Type right) const {
  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return std::any_cast<Integer>(left) > std::any_cast<Integer>(right);
  if(left.type_id == IdDecimal)
    return std::any_cast<Decimal>(left) > std::any_cast<Decimal>(right);
  if(left.type_id == IdBoolean)
    return std::any_cast<Boolean>(left) > std::any_cast<Boolean>(right);
  throw OperationException(Python3Typename[left.type_id], Python3Typename[right.type_id], ">");
}
Python3Type Function::OperationNGreater(Python3Type left, Python3Type right) const {
  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return std::any_cast<Integer>(left) <= std::any_cast<Integer>(right);
  if(left.type_id == IdDecimal)
    return std::any_cast<Decimal>(left) <= std::any_cast<Decimal>(right);
  if(left.type_id == IdBoolean)
    return std::any_cast<Boolean>(left) <= std::any_cast<Boolean>(right);
  throw OperationException(Python3Typename[left.type_id], Python3Typename[right.type_id], "<=");
}
Python3Type Function::OperationNLesser(Python3Type left, Python3Type right) const {
  arith_type_cast()(left, right);
  if(left.type_id == IdInteger)
    return std::any_cast<Integer>(left) >= std::any_cast<Integer>(right);
  if(left.type_id == IdDecimal)
    return std::any_cast<Decimal>(left) >= std::any_cast<Decimal>(right);
  if(left.type_id == IdBoolean)
    return std::any_cast<Boolean>(left) >= std::any_cast<Boolean>(right);
  throw OperationException(Python3Typename[left.type_id], Python3Typename[right.type_id], ">=");
}

Python3Type Function::OperationAnd(const Python3Type& left, const Python3Type& right) {
  bool left_res;
  if(left.type_id == IdInteger)
    left_res = std::any_cast<Integer>(left);
  else if(left.type_id == IdDecimal)
    left_res = std::any_cast<Decimal>(left);
  else if(left.type_id == IdBoolean)
    left_res = std::any_cast<Boolean>(left);
  else throw OperationException(Python3Typename[left.type_id], Python3Typename[right.type_id], "and");
  bool right_res;
  if(left.type_id == IdInteger)
    right_res = std::any_cast<Integer>(right);
  else if(left.type_id == IdDecimal)
    right_res = std::any_cast<Decimal>(right);
  else if(left.type_id == IdBoolean)
    right_res = std::any_cast<Boolean>(right);
  else throw OperationException(Python3Typename[left.type_id], Python3Typename[right.type_id], "and");
  return left_res && right_res;
}
Python3Type Function::OperationOr(const Python3Type& left, const Python3Type& right) {
  bool left_res;
  if(left.type_id == IdInteger)
    left_res = std::any_cast<Integer>(left);
  else if(left.type_id == IdDecimal)
    left_res = std::any_cast<Decimal>(left);
  else if(left.type_id == IdBoolean)
    left_res = std::any_cast<Boolean>(left);
  else throw OperationException(Python3Typename[left.type_id], Python3Typename[right.type_id], "or");
  bool right_res;
  if(left.type_id == IdInteger)
    right_res = std::any_cast<Integer>(right);
  else if(left.type_id == IdDecimal)
    right_res = std::any_cast<Decimal>(right);
  else if(left.type_id == IdBoolean)
    right_res = std::any_cast<Boolean>(right);
  else throw OperationException(Python3Typename[left.type_id], Python3Typename[right.type_id], "or");
  return left_res || right_res;
}
Python3Type Function::OperationNot(const Python3Type& obj) {
  bool res;
  if(obj.type_id == IdInteger)
    res = std::any_cast<Integer>(obj);
  else if(obj.type_id == IdDecimal)
    res = std::any_cast<Decimal>(obj);
  else if(obj.type_id == IdBoolean)
    res = std::any_cast<Boolean>(obj);
  else throw OperationException(Python3Typename[obj.type_id], "not");
  return !res;
}
