#pragma once
#ifndef PYTHON3_TYPE_DEFINITION_H
#define PYTHON3_TYPE_DEFINITION_H

#include <iostream>
#include <any>
#include <vector>

enum TypeId: int;

class Python3Type;
class NoneType {
};
class integer {
  using integer_type = long long; // TODO: int2048
  integer_type value = 0;
  explicit integer(const integer_type value_);
  operator integer_type () const;
};
class decimal {
  double value = 0.0;
  explicit decimal(const double value_);
  operator double () const;
};
class str {
  std::string value;
  explicit str(const std::string &value_);
  operator std::string () const;
};
class boolean {
  bool value = false;
  explicit boolean (const bool value_);
  operator bool () const;
};
class tuple {
  using tuple_type = std::vector<Python3Type>;
  tuple_type value;
  explicit tuple (const tuple_type &value_);
  operator tuple_type () const;
};
class Python3Type {
  int type_id;
  std::any value;
  // default value: None
  Python3Type();
  Python3Type(const Python3Type &);
  Python3Type(const NoneType &);
  Python3Type(const integer &);
  Python3Type(const decimal &);
  Python3Type(const str &);
  Python3Type(const boolean &);
  Python3Type(const tuple &);

  Python3Type& operator=(const Python3Type &);
  Python3Type& operator=(const NoneType &);
  Python3Type& operator=(const integer &);
  Python3Type& operator=(const decimal &);
  Python3Type& operator=(const str &);
  Python3Type& operator=(const boolean &);
  Python3Type& operator=(const tuple &);
};


#endif // PYTHON3_TYPE_DEFINITION_H