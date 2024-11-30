#pragma once
#ifndef CODE_INTERPRETER_H
#define CODE_INTERPRETER_H

#include "BasicStructure.h"

inline std::vector<Function> functions; // the global function storage
inline CodeSuite MainSuite(nullptr); // __main__ in Shell-like form. It's parent_suit is nullptr;
inline Python3Type TempVar; // the global, shared storage for variables.


#endif // CODE_INTERPRETER_H