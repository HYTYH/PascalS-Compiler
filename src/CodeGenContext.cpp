//
// Created by chengmin on 2021/3/13
//

#include "CodeGenContext.h"

#include "llvm/IR/ValueSymbolTable.h"

llvm::LLVMContext &globalInstance = GlobalLLVMContext::getGlobalContext();

// GlobalLLVMContext
llvm::LLVMContext &GlobalLLVMContext::getGlobalContext() {
    static llvm::LLVMContext globalInstance;
    return globalInstance;
}

// CodeGenContext
CodeGenContext::CodeGenContext() : builder(globalInstance) {
    this->module = new llvm::Module("", globalInstance);
}

llvm::Value *CodeGenContext::getValue(std::string name) {
    llvm::Value *val = this->curFunc()->getValueSymbolTable().lookup(name);
    if (val == nullptr) {
        val = this->module->getGlobalVariable(name);
        if (val == nullptr || !this->hasSymbol(name)) {
            throw std::domain_error("Undeclared constant or variable: " + name + "!");
        }
    }
    return val;
}

void CodeGenContext::setValue(std::string name, llvm::Value *value) {
    llvm::cast<llvm::GlobalVariable>(this->getValue(name))->setInitializer(llvm::cast<llvm::Constant>(value));
}

void CodeGenContext::setConstant(std::string name, llvm::Value *value) {
    if (this->hasSymbol(name)) {
        throw std::domain_error("Repeatedly declare identifier: " + name + "!");
    }
    this->symbolTable[this->curFunc()].insert(name);
    new llvm::GlobalVariable(*this->module, value->getType(), true, llvm::GlobalValue::ExternalLinkage, llvm::cast<llvm::Constant>(value), name);
}

llvm::Value *CodeGenContext::setVariable(std::string name, llvm::Type *type) {
    if (this->hasSymbol(name)) {
        throw std::domain_error("Repeatedly declare identifier: " + name + "!");
    }
    this->symbolTable[this->curFunc()].insert(name);
    if (type->isArrayTy()) {
        return new llvm::GlobalVariable(*this->module, type, false, llvm::GlobalValue::CommonLinkage, llvm::ConstantAggregateZero::get(type->getContainedType(0)), name);
    } else if (type->isDoubleTy()) {
        return new llvm::GlobalVariable(*this->module, type, false, llvm::GlobalValue::CommonLinkage, llvm::ConstantFP::get(type, 0), name);
    } else {
        return new llvm::GlobalVariable(*this->module, type, false, llvm::GlobalValue::CommonLinkage, llvm::ConstantInt::get(type, 0), name);
    }
}

void CodeGenContext::setParent(llvm::Function *child, llvm::Function *parent) {
    this->functionParent[child] = parent;
}

bool CodeGenContext::isGlobal() {
    return this->functionParent[this->curFunc()] == nullptr;
}

llvm::Function *CodeGenContext::curFunc() {
    return this->curBlock()->getParent();
}

bool CodeGenContext::hasSymbol(std::string name) {
    llvm::Function *func = this->curFunc();
    while (func != nullptr) {
        std::set<std::string> s = this->symbolTable[func];
        if (s.find(name) != s.end()) {
            return true;
        }
        func = this->functionParent[func];
    }
    return false;
}

llvm::BasicBlock *CodeGenContext::curBlock() {
    return this->blocks.top()->block;
}

std::map<std::string, llvm::Value *> &CodeGenContext::curBlockLocals() {
    return this->blocks.top()->locals;
}

void CodeGenContext::pushBlock(llvm::BasicBlock *block) {
    CodeGenBlock *newBlock = new CodeGenBlock();
    newBlock->returnValue = nullptr;
    newBlock->block = block;
    this->blocks.push(newBlock);
}

void CodeGenContext::popBlock() {
    CodeGenBlock *top = this->blocks.top();
    this->blocks.pop();
    delete top;
}