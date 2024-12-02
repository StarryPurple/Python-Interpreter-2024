#include "Evalvisitor.h"

std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) override {
  for(auto stmt_ctx: ctx->stmt())
    visit(stmt_ctx);
  // Compiling finished. Run the program. HA, HA, HA.
  // Since when did I change it into a compiler?
  // Right. It can't be one.
  Interpreter.RunNewOperations();
  // nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
  visit(ctx->parameters());
  Interpreter.StartFunctionDef(ctx->NAME()->toString());
  visit(ctx->suite());
  Interpreter.EndFunctionDef();
  // nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) override {
  if(auto typed_args_list_ctx = ctx->typedargslist(); typed_args_list_ctx != nullptr)
    visit(typed_args_list_ctx);
  // nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
  // funcdef_mode is on. Right? Right???
  int param_cnt = ctx->tfpdef().size();
  int inited_param_cnt = ctx->test().size();
  int uninited_param_cnt = param_cnt - inited_param_cnt;
  for(int i = 0; i < uninited_param_cnt; i++) {
    auto param_name = std::any_cast<std::string>(visit(ctx->tfpdef(i)));
    Interpreter.DefineVariable(param_name, CodeInterpreter::ConstNone);
  }
  for(int i = 0; i < inited_param_cnt; i++) {
    auto param_name = std::any_cast<std::string>(visit(ctx->tfpdef(uninited_param_cnt + i)));
    auto init_value = std::any_cast<Python3Type>(visit(ctx->test(i)));
    Interpreter.DefineVariable(param_name, init_value);
  }
  // nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
  // return a std::string.
  return ctx->NAME()->toString();
}
std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) override {
  if(auto simple_stmt_ctx = ctx->simple_stmt(); simple_stmt_ctx != nullptr)
    visit(simple_stmt_ctx);
  else if(auto compound_stmt_ctx = ctx->compound_stmt(); compound_stmt_ctx != nullptr) // redundant
    visit(compound_stmt_ctx);
  // nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
  visit(ctx->small_stmt());
  // nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
  if(auto expr_stmt_ctx = ctx->expr_stmt(); expr_stmt_ctx != nullptr)
    visit(expr_stmt_ctx);
  else if(auto flow_stmt_ctx = ctx->flow_stmt(); flow_stmt_ctx != nullptr)
    visit(flow_stmt_ctx);
  // nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {

}
std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) override {

}
std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {

}
std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
  // I know you are here.
  // do nothing.
  return nullptr;
}
std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
  // I know you are here.
  // do nothing.
  return nullptr;
}
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
  // push the returning testlist into the stack.
  visit(ctx->testlist());
  return nullptr;
}
std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {

}
std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {

}
std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {

}
std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) override {

}
std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) override {
  // returns the names of all encountered variables
  visit(ctx->or_test());
  if(!Interpreter.funcdef_mode)
    Interpreter.RunNewOperations();
  return nullptr;
}
std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) override {
  visit(ctx->and_test(0));
  for(int i = 1; i < ctx->and_test().size(); ++i) {
    visit(ctx->and_test(i));
    Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprOr));
    Interpreter.AppendOperation(Interpreter.StackPopOperation());
  }
  if(!Interpreter.funcdef_mode)
    Interpreter.RunNewOperations();
  return nullptr;
}
std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) override {
  visit(ctx->not_test(0));
  for(int i = 1; i < ctx->not_test().size(); ++i) {
    visit(ctx->not_test(i));
    Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprAnd));
    Interpreter.AppendOperation(Interpreter.StackPopOperation());
  }
  if(!Interpreter.funcdef_mode)
    Interpreter.RunNewOperations();
  return nullptr;
}
std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) override {
  if(auto not_test_ctx = ctx->not_test(); not_test_ctx != nullptr) {
    visit(not_test_ctx);
  } else if(auto comparison_ctx = ctx->comparison(); comparison_ctx != nullptr)
    visit(comparison_ctx);
  Interpreter.AppendOperation(Interpreter.SelfOperation(Operation::FunctionOperations::OprNot));
  if(!Interpreter.funcdef_mode)
    Interpreter.RunNewOperations();
  return nullptr;
}
std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) override {
  visit(ctx->arith_expr(0));
  for(int i = 1; i < ctx->arith_expr().size(); i++) {
    auto comp_sign = std::any_cast<std::string>(visit(ctx->comp_op(i - 1)));
    visit(ctx->arith_expr(i));
    if(comp_sign == "<")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprLesser));
    else if(comp_sign == ">")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprGreater));
    else if(comp_sign == "<=")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprNGreater));
    else if(comp_sign == ">=")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprNLesser));
    else if(comp_sign == "==")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprEqual));
    else if(comp_sign == "!=")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprNEqual));
    Interpreter.AppendOperation(Interpreter.StackPopOperation());
  }
  if(!Interpreter.funcdef_mode)
    Interpreter.RunNewOperations();
  return nullptr;
}
std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) override {
  if(ctx->LESS_THAN() != nullptr)
    return ctx->LESS_THAN()->toString();
  else if(ctx->GREATER_THAN() != nullptr)
    return ctx->GREATER_THAN()->toString();
  else if(ctx->EQUALS() != nullptr)
    return ctx->EQUALS()->toString();
  else if(ctx->GT_EQ() != nullptr)
    return ctx->GT_EQ()->toString();
  else if(ctx->LT_EQ() != nullptr)
    return ctx->LT_EQ()->toString();
  else if(ctx->NOT_EQ_2() != nullptr)
    return ctx->NOT_EQ_2()->toString();
}
std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
  visit(ctx->term(0));
  for(int i = 1; i < ctx->term().size(); i++) {
    auto ASop = std::any_cast<std::string>(visit(ctx->addorsub_op(i - 1)));
    visit(ctx->term(i));
    if(ASop == "+")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprAdd));
    else if(ASop == "-")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprSub));
    Interpreter.AppendOperation(Interpreter.StackPopOperation());
  }
  if(!Interpreter.funcdef_mode)
    Interpreter.RunNewOperations();
  return nullptr;
}
std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) override {
  if(ctx->ADD() != nullptr)
    return ctx->ADD()->toString();
  else if(ctx->MINUS() != nullptr)
    return ctx->MINUS()->toString();
}
std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) override {
  visit(ctx->factor(0));
  for(int i = 1; i < ctx->factor().size(); i++) {
    auto MDMop = std::any_cast<std::string>(visit(ctx->muldivmod_op(i - 1)));
    visit(ctx->factor(i));
    if(MDMop == "*")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprMul));
    else if(MDMop == "/")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprDiv));
    else if(MDMop == "//")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprIDiv));
    else if(MDMop == "%")
      Interpreter.AppendOperation(Interpreter.ArithOperation(Operation::FunctionOperations::OprMod));
    Interpreter.AppendOperation(Interpreter.StackPopOperation());
  }
  if(!Interpreter.funcdef_mode)
    Interpreter.RunNewOperations();
  return nullptr;
}
std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) override {
  if(ctx->STAR() != nullptr)
    return ctx->STAR()->toString();
  else if(ctx->DIV() != nullptr)
    return ctx->DIV()->toString();
  else if(ctx->IDIV() != nullptr)
    return ctx->IDIV()->toString();
  else if(ctx->MOD() != nullptr)
    return ctx->MOD()->toString();
}
std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) override {
  if(auto factor_ctx = ctx->factor(); factor_ctx != nullptr) {
    bool is_add;
    if(ctx->ADD() != nullptr)
      is_add = true;
    else if(ctx->MINUS() != nullptr)
      is_add = false;
    visit(factor_ctx);
    if(!is_add)
      Interpreter.AppendOperation(Interpreter.SelfOperation(Operation::FunctionOperations::OprNeg));
  } else if(auto atom_expr_ctx = ctx->atom_expr(); atom_expr_ctx != nullptr) {
    visit(atom_expr_ctx);
  }
  if(!Interpreter.funcdef_mode)
    Interpreter.RunNewOperations();
  return nullptr;
}
std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {

}
std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) override {

}
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) override {
  // returns the collected data.
  auto name_ctx = ctx->NAME();
  auto number_ctx = ctx->NUMBER();
  auto none_ctx = ctx->NONE();
  auto true_ctx = ctx->TRUE();
  auto false_ctx = ctx->FALSE();
  auto format_string_ctx = ctx->format_string();
  auto test_ctx = ctx->test();
  auto string_ctx_vector = ctx->STRING();
  if(name_ctx != nullptr)
    return std::make_pair("name", name_ctx->toString());
  else if(number_ctx != nullptr)
    return std::make_pair("number", number_ctx->toString());
  else if(string_ctx_vector.size() > 0) {
    std::string res = "";
    for(auto str: string_ctx_vector) {
      res += str->toString();
    }
    return std::make_pair("string", res);
  } else if(none_ctx != nullptr)
    return std::make_pair("none", CodeInterpreter::ConstNone);
  else if(true_ctx != nullptr)
    return std::make_pair("true", CodeInterpreter::ConstTrue);
  else if(false_ctx != nullptr)
    return std::make_pair("false", CodeInterpreter::ConstFalse);
  else if(test_ctx != nullptr) {
    auto res = visit(test_ctx);
    return std::make_pair("parenthesed", res);
  } else if(format_string_ctx != nullptr) {
    auto res = visit(format_string_ctx);
    return std::make_pair("format_string", res);
  }
}
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) override {
  // distinctly
  auto f_string_vector = ctx->FORMAT_STRING_LITERAL();
  auto testlist_vector = ctx->testlist();
  for()
}
std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) override {
  // push it into the stack as a vector

}
std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) override {

}
std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) override {

}