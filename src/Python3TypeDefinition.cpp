#include "Python3TypeDefinition.h"


Str operator+(const Str &left, const Str &right) {
  Str res = left;
  res += right;
  return res;
}
Str operator*(const Integer &integer, const Str &str) {
  if(integer < 0) return {};
  Str res;
  for(Integer i = 0; i < integer; i++)
    res += str;
  return res;
}
Str operator*(const Str &str, const Integer &integer) {
  return operator*(integer, str);
}

Tuple operator+(const Tuple &left, const Tuple &right) {
  Tuple res = left;
  res.insert(res.end(), right.begin(), right.end());
  return res;
}
Tuple operator*(const Integer &integer, const Tuple &tuple) {
  if(integer < 0) return {};
  Tuple res;
  for(Integer i = 0; i < integer; i++)
    res.insert(res.end(), tuple.begin(), tuple.end());
  return res;
}
Tuple operator*(const Tuple &tuple, const Integer &integer) {
  return operator*(integer, tuple);
}



Python3Type::Python3Type(): type_id(Python3TypeId::IdNoneType), value(NoneType()) {}
Python3Type::Python3Type(const Python3Type &other) = default;
Python3Type::Python3Type(const NoneType &input): type_id(Python3TypeId::IdNoneType), value(input) {}
Python3Type::Python3Type(const Integer &input): type_id(Python3TypeId::IdInteger), value(input) {}
Python3Type::Python3Type(const Decimal &input): type_id(Python3TypeId::IdDecimal), value(input) {}
Python3Type::Python3Type(const Str &input): type_id(Python3TypeId::IdStr), value(input) {}
Python3Type::Python3Type(const Boolean &input): type_id(Python3TypeId::IdBoolean), value(input) {}
Python3Type::Python3Type(const Tuple &input): type_id(Python3TypeId::IdTuple), value(input) {}

Python3Type& Python3Type::operator=(const NoneType &input): type_id(Python3TypeId::IdNoneType), value(input) {return *this;}
Python3Type& Python3Type::operator=(const Integer &input): type_id(Python3TypeId::IdInteger), value(input) {return *this;}
Python3Type& Python3Type::operator=(const Decimal &input): type_id(Python3TypeId::IdDecimal), value(input) {return *this;}
Python3Type& Python3Type::operator=(const Str &input): type_id(Python3TypeId::IdStr), value(input) {return *this;}
Python3Type& Python3Type::operator=(const Boolean &input): type_id(Python3TypeId::IdBoolean), value(input) {return *this;}
Python3Type& Python3Type::operator=(const Tuple &input): type_id(Python3TypeId::IdTuple), value(input) {return *this;}
Python3Type& Python3Type::operator=(const Python3Type &) = default;

Python3Type::operator std::any() const {return value;}
bool Python3Type::operator==(const Python3Type &other) const {
  if(type_id != other.type_id) return false;
  if(type_id == Python3TypeId::IdNoneType) return true;
  if(type_id == Python3TypeId::IdInteger) return std::any_cast<Integer>(value) == std::any_cast<Integer>(other.value);
  if(type_id == Python3TypeId::IdDecimal) return std::any_cast<Decimal>(value) == std::any_cast<Decimal>(other.value);
  if(type_id == Python3TypeId::IdStr) return std::any_cast<Str>(value) == std::any_cast<Str>(other.value);
  if(type_id == Python3TypeId::IdBoolean) return std::any_cast<Boolean>(value) == std::any_cast<Boolean>(other.value);
  if(type_id == Python3TypeId::IdTuple) return std::any_cast<Tuple>(value) == std::any_cast<Tuple>(other.value);
}
bool Python3Type::operator!=(const Python3Type &other) const {
  return !operator==(other);
}
// Identity check.
bool Python3Type::is(const Python3Type &other) const {
  return this == &other;
}
