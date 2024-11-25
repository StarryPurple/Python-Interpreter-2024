#include "Python3TypeDefinition.h"


enum TypeId {
  IdNoneType = 0, IdInteger = 1, IdDecimal = 2, IdStr = 3, IdBoolean = 4, IdTuple = 5
};


integer::integer(const integer_type value_): value(value_) {}
integer::operator integer_type () const {return value;}

decimal::decimal(const double value_): value(value_) {}
decimal::operator double () const {return value;}

str::str(const std::string &value_): value(value_) {}
str::operator std::string () const {return value;}

boolean::boolean(const bool value_): value(value_) {}
boolean::operator bool () const {return value;}

tuple::tuple(const tuple_type &value_): value(value_) {}
tuple::operator tuple_type () const {return value;}

Python3Type::Python3Type(): type_id(IdNoneType), value(NoneType()) {}
Python3Type::Python3Type(const Python3Type &other) = default;
Python3Type::Python3Type(const NoneType &input): type_id(IdNoneType), value(input) {}
Python3Type::Python3Type(const integer &input): type_id(IdInteger), value(input) {}
Python3Type::Python3Type(const decimal &input): type_id(IdDecimal), value(input) {}
Python3Type::Python3Type(const str &input): type_id(IdStr), value(input) {}
Python3Type::Python3Type(const boolean &input): type_id(IdBoolean), value(input) {}
Python3Type::Python3Type(const tuple &input): type_id(IdTuple), value(input) {}

Python3Type& Python3Type::operator=(const NoneType &input): type_id(IdNoneType), value(input) {return *this;}
Python3Type& Python3Type::operator=(const integer &input): type_id(IdInteger), value(input) {return *this;}
Python3Type& Python3Type::operator=(const decimal &input): type_id(IdDecimal), value(input) {return *this;}
Python3Type& Python3Type::operator=(const str &input): type_id(IdStr), value(input) {return *this;}
Python3Type& Python3Type::operator=(const boolean &input): type_id(IdBoolean), value(input) {return *this;}
Python3Type& Python3Type::operator=(const tuple &input): type_id(IdTuple), value(input) {return *this;}
Python3Type& Python3Type::operator=(const Python3Type &) = default;
