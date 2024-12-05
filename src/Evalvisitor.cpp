// when I say return type is Value, I mean the return type should be among
// Integer, Decimal, Boolean, and String.

#include "Evalvisitor.h"

using namespace Interpreter;

std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
  // [discarded]return ConstNone.
  // return std::string.
  for(const auto stmt: ctx->stmt())
    visit(stmt);
  return "Interpretation ended successfully. Thank you for using Python Interpreter. Bye!\n";
}
std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
  // return empty tuple.
  std::string function_name = ctx->NAME()->getText();
  /*
  if(function_name == "print" || function_name == "int" || function_name == "float"
    || function_name == "str" || function_name == "bool" || function_name == "None"
    || function_name ==  "True" || function_name == "False" || function_name == "def"
    || function_name == "return" || function_name == "break" || function_name == "continue"
    || function_name == "if" || function_name == "elif" || function_name == "else"
    || function_name == "while" || function_name == "or" || function_name == "and"
    || function_name == "not")
    throw std::runtime_error("Use of reserved keyword in function definition");
  */
  auto param_list = std::any_cast<
    std::vector<std::pair<std::string, std::any>>
    >(visit(ctx->parameters()));
  FunctionSuite function(param_list);
  function.func_name = function_name;
  function_map.insert({function_name, function_map.size()});
  function_list.push_back({function, ctx->suite()});
  return Tuple();
}
std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
  // return type: std::vector<std::pair<std::string, std::any>>
  auto args_list = ctx->typedargslist();
  if(args_list)
    return visit(args_list);
  else return std::vector<std::pair<std::string, std::any>>();
}
std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) {
  // return type: std::vector<std::pair<std::string, std::any>>
  std::size_t param_cnt = ctx->tfpdef().size();
  std::size_t inited_cnt = ctx->test().size();
  std::size_t uninited_cnt = param_cnt - inited_cnt;
  std::vector<std::pair<std::string, std::any>> res;
  for(std::size_t i = 0; i < uninited_cnt; i++) {
    auto var_name = std::any_cast<std::string>(visit(ctx->tfpdef(i)));
    auto init_val = ConstNone;
    res.push_back({var_name, init_val});
  }
  for(std::size_t i = 0; i < inited_cnt; i++) {
    auto var_name = std::any_cast<std::string>(visit(ctx->tfpdef(uninited_cnt + i)));
    auto init_val = visit(ctx->test(i));
    res.push_back({var_name, init_val});
  }
  return res;
}
std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) {
  // return type: std::string
  std::string param_name = ctx->NAME()->getText();
  /*
  if(param_name == "print" || param_name == "int" || param_name == "float"
    || param_name == "str" || param_name == "bool" || param_name == "None"
    || param_name ==  "True" || param_name == "False" || param_name == "def"
    || param_name == "return" || param_name == "break" || param_name == "continue"
    || param_name == "if" || param_name == "elif" || param_name == "else"
    || param_name == "while" || param_name == "or" || param_name == "and"
    || param_name == "not")
    throw std::runtime_error("Use of reserved keyword in function definition");
  */
  return param_name;
}
std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
  // return type: Interpreter::Tuple
  if(project.IsBreak() || project.IsContinue() || project.IsReturn())
    return Tuple();
  auto simple_ctx = ctx->simple_stmt();
  auto compound_ctx = ctx->compound_stmt();
  if(simple_ctx)
    return std::any_cast<Tuple>(visit(simple_ctx));
  else return std::any_cast<Tuple>(visit(compound_ctx));
}
std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
  if(project.IsBreak() || project.IsContinue() || project.IsReturn())
    return Tuple();
  // return type: Interpreter::Tuple
  return std::any_cast<Tuple>(visit(ctx->small_stmt()));
}
std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
  // return type: Interpreter::Tuple
  auto expr_ctx = ctx->expr_stmt();
  auto flow_ctx = ctx->flow_stmt();
  if(expr_ctx)
    return std::any_cast<Tuple>(visit(expr_ctx));
  else return std::any_cast<Tuple>(visit(flow_ctx));
}
std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
  // return type: empty Tuple
  if(ctx->augassign()) {
    // an augmented assignment
    std::string opr = std::any_cast<std::string>(visit(ctx->augassign()));
    auto value_list = std::any_cast<Tuple>(visit(ctx->testlist(1)));
    tuple_unzip(value_list); // Important!
    /*
    // value list may contain 1-sized tuples (as function results)
    // wo we must unzip it、
    while(value_list.size() == 1 && std::any_cast<Tuple>(&value_list[0]) != nullptr) {
      value_list = std::any_cast<Tuple>(value_list[0]);
    }*/
    /*
    for(std::any &val: value_list)
      if(std::any_cast<Tuple>(&val) != nullptr) {
        Tuple tmp = std::any_cast<Tuple>(val);
        if(tmp.size() != 1)
          throw std::runtime_error("Augmented assignment with zero / multiple parameters");
        type_trait(tmp[0]);
        val = tmp[0];
      }*/
    std::vector<std::string> var_name_list = testlist_splitter(ctx->testlist(0)->getText());
    /*
    if(var_name_list.size() != value_list.size())
      throw std::runtime_error("Assignment with different parameter numbers");
    // augmented assignment is illegal for tuples?
    if(var_name_list.size() != 1)
      throw std::runtime_error("Augmented assignment towards illegal type: \"tuple\"");*/
    // In case some std::any<std::any> happens
    if(opr == "+=") {
      for(std::size_t i = 0; i < var_name_list.size(); i++)
        project.Variable(var_name_list[i]) += value_list[i];
    } else if(opr == "-=") {
      for(std::size_t i = 0; i < var_name_list.size(); i++)
        project.Variable(var_name_list[i]) -= value_list[i];
    } else if(opr == "*=") {
      for(std::size_t i = 0; i < var_name_list.size(); i++)
        project.Variable(var_name_list[i]) *= value_list[i];
    } else if(opr == "/=") {
      for(std::size_t i = 0; i < var_name_list.size(); i++)
        project.Variable(var_name_list[i]) /= value_list[i];
    } else if(opr == "//=") {
      for(std::size_t i = 0; i < var_name_list.size(); i++)
        project.Variable(var_name_list[i]) |= value_list[i];
    } else if(opr == "%=") {
      for(std::size_t i = 0; i < var_name_list.size(); i++)
        project.Variable(var_name_list[i]) %= value_list[i];
    }
    return Tuple();
  }
  // else a multiple assignment
  std::size_t len = ctx->testlist().size();
  auto value_list = std::any_cast<Tuple>(visit(ctx->testlist(len - 1)));
  tuple_unzip(value_list); // Important!
  while(value_list.size() == 1 && std::any_cast<Tuple>(&value_list[0]) != nullptr) {
    value_list = std::any_cast<Tuple>(value_list[0]);
  }
  for(std::size_t i = len - 1; i >= 1; i--) {
    // (unsigned long)i < 0 can't happen...
    std::vector<std::string> var_name_list = testlist_splitter(ctx->testlist(i - 1)->getText());
    /*if(var_name_list.size() != value_list.size())
      throw std::runtime_error("Assignment with different parameter numbers");*/
    for(std::size_t j = 0; j < var_name_list.size(); j++)
      project.Variable(var_name_list[j]) = value_list[j];
  }
  return Tuple();
}
std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {
  // return type: std::string (what the operator is)
  if(ctx->ADD_ASSIGN()) return std::make_any<std::string>("+=");
  else if(ctx->SUB_ASSIGN()) return std::make_any<std::string>("-=");
  else if(ctx->MULT_ASSIGN()) return std::make_any<std::string>("*=");
  else if(ctx->DIV_ASSIGN()) return std::make_any<std::string>("/=");
  else if(ctx->IDIV_ASSIGN()) return std::make_any<std::string>("//=");
  else if(ctx->MOD_ASSIGN()) return std::make_any<std::string>("%=");
}
std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
  // return type: Interpreter::Tuple
  if(auto break_ctx = ctx->break_stmt(); break_ctx)
    return std::any_cast<Tuple>(visit(break_ctx));
  else if(auto continue_ctx = ctx->continue_stmt(); continue_ctx)
    return std::any_cast<Tuple>(visit(continue_ctx));
  else if(auto return_ctx = ctx->return_stmt(); return_ctx)
    return std::any_cast<Tuple>(visit(return_ctx));
}
std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
  // return empty Tuple
  project.CallBreak();
  return Tuple();
}
std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
  // return empty Tuple
  project.CallContinue();
  return Tuple();
}
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
  // return type: Interpreter::Tuple (the value of result)
  auto testlist_ctx = ctx->testlist();
  if(testlist_ctx) {
    auto res = std::any_cast<Tuple>(visit(testlist_ctx));
    project.CallReturn();
    project.AddResult(res);
    return res;
  }
  else {
    project.CallReturn();
    project.AddResult(Tuple());
    return Tuple(); // empty for nothing to return
  }
}
std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
  // return type: Interpreter::Tuple
  if(auto if_ctx = ctx->if_stmt(); if_ctx)
    return std::any_cast<Tuple>(visit(if_ctx));
  else if(auto while_ctx = ctx->while_stmt(); while_ctx)
    return std::any_cast<Tuple>(visit(while_ctx));
  else if(auto funcdef_ctx = ctx->funcdef(); funcdef_ctx)
    return std::any_cast<Tuple>(visit(funcdef_ctx));
}
std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
  // return type: Interpreter::Tuple
  std::size_t branch_cnt = ctx->test().size();
  for(int i = 0; i < branch_cnt; i++)
    if(to_Boolean(visit(ctx->test(i)))) {
      return std::any_cast<Tuple>(visit(ctx->suite(i)));
    }
  if(ctx->ELSE())
    return std::any_cast<Tuple>(visit(ctx->suite(branch_cnt)));
  return Tuple();
}
std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
  std::any res = Tuple();
  while(to_Boolean(visit(ctx->test()))) {
    res = std::any_cast<Tuple>(visit(ctx->suite()));
    if(project.IsReturn()) {
      return res;
    }
    if(project.IsBreak()) {
      project.EndBreak();
      break;
    } else if(project.IsContinue()) {
      project.EndContinue();
      continue;
    }
  }
  return res;
}
std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
  // return type: Interpreter::Tuple (if there's a return line inside the suite.)
  // if no return line / return nothing, returns an empty Tuple.
  std::any res = Tuple();
  if(auto simple_ctx = ctx->simple_stmt(); simple_ctx) {
    res = std::any_cast<Tuple>(visit(simple_ctx));
    if(project.IsReturn()) {
      res = project.GiveResult();
      // project.EndReturn(); // redundant
      return res;
    }
  }
  for(auto stmt_ctx: ctx->stmt()) {
    res = std::any_cast<Tuple>(visit(stmt_ctx));
    if(project.IsReturn()) {
      res = project.GiveResult();
      // project.EndReturn(); // redundant
      return res;
    }
    // assert(to_Boolean(res == ConstNone));
  }
  return res;
}
std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
  // return type: Value (the value of the test expression)
  if(project.IsBreak() || project.IsContinue() || project.IsReturn())
    return Tuple();
  auto res = visit(ctx->or_test());
  return res;
}
std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
  // return type: Value (the value of the test expression), may be a Tuple
  std::any res = visit(ctx->and_test(0));
  // unzip(res);
  for(std::size_t i = 1; i < ctx->and_test().size(); i++) {
    if(to_Boolean(res))
      return res;
    res = visit(ctx->and_test(i));
  }
  return res;
}
std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
  // return type: Value (the value of the test expression), may be a Tuple
  std::any res = visit(ctx->not_test(0));
  // unzip(res);
  for(std::size_t i = 1; i < ctx->not_test().size(); i++) {
    if(!to_Boolean(res))
      return res;
    res = visit(ctx->not_test(i));
  }

  return res;
}
std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
  // return type: Value (the value of the test expression), may be a Tuple
  if(const auto not_test_ctx = ctx->not_test(); not_test_ctx) {
    auto tmp = visit(not_test_ctx);
    /*
    if(std::any_cast<Tuple>(&tmp) != nullptr) {
      Tuple ttmp = std::any_cast<Tuple>(tmp);
      if(ttmp.size() != 1)
        throw std::runtime_error("Invalid type \"Tuple\" in operator \"not\"");
      tmp = ttmp[0];
      type_trait(tmp);
    }
    */
    auto res = !to_Boolean(tmp);
    return res;
  }
  else if(const auto comp_ctx = ctx->comparison(); comp_ctx) {
    auto res = visit(comp_ctx);
    return res;
  }
}
std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
  // return type: Value (the value of the expression), may be a Tuple
  auto res = visit(ctx->arith_expr(0));
  // unzip(res);
  if(ctx->arith_expr().size() == 1)
    return res;
  std::any ans;
  for(std::size_t i = 1; i < ctx->arith_expr().size(); i++) {
    auto opr = std::any_cast<std::string>(visit(ctx->comp_op(i - 1)));
    auto tmp = visit(ctx->arith_expr(i));
    // assert(std::any_cast<std::any>(&res) == nullptr);
    if(opr == ">") ans = (res > tmp);
    else if(opr == "<") ans = (res < tmp);
    else if(opr == "==") ans = (res == tmp);
    else if(opr == "!=") ans = (res != tmp);
    else if(opr == "<=") ans = (res <= tmp);
    else if(opr == ">=") ans = (res >= tmp);
    if(!to_Boolean(ans))
      return ans; // false
    res = tmp;
  }
  return ans;
}
std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {
  // return type: std::string (what the operator is)
  if(ctx->LESS_THAN()) return std::make_any<std::string>("<");
  else if(ctx->GREATER_THAN()) return std::make_any<std::string>(">");
  else if(ctx->EQUALS()) return std::make_any<std::string>("==");
  else if(ctx->GT_EQ()) return std::make_any<std::string>(">=");
  else if(ctx->LT_EQ()) return std::make_any<std::string>("<=");
  else if(ctx->NOT_EQ_2()) return std::make_any<std::string>("!=");
}
std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
  // return type: Value (the value of the expression), may be a Tuple
  auto res = visit(ctx->term(0));
  // unzip(res);
  for(std::size_t i = 1; i < ctx->term().size(); i++) {
    auto opr = std::any_cast<std::string>(visit(ctx->addorsub_op(i - 1)));
    auto tmp = visit(ctx->term(i));
    if(opr == "+") res = res + tmp;
    else if(opr == "-") res = res - tmp;
  }
  return res;
}
std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) {
  // return type: std::string (what the operator is)
  if(ctx->ADD()) return std::make_any<std::string>("+");
  else if(ctx->MINUS()) return std::make_any<std::string>("-");
}
std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
  // return type: Value (the value of the expression), may be a Tuple
  auto res = visit(ctx->factor(0));
  // unzip(res);
  for(std::size_t i = 1; i < ctx->factor().size(); i++) {
    auto opr = std::any_cast<std::string>(visit(ctx->muldivmod_op(i - 1)));
    auto tmp = visit(ctx->factor(i));
    if(opr == "*") res = res * tmp;
    else if(opr == "/") res = res / tmp;
    else if(opr == "//") res = res | tmp;
    else if(opr == "%")
      res = res % tmp;
  }
  return res;
}
std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) {
  // return type: std::string (what the operator is)
  if(ctx->STAR()) return std::make_any<std::string>("*");
  else if(ctx->DIV()) return std::make_any<std::string>("/");
  else if(ctx->IDIV()) return std::make_any<std::string>("//");
  else if(ctx->MOD()) return std::make_any<std::string>("%");
}
std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
  // return type: Value (the value of the expression), may be a Tuple
  if(auto fct = ctx->factor(); fct != nullptr) {
    if(ctx->ADD()) {
      auto res = +visit(fct);
      // convert_single(res);
      return res;
    }
    else if(ctx->MINUS()) {
      auto res = -visit(fct);
      // convert_single(res);
      return res;
    }
  }
  auto res = visit(ctx->atom_expr());
  return res;
}
std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
  // return type: Value (the value of the expression)
  // or: Interpreter::Tuple (the result of the function)
  auto atom_ctx = ctx->atom();
  auto trailer_ctx = ctx->trailer();
  if(trailer_ctx == nullptr){
    auto res = visit(atom_ctx); // A rvalue
    // unzip(res);
    return res;
    }

  // else, a function call
  func_call_sign = true;
  std::string func_name = std::any_cast<std::string>(visit(atom_ctx)); // It must be a function name.
  func_call_sign = false;
  auto initialize_list = std::any_cast<FunctionSuite::Initialize_List>(visit(trailer_ctx));

  // special judge for builtin functions
  if(func_name == "print") {
    Tuple res;
    for(auto [var_name, val]: initialize_list){
      /*if(var_name != FunctionSuite::UnassignedName)
        throw std::runtime_error("No variable name in \"print()\"");*/
      res.push_back(val);
    }
    print(res);
    return ConstNone;
  } else if(func_name == "int") {
    /*if(initialize_list.size() != 1)
      throw std::runtime_error("Wrong parameter number for \"int()\" (It should be 1)");*/
    auto [var_name, val] = initialize_list[0];
    /*if(var_name != FunctionSuite::UnassignedName)
      throw std::runtime_error("No variable name in \"int()\"");*/
    return to_Integer(val);
  } else if(func_name == "float") {
    /*if(initialize_list.size() != 1)
      throw std::runtime_error("Wrong parameter number for \"float()\" (It should be 1)");*/
    auto [var_name, val] = initialize_list[0];
    /*if(var_name != FunctionSuite::UnassignedName)
      throw std::runtime_error("No variable name in \"float()\"");*/
    return to_Decimal(val);
  } else if(func_name == "str") {
    /*if(initialize_list.size() != 1)
      throw std::runtime_error("Wrong parameter number for \"str()\" (It should be 1)");*/
    auto [var_name, val] = initialize_list[0];
    /*if(var_name != FunctionSuite::UnassignedName)
      throw std::runtime_error("No variable name in \"instrt()\"");*/
    return to_String(val);
  } else if(func_name == "bool") {
    /*if(initialize_list.size() != 1)
      throw std::runtime_error("Wrong parameter number for \"bool()\" (It should be 1)");*/
    auto [var_name, val] = initialize_list[0];
    /*if(var_name != FunctionSuite::UnassignedName)
      throw std::runtime_error("No variable name in \"bool()\"");*/
    return to_Boolean(val);
  }

  // other functions.
  // std::cerr << "calling " << func_name << std::endl;
  /*if(function_map.count(func_name) == 0)
    throw std::runtime_error("Call of undefined function \"" + func_name + "\"");*/
  std::size_t func_ord = function_map[func_name];
  auto [function, suite_ctx] = function_list[func_ord];
  for(std::size_t i = 0, anon_pos = -1; i < initialize_list.size(); i++) {
    auto [var_name, init_val] = initialize_list[i];
    if(var_name == FunctionSuite::UnassignedName) {
      anon_pos++;
      /*if(i != anon_pos)
        throw std::runtime_error("Unnamed argument before named one(s) in function call");*/
      function.variable_space[i] = init_val;
    } else {
      /*if(function.variable_map.count(var_name) == 0)
        throw std::runtime_error("Undefined argument name in call of function");*/
      std::size_t var_ord = function.variable_map[var_name];
      /*if(var_ord <= anon_pos)
        throw std::runtime_error("Reassign of an position-assigned argument");*/
      function.variable_space[var_ord] = init_val;
    }
  }
  /*
  for(const auto& init_val: function.variable_space)
    if(std::any_cast<bool>(init_val == ConstNone)) // A leak of namespace Interpreter?
      throw std::runtime_error("Unassigned argument in function call");*/
  project.CallFunction(function);
  visit(suite_ctx); // should be an Interpreter::Tuple in std::any
  auto res = std::any_cast<Tuple>(project.GiveResult());
  project.ExitFunction();
  tuple_unzip(res);
  if(res.size() == 1) return res[0];
  return res;
}
std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
  // return type: FunctionSuite::Initialize_List (it is what you think it is)
  if(auto arglist_ctx = ctx->arglist(); arglist_ctx)
    return std::any_cast<FunctionSuite::Initialize_List>(visit(arglist_ctx)); // redundant cast?
  else return FunctionSuite::Initialize_List();
}
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
  // return type: Value (the value of the expression), may be a Tuple?
  if(auto name_ctx = ctx->NAME(); name_ctx) {
    std::string vf_name = name_ctx->getText();
    if(func_call_sign)
      return vf_name; // call of a function
    else {
      auto res = project.Variable(vf_name);
      // assert(std::any_cast<std::any>(&res) == nullptr);
      return res;
    }
  } else if(auto number_ctx = ctx->NUMBER(); number_ctx) {
    std::string number_str = number_ctx->getText();
    if(number_str.find('.') == std::string::npos) {
      Integer res = to_Integer(number_str);
      return res;
    } else {
      Decimal res = to_Decimal(number_str);
      return res;
    }
  } else if(!ctx->STRING().empty()) {
    std::string res = "";
    for(auto str_ptr: ctx->STRING()) {
      // assert(str_ptr);
      std::string append = str_ptr->getText();
      res += append.substr(1, append.length() - 2); // exclude the starting and ending "" or ''
    }
    return res;
  } else if(ctx->NONE())
    return ConstNone;
  else if(ctx->TRUE())
    return true;
  else if(ctx->FALSE())
    return false;
  else if(auto test_ctx = ctx->test(); test_ctx) {
    auto res = visit(test_ctx);
    // assert(std::any_cast<std::any>(&res) == nullptr);
    return res;
  }
  else if(auto fstring_ctx = ctx->format_string(); fstring_ctx) {
    auto res = visit(fstring_ctx);
    // assert(std::any_cast<std::any>(&res) == nullptr);
    return res;
  }
}
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {
  // return type: std::string (the final string)
  // todo: complete it later on.
  std::string res = "";
  std::size_t fmted_cnt = ctx->testlist().size();
  std::size_t raw_cnt = ctx->FORMAT_STRING_LITERAL().size();
  bool string_type = true; // raw, literal string comes first
  std::string raw_string = ctx->getText();
  if(raw_string[2] == '{' && raw_string[3] != '{')
    string_type = false; // formatted string comes first
  for(std::size_t i = 0; i < std::min(fmted_cnt, raw_cnt); i++) {
    auto test_list = std::any_cast<Tuple>(visit(ctx->testlist(i)));
    std::string str = "";
    for(int j = 0; j < test_list.size(); j++) {
      str = str + to_String(test_list[j]);
      if(j != test_list.size() - 1)
        str += " ";
    }
    std::string literal = ctx->FORMAT_STRING_LITERAL(i)->getText();
    std::string dealt = "";
    for(std::size_t j = 0; j < literal.length(); j++) {
      dealt += literal[j];
      if(literal[j] == literal[j + 1] && (literal[j] == '{' && literal[j] == '}'))
        j++;
    }
    if(string_type)
      res += dealt + str;
    else res += str + dealt;
  }
  if(fmted_cnt > raw_cnt) {
    auto test_list = std::any_cast<Tuple>(visit(ctx->testlist(fmted_cnt - 1)));
      std::string str = "";
      for(std::size_t i = 0; i < test_list.size(); i++) {
        str = str + to_String(test_list[i]);
        if(i != test_list.size() - 1)
          str += " ";
      }
    res += str;
  }
  else if(raw_cnt > fmted_cnt) {
    std::string literal = ctx->FORMAT_STRING_LITERAL(raw_cnt - 1)->getText();
    std::string dealt = "";
    for(std::size_t i = 0; i < literal.length(); i++) {
      dealt += literal[i];
      if(literal[i] == literal[i + 1] && (literal[i] == '{' && literal[i] == '}'))
        i++;
    }
    res += dealt;
  }
  return res;
}
std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
  // return type: Interpreter::Tuple (containing the rvalues of tests), may be a Tuple of Tuples
  Tuple res;
  for(const auto test_ctx: ctx->test()) {
    auto tmp = visit(test_ctx);
    res.push_back(tmp);
  }
  return res;
}
std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
  // return type: FunctionSuite::Initialize_List (it is what you think it is)
  FunctionSuite::Initialize_List res;
  for(const auto argument_ctx: ctx->argument())
    res.push_back(std::any_cast<FunctionSuite::Initialize_Pair>(visit(argument_ctx)));
  return res;
}
std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
  // return type: FunctionSuite::Initialize_Pair (it's what you think it is)
  // If there's an initial value with no name, we'll temporarily name it "#AnonVar"
  // This name isn't a valid variable name (Right?), so no crash here.
  if(ctx->ASSIGN()) {
    // it has a name
    std::string var_name = ctx->test(0)->getText();
    // WHY CAN'T YOU CHANGE THE RULES? A (NAME "=" test) rule would be nice.
    std::any init_val = visit(ctx->test(1));
    return FunctionSuite::Initialize_Pair(var_name, init_val);
  }
  // else it doesn't have a name
  std::string var_name = FunctionSuite::UnassignedName;
  std::any init_val = visit(ctx->test(0));
  return FunctionSuite::Initialize_Pair(var_name, init_val);
}