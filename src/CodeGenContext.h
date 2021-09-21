//
// Created by chengmin on 2021/3/13
//

#ifndef PASCALSCOMPILER_CODEGENCONTEXT_H
#define PASCALSCOMPILER_CODEGENCONTEXT_H

#include <map>
#include <set>
#include <stack>
#include <vector>

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

class GlobalLLVMContext {
  public:
    // Single Instance getGlobalContext
    static llvm::LLVMContext &getGlobalContext();

    // protect the single mode
    GlobalLLVMContext() = delete;
    GlobalLLVMContext(GlobalLLVMContext const &) = delete;
    void operator=(GlobalLLVMContext const &) = delete;
};

class CodeGenBlock {
  public:
    llvm::BasicBlock *block;
    llvm::Value *returnValue;
    CodeGenBlock *parent;
    std::map<std::string, llvm::Value *> locals;
};

class CodeGenContext {
  private:
    // container class for LLVM IR
    std::stack<CodeGenBlock *> blocks;
    std::map<llvm::Function *, std::set<std::string>> symbolTable;
    std::map<llvm::Function *, llvm::Function *> functionParent;
    bool hasSymbol(std::string name);

  public:
    llvm::Module *module;
    llvm::IRBuilder<> builder;

    CodeGenContext();

    llvm::Value *getValue(std::string name);
    void setValue(std::string name, llvm::Value *value);
    void setConstant(std::string name, llvm::Value *value);
    llvm::Value *setVariable(std::string name, llvm::Type *type);
    void setParent(llvm::Function *child, llvm::Function *parent);
    bool isGlobal();

    llvm::Function *curFunc();
    llvm::BasicBlock *curBlock();
    std::map<std::string, llvm::Value *> &curBlockLocals();
    void pushBlock(llvm::BasicBlock *block);
    void popBlock();
};

#endif
