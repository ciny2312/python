#include "Python3ParserBaseVisitor.h"
#include "Evalvisitor.h"
#include "int2048.h"
  std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
  //  std::cout<<ctx->getText()<<std::endl;
    std::string x = ctx->NAME()->getText();
    fuc_num++;
    fucTable[x]=fuc_num;
    fuc[fuc_num].fuc_register(ctx->suite());
    fuc[fuc_num].var.clear();
    fuc[fuc_num].var_val.clear();
    visit(ctx->parameters());
    return 1;
  }

  std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx)  {
    if(ctx->typedargslist())
      return visit(ctx->typedargslist());
    nothing x;
    return x;
  }

  std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) {
    auto array = ctx ->tfpdef();
    auto val = ctx -> test();
    for(int i=0;i< array.size();i++){
      fuc[fuc_num].fuc_register_var(std::any_cast<std::string>(visit(array[i])));
    }
    for(int i=0;i<val.size();i++){
      fuc[fuc_num].fuc_register_var_val(visit(val[i]));
    }
    return 1;
  }

  std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) {
    return ctx->NAME()->getText();
  }

  std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
    do_return x;
    if(ctx->testlist()){
      x.ans=std::any_cast<std::vector<std::any> >(visit(ctx->testlist()));
    }
    return x;
  }