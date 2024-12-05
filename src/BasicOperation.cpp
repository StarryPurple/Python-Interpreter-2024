#include "BasicOperation.h"

namespace Interpreter {

VType type_trait(const std::any &obj) {
  if(auto ptr = std::any_cast<Integer>(&obj); ptr)
    return VType::tInteger;
  else if(auto ptr = std::any_cast<Decimal>(&obj); ptr)
    return VType::tDecimal;
  else if(auto ptr = std::any_cast<Boolean>(&obj); ptr)
    return VType::tBoolean;
  else if(auto ptr = std::any_cast<String>(&obj); ptr)
    return VType::tString;
  else if(auto ptr = std::any_cast<None>(&obj); ptr)
    return VType::tNone;
  // else throw std::runtime_error("Type trait fails.");
}

Integer to_Integer(const std::any &obj) {
  VType type = type_trait(obj);
  switch(type) {
    case VType::tInteger: return std::any_cast<Integer>(obj); // return self
    case VType::tDecimal: return std::any_cast<Decimal>(obj); // double -> long long
    case VType::tBoolean: return std::any_cast<Boolean>(obj) ? 1 : 0;
    case VType::tString: return std::any_cast<String>(obj); // int2048 builtin
    // default: throw std::runtime_error("Value conversion to type \"Integer\" fails.");
  }
}

Decimal to_Decimal(const std::any &obj) {
  VType type = type_trait(obj);
  switch(type) {
    case VType::tInteger: return std::any_cast<Integer>(obj); // int2048 builtin
    case VType::tDecimal: return std::any_cast<Decimal>(obj); // return self
    case VType::tBoolean: return std::any_cast<Boolean>(obj) ? 1.0 : 0.0;
    case VType::tString: return std::stod(std::any_cast<String>(obj));
    // default: throw std::runtime_error("Value conversion to type \"Decimal\" fails.");
  }
}

Boolean to_Boolean(const std::any &obj) {
  // Tuple special judge
  if(std::any_cast<Tuple>(&obj) != nullptr) {
    Tuple tmp = std::any_cast<Tuple>(obj);/*
    if(tmp.size() != 1)
      throw std::runtime_error("Don't converse Tuples to Boolean.");*/
    return to_Boolean(tmp[0]);
  }
  VType type = type_trait(obj);
  switch(type) {
    case VType::tInteger: return !(std::any_cast<Integer>(obj).is_zero());
    case VType::tDecimal: return std::any_cast<Decimal>(obj) != 0;
    case VType::tBoolean: return std::any_cast<Boolean>(obj);
    case VType::tString: return std::any_cast<String>(obj) != "";
    case VType::tNone: return true; // Is this line acceptable?
    default: throw std::runtime_error("Value conversion to type \"Boolean\" fails.");
  }
}

String to_String(const std::any &obj) {
  VType type = type_trait(obj);
  switch(type) {
    case VType::tInteger: return std::any_cast<Integer>(obj); // int2048 builtin
    case VType::tDecimal: return std::to_string(std::any_cast<Decimal>(obj));
    case VType::tBoolean: return std::any_cast<Boolean>(obj) ? "True" : "False";
    case VType::tString: return std::any_cast<String>(obj);
    case VType::tNone: return "None";
    default: throw std::runtime_error("Value conversion to type \"String\" fails.");
  }
}

std::any operator+(const std::any &objL, const std::any &objR) {
  VType typeL = type_trait(objL), typeR = type_trait(objR);
  if(typeL == VType::tNone || typeR == VType::tNone)
    throw std::runtime_error("Failure in operator \"+\".");
  else if(typeL == VType::tString && typeR == VType::tString)
    return std::any_cast<String>(objL) + std::any_cast<String>(objR);
  else if(typeL == VType::tString || typeR == VType::tString)
    throw std::runtime_error("Failure in operator \"+\".");
  else if(typeL == VType::tDecimal || typeR == VType::tDecimal)
    return to_Decimal(objL) + to_Decimal(objR);
  else return to_Integer(objL) + to_Integer(objR);
}

void operator+=(std::any &objL, const std::any &objR) {
  objL = objL + objR;
}

std::any operator-(const std::any &objL, const std::any &objR) {
  VType typeL = type_trait(objL), typeR = type_trait(objR);
  if(typeL == VType::tNone || typeR == VType::tNone)
    throw std::runtime_error("Failure in operator \"-\".");
  else if(typeL == VType::tString || typeR == VType::tString)
    throw std::runtime_error("Failure in operator \"-\".");
  else if(typeL == VType::tDecimal || typeR == VType::tDecimal)
    return to_Decimal(objL) - to_Decimal(objR);
  else return to_Integer(objL) - to_Integer(objR);
}

void operator-=(std::any &objL, const std::any &objR) {
  objL = objL - objR;
}

std::any operator*(const std::any &objL, const std::any &objR) {
  VType typeL = type_trait(objL), typeR = type_trait(objR);
  if(typeL == VType::tNone || typeR == VType::tNone)
    throw std::runtime_error("Failure in operator \"*\".");
  else if(typeL == VType::tString && typeR == VType::tInteger) {
    Integer times = std::any_cast<Integer>(objR);
    if(times.storage_sign == -1 && !times.is_zero())
      throw std::runtime_error("Failure in operator \"*\".");
    String str = std::any_cast<String>(objL);
    String res = "";
    while(!times.is_zero()) {
      res += str;
      times -= 1;
    }
    return res;
  } else if(typeL == VType::tInteger && typeR == VType::tString) {
    Integer times = std::any_cast<Integer>(objL);
    if(times.storage_sign == -1 && !times.is_zero())
      throw std::runtime_error("Failure in operator \"*\".");
    String str = std::any_cast<String>(objR);
    String res = "";
    while(!times.is_zero()) {
      res += str;
      times -= 1;
    }
    return res;
  } else if(typeL == VType::tString || typeR == VType::tString)
    throw std::runtime_error("Failure in operator \"*\".");
  else if(typeL == VType::tDecimal || typeR == VType::tDecimal)
    return to_Decimal(objL) * to_Decimal(objR);
  else return to_Integer(objL) * to_Integer(objR);
}

void operator*=(std::any &objL, const std::any &objR) {
  objL = objL * objR;
}

std::any operator/(const std::any &objL, const std::any &objR) {
  VType typeL = type_trait(objL), typeR = type_trait(objR);
  if(typeL == VType::tNone || typeR == VType::tNone)
    throw std::runtime_error("Failure in operator \"/\".");
  else if(typeL == VType::tString || typeR == VType::tString)
    throw std::runtime_error("Failure in operator \"/\".");
  return to_Decimal(objL) / to_Decimal(objR);
}

void operator/=(std::any &objL, const std::any &objR) {
  objL = objL / objR;
}

std::any operator|(const std::any &objL, const std::any &objR) {
  VType typeL = type_trait(objL), typeR = type_trait(objR);
  if(typeL == VType::tNone || typeR == VType::tNone)
    throw std::runtime_error("Failure in operator \"//\".");
  else if(typeL == VType::tString || typeR == VType::tString)
    throw std::runtime_error("Failure in operator \"//\".");
  else if(typeL == VType::tDecimal || typeR == VType::tDecimal)
    return std::floor(to_Decimal(objL) / to_Decimal(objR));
  else return to_Integer(objL) / to_Integer(objR);
}

void operator|=(std::any &objL, const std::any &objR) {
  objL = objL | objR;
}

std::any operator%(const std::any &objL, const std::any &objR) {
  VType typeL = type_trait(objL), typeR = type_trait(objR);
  if(typeL == VType::tNone || typeR == VType::tNone)
    throw std::runtime_error("Failure in operator \"%\".");
  else if(typeL == VType::tString || typeR == VType::tString)
    throw std::runtime_error("Failure in operator \"%\".");
  else if(typeL == VType::tDecimal || typeR == VType::tDecimal) {
    Decimal DL = to_Decimal(objL), DR = to_Decimal(objR);
    return DL - std::floor(DL / DR) * DR;
  }
  else return to_Integer(objL) % to_Integer(objR);
}

void operator%=(std::any &objL, const std::any &objR) {
  objL = objL % objR;
}

std::any operator+(const std::any &obj) {
  return obj;
}

std::any operator-(const std::any &obj) {
  VType type = type_trait(obj);
  if(type == VType::tInteger)
    return -std::any_cast<Integer>(obj);
  else if(type == VType::tDecimal)
    return -std::any_cast<Decimal>(obj);
  else if(type == VType::tBoolean)
    return std::any_cast<Boolean>(obj) ? -1 : 0;
  else throw std::runtime_error("Failure in Abel inversion \"-\".");
}

std::any operator==(const std::any &objL, const std::any &objR) {
  VType typeL = type_trait(objL), typeR = type_trait(objR);
  if(typeL == VType::tNone && typeR == VType::tNone)
    return true;
  else if(typeL == VType::tNone || typeR == VType::tNone)
    return false;
  else if(typeL == VType::tString && typeR == VType::tString)
    return std::any_cast<String>(objL) == std::any_cast<String>(objR);
  else if(typeL == VType::tString && std::any_cast<String>(objL).empty())
    return to_Boolean(objR) == false;
  else if(typeR == VType::tString && std::any_cast<String>(objR).empty())
    return to_Boolean(objL) == false;
  else if(typeL == VType::tString || typeR == VType::tString)
    return false;
  else if(typeL == VType::tDecimal || typeR == VType::tDecimal) {
    Decimal dlt = to_Decimal(objL) - to_Decimal(objR);
    return (dlt < 0.0000004 && -dlt < 0.0000004);
  }
  else if(typeL == VType::tInteger || typeR == VType::tInteger)
    return to_Integer(objL) == to_Integer(objR);
  else return std::any_cast<Boolean>(objL) == std::any_cast<Boolean>(objR);
}

std::any operator!=(const std::any &objL, const std::any &objR) {
  return !std::any_cast<Boolean>(objL == objR);
}

std::any operator<(const std::any &objL, const std::any &objR) {
  VType typeL = type_trait(objL), typeR = type_trait(objR);
  if(typeL == VType::tNone && typeR == VType::tNone)
    return false;
  else if(typeL == VType::tNone || typeR == VType::tNone)
    return false;
  else if(typeL == VType::tString && typeR == VType::tString)
    return std::any_cast<String>(objL) < std::any_cast<String>(objR);
  else if(typeL == VType::tString || typeR == VType::tString)
    return false;
  else if(typeL == VType::tDecimal || typeR == VType::tDecimal) {
    Decimal res = to_Decimal(objR) - to_Decimal(objL);
    return res > 0.0000004;
  }
  else return to_Integer(objL) < to_Integer(objR);
}

std::any operator>(const std::any &objL, const std::any &objR) {
  return objR < objL;
}

std::any operator<=(const std::any &objL, const std::any &objR) {
  return !std::any_cast<Boolean>(objL > objR);
}

std::any operator>=(const std::any &objL, const std::any &objR) {
  return objR <= objL;
}

void print(const std::any &obj) {
  std::string res = to_String(obj);
  for(std::size_t i = res.length() - 1; i > 0; i--)
    if(res[i - 1] == '\\')
      if(res[i] == 'n') {
        res.erase(i, 1);
        res[i - 1] = '\n';
      } else if(res[i] == 't') {
        res.erase(i, 1);
        res[i - 1] = '\t';
      } else if(res[i] == '\"') {
        res.erase(i, 1);
        res[i - 1] = '\"';
      } else if(res[i] == '\'') {
        res.erase(i, 1);
        res[i - 1] = '\'';
      }
  std::cout << res;
}

void print(const Tuple &tuple) {
  Tuple ntuple = tuple;
  tuple_unzip(ntuple);
  auto size = ntuple.size();
  for(const auto &obj: ntuple) {
    print(obj);
    size--;
    if(size > 0)
      std::cout << " ";
  }
  std::cout << "\n";
}

std::vector<std::string> testlist_splitter(const std::string &testlist_str) {
  std::vector<std::string> res;
  std::string name = "";
  for(auto ch: testlist_str) {
    if(ch == ' ') continue;
    else if(ch == ',') {
      res.push_back(name);
      name = "";
      continue;
    } else name += ch;
  }
  if(name != "")
    res.push_back(name);
  return res;
}

void tuple_unzip(Tuple &obj) {
  Tuple res;
  for(auto val: obj) {
    if(std::any_cast<Tuple>(&val) != nullptr) {
      Tuple tmp = std::any_cast<Tuple>(val);
      tuple_unzip(tmp);
      res.insert(res.end(), tmp.begin(), tmp.end());
    } else res.push_back(val);
  }
  obj = res;
}

}
