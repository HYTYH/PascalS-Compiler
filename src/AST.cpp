//
// Created by chengmin on 2021/3/13
//

#include "AST.h"
#include "llvm/IR/ValueSymbolTable.h"

using namespace AST;

extern llvm::LLVMContext &globalInstance;
extern bool printAstInfo;

// ProgramNode
ProgramNode::ProgramNode(ProgramHeadNode *programHead, ProgramBodyNode *programBody) {
    this->programHead = programHead;
    this->programBody = programBody;
    this->type = PROGRAM;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string ProgramNode::description() {
    return std::string("ProgramNode");
}

llvm::Value *ProgramNode::CodeGen(CodeGenContext &context) {
    this->programHead->CodeGen(context);
    this->programBody->CodeGen(context);
    context.builder.CreateRetVoid();
    return 0;
}

// ProgramHeadNode
ProgramHeadNode::ProgramHeadNode(IdNode *id, IdListNode *idList) {
    this->id = id;
    this->idList = idList;
    this->type = PROGRAM_HEAD;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string ProgramHeadNode::description() {
    return std::string("ProgramHeadNode");
}

llvm::Value *ProgramHeadNode::CodeGen(CodeGenContext &context) {

    std::vector<llvm::Type *> paramTypes;
    llvm::FunctionType *funcType = llvm::FunctionType::get(context.builder.getVoidTy(), paramTypes, false);
    llvm::Function *func = llvm::Function::Create(funcType, llvm::GlobalValue::LinkageTypes::ExternalLinkage,
                                                  "main", context.module);
    context.setParent(func, nullptr);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(globalInstance, "", func);
    context.pushBlock(block);
    context.builder.SetInsertPoint(block);

    for (std::string id : this->idList->idList) {
        context.builder.CreateGlobalStringPtr("", id);
    }

    return 0;
}

// ProgramBodyNode
ProgramBodyNode::ProgramBodyNode(ConstDeclsNode *constDecls,
                                 TypeDeclsNode *typeDecls,
                                 VarDeclsNode *varDecls,
                                 SubProgDeclsNode *subProgDecls,
                                 CompoundStatNode *compoundStat) {
    this->type = PROGRAM_BODY;
    this->constDecls = constDecls;
    this->typeDecls = typeDecls;
    this->varDecls = varDecls;
    this->subProgDecls = subProgDecls;
    this->compoundStat = compoundStat;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string ProgramBodyNode::description() {
    return std::string("ProgramBodyNode");
}

llvm::Value *ProgramBodyNode::CodeGen(CodeGenContext &context) {
    this->constDecls->CodeGen(context);
    this->typeDecls->CodeGen(context);
    this->varDecls->CodeGen(context);
    this->subProgDecls->CodeGen(context);
    this->compoundStat->CodeGen(context);
    return 0;
}

// IdListNode
IdListNode::IdListNode() {
    type = ID_LIST;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void IdListNode::insertId(const char *id) {
    std::string idName(id);
    this->idList.push_back(idName);
}

std::string IdListNode::description() {
    return std::string("IdListNode");
}

llvm::Value *IdListNode::CodeGen(CodeGenContext &context) {
    return 0;
}

// IdNode
IdNode::IdNode(const std::string &name) {
    this->type = ID;
    this->idName = name;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

IdNode::IdNode(const char *name) {
    this->type = ID;
    this->idName = std::string(name);
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string IdNode::description() {
    return std::string("IdNode");
}

llvm::Value *IdNode::CodeGen(CodeGenContext &context) {
    llvm::Value *val = context.getValue(this->idName);
    return context.builder.CreateLoad(val);
}

// VarDeclsNode
VarDeclsNode::VarDeclsNode() {
    this->type = VAR_DECLS;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void VarDeclsNode::insertVarDecl(VarDeclNode *varDel) {
    this->varDeclList.push_back(varDel);
}

std::string VarDeclsNode::description() {
    return std::string("VarDeclsNode");
}

llvm::Value *VarDeclsNode::CodeGen(CodeGenContext &context) {
    for (VarDeclNode *varDecl : this->varDeclList) {
        varDecl->CodeGen(context);
    }
    return 0;
}

// VarDeclNode
VarDeclNode::VarDeclNode(IdListNode *idList, TypeNode *typeNode) {
    this->type = VAR_DECL;
    this->idList = idList;
    this->typeNode = typeNode;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string VarDeclNode::description() {
    return std::string("VarDeclNode");
}

llvm::Value *VarDeclNode::CodeGen(CodeGenContext &context) {
    for (std::string id : this->idList->idList) {
        llvm::Type *vartype = this->typeNode->convertToLLVMType(context);
        if (context.isGlobal()) {
            context.setVariable(id, vartype);
        } else {
            context.builder.CreateAlloca(vartype, nullptr, id);
        }
    }
    return 0;
}

// CompoundStatNode
CompoundStatNode::CompoundStatNode(StatementListNode *statementList) {
    this->type = COMPOUND_STATE;
    this->statementList = statementList;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string CompoundStatNode::description() {
    return std::string("CompoundStatNode");
}

llvm::Value *CompoundStatNode::CodeGen(CodeGenContext &context) {
    this->statementList->CodeGen(context);
}

// StatementListNode
StatementListNode::StatementListNode() {
    this->type = STATEMENT_LIST;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string StatementListNode::description() {
    return std::string("StatementListNode");
}

void StatementListNode::insertStatement(StatementNode *statementNode) {
    this->statementList.push_back(statementNode);
}

llvm::Value *StatementListNode::CodeGen(CodeGenContext &context) {
    for (StatementNode *statement : this->statementList) {
        statement->CodeGen(context);
    }
    return 0;
}

// AssignExprStatNode
AssignExprStatNode::AssignExprStatNode(VariableNode *variable, ExpressionNode *expression) {
    this->variable = variable;
    this->expression = expression;
    this->type = ASSIGN_EXPR_STATE;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string AssignExprStatNode::description() {
    return std::string("AssignExprStatNode");
}

llvm::Value *AssignExprStatNode::CodeGen(CodeGenContext &context) {
    llvm::Value *L = this->variable->CodeGen(context);
    llvm::GlobalVariable *g = llvm::dyn_cast<llvm::GlobalVariable>(L);
    if (g != nullptr && g->isConstant()) {
        throw std::logic_error("Constant " + this->variable->id->idName + "cannot be assigned!");
    }
    llvm::Value *R = this->expression->CodeGen(context);
    llvm::Type *LTy = context.builder.CreateLoad(L)->getType();
    llvm::Type *RTy = R->getType();

    if (LTy->isDoubleTy() && RTy->isIntegerTy()) {
        R = context.builder.CreateSIToFP(R, LTy);
    } else if (LTy->isIntegerTy() && RTy->isDoubleTy()) {
        R = context.builder.CreateFPToSI(R, LTy);
    }
    return context.builder.CreateStore(R, L);
}

// SimpleAddopNode
SimpleAddopNode::SimpleAddopNode(SimpleExprNode *simpleExpr, AddopType addopType, UnsignedTermNode *term) {
    this->type = SIMPLE_ADDOP;
    this->simpleExpr = simpleExpr;
    this->term = term;
    this->addopType = addopType;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string SimpleAddopNode::description() {
    return std::string("SimpleAddopNode");
}

llvm::Value *SimpleAddopNode::CodeGen(CodeGenContext &context) {
    llvm::Value *R = this->term->CodeGen(context);
    if (simpleExpr == nullptr) {
        return R;
    }
    llvm::Value *L = this->simpleExpr->CodeGen(context);
    if (L->getType()->isDoubleTy() || R->getType()->isDoubleTy()) {
        // only arithmetic
        if (!L->getType()->isDoubleTy()) {
            L = context.builder.CreateSIToFP(L, context.builder.getDoubleTy());
        }
        if (!R->getType()->isDoubleTy()) {
            R = context.builder.CreateSIToFP(R, context.builder.getDoubleTy());
        }
        switch (this->addopType) {
        case AddopType::PLUS:
            return context.builder.CreateFAdd(L, R, "add");
        case AddopType::MINUS:
            return context.builder.CreateFSub(L, R, "sub");
        default:
            throw std::logic_error("Invalid binary operator!");
        }
    } else {
        switch (this->addopType) {
        case AddopType::PLUS:
            return context.builder.CreateAdd(L, R, "add");
        case AddopType::MINUS:
            return context.builder.CreateSub(L, R, "sub");
        case AddopType::OR:
            return context.builder.CreateOr(L, R, "or");
        default:
            throw std::logic_error("Invalid binary operator!");
        }
    }
}

// SignedTermNode
SignedTermNode::SignedTermNode(SignedType signedType, UnsignedTermNode *unsignedTerm) {
    this->signedType = signedType;
    this->unsignedTerm = unsignedTerm;
    this->type = SIGNED_TERM;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string SignedTermNode::description() {
    return std::string("SignedTermNode");
}

llvm::Value *SignedTermNode::CodeGen(CodeGenContext &context) {
    llvm::Value *val = this->unsignedTerm->CodeGen(context);
    switch (this->signedType) {
    case SignedType::MINUS:
        if (val->getType()->isDoubleTy()) {
            return context.builder.CreateFNeg(val, "neg");
        } else {
            return context.builder.CreateNeg(val, "neg");
        }
    default:
        return val;
    }
}

// UnsignedTermNode
UnsignedTermNode::UnsignedTermNode(MulopType mulopType, UnsignedTermNode *leftTerm, FactorNode *rightFactor) {
    this->type = UNSIGNED_TERM;
    this->leftTerm = leftTerm;
    this->rightFactor = rightFactor;
    this->mulopType = mulopType;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

UnsignedTermNode::UnsignedTermNode(FactorNode *rightFactor) {
    this->type = UNSIGNED_TERM;
    this->mulopType = MulopType::NONEMUL;
    this->leftTerm = nullptr;
    this->rightFactor = rightFactor;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string UnsignedTermNode::description() {
    return std::string("UnsignedTermNode");
}

llvm::Value *UnsignedTermNode::CodeGen(CodeGenContext &context) {
    llvm::Value *R = this->rightFactor->CodeGen(context);
    if (leftTerm == nullptr) {
        return R;
    }
    llvm::Value *L = this->leftTerm->CodeGen(context);
    if (L->getType()->isDoubleTy() || R->getType()->isDoubleTy()) {
        // only arithmetic
        if (!L->getType()->isDoubleTy()) {
            L = context.builder.CreateSIToFP(L, context.builder.getDoubleTy());
        }
        if (!R->getType()->isDoubleTy()) {
            R = context.builder.CreateSIToFP(R, context.builder.getDoubleTy());
        }
        switch (this->mulopType) {
        case MulopType::MULT:
            return context.builder.CreateFMul(L, R, "mul");
        case MulopType::DIV:
            return context.builder.CreateFDiv(L, R, "div");
        case MulopType::MOD:
            return context.builder.CreateFRem(L, R, "mod");
        default:
            throw std::logic_error("Invalid binary operator!");
        }
    } else {
        switch (this->mulopType) {
        case MulopType::MULT:
            return context.builder.CreateMul(L, R, "mul");
        case MulopType::INTDIV:
            return context.builder.CreateSDiv(L, R, "div");
        case MulopType::MOD:
            return context.builder.CreateSRem(L, R, "mod");
        case MulopType::AND:
            return context.builder.CreateAnd(L, R, "and");
        default:
            throw std::logic_error("Invalid binary operator!");
        }
    }
}

// IntegerNode
IntegerNode::IntegerNode(int value) {
    this->type = INTEGER;
    this->value = value;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string IntegerNode::description() {
    return std::string("IntegerNode");
}

llvm::Value *IntegerNode::CodeGen(CodeGenContext &context) {
    return llvm::ConstantInt::get(context.builder.getInt32Ty(), this->value, true);
}

// CharNode
CharNode::CharNode(char value) {
    this->type = CHAR;
    this->value = value;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string CharNode::description() {
    return std::string("CharNode");
}

llvm::Value *CharNode::CodeGen(CodeGenContext &context) {
    return llvm::ConstantInt::get(context.builder.getInt8Ty(), this->value, true);
}

// BooleanNode
BooleanNode::BooleanNode(bool value) {
    this->type = BOOLEAN;
    this->value = value;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string BooleanNode::description() {
    return std::string("BooleanNode");
}

llvm::Value *BooleanNode::CodeGen(CodeGenContext &context) {
    return llvm::ConstantInt::get(context.builder.getInt1Ty(), this->value, true);
}

// RealNode
RealNode::RealNode(double value) {
    this->type = REAL;
    this->value = value;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string RealNode::description() {
    return std::string("RealNode");
}

llvm::Value *RealNode::CodeGen(CodeGenContext &context) {
    return llvm::ConstantFP::get(context.builder.getDoubleTy(), this->value);
}

// VariableNode
VariableNode::VariableNode(IdNode *id, IdVarPartsNode *idVarParts) {
    this->type = VARIABLE;
    this->id = id;
    this->idVarParts = idVarParts;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string VariableNode::description() {
    return std::string("VariableNode");
}

llvm::Value *VariableNode::CodeGen(CodeGenContext &context) {
    llvm::Value *value = context.getValue(this->id->idName);
    llvm::Type *vartype = value->getType();
    if (!vartype->getContainedType(0)->isArrayTy()) {
        return value;
    }

    if (!this->idVarParts->idVarParts[0]->varPartType == IdVarPartNode::EXPRESSION_LIST_PART) {
        throw std::logic_error("The variable varparts is illegal");
    }

    ExprListNode *exprList = this->idVarParts->idVarParts[0]->expressionList;
    ExpressionNode *expr = exprList->expressions[0];
    llvm::Value *exprValue = expr->CodeGen(context);
    llvm::Type *exprType = exprValue->getType();
    if (exprType != context.builder.getInt32Ty()) {
        throw std::logic_error("The array index type is illegal");
    }

    llvm::ArrayType *arrayType = (llvm::ArrayType *)vartype->getContainedType(0);

    llvm::Value *Idxs[] = {llvm::ConstantInt::get(context.builder.getInt32Ty(), 0, true), exprValue};

    llvm::Value *gep = context.builder.CreateGEP(value, Idxs);

    if (this->isLeft) {
        return gep;
    }
    return context.builder.CreateLoad(gep, "load");
}

// ConstDeclsNode
ConstDeclsNode::ConstDeclsNode() {
    this->type = CONST_DECLS;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void ConstDeclsNode::insertConstDecl(ConstDeclNode *constDecl) {
    this->constDeclList.push_back(constDecl);
}

std::string ConstDeclsNode::description() {
    return std::string("ConstDeclsNode");
}

llvm::Value *ConstDeclsNode::CodeGen(CodeGenContext &context) {
    for (ConstDeclNode *constDecl : this->constDeclList) {
        constDecl->CodeGen(context);
    }
    return 0;
}

// ConstDeclNode
ConstDeclNode::ConstDeclNode(IdNode *id, ConstVarNode *constVar) {
    this->type = CONST_DECL;
    this->id = id;
    this->constVar = constVar;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string ConstDeclNode::description() {
    return std::string("ConstDeclNode");
}

llvm::Value *ConstDeclNode::CodeGen(CodeGenContext &context) {
    llvm::Value *val = this->constVar->CodeGen(context);
    if (context.isGlobal()) {
        context.setConstant(this->id->idName, val);
    } else {
        context.builder.CreateStore(val, context.builder.CreateAlloca(val->getType(), nullptr, this->id->idName));
    }
    return 0;
}

// ConstVarNode
ConstVarNode::ConstVarNode(ConstSignType constSignType, UnsignConstVarNode *unsignConstVar) {
    this->type = CONST_VAR;
    this->constSignType = constSignType;
    this->unsignConstVar = unsignConstVar;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string ConstVarNode::description() {
    return std::string("ConstVarNode");
}

llvm::Value *ConstVarNode::CodeGen(CodeGenContext &context) {
    llvm::Value *val;
    switch (this->unsignConstVar->type) {
    case ID:
        val = dynamic_cast<IdNode *>(this->unsignConstVar)->CodeGen(context);
        break;
    default:
        val = this->unsignConstVar->CodeGen(context);
    }
    switch (this->constSignType) {
    case ConstSignType::MINUS:
        if (val->getType()->isDoubleTy()) {
            return context.builder.CreateFNeg(val, "neg");
        } else {
            return context.builder.CreateNeg(val, "neg");
        }
    default:
        return val;
    }
}

// TypeDeclsNode
TypeDeclsNode::TypeDeclsNode() {
    this->type = TYPE_DECLS;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void TypeDeclsNode::insertTypeDecl(TypeDeclNode *typeDecl) {
    this->typeDeclList.push_back(typeDecl);
}

std::string TypeDeclsNode::description() {
    return std::string("TypeDeclsNode");
}

llvm::Value *TypeDeclsNode::CodeGen(CodeGenContext &context) {
    for (TypeDeclNode *typeDecl : this->typeDeclList) {
        typeDecl->CodeGen(context);
    }
    return 0;
}

// TypeDeclNode
TypeDeclNode::TypeDeclNode(IdNode *id, TypeNode *typeNode) {
    this->type = TYPE_DECL;
    this->id = id;
    this->typeNode = typeNode;
}

std::string TypeDeclNode::description() {
    return std::string("TypeDeclNode");
}

llvm::Value *TypeDeclNode::CodeGen(CodeGenContext &context) {
    // TODO
    return 0;
}

// StandardTypeNode
StandardTypeNode::StandardTypeNode(StandardType standardType) {
    this->type = STANDARD_TYPE;
    this->standardType = standardType;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string StandardTypeNode::description() {
    return std::string("StandardTypeNode");
}

llvm::Type *StandardTypeNode::convertToLLVMType(CodeGenContext &context) {
    switch (this->standardType) {
    case StandardTypeNode::StandardType::integer:
        return llvm::Type::getInt32Ty(globalInstance);
    case StandardTypeNode::StandardType::character:
        return llvm::Type::getInt8Ty(globalInstance);
    case StandardTypeNode::StandardType::boolean:
        return llvm::Type::getInt1Ty(globalInstance);
    case StandardTypeNode::StandardType::real:
        return llvm::Type::getDoubleTy(globalInstance);
    }
}

// RecordTypeNode
RecordTypeNode::RecordTypeNode(VarDeclNode *varDecl) {
    this->type = RECORD_TYPE;
    this->varDecl = varDecl;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string RecordTypeNode::description() {
    return std::string("RecordTypeNode");
}

llvm::Type *RecordTypeNode::convertToLLVMType(CodeGenContext &context) {
    // TODO
    return 0;
}

// ArrayTypeNode
ArrayTypeNode::ArrayTypeNode(PeriodsNode *periods, TypeNode *typeNode) {
    this->type = ARRAY_TYPE;
    this->periods = periods;
    this->typeNode = typeNode;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string ArrayTypeNode::description() {
    return std::string("ArrayTypeNode");
}

llvm::Type *ArrayTypeNode::convertToLLVMType(CodeGenContext &context) {
    if (this->periods->periodList.size() > 1) {
        std::cout << "Warning: Array doesn't support multi-dimension!" << std::endl;
    }

    llvm::Value *num = this->periods->periodList[0]->CodeGen(context);
    llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(num);
    int arrayLength = CI->getSExtValue();

    llvm::Type *arrayType = this->typeNode->convertToLLVMType(context);

    return llvm::ArrayType::get(arrayType, arrayLength);
}

// PeriodsNode
PeriodsNode::PeriodsNode() {
    this->type = PERIODS;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void PeriodsNode::insertPeriod(PeriodNode *period) {
    this->periodList.push_back(period);
}

std::string PeriodsNode::description() {
    return std::string("PeriodsNode");
}

// PeriodNode
PeriodNode::PeriodNode(ConstVarNode *startConstVar, ConstVarNode *endConstVar) {
    this->type = PERIOD;
    this->startConstVar = startConstVar;
    this->endConstVar = endConstVar;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string PeriodNode::description() {
    return std::string("PeriodNode");
}

llvm::Value *PeriodNode::CodeGen(CodeGenContext &context) {
    llvm::Value *startValue = this->startConstVar->CodeGen(context);
    llvm::Value *endValue = this->endConstVar->CodeGen(context);
    int start = llvm::dyn_cast<llvm::ConstantInt>(startValue)->getSExtValue();
    int end = llvm::dyn_cast<llvm::ConstantInt>(endValue)->getSExtValue();
    return llvm::ConstantInt::get(context.builder.getInt32Ty(), end - start + 1);
}

// SubProgDeclsNode
SubProgDeclsNode::SubProgDeclsNode() {
    this->type = SUB_PROGRAM_DECLS;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void SubProgDeclsNode::insertSubProgDecl(SubProgDeclNode *subProgDecl) {
    this->subProgDecls.push_back(subProgDecl);
}

std::string SubProgDeclsNode::description() {
    return std::string("SubProgDeclsNode");
}

llvm::Value *SubProgDeclsNode::CodeGen(CodeGenContext &context) {
    for (SubProgDeclNode *subProgDecl : this->subProgDecls) {
        subProgDecl->CodeGen(context);
    }
    return 0;
}

// SubProgDeclNode
SubProgDeclNode::SubProgDeclNode(SubProgHeadNode *subProgHead, ProgramBodyNode *programBody) {
    this->type = SUB_PROGRAM_DECL;
    this->subProgHead = subProgHead;
    this->programBody = programBody;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string SubProgDeclNode::description() {
    return std::string("SubProgDeclNode");
}

llvm::Value *SubProgDeclNode::CodeGen(CodeGenContext &context) {
    llvm::BasicBlock *oldBlock = context.curBlock();
    llvm::Value *ret = this->subProgHead->CodeGen(context);
    this->programBody->CodeGen(context);
    if (ret != nullptr) {
        context.builder.CreateRet(context.builder.CreateLoad(ret));
    } else {
        context.builder.CreateRetVoid();
    }
    while (context.curBlock() != oldBlock) {
        context.popBlock();
    }
    context.builder.SetInsertPoint(oldBlock);
    return 0;
}

// FunctionHeadNode
FunctionHeadNode::FunctionHeadNode(IdNode *id, ParameterListsNode *parameterLists, StandardTypeNode *returnType) {
    this->type = FUNCTION_HEAD;
    this->id = id;
    this->parameterLists = parameterLists;
    this->returnType = returnType;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string FunctionHeadNode::description() {
    return std::string("FunctionHeadNode");
}

llvm::Value *FunctionHeadNode::CodeGen(CodeGenContext &context) {
    std::vector<llvm::Type *> paramTypes;
    ValueParameterNode *valueParameter;
    for (ParameterListNode *parameterList : this->parameterLists->parameterLists) {
        if (parameterList->type == VAR_PARAMETER) {
            valueParameter = dynamic_cast<VarParameterNode *>(parameterList)->valueParameter;
        } else {
            valueParameter = dynamic_cast<ValueParameterNode *>(parameterList);
        }
        for (std::string id : valueParameter->idList->idList) {
            paramTypes.push_back(valueParameter->idListType->convertToLLVMType(context));
        }
    }
    llvm::Type *retType;
    if (this->returnType != nullptr) {
        retType = this->returnType->convertToLLVMType(context);
    } else {
        retType = context.builder.getVoidTy();
    }
    llvm::FunctionType *funcType = llvm::FunctionType::get(retType, paramTypes, false);
    llvm::Function *func = llvm::Function::Create(funcType, llvm::GlobalValue::LinkageTypes::ExternalLinkage, this->id->idName, context.module);
    context.setParent(func, context.curFunc());
    llvm::BasicBlock *block = llvm::BasicBlock::Create(globalInstance, "", func);
    context.pushBlock(block);
    context.builder.SetInsertPoint(block);

    llvm::Function::arg_iterator arg_iter = func->arg_begin();
    for (ParameterListNode *parameterList : this->parameterLists->parameterLists) {
        if (parameterList->type == VAR_PARAMETER) {
            valueParameter = dynamic_cast<VarParameterNode *>(parameterList)->valueParameter;
        } else {
            valueParameter = dynamic_cast<ValueParameterNode *>(parameterList);
        }
        for (std::string id : valueParameter->idList->idList) {
            std::string arg = id + "_";
            arg_iter->setName(arg);
            context.builder.CreateStore(context.getValue(arg), context.builder.CreateAlloca(arg_iter->getType(), nullptr, id));
            arg_iter++;
        }
    }
    return this->returnType ? context.builder.CreateAlloca(retType, nullptr, this->id->idName) : nullptr;
}

// ParameterListsNode
ParameterListsNode::ParameterListsNode() {
    this->type = PARAMETER_LISTS;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void ParameterListsNode::insertParameterList(ParameterListNode *parameterList) {
    this->parameterLists.push_back(parameterList);
}

std::string ParameterListsNode::description() {
    return std::string("ParameterListsNode");
}

llvm::Value *ParameterListsNode::CodeGen(CodeGenContext &context) {
    return 0;
}

// VarParameterNode
VarParameterNode::VarParameterNode(ValueParameterNode *valueParameter) {
    this->type = VAR_PARAMETER;
    this->valueParameter = valueParameter;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string VarParameterNode::description() {
    return std::string("VarParameterNode");
}

llvm::Value *VarParameterNode::CodeGen(CodeGenContext &context) {
    return 0;
}

// ValueParameterNode
ValueParameterNode::ValueParameterNode(IdListNode *idList, StandardTypeNode *idListType) {
    this->type = VALUE_PARAMETER;
    this->idList = idList;
    this->idListType = idListType;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string ValueParameterNode::description() {
    return std::string("ValueParameterNode");
}

llvm::Value *ValueParameterNode::CodeGen(CodeGenContext &context) {
    return 0;
}

// NotFactorNode
NotFactorNode::NotFactorNode(FactorNode *factor) {
    this->type = NOT_FACTOR;
    this->factor = factor;
    this->notNums = 1;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void NotFactorNode::increaseNotNums() {
    this->notNums += 1;
}

std::string NotFactorNode::description() {
    return std::string("NotFactorNode");
}

llvm::Value *NotFactorNode::CodeGen(CodeGenContext &context) {
    llvm::Value *val = this->factor->CodeGen(context);
    if (this->notNums % 2 == 0) {
        return val;
    }
    return context.builder.CreateNot(val, "not");
}

// RelopExprNode
RelopExprNode::RelopExprNode(SimpleExprNode *leftExpression, RelopType relopType, SimpleExprNode *rightExpression) {
    this->type = RELOP_EXPR;
    this->leftExpression = leftExpression;
    this->rightExpression = rightExpression;
    this->relopType = relopType;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string RelopExprNode::description() {
    return std::string("RelopExprNode");
}

llvm::Value *RelopExprNode::CodeGen(CodeGenContext &context) {
    llvm::Value *R = rightExpression->CodeGen(context);
    if (leftExpression == nullptr) {
        return R;
    }
    llvm::Value *L = leftExpression->CodeGen(context);
    if (L->getType()->isDoubleTy() || R->getType()->isDoubleTy()) {
        if (!L->getType()->isDoubleTy()) {
            L = context.builder.CreateSIToFP(L, context.builder.getDoubleTy());
        }
        if (!R->getType()->isDoubleTy()) {
            R = context.builder.CreateSIToFP(R, context.builder.getDoubleTy());
        }
        switch (this->relopType) {
        case RelopType::EQ:
            return context.builder.CreateFCmpUEQ(L, R, "eq_cmp");
        case RelopType::NE:
            return context.builder.CreateFCmpUNE(L, R, "ne_cmp");
        case RelopType::LT:
            return context.builder.CreateFCmpULT(L, R, "lt_cmp");
        case RelopType::LE:
            return context.builder.CreateFCmpULE(L, R, "le_cmp");
        case RelopType::GT:
            return context.builder.CreateFCmpUGT(L, R, "gt_cmp");
        case RelopType::GE:
            return context.builder.CreateFCmpUGE(L, R, "ge_cmp");
        default:
            throw std::logic_error("Invalid binary operator!");
        }
    } else {
        switch (this->relopType) {
        case RelopType::EQ:
            return context.builder.CreateICmpEQ(L, R, "eq_cmp");
        case RelopType::NE:
            return context.builder.CreateICmpNE(L, R, "ne_cmp");
        case RelopType::LT:
            return context.builder.CreateICmpSLT(L, R, "lt_cmp");
        case RelopType::LE:
            return context.builder.CreateICmpSLE(L, R, "le_cmp");
        case RelopType::GT:
            return context.builder.CreateICmpSGT(L, R, "gt_cmp");
        case RelopType::GE:
            return context.builder.CreateICmpSGE(L, R, "ge_cmp");
        default:
            throw std::logic_error("Invalid binary operator!");
        }
    }
}

// CallProceStatNode

CallProceStatNode::CallProceStatNode(IdNode *id, ExprListNode *exprList) {
    this->type = CALL_PROCE_STATE;
    this->id = id;
    this->exprList = exprList;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string CallProceStatNode::description() {
    return std::string("CallProceStatNode");
}

llvm::Value *CallProceStatNode::CodeGen(CodeGenContext &context) {
    llvm::Value *sysCall = this->sysCodeGenIfNeeded(context);
    if (sysCall != 0) {
        return sysCall;
    }

    llvm::Function *callee = context.module->getFunction(this->id->idName);
    if (callee == nullptr) {
        throw std::domain_error("Undeclared function or procedure: " + this->id->idName + "!");
    }
    llvm::Function::arg_iterator arg_iter = context.curFunc()->arg_begin();
    std::vector<llvm::Value *> args;
    if (callee->arg_size()) {
        for (ExpressionNode *expr : this->exprList->expressions) {
            args.push_back(expr->CodeGen(context));
            arg_iter++;
        }
    }

    return context.builder.CreateCall(callee, args);
}

llvm::Value *CallProceStatNode::sysCodeGenIfNeeded(CodeGenContext &context) {
    if (this->id->idName == "write" || this->id->idName == "writeln") {
        auto *char_ptr = context.builder.getInt8Ty()->getPointerTo();
        auto *printf_type = llvm::FunctionType::get(context.builder.getInt32Ty(), char_ptr, true);
        auto *printf_func = context.module->getOrInsertFunction("printf", printf_type);
        std::vector<llvm::Value *> printf_args;
        std::string printf_format;
        int exprListSize = this->exprList->expressions.size();
        for (size_t i = 0; i < exprListSize; i++) {
            ExpressionNode *exp = this->exprList->expressions[i];
            llvm::Value *val = exp->CodeGen(context);
            if (val->getType() == context.builder.getInt32Ty() || val->getType() == context.builder.getInt1Ty()) {
                printf_format += "%d";
                printf_args.push_back(val);
            } else if (val->getType() == context.builder.getInt8Ty()) {
                printf_format += "%c";
                printf_args.push_back(val);
            } else if (val->getType()->isDoubleTy()) {
                printf_format += "%f";
                printf_args.push_back(val);
            }
            if (i != exprListSize - 1) {
                printf_format += " ";
            }
        }
        if (this->id->idName == "writeln")
            printf_format += "\n";
        printf_args.insert(printf_args.begin(), context.builder.CreateGlobalStringPtr(printf_format));
        return context.builder.CreateCall(printf_func, printf_args);
    }
    return 0;
}

// ExprListNode
ExprListNode::ExprListNode() {
    this->type = EXPR_LIST;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void ExprListNode::insertExpression(ExpressionNode *expression) {
    this->expressions.push_back(expression);
}

std::string ExprListNode::description() {
    return std::string("ExprListNode");
}

llvm::Value *ExprListNode::CodeGen(CodeGenContext &context) {
    for (ExpressionNode *expression : this->expressions) {
        expression->CodeGen(context);
    }
    return 0;
}

// IfThenStatNode
IfThenStatNode::IfThenStatNode(ExpressionNode *expression, StatementNode *thenStatement, StatementNode *elseStatement) {
    this->type = IF_THEN_STATE;
    this->expression = expression;
    this->thenStatement = thenStatement;
    this->elseStatement = elseStatement;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string IfThenStatNode::description() {
    return std::string("IfThenStatNode");
}

llvm::Value *IfThenStatNode::CodeGen(CodeGenContext &context) {
    // 调用条件的生成代码
    llvm::Value *expr = this->expression->CodeGen(context);
    if (expr == nullptr) {
        return 0;
    }

    if (!expr->getType()->isIntegerTy(1)) {
        throw std::logic_error("Incompatible type in if condition: expect boolean!");
    }

    // 创建then代码块
    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(globalInstance, "then", context.curFunc());
    // 创建else代码块
    llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(globalInstance, "else", context.curFunc());
    // 创建merge块
    llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(globalInstance, "merge", context.curFunc());

    // 接着在其中选择分支
    context.builder.CreateCondBr(expr, thenBlock, elseBlock);

    context.pushBlock(thenBlock);
    context.builder.SetInsertPoint(thenBlock);
    if (this->thenStatement != nullptr)
        this->thenStatement->CodeGen(context);
    context.builder.CreateBr(mergeBlock);
    context.popBlock();

    context.pushBlock(elseBlock);
    context.builder.SetInsertPoint(elseBlock);
    if (this->elseStatement != nullptr)
        this->elseStatement->CodeGen(context);
    context.builder.CreateBr(mergeBlock);
    context.popBlock();

    // 插入merge块
    context.pushBlock(mergeBlock);
    context.builder.SetInsertPoint(mergeBlock);
    return 0;
}

// CaseStatNode
CaseStatNode::CaseStatNode(ExpressionNode *expression, CaseBodyNode *caseBody) {
    this->type = CASE_STATE;
    this->expression = expression;
    this->caseBody = caseBody;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string CaseStatNode::description() {
    return std::string("CaseStatNode");
}

llvm::Value *CaseStatNode::CodeGen(CodeGenContext &context) {
    // 创建exitBlock
    llvm::BasicBlock *exitBlock = llvm::BasicBlock::Create(globalInstance, "exit", context.curFunc());

    llvm::Value *expr = this->expression->CodeGen(context);
    llvm::Type *type = expr->getType();
    if (!expr->getType()->isIntegerTy()) {
        throw std::logic_error("Case label must be integral type!");
    }

    llvm::Function *func = context.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *afterBlock = llvm::BasicBlock::Create(globalInstance, "after", func);
    llvm::BasicBlock *defaultBlock = afterBlock;

    llvm::SwitchInst *switchBlock = context.builder.CreateSwitch(expr, defaultBlock, this->caseBody->branchList->branches.size());
    this->caseBody->CodeGen(context, switchBlock, exitBlock, type);

    context.builder.SetInsertPoint(afterBlock);
    context.builder.CreateBr(exitBlock);
    context.builder.SetInsertPoint(exitBlock);

    return afterBlock;
}

// CaseBodyNode
CaseBodyNode::CaseBodyNode(BranchListNode *branchList) {
    this->type = CASE_BODY;
    this->branchList = branchList;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string CaseBodyNode::description() {
    return std::string("CaseBodyNode");
}

llvm::Value *CaseBodyNode::CodeGen(CodeGenContext &context, llvm::SwitchInst *sw, llvm::BasicBlock *bb, llvm::Type *ty) {
    this->branchList->CodeGen(context, sw, bb, ty);
    return 0;
}

// BranchListNode
BranchListNode::BranchListNode() {
    this->type = BRANCH_LIST;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void BranchListNode::insertBranch(BranchNode *branch) {
    this->branches.push_back(branch);
}

std::string BranchListNode::description() {
    return std::string("BranchListNode");
}

llvm::Value *BranchListNode::CodeGen(CodeGenContext &context, llvm::SwitchInst *sw, llvm::BasicBlock *bb, llvm::Type *ty) {
    for (auto branch : this->branches) {
        branch->CodeGen(context, sw, bb, ty);
    }
    return 0;
}

// BranchNode
BranchNode::BranchNode(ConstListNode *constList, StatementNode *statement) {
    this->type = BRANCH;
    this->constList = constList;
    this->statement = statement;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string BranchNode::description() {
    return std::string("BranchNode");
}

llvm::Value *BranchNode::CodeGen(CodeGenContext &context, llvm::SwitchInst *sw, llvm::BasicBlock *bb, llvm::Type *ty) {

    llvm::Function *func = context.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *branchBlock = llvm::BasicBlock::Create(globalInstance, "branch", func);

    context.builder.SetInsertPoint(branchBlock);

    llvm::Value *branchStat = statement->CodeGen(context);
    llvm::IntegerType *intType = llvm::dyn_cast<llvm::IntegerType>(ty);

    for (ConstVarNode *currentVar : this->constList->constVars) {
        if (currentVar->CodeGen(context)->getType()->isIntegerTy()) {
            sw->addCase((llvm::ConstantInt *)currentVar->CodeGen(context), branchBlock);
        }
    }

    return context.builder.CreateBr(bb);
}

// ConstListNode
ConstListNode::ConstListNode() {
    this->type = CONST_LIST;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void ConstListNode::insertConstVar(ConstVarNode *constVar) {
    this->constVars.push_back(constVar);
}

std::string ConstListNode::description() {
    return std::string("ConstListNode");
}

llvm::Value *ConstListNode::CodeGen(CodeGenContext &context) {
    for (ConstVarNode *constVar : this->constVars) {
        constVar->CodeGen(context);
    }
    return 0;
}

// WhileStatNode
WhileStatNode::WhileStatNode(ExpressionNode *expression, StatementNode *statement) {
    this->type = WHILE_STATE;
    this->expression = expression;
    this->statement = statement;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string WhileStatNode::description() {
    return std::string("WhileStatNode");
}

llvm::Value *WhileStatNode::CodeGen(CodeGenContext &context) {
    llvm::BasicBlock *loopStartBlock = llvm::BasicBlock::Create(globalInstance, "WHILEloopStart", context.curFunc());
    llvm::BasicBlock *loopStatementBlock = llvm::BasicBlock::Create(globalInstance, "WHILEloopState", context.curFunc());
    llvm::BasicBlock *loopEndBlock = llvm::BasicBlock::Create(globalInstance, "WHILEloopEnd", context.curFunc());

    context.builder.CreateBr(loopStartBlock);
    context.pushBlock(loopStartBlock);
    context.builder.SetInsertPoint(loopStartBlock);

    llvm::Value *testValue = this->expression->CodeGen(context);
    llvm::Value *ret = context.builder.CreateCondBr(testValue, loopStatementBlock, loopEndBlock);

    context.pushBlock(loopStatementBlock);
    context.builder.SetInsertPoint(loopStatementBlock);
    this->statement->CodeGen(context);
    context.builder.CreateBr(loopStartBlock);

    context.popBlock();
    context.pushBlock(loopEndBlock);
    context.builder.SetInsertPoint(loopEndBlock);

    return ret;
}

// RepeatStatNode
RepeatStatNode::RepeatStatNode(ExpressionNode *expression, StatementListNode *statementList) {
    this->type = REPEAT_STATE;
    this->expression = expression;
    this->statementList = statementList;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string RepeatStatNode::description() {
    return std::string("RepeatStatNode");
}

llvm::Value *RepeatStatNode::CodeGen(CodeGenContext &context) {
    llvm::BasicBlock *loopStmtBlock = llvm::BasicBlock::Create(globalInstance, "REPEATloopStmt", context.curFunc());
    llvm::BasicBlock *loopEndBlock = llvm::BasicBlock::Create(globalInstance, "REPEATloopEnd", context.curFunc());
    llvm::BasicBlock *loopExitBlock = llvm::BasicBlock::Create(globalInstance, "REPEATloopExit", context.curFunc());

    context.builder.CreateBr(loopStmtBlock);
    context.builder.SetInsertPoint(loopStmtBlock);
    context.pushBlock(loopStmtBlock);

    this->statementList->CodeGen(context);
    context.builder.CreateBr(loopEndBlock);
    context.popBlock();

    context.pushBlock(loopEndBlock);
    context.builder.SetInsertPoint(loopEndBlock);
    llvm::Value *test = this->expression->CodeGen(context);
    llvm::Value *ret = context.builder.CreateCondBr(test, loopExitBlock, loopStmtBlock);
    context.popBlock();

    context.pushBlock(loopExitBlock);
    context.builder.SetInsertPoint(loopExitBlock);

    return ret;
}

// ForStatNode
ForStatNode::ForStatNode(IdNode *id, ExpressionNode *assigExpression, UpDownType updownType, ExpressionNode *updownToExpression, StatementNode *statement) {
    this->type = FOR_STATE;
    this->id = id;
    this->assigExpression = assigExpression;
    this->updownToExpression = updownToExpression;
    this->updownType = updownType;
    this->statement = statement;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string ForStatNode::description() {
    return std::string("ForStatNode");
}

llvm::Value *ForStatNode::CodeGen(CodeGenContext &context) {
    llvm::BasicBlock *loopEntryBlock = llvm::BasicBlock::Create(globalInstance, "FORloopEntry", context.curFunc());
    llvm::BasicBlock *loopStmtBlock = llvm::BasicBlock::Create(globalInstance, "FORloopStmt", context.curFunc());
    llvm::BasicBlock *loopEndBlock = llvm::BasicBlock::Create(globalInstance, "FORloopEnd", context.curFunc());
    llvm::BasicBlock *loopExitBlock = llvm::BasicBlock::Create(globalInstance, "FORloopExit", context.curFunc());

    context.builder.CreateBr(loopEntryBlock);
    context.pushBlock(loopEntryBlock);
    context.builder.SetInsertPoint(loopEntryBlock);

    VariableNode *var = new VariableNode(this->id, nullptr);
    AssignExprStatNode *assgin = new AssignExprStatNode(var, this->assigExpression);
    assgin->CodeGen(context);

    context.builder.CreateBr(loopStmtBlock);
    context.popBlock();
    context.pushBlock(loopStmtBlock);
    context.builder.SetInsertPoint(loopStmtBlock);
    this->statement->CodeGen(context);

    context.builder.CreateBr(loopEndBlock);
    context.popBlock();
    context.pushBlock(loopEndBlock);
    context.builder.SetInsertPoint(loopEndBlock);

    SimpleExprNode *term1 = new UnsignedTermNode(new IntegerNode(1));
    SimpleAddopNode::AddopType incOrDec = this->updownType == TO ? SimpleAddopNode::PLUS : SimpleAddopNode::MINUS;
    UnsignedTermNode *idTerm = new UnsignedTermNode(this->id);
    VariableNode *idVar = new VariableNode(this->id, nullptr);

    SimpleAddopNode *simpleAddop = new SimpleAddopNode(term1, incOrDec, idTerm);
    AssignExprStatNode *assginAdd = new AssignExprStatNode(idVar, simpleAddop);
    assginAdd->CodeGen(context);

    RelopExprNode *relopGE = new RelopExprNode(idTerm, RelopExprNode::GT, (SimpleExprNode *)this->updownToExpression);
    llvm::Value *testValue = relopGE->CodeGen(context);
    llvm::Value *ret = context.builder.CreateCondBr(testValue, loopExitBlock, loopStmtBlock);

    context.popBlock();
    context.pushBlock(loopExitBlock);
    context.builder.SetInsertPoint(loopExitBlock);

    return ret;
}

// IdVarPartsNode
IdVarPartsNode::IdVarPartsNode() {
    this->type = ID_VAR_PARTS;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

void IdVarPartsNode::insertIdVarPart(IdVarPartNode *idVarPart) {
    this->idVarParts.push_back(idVarPart);
}

std::string IdVarPartsNode::description() {
    return std::string("IdVarPartsNode");
}

llvm::Value *IdVarPartsNode::CodeGen(CodeGenContext &context) {
    // TODO
    return 0;
}

// IdVarPartNode
IdVarPartNode::IdVarPartNode(ExprListNode *expressionList) {
    this->type = ID_VAR_PART;
    this->varPartType = EXPRESSION_LIST_PART;
    this->expressionList = expressionList;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

IdVarPartNode::IdVarPartNode(IdNode *id) {
    this->type = ID_VAR_PART;
    this->varPartType = ID_PART;
    this->id = id;
    if (printAstInfo)
        std::cout << "Generate: new " << this->description() << std::endl;
}

std::string IdVarPartNode::description() {
    return std::string("IdVarPartNode");
}

llvm::Value *IdVarPartNode::CodeGen(CodeGenContext &context) {
    // TODO
    return 0;
}
