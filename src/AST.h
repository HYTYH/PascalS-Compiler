//
// Created by chengmin on 2021/3/13
//

#ifndef PASCALSCOMPILER_AST_H
#define PASCALSCOMPILER_AST_H

#include <iostream>
#include <string>
#include <vector>

#include "llvm/IR/Value.h"

#include "CodeGenContext.h"

namespace AST {
    
    // define enum type
    enum NodeType {
        BASE = 0,
        PROGRAM,
        PROGRAM_HEAD,
        PROGRAM_BODY,
        ID,
        ID_LIST,
        CONST_DECLS,
        CONST_DECL,
        TYPE_DECLS,
        TYPE_DECL,
        VAR_DECLS,
        VAR_DECL,
        SUB_PROGRAM_DECLS,
        COMPOUND_STATE,
        INTEGER,
        CHAR,
        BOOLEAN,
        REAL,
        STANDARD_TYPE,
        RECORD_TYPE,
        ARRAY_TYPE,
        STATEMENT_LIST,
        ASSIGN_EXPR_STATE,
        SIMPLE_ADDOP,
        SIGNED_TERM,
        UNSIGNED_TERM,
        VARIABLE,
        CONST_VAR,
        PERIODS,
        PERIOD,
        SUB_PROGRAM_DECL,
        FUNCTION_HEAD,
        PROCEDURE_HEAD,
        PARAMETER_LISTS,
        VAR_PARAMETER,
        VALUE_PARAMETER,
        NOT_FACTOR,
        RELOP_EXPR,
        CALL_PROCE_STATE,
        EXPR_LIST,
        IF_THEN_STATE,
        CASE_STATE,
        CASE_BODY,
        BRANCH_LIST,
        BRANCH,
        CONST_LIST,
        WHILE_STATE,
        REPEAT_STATE,
        FOR_STATE,
        ID_VAR_PARTS,
        ID_VAR_PART
    };

    // AST Node Class

    // program
    class ProgramNode;        // program
    class ProgramHeadNode;    // program_head
    class ProgramBodyNode;    // program_body
    class IdNode;             // id
    class IdListNode;         // identifier_list

    // const_declarations
    class ConstDeclsNode;     // const_declarations
    class ConstDeclNode;      // const_declaration
    class ConstVarNode;       // const_variable

    // type_declarations
    class TypeDeclsNode;      // type_declarations
    class TypeDeclNode;       // type_declaration
    class TypeNode;           // type
    class StandardTypeNode;   // standard_type
    class RecordTypeNode;     // record_type
    class ArrayTypeNode;      // array_type
    class PeriodsNode;        // periods
    class PeriodNode;         // period

    // var_declarations
    class VarDeclsNode;       // var_declarations
    class VarDeclNode;        // var_declaration

    // subprogram_declarations
    class SubProgDeclsNode;   // subprogram_declarations
    class SubProgDeclNode;    // subprogram_declaration
    class SubProgHeadNode;    // subprogram_head
    class FunctionHeadNode;   // function_head
    class ParameterListsNode; // parameter_lists
    class ParameterListNode;  // parameter_list
    class VarParameterNode;   // var_parameter
    class ValueParameterNode; // value_parameter

    // compound_statement
    class CompoundStatNode;   // compound_statement
    class StatementListNode;  // statement_list

    // statement
    class StatementNode;      // statement
    class AssignExprStatNode; // assignment_expression_statement
    class CallProceStatNode;  // call_procedure_statement
    class IfThenStatNode;     // if_then_statement
    class CaseStatNode;       // case_statement
    class WhileStatNode;      // while_statement
    class RepeatStatNode;     // repeat_statement
    class ForStatNode;        // for_statement

    // assignment_expression_statement
    class VariableNode;       // variable
    class IdVarPartsNode;     // id_varparts
    class IdVarPartNode;      // id_varpart
    class ExpressionNode;     // expression

    // call_procedure_statement
    class ExprListNode;       // expression_list

