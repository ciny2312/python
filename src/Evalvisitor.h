#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3ParserBaseVisitor.h"
#include "int2048.h"

class EvalVisitor : public Python3ParserBaseVisitor {
  class Scope {
    std::map<std::string, std::any> varTable;
  public:
    void var_register(std::string x, std::any y) { varTable[x] = y; }
    std::any var_query(std::string x) { return varTable[x]; }
  };
  class nothing{};
  Scope scope;
  sjtu::int2048 just_try;
public:
  /*	std::any visit(antlr4::tree::ParseTree *tree) override {
                  std::cout << tree->getText() << std::endl;
          return tree->accept(this);
      }
  */

  // TODO: override all methods of Python3ParserBaseVisitor
  std::any visitFile_input(Python3Parser::File_inputContext *ctx) override {
    auto array = ctx->stmt();
    for (auto x : array) {
      visit(x);
    }
    return 1;
  }
  std::any visitStmt(Python3Parser::StmtContext *ctx) override {
    if (ctx->simple_stmt()) {
      return visit(ctx->simple_stmt());
    }
    return visit(ctx->compound_stmt());
  }

  std::any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
    return visit(ctx->small_stmt());
  }

  std::any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
    auto x1 = ctx->expr_stmt();
    if (x1 != nullptr) {
      visit(x1);
    }
    auto x2 = ctx->flow_stmt();
    if (x2 != nullptr) {
      visit(x2);
    }
    return 1;
  }


  std::any add(std::any x, std::any y) {
    if(std::any_cast<double>(&x)||std::any_cast<double>(&y)){
      double ans=0;
      if(std::any_cast<double >(&x)) ans+=std::any_cast<double>(x);
      else if(std::any_cast<int>(&x)) ans+=std::any_cast<int>(x);
      else ans+=std::any_cast<bool>(x);
      if(std::any_cast<double >(&y)) ans+=std::any_cast<double>(y);
      else if(std::any_cast<int >(&y)) ans+=std::any_cast<int>(y);
      else ans+=std::any_cast<bool>(y);
      return ans;
    }
    else{
      int ans=0;
      if(std::any_cast<int>(&x)) ans+=std::any_cast<int>(x);
      else ans+=std::any_cast<bool>(x);
      if(std::any_cast<int >(&y)) ans+=std::any_cast<int>(y);
      else ans+=std::any_cast<bool>(y);
      return ans;
    }
    return std::any_cast<std::string>(x) + std::any_cast<std::string>(y);
  }
  std::any del(std::any x, std::any y) {
    if(std::any_cast<double>(&x)||std::any_cast<double>(&y)){
      double ans=0;
      if(std::any_cast<double >(&x)) ans+=std::any_cast<double>(x);
      else if(std::any_cast<int>(&x)) ans+=std::any_cast<int>(x);
      else ans+=std::any_cast<bool>(x);
      if(std::any_cast<double>(&y)) ans-=std::any_cast<double>(y);
      else if(std::any_cast<int>(&y)) ans-=std::any_cast<int>(y);
      else ans-=std::any_cast<bool>(y);
      return ans;
    }
    else{
      int ans=0;
      if(std::any_cast<int>(&x)) ans+=std::any_cast<int>(x);
      else ans+=std::any_cast<bool>(x);
      if(std::any_cast<int >(&y)) ans-=std::any_cast<int>(y);
      else ans-=std::any_cast<bool>(y);
      return ans;
    }
  }
  std::any mul(std::any x, std::any y) {
    if(std::any_cast<double>(&x)||std::any_cast<double>(&y)){
      double ans=1;
      if(std::any_cast<double>(&x)) ans*=std::any_cast<double>(x);
      else if(std::any_cast<int>(&x)) ans*=std::any_cast<int>(x);
      else ans*=std::any_cast<bool>(x);
      if(std::any_cast<double>(&y)) ans*=std::any_cast<double>(y);
      else if(std::any_cast<int>(&y)) ans*=std::any_cast<int>(y);
      else ans*=std::any_cast<bool>(y);
      return ans;
    }
    else if(std::any_cast<std::string>(&x)||std::any_cast<std::string>(&y)){
      std::string ans;
      if(std::any_cast<std::string>(&x)){
        ans=std::any_cast<std::string>(x);
        int num=std::any_cast<int>(y);
        while(num){
          if(num%2==1) ans=ans+ans;
          num/=2;
        }
      }
      if(std::any_cast<std::string>(&y)){
        ans=std::any_cast<std::string>(y);
        int num=std::any_cast<int>(x);
        while(num){
          if(num%2==1) ans=ans+ans;
          num/=2;
        }
      }
      return ans;
    }
    else{
      int ans=1;
      if(std::any_cast<int>(&x)) ans*=std::any_cast<int>(x);
      else ans*=std::any_cast<bool>(x);
      if(std::any_cast<int>(&y)) ans*=std::any_cast<int>(y);
      else ans*=std::any_cast<bool>(y);
      return ans;
    }
  }
  std::any div1(std::any x, std::any y) {
    double ans=0;
    if(std::any_cast<double>(&x)) ans=std::any_cast<double>(x);
    else if(std::any_cast<int>(&x)) ans=std::any_cast<int>(x);
    else ans=std::any_cast<bool>(x);
    if(std::any_cast<double>(&y)) ans/=std::any_cast<double>(y);
    else if(std::any_cast<int>(&y)) ans/=std::any_cast<int>(y);
    else ans/=std::any_cast<bool>(y);
    return ans;
  }
  std::any div2(std::any x, std::any y) {
    int ans=0;
    if(std::any_cast<int>(&x)) ans=std::any_cast<int>(x);
    else ans=std::any_cast<bool>(x);
    if(std::any_cast<int>(&y)) ans/=std::any_cast<int>(y);
    else ans/=std::any_cast<bool>(y);
    return ans;
  }


  std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
    auto array = ctx->testlist();
    auto au = ctx->augassign();
    if (au == nullptr) {
      auto val =
          std::any_cast<std::vector<std::any>>(visit(array[array.size() - 1]));
      for (int i = array.size() - 2; i >= 0; i--) {
        auto x = std::any_cast<std::vector<std::any>>(visit(array[i]));
        for (int j = 0; j < val.size(); j++) {
          scope.var_register(std::any_cast<std::string>(x[j]), val[j]);
        }
      }
    } else if (au->getText() == "+=") {
      auto val = visit(array[1]);
      auto x = array[0]->getText();
      std::any v = scope.var_query(x);
      scope.var_register(x, add(v, val));
    } else if (au->getText() == "-=") {
      auto val = visit(array[1]);
      auto x = array[0]->getText();
      std::any v = scope.var_query(x);
      scope.var_register(x, del(v, val));
    } else if (au->getText() == "*=") {
      auto val = visit(array[1]);
      auto x = array[0]->getText();
      std::any v = scope.var_query(x);
      scope.var_register(x, mul(v, val));

    } else if (au->getText() == "/=") {
      auto val = visit(array[1]);
      auto x = array[0]->getText();
      std::any v = scope.var_query(x);
      scope.var_register(x, div1(v, val));

    } else if (au->getText() == "//=") {
      auto val = visit(array[1]);
      auto x = array[0]->getText();
      std::any v = scope.var_query(x);
      scope.var_register(x, div2(v, val));

    } else if (au->getText() == "%=") {
      auto val = visit(array[1]);
      auto x = array[0]->getText();
      std::any v = scope.var_query(x);
      scope.var_register(x, del(v, mul(div2(v,val),val)));
    }
    return 1;
  }

  std::any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
    if(ctx->break_stmt()) return visit(ctx->break_stmt());
    if(ctx->return_stmt()) return visit(ctx->return_stmt());
    return visit(ctx->continue_stmt());
  }

  std::any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  std::any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  std::any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
    return visit(ctx->testlist());
  }

  std::any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
    if(ctx->if_stmt()) return visit(ctx->if_stmt());
    if(ctx->while_stmt()) return visit(ctx->while_stmt());
    return visit(ctx->funcdef());
  }

  std::any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
    auto array = ctx->test();
    for(int i=0;i < array.size(); i++){
      if(std::any_cast<bool>(visit(array[i]))){
        return visit(ctx->suite(i));
      }
    }
    return visit(ctx->suite(array.size()));
  }

  std::any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
    while(std::any_cast<bool>(visit(ctx->test()))){
      visit(ctx->suite());
    }
    return 1;
  }

  std::any visitSuite(Python3Parser::SuiteContext *ctx) override {
    if(ctx->simple_stmt()){
      return visit(ctx->simple_stmt());
    }
    auto array = ctx->stmt();
    for(int i=0;i<array.size();i++){
      visit(array[i]);
    }
    return 1;
  }

  std::any visitTestlist(Python3Parser::TestlistContext *ctx) override {
    auto array = ctx->test();
    std::vector<std::any> ans;
    for (int i = 0; i < array.size(); i++)
      ans.push_back(visit(array[i]));
  //  std::cout<<"****"<<ctx->getText()<<std::endl;
    return ans;
  }

  std::any visitTest(Python3Parser::TestContext *ctx) override {
  //  std::cout<<"*"<<ctx->getText()<<std::endl;
    return visit(ctx->or_test());
  //  std::cout<<"**"<<ctx->getText()<<std::endl;
  }

  std::any visitOr_test(Python3Parser::Or_testContext *ctx) override {
    bool flag = false;
    auto array = ctx->and_test();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    for (auto x : array) {
      flag |= std::any_cast<bool>(visit(x));
    }
    return flag;
  }

  std::any visitAnd_test(Python3Parser::And_testContext *ctx) override {
    bool flag = true;
    auto array = ctx->not_test();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    for (auto x : array) {
      flag &= std::any_cast<bool>(visit(x));
    }
    return flag;
  }

  std::any visitNot_test(Python3Parser::Not_testContext *ctx) override {
    if (ctx->not_test()) {
      return !std::any_cast<bool>(visit(ctx->not_test()));
    }
    return visit(ctx->comparison());
  }

  std::any visitComparison(Python3Parser::ComparisonContext *ctx) override {
    auto array = ctx->arith_expr();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    auto op = ctx->comp_op();
    bool flag = true;
    for (int i = 0; i < array.size(); i++) {
      if (op[i]->getText() == "==") {


      }
    }
    return flag;
  }

  std::any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
    auto array = ctx->term();
    auto op = ctx->addorsub_op();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    std::any ans = visit(array[0]);
    for(int i=1;i<array.size();i++){
      if(op[i-1]->getText()=="+"){
        ans=add(ans,array[i]);
      }
      else{
        ans=del(ans,array[i]);
      }
    }
    return ans;
  /*  if (std::any_cast<std::string *>(ans)) {
      std::string val = std::any_cast<std::string>(ans);
      for (int i = 1; i < array.size(); i++) {
        val = val + std::any_cast<std::string>(visit(array[i]));
      }
      return val;
    }
    std::vector<std::any> v;
    bool flag = 0;
    for (int i = 0; i < array.size(); i++) {
      v[i] = visit(array[i]);
      if (std::any_cast<double *>(v[i]))
        flag = 1;
    }
    if (flag) {
      double val;
      if (std::any_cast<int *>(v[0]))
        val = std::any_cast<int>(v[0]);
      if (std::any_cast<bool *>(v[0]))
        val = std::any_cast<bool>(v[0]);
      if (std::any_cast<double *>(v[0]))
        val = std::any_cast<double>(v[0]);
      for (int i = 1; i < array.size(); i++) {
        if (op[i-1]->getText() == "+") {
          if (std::any_cast<int *>(v[i]))
            val += std::any_cast<int>(v[i]);
          if (std::any_cast<bool *>(v[i]))
            val += std::any_cast<bool>(v[i]);
          if (std::any_cast<double *>(v[i]))
            val += std::any_cast<double>(v[i]);
        } else {
          if (std::any_cast<int *>(v[i]))
            val -= std::any_cast<int>(v[i]);
          if (std::any_cast<bool *>(v[i]))
            val -= std::any_cast<bool>(v[i]);
          if (std::any_cast<double *>(v[i]))
            val -= std::any_cast<double>(v[i]);
        }
      }
      return val;
    }
    int val = 0;
    if (std::any_cast<int *>(v[0]))
      val = std::any_cast<int>(v[0]);
    if (std::any_cast<bool *>(v[0]))
      val = std::any_cast<bool>(v[0]);
    for (int i = 1; i < array.size(); i++) {
      if (op[i-1]->getText() == "+") {
        if (std::any_cast<int *>(v[i]))
          val += std::any_cast<int>(v[i]);
        if (std::any_cast<bool *>(v[i]))
          val += std::any_cast<bool>(v[i]);
      } else {
        if (std::any_cast<int *>(v[i]))
          val -= std::any_cast<int>(v[i]);
        if (std::any_cast<bool *>(v[i]))
          val -= std::any_cast<bool>(v[i]);
      }
    }
    return val;*/
  }
  
  std::any visitTerm(Python3Parser::TermContext *ctx) override {
    auto array = ctx->factor();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    auto op = ctx->muldivmod_op();
    std::any ans = visit(array[0]);
    for (int i = 1; i < array.size(); i++) {
      if(op[i-1]->getText()=="*")
        ans = mul(ans, visit(array[i]));
      else if(op[i-1]->getText()=="/")
        ans = div1(ans,visit(array[i]));
      else if(op[i-1]->getText()=="//")
        ans = div2(ans,visit(array[i]));
      else{
        std::any val=visit(array[i]);
        ans = del(ans,mul(div2(ans,val),val));
      }
    }
    return ans;
  }

  std::any visitFactor(Python3Parser::FactorContext *ctx) override {
    if (ctx->factor()) {
      if (ctx->ADD())
        return visit(ctx->factor());
      else {
        std::any x = visit(ctx->factor());
        if(std::any_cast<int>(&x))
          return -std::any_cast<int>(x);
        if(std::any_cast<double>(&x))
          return -std::any_cast<double>(x);
      }
    }
    return visit(ctx->atom_expr());
  }

  std::any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
    if (ctx->trailer()) {
      std::string fuc = ctx->atom()->getText();
      auto array = std::any_cast<std::vector<std::any>>(visit(ctx->trailer()));
      if (fuc == "print") {
        //    std::cout<<"BOOL"<<array.size()<<std::endl;
        for (int i = 0; i < array.size(); i++) {
          if (std::any_cast<int>(&array[i])){
            std::cout << std::any_cast<int>(array[i]) << ' ';
          }
          if (std::any_cast<double>(&array[i]))
            printf("%.6lf ", std::any_cast<double>(array[i]));
          
          if (std::any_cast<std::string>(&array[i])) {
            std::cout << std::any_cast<std::string>(array[i]) << ' ';
          }

          if (std::any_cast<bool>(&array[i])) {
            if (std::any_cast<bool>(array[i]) == true)
              std::cout << "True" << ' ';
            else
              std::cout << "False" << ' ';
          }
          if(std::any_cast<nothing>(&array[i])){
            std::cout<< "None" <<' ';
          }
        }
        printf("\n");
        return 1;
      }
      if (fuc == "int") {
        if(array.size()>1) std::cerr<<"(int)more than one"<<std::endl;
        int ans;
        if(std::any_cast<int>(&array[0])) ans=std::any_cast<int>(array[0]);
        if(std::any_cast<bool>(&array[0])) ans=std::any_cast<bool>(array[0]);
        if(std::any_cast<double>(&array[0])) ans=std::any_cast<double>(array[0]);
        return ans;
      }
      if (fuc == "float") {
        if(array.size()>1) std::cerr<<"(double)more than one"<<std::endl;
        double ans;
        if(std::any_cast<int>(&array[0])) ans=std::any_cast<int>(array[0]);
        if(std::any_cast<bool>(&array[0])) ans=std::any_cast<bool>(array[0]);
        if(std::any_cast<double>(&array[0])) ans=std::any_cast<double>(array[0]);
        return ans;
      }
      if (fuc == "str") {
        if(array.size()>1) std::cerr<<"(str)more than one"<<std::endl;
        std::string ans;
        if(std::any_cast<int>(&array[0])) ans=std::any_cast<int>(array[0]);
        if(std::any_cast<bool>(&array[0])) ans=std::any_cast<bool>(array[0]);
        if(std::any_cast<double>(&array[0])) ans=std::any_cast<double>(array[0]);
        return ans;
      }
      if (fuc == "bool") {
        if(array.size()>1) std::cerr<<"(bool)more than one"<<std::endl;
        bool ans;
        if(std::any_cast<int>(&array[0])) ans=std::any_cast<int>(array[0]);
        if(std::any_cast<bool>(&array[0])) ans=std::any_cast<bool>(array[0]);
        if(std::any_cast<double>(&array[0])) ans=std::any_cast<double>(array[0]);
        if(std::any_cast<std::string>(&array[0])){
          if(std::any_cast<std::string>(array[0])=="") ans=false;
          else ans=true;
        }
        return ans;
      }
    }
    return visit(ctx->atom());
  }

  std::any visitTrailer(Python3Parser::TrailerContext *ctx) override {
    if (ctx->arglist())
      return visit(ctx->arglist());
    antlr4::tree::TerminalNode *x;
    return x;
  }

  int string_to_int(std::string s) {
    int ans = 0;
    for (int i = 0; i < s.length(); i++) {
      ans = ans * 10 + s[i] - '0';
    }
    return ans;
  }

  std::string get_string(std::string x){
    std::string ans=x.substr(1,x.size()-2);
    return ans;
  }

  std::any visitAtom(Python3Parser::AtomContext *ctx) override {
    if (ctx->NUMBER()) {
      return string_to_int(ctx->NUMBER()->getText());
    }
    if (ctx->NAME()) {
      return scope.var_query(ctx->NAME()->getText());
    }
    if (ctx->FALSE()) {
    //  std::cout<<"FALSE"<<std::endl;
      return false;
    }
    if (ctx->TRUE()) {
      return true;
    }
    if (ctx->STRING(0)) {
      std::string ans = "";
      auto array = ctx->STRING();
      for (int i = 0; i < array.size(); i++) {
    //  std::cout<<"STRING: "<<std::endl;
        ans = ans + get_string(array[i]->getText());
    //  std::cout<<"STRING_END: "<<std::endl;
      }
      return ans;
    }
    if (ctx->test()) {
      return visit(ctx->test());
    }
    EvalVisitor:: nothing x;
    return x;
  }

  std::any visitArglist(Python3Parser::ArglistContext *ctx) override {
    auto array = ctx->argument();
    std::vector<std::any> ans;
    for (int i = 0; i < array.size(); i++) {
      ans.push_back(visit(array[i]));
    }
    return ans;
  }

  std::any visitArgument(Python3Parser::ArgumentContext *ctx) override {
    auto array = ctx->test();
    if(array.size()==1){
      return visit(array[0]);
    }
    return visit(array[1]);
  }
};

#endif // PYTHON_INTERPRETER_EVALVISITOR_H
