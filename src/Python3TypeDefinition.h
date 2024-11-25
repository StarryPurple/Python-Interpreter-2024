#pragma once
#ifndef PYTHON3_TYPE_DEFINITION_H
#define PYTHON3_TYPE_DEFINITION_H

#include <iostream>
#include <any>
#include <vector>

enum Python3TypeId {
  IdNoneType = 0, IdInteger = 1, IdDecimal = 2, IdStr = 3, IdBoolean = 4, IdTuple = 5
};

inline std::string Python3Typename[6] = {
 "None", "int", "float", "str", "bool", "tuple"};

class Python3Type;
class NoneType {
public:
  NoneType();
}
using Integer = long long; // TODO: int2048
using Decimal = double;
using Str = std::string;
using Boolean = bool;
using Tuple = std::vector<Python3Type>;

Str operator+(const Str &, const Str &);
Str operator*(const Integer &, const Str &);
Str operator*(const Str &, const Integer &);
Tuple operator+(const Tuple &, const Tuple &);
Tuple operator*(const Integer &, const Tuple &);
Tuple operator*(const Tuple &, const Integer &);

class Python3Type {
public:
  std::any value;
  Python3TypeId type_id;
  // default value: None
  Python3Type();
  Python3Type(const Python3Type &);
  Python3Type(const NoneType &);
  Python3Type(const Integer &);
  Python3Type(const Decimal &);
  Python3Type(const Str &);
  Python3Type(const Boolean &);
  Python3Type(const Tuple &);

  Python3Type& operator=(const Python3Type &);
  Python3Type& operator=(const NoneType &);
  Python3Type& operator=(const Integer &);
  Python3Type& operator=(const Decimal &);
  Python3Type& operator=(const Str &);
  Python3Type& operator=(const Boolean &);
  Python3Type& operator=(const Tuple &);

  operator std::any () const;
  bool operator==(const Python3Type &) const;
  bool operator!=(const Python3Type &) const;
};


#endif // PYTHON3_TYPE_DEFINITION_H