    // case_statement
    class CaseBodyNode;       // case_body
    class BranchListNode;     // branch_list
    class BranchNode;         // branch
    class ConstListNode;      // const_list

    // expression
    class RelopExprNode;      // relop_expression
    class SimpleExprNode;     // simple_expression
    class SignedTermNode;     // +term | -term
    class UnsignedTermNode;   // term
    class SimpleAddopNode;    // simple_expression addop term
    class FactorNode;         // factor
    class NotFactorNode;      // not factor
    class UnsignConstVarNode; // unsign_const_variable
    class NumNode;            // num
    class IntegerNode;        // int
    class CharNode;           // char
    class BooleanNode;        // boolean
    class RealNode;           // real

    // define AST Node Class

    // BaseNode
    class BaseNode {
      public:
        enum NodeType type = BASE;

        virtual ~BaseNode() {}
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
    };

    // ProgramNode
    class ProgramNode : public BaseNode {
      public:
        ProgramHeadNode *programHead;
        ProgramBodyNode *programBody;

        ProgramNode(ProgramHeadNode *programHead, ProgramBodyNode *programBody);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ProgramHeadNode
    class ProgramHeadNode : public BaseNode {
      public:
        IdNode *id;
        IdListNode *idList;

        ProgramHeadNode(IdNode *id, IdListNode *idList);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // IdListNode
    class IdListNode : public BaseNode {
      public:
        std::vector<std::string> idList;

        IdListNode();

        void insertId(const char *id);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ProgramBodyNode
    class ProgramBodyNode : public BaseNode {
      public:
        ConstDeclsNode *constDecls;
        TypeDeclsNode *typeDecls;
        VarDeclsNode *varDecls;
        SubProgDeclsNode *subProgDecls;
        CompoundStatNode *compoundStat;

        ProgramBodyNode(ConstDeclsNode *constDecls,
        TypeDeclsNode *typeDecls,
        VarDeclsNode *varDecls,
        SubProgDeclsNode *subProgDecls,
        CompoundStatNode *compoundStat);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // VarDeclsNode
    class VarDeclsNode: public BaseNode {
      public:
        std::vector<VarDeclNode *> varDeclList;

        VarDeclsNode();

        void insertVarDecl(VarDeclNode *varDel);
        
        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // VarDeclNode
    class VarDeclNode: public BaseNode {
      public:
        IdListNode *idList;
        TypeNode *typeNode;

        VarDeclNode(IdListNode *idList, TypeNode *typeNode);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // StatementNode
    class StatementNode : public BaseNode {
      public:
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
    };

    // CompoundStatNode
    class CompoundStatNode : public StatementNode {
      public:
        StatementListNode *statementList;
        
        CompoundStatNode(StatementListNode *statementList);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // StatementListNode
    class StatementListNode : public BaseNode {
      public:
        std::vector<StatementNode *> statementList;

        StatementListNode();

        void insertStatement(StatementNode *statementNode);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // AssignExprStatNode
    class AssignExprStatNode : public StatementNode {
      public:
        VariableNode *variable;
        ExpressionNode *expression;

        AssignExprStatNode(VariableNode *variable, ExpressionNode *expression);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // FactorNode
    class FactorNode : public BaseNode {
      public:
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
    };

    // NotFactorNode
    class NotFactorNode : public FactorNode {
      public:
        FactorNode *factor;
        int notNums; // When Construct The Default Value is 1 

        NotFactorNode(FactorNode *factor);
        
        void increaseNotNums();

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ExpressionNode
    class ExpressionNode : public FactorNode {
      public:
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
    };

    // RelopExprNode
    class RelopExprNode : public ExpressionNode {
      public:
        enum RelopType {
          EQ,
          NE,
          LT,
          LE,
          GT,
          GE
        };

        SimpleExprNode *leftExpression;
        RelopType relopType;
        SimpleExprNode *rightExpression;

        RelopExprNode(SimpleExprNode *leftExpression, RelopType relopType, SimpleExprNode *rightExpression);
        
        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };
    
