#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3ParserBaseVisitor.h"
#include "int2048.h"
#include "Evalvisitor.h"

class EvalVisitor : public Python3ParserBaseVisitor {
public:
  std::any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
    std::string x = ctx->NAME()->getText();
    fuc_num++;
    fuc_name[x]=fuc_num;
    fuc[fuc_num].fuc_register(ctx->suite());
    visit(ctx->parameters());
    return 1;
  }

  std::any visitParameters(Python3Parser::ParametersContext *ctx) override {
    if(ctx->typedargslist())
      return visit(ctx->typedargslist());
    nothing x;
    return x;
  }

  std::any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
    auto array = ctx ->tfpdef();
    auto val = ctx -> test();
    for(int i=0;i< array.size();i++){
      fuc[fuc_num].register_var(std::any_cast<std::string>(visit(array[i])));
    }
    for(int i=0;i<val.size();i++){
      fuc[fuc_num].register_var_val(visit(val[i]));
    }
  }

  std::any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
    return ctx->NAME()->getText();
  }

  std::any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
    do_return x;
    if(ctx->testlist()){
      x.ans=std::any_cast<std::vector<std::any> >(visit(ctx->testlist()));
    }
    return x;
  }
};

#endif // PYTHON_INTERPRETER_EVALVISITOR_H
