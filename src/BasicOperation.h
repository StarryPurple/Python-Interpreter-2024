#pragma once
#ifndef BASIC_OPERATION_H
#define BASIC_OPERATION_H

#include "int2048.h"
#include <iostream>
#include <any>
#include <vector>

namespace Interpreter {

using Integer = sjtu::int2048;
using Decimal = double;
using Boolean = bool;
using String = std::string;
struct None{};

using Tuple = std::vector<std::any>;

const auto ConstNone = None();

enum class VType {
  tInteger, tDecimal, tBoolean, tString, tNone,
};

VType type_trait(const std::any &);

void unzip(std::any &);

// builtin type-cast function.

Integer to_Integer(const std::any &);
Decimal to_Decimal(const std::any &);
Boolean to_Boolean(const std::any &);
String to_String(const std::any &);

// builtin arithmetic operations.

std::any operator+(const std::any &, const std::any &);
void operator+=(std::any &, const std::any &);

std::any operator-(const std::any &, const std::any &);
void operator-=(std::any &, const std::any &);

std::any operator*(const std::any &, const std::any &);
void operator*=(std::any &, const std::any &);

std::any operator/(const std::any &, const std::any &);
void operator/=(std::any &, const std::any &);

// integer division operator "//"
std::any operator|(const std::any &, const std::any &);
// integer division assignment operator "//="
void operator|=(std::any &, const std::any &);

std::any operator%(const std::any &, const std::any &);
void operator%=(std::any &, const std::any &);

std::any operator+(const std::any &);
std::any operator-(const std::any &);

// builtin comparison operators.
// return (Boolean) true or false packed in std::any.

std::any operator==(const std::any &, const std::any &);
std::any operator!=(const std::any &, const std::any &);

std::any operator<(const std::any &, const std::any &);
std::any operator>(const std::any &, const std::any &);
std::any operator<=(const std::any &, const std::any &);
std::any operator>=(const std::any &, const std::any &);

// builtin printer.

void print(const std::any &);
void print(const Tuple &);

// builtin testlist splitter with spilt sign ",". removes whitespaces.
// e.g.   "  1a e,  sa\n ,  e@eq " -> {"1ae", "sa\n", "e@eq"}
std::vector<std::string> testlist_splitter(const std::string &);

// Sometimes we need to turn a 1-size Tuple to Value.

void convert_single(std::any &);

}

#endif // BASIC_OPERATION_H