    // SimpleExprNode
    class SimpleExprNode : public ExpressionNode {
      public:
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
    };

    // SignedTermNode
    class SignedTermNode : public SimpleExprNode {
      public:
        enum SignedType {
            MINUS,
            POSITIVE
        };
        
        SignedType signedType;
        UnsignedTermNode *unsignedTerm;

        SignedTermNode(SignedType signedType, UnsignedTermNode *unsignedTerm);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // UnsignedTermNode
    class UnsignedTermNode : public SimpleExprNode {
      public:
        enum MulopType {
          MULT,
          DIV,
          INTDIV,
          MOD,
          AND,
          NONEMUL
        };
        
        MulopType mulopType;
        UnsignedTermNode *leftTerm;
        FactorNode *rightFactor;

        UnsignedTermNode(MulopType mulopType, UnsignedTermNode *leftTerm, FactorNode *rightFactor);
        UnsignedTermNode(FactorNode *rightFactor);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // SimpleAddopNode
    class SimpleAddopNode : public SimpleExprNode {
      public:
        enum AddopType {
          PLUS,
          MINUS,
          OR
        };

        SimpleExprNode *simpleExpr;
        AddopType addopType;
        UnsignedTermNode *term;

        SimpleAddopNode(SimpleExprNode *simpleExpr, AddopType addopType, UnsignedTermNode *term);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // UnsignConstVarNode
    class UnsignConstVarNode : public FactorNode {
      public:
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
    };

    // IdNode
    class IdNode : public UnsignConstVarNode {
      public:
        std::string idName;

        IdNode(const std::string &name);
        IdNode(const char *name);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // NumNode
    class NumNode : public UnsignConstVarNode {
      public:
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
    };

    // IntegerNode
    class IntegerNode : public NumNode {
      public:
        int value;

        IntegerNode(int value);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // CharNode
    class CharNode : public UnsignConstVarNode {
      public:
        char value;

        CharNode(char value);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // BooleanNode
    class BooleanNode : public NumNode {
      public:
        bool value;

        BooleanNode(bool value);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // RealNode
    class RealNode : public NumNode {
      public:
        double value;

        RealNode(double value);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // VariableNode
    class VariableNode : public FactorNode {
      public:
        IdNode *id;
        bool isLeft = false;
        IdVarPartsNode *idVarParts;

        VariableNode(IdNode *id, IdVarPartsNode *idVarParts);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ConstDeclsNode
    class ConstDeclsNode : public BaseNode {
      public:
        std::vector<ConstDeclNode *> constDeclList;

        ConstDeclsNode();

        void insertConstDecl(ConstDeclNode *constDecl);
        
        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ConstDeclNode
    class ConstDeclNode : public BaseNode {
      public:
        IdNode *id;
        ConstVarNode *constVar;

        ConstDeclNode(IdNode *id, ConstVarNode *constVar);
        
        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ConstVarNode
    class ConstVarNode : public BaseNode {
      public:
        enum ConstSignType {
            MINUS,
            POSITIVE,
            NONE
        };

        ConstSignType constSignType;
        UnsignConstVarNode *unsignConstVar;

        ConstVarNode(ConstSignType constSignType, UnsignConstVarNode *unsignConstVar);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // TypeDeclsNode
    class TypeDeclsNode : public BaseNode {
      public:
        std::vector<TypeDeclNode *> typeDeclList;

        TypeDeclsNode();

        void insertTypeDecl(TypeDeclNode *typeDecl);
        
        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // TypeDeclNode
    class TypeDeclNode : public BaseNode {
      public:
        IdNode *id;
        TypeNode *typeNode;

        TypeDeclNode(IdNode *id, TypeNode *typeNode);
       
        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // TypeNode
    class TypeNode : public BaseNode {
      public:
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
        virtual llvm::Type *convertToLLVMType(CodeGenContext &context) = 0;
    };

