#include "Evalvisitor.h"

std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) override {
  for(auto stmt_ctx: ctx->stmt())
    visit(stmt_ctx);
  // Compiling finished. Run the program. HA, HA, HA.
  // Since when did I change it into a compiler?
  // Right. It can't be one.
  code_interpreter_.RunMainFunction();
  // nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
  code_interpreter_.StartFunctionDef(ctx->NAME()->toString());
  visit(ctx->parameters());
  visit(ctx->suite());
  code_interpreter_.EndFunctionDef();
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
  int param_cnt = ctx->tfpdef().size();
  int inited_param_cnt = ctx->test().size();
  int uninited_param_cnt = param_cnt - inited_param_cnt;
  for(int i = 0; i < uninited_param_cnt; i++) {
    auto param_name = std::any_cast<std::string>(visit(ctx->tfpdef(i)));
    code_interpreter_.DefineVariable(param_name, CodeInterpreter::ConstNone);
  }
  for(int i = 0; i < inited_param_cnt; i++)
}
std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
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

}
std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {

}
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {

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

}
std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) override {

}
std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) override {

}
std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) override {

}
std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) override {

}
std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) override {

}
std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {

}
std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) override {

}
std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) override {

}
std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) override {

}
std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) override {

}
std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {

}
std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) override {

}
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) override {

}
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) override {

}
std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) override {

}
std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) override {

}
std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) override {

}