    // StandardTypeNode
    class StandardTypeNode : public TypeNode {
      public:
        enum StandardType {
            integer,
            character,
            boolean,
            real
        };

        StandardType standardType;

        StandardTypeNode(StandardType standardType);

        virtual llvm::Type *convertToLLVMType(CodeGenContext &context);
        virtual std::string description();

        virtual llvm::Value *CodeGen(CodeGenContext &context) {};
    };

    // RecordTypeNode
    class RecordTypeNode : public TypeNode {
      public:
        VarDeclNode *varDecl;

        RecordTypeNode(VarDeclNode *varDecl);

        virtual llvm::Type *convertToLLVMType(CodeGenContext &context);
        virtual std::string description();

        virtual llvm::Value *CodeGen(CodeGenContext &context) {};
    };

    // ArrayTypeNode
    class ArrayTypeNode : public TypeNode {
      public:
        PeriodsNode *periods;
        TypeNode *typeNode;

        ArrayTypeNode(PeriodsNode *periods, TypeNode *typeNode);

        virtual llvm::Type *convertToLLVMType(CodeGenContext &context);
        virtual std::string description();

        virtual llvm::Value *CodeGen(CodeGenContext &context) {};
    };

    // PeriodsNode
    class PeriodsNode : public BaseNode {
      public:
        std::vector<PeriodNode *> periodList;

        PeriodsNode();

        void insertPeriod(PeriodNode *period);
        
        virtual llvm::Value *CodeGen(CodeGenContext &context){};
        virtual std::string description();
    };

    // PeriodNode
    class PeriodNode : public BaseNode {
      public:
        ConstVarNode *startConstVar;
        ConstVarNode *endConstVar;

        PeriodNode(ConstVarNode *startConstVar, ConstVarNode *endConstVar);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // SubProgDeclsNode
    class SubProgDeclsNode : public BaseNode {
      public:
        std::vector<SubProgDeclNode *> subProgDecls;

        SubProgDeclsNode();

        void insertSubProgDecl(SubProgDeclNode *subProgDecl);
        
        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // SubProgDeclNode
    class SubProgDeclNode : public BaseNode {
      public:
        SubProgHeadNode *subProgHead;
        ProgramBodyNode *programBody;

        SubProgDeclNode(SubProgHeadNode *subProgHead, ProgramBodyNode *programBody);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // SubProgHeadNode
    class SubProgHeadNode : public BaseNode {
      public:
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
    };

    // FunctionHeadNode
    class FunctionHeadNode : public SubProgHeadNode {
      public:
        IdNode *id;
        ParameterListsNode *parameterLists;
        StandardTypeNode *returnType;

        FunctionHeadNode(IdNode *id, ParameterListsNode *parameterLists, StandardTypeNode *returnType);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ParameterListsNode
    class ParameterListsNode : public BaseNode {
      public:
        std::vector<ParameterListNode *> parameterLists;

        ParameterListsNode();

        void insertParameterList(ParameterListNode *parameterList);
        
        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ParameterListNode
    class ParameterListNode : public BaseNode {
      public:
        virtual llvm::Value *CodeGen(CodeGenContext &context) = 0;
        virtual std::string description() = 0;
    };

    // VarParameterNode
    class VarParameterNode : public ParameterListNode {
      public:
        ValueParameterNode *valueParameter;

        VarParameterNode(ValueParameterNode *valueParameter);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ValueParameterNode
    class ValueParameterNode : public ParameterListNode {
      public:
        IdListNode *idList;
        StandardTypeNode *idListType;

        ValueParameterNode(IdListNode *idList, StandardTypeNode *idListType);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // CallProceStatNode
    class CallProceStatNode : public StatementNode, public FactorNode {
      public: 
        NodeType type; // reload to avoid ambigious
        IdNode *id;
        ExprListNode *exprList;

        CallProceStatNode(IdNode *id, ExprListNode *exprList);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();

      private:
        llvm::Value *sysCodeGenIfNeeded(CodeGenContext &context);
    };

    // ExprListNode
    class ExprListNode : public BaseNode {
      public:
        std::vector<ExpressionNode *> expressions;

        ExprListNode();

        void insertExpression(ExpressionNode *expression);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // IfThenStatNode
    class IfThenStatNode : public StatementNode {
      public:
        ExpressionNode *expression;
        StatementNode *thenStatement;
        StatementNode *elseStatement;

        IfThenStatNode(ExpressionNode *expression, StatementNode *thenStatement, StatementNode *elseStatement);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // CaseStatNode
    class CaseStatNode : public StatementNode {
      public:
        ExpressionNode *expression;
        CaseBodyNode *caseBody;

        CaseStatNode(ExpressionNode *expression, CaseBodyNode *caseBody);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // CaseBodyNode
    class CaseBodyNode : public BaseNode {
      public:
        BranchListNode *branchList;

        CaseBodyNode(BranchListNode *branchList);

        virtual llvm::Value *CodeGen(CodeGenContext &context, llvm::SwitchInst *sw, llvm::BasicBlock *bb, llvm::Type *ty);
        virtual llvm::Value *CodeGen(CodeGenContext &context){};
        virtual std::string description();
    };

    // BranchListNode
    class BranchListNode : public BaseNode {
      public:
        std::vector<BranchNode *> branches;

        BranchListNode();

        void insertBranch(BranchNode *branch);

        virtual llvm::Value *CodeGen(CodeGenContext &context, llvm::SwitchInst *sw, llvm::BasicBlock *bb, llvm::Type *ty);
        virtual llvm::Value *CodeGen(CodeGenContext &context){};
        virtual std::string description();
    };

    // BranchNode
    class BranchNode : public BaseNode {
      public:
        ConstListNode *constList;
        StatementNode *statement;

        BranchNode(ConstListNode *constList, StatementNode *statement);

        virtual llvm::Value *CodeGen(CodeGenContext &context, llvm::SwitchInst *sw, llvm::BasicBlock *bb, llvm::Type *ty);
        virtual llvm::Value *CodeGen(CodeGenContext &context){};
        virtual std::string description();
    };

    // ConstListNode
    class ConstListNode : public BaseNode {
      public:
        std::vector<ConstVarNode *> constVars;

        ConstListNode();

        void insertConstVar(ConstVarNode *constVar);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // WhileStatNode
    class WhileStatNode : public StatementNode {
      public:
        ExpressionNode *expression;
        StatementNode *statement;

        WhileStatNode(ExpressionNode *expression, StatementNode *statement);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // RepeatStatNode
    class RepeatStatNode : public StatementNode {
      public:
        ExpressionNode *expression;
        StatementListNode *statementList;

        RepeatStatNode(ExpressionNode *expression, StatementListNode *statementList);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // ForStatNode
    class ForStatNode : public StatementNode {
      public:
        enum UpDownType {
            TO,
            DOWNTO
        };
        
        IdNode *id;
        ExpressionNode *assigExpression;
        UpDownType updownType;
        ExpressionNode *updownToExpression;
        StatementNode *statement;

        ForStatNode(IdNode *id, ExpressionNode *assigExpression, UpDownType updownType, ExpressionNode *updownToExpression, StatementNode *statement);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    // IdVarPartsNode
    class IdVarPartsNode : public BaseNode {
      public:
        std::vector<IdVarPartNode *> idVarParts;

        IdVarPartsNode();

        void insertIdVarPart(IdVarPartNode *idVarPart);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

    class IdVarPartNode : public BaseNode {
      public:
        enum IdVarPartType {
          EXPRESSION_LIST_PART,
          ID_PART
        };

        IdVarPartType varPartType;
        ExprListNode *expressionList;
        IdNode *id;

        IdVarPartNode(ExprListNode *expressionList);
        IdVarPartNode(IdNode *id);

        virtual llvm::Value *CodeGen(CodeGenContext &context);
        virtual std::string description();
    };

} // namespace AST

#endif
