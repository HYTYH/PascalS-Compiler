%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "AST.h"
#include "parser.h"
#include "errorHandle.h"

void yyerror(char *msg);
int runbison();

AST::ProgramNode* astRoot;
extern FILE *file;
extern FILE *yyin;
extern FILE *yyout;
extern int yylex(void);
%}

%locations

%union {
    bool b;
    int t;
    double f;
    char c;
    char* s;
    AST::BaseNode*              ast_BaseNode;
    AST::ProgramNode*           ast_ProgramNode;
    AST::ProgramHeadNode*       ast_ProgramHeadNode;
    AST::ProgramBodyNode*       ast_ProgramBodyNode;
    AST::IdListNode*            ast_IdListNode;
    AST::VarDeclsNode*          ast_VarDeclsNode;
    AST::VarDeclNode*           ast_VarDeclNode;
    AST::IdVarPartsNode*        ast_IdVarPartsNode;
    AST::IdVarPartNode*         ast_IdVarPartNode;
    AST::TypeDeclNode*          ast_TypeDeclNode;
    AST::CompoundStatNode*      ast_CompoundStatNode;
    AST::StatementListNode*     ast_StatementListNode;
    AST::StatementNode*         ast_StatementNode;
    AST::ExpressionNode*        ast_ExpressionNode;
    AST::SimpleExprNode*        ast_SimpleExprNode;
    AST::SignedTermNode*        ast_SignedTermNode;
    AST::UnsignedTermNode*      ast_UnsignedTermNode;
    AST::FactorNode*            ast_FactorNode;
    AST::NotFactorNode*         ast_NotFactorNode;
    AST::UnsignConstVarNode*    ast_UnsignConstVarNode;
    AST::TypeNode*              ast_TypeNode;
    AST::StandardTypeNode*      ast_StandardTypeNode;
    AST::IdNode*                ast_IdNode;
    AST::NumNode*               ast_NumNode;
    AST::IntegerNode*           ast_IntegerNode;
    AST::VariableNode*          ast_VariableNode;
    AST::ConstDeclsNode*        ast_ConstDeclsNode;
    AST::ConstVarNode*          ast_ConstVarNode;
    AST::TypeDeclsNode*         ast_TypeDeclsNode;
    AST::RecordTypeNode*        ast_RecordTypeNode;
    AST::ArrayTypeNode*         ast_ArrayTypeNode;
    AST::PeriodsNode*           ast_PeriodsType;
    AST::PeriodNode*            ast_PeriodType;
    AST::SubProgDeclsNode*      ast_SubProgDeclsNode;
    AST::SubProgDeclNode*       ast_SubProgDeclNode;
    AST::SubProgHeadNode*       ast_SubProgHeadNode;
    AST::FunctionHeadNode*      ast_FunctionHeadNode;
    AST::ParameterListsNode*    ast_ParameterListsNode;
    AST::ParameterListNode*     ast_ParameterListNode;
    AST::VarParameterNode*      ast_VarParameterNode;
    AST::ValueParameterNode*    ast_ValueParameterNode;
    AST::CallProceStatNode*     ast_CallProceStatNode;
    AST::CaseBodyNode*          ast_CaseBodyNode;
    AST::ExprListNode*          ast_ExprListNode;
    AST::BranchListNode*        ast_BranchListNode;
    AST::BranchNode*            ast_BranchNode;
    AST::ConstListNode*         ast_ConstListNode;
    AST::RelopExprNode*         ast_RelopExprNode;
}

/*四种数据类型*/
%token <t> NE GE GT LE LT AS
%token <f> REALNUM
%token <t> INTNUM
%token <c> CHAR EQ OP CP PLUS MINUS MULTIPLY DIVIDE OB CB DOT COMMA COLON SEMI LETTER
%token <b> TRUE FALSE
%token BEGINN DO DOWNTO REPEAT END THEN TO FOR UNTIL WHILE
%token FUNCTION PROCEDURE PROGRAM VAR RECORD TYPE ARRAY CONST
%token IF ELSE CASE OF AND OR NOT BOOLEAN DIV MOD INTEGER REAL
%token <s> ID

%type <ast_ProgramNode> program
%type <ast_ProgramHeadNode> program_head
%type <ast_ProgramBodyNode> program_body
%type <ast_VarDeclsNode> var_parts var_declarations
%type <ast_VarDeclNode> var_declaration
%type <ast_CompoundStatNode> compound_statement
%type <ast_StatementListNode> statement_list
%type <ast_StatementNode> statement
%type <ast_IdListNode> identifier_list
%type <ast_TypeNode> type
%type <ast_StandardTypeNode> standard_type
%type <ast_ExpressionNode> expression
%type <ast_SimpleExprNode> simple_expression
%type <ast_UnsignedTermNode> term
%type <ast_FactorNode> factor
%type <ast_VariableNode> variable
%type <ast_UnsignConstVarNode> unsign_const_variable
%type <ast_ConstDeclsNode> const_parts const_declarations
%type <ast_ConstVarNode> const_variable
%type <ast_TypeDeclsNode> type_parts type_declarations
%type <ast_ArrayTypeNode> array_type
%type <ast_PeriodsType> periods
%type <ast_PeriodType> period 
%type <ast_SubProgDeclsNode> subprogram_declarations
%type <ast_SubProgDeclNode> subprogram_declaration
%type <ast_SubProgHeadNode> subprogram_head
%type <ast_ParameterListsNode> parameter_parts parameter_lists
%type <ast_ParameterListNode> parameter_list
%type <ast_CallProceStatNode> call_procedure_statement
%type <ast_CaseBodyNode> case_body
%type <ast_IdVarPartsNode> id_varparts
%type <ast_IdVarPartNode> id_varpart
%type <ast_ExprListNode> expression_list
%type <ast_BranchListNode> branch_list
%type <ast_BranchNode> branch
%type <ast_ConstListNode> const_list

%start S

%%

S : program
  ;

program : program_head program_body DOT { $$ = new AST::ProgramNode($1,$2); astRoot=$$; }
        ;

program_head : PROGRAM ID OP identifier_list CP SEMI { $$ = new AST::ProgramHeadNode(new AST::IdNode($2), $4); }
             ;

program_body : const_parts type_parts var_parts subprogram_declarations compound_statement 
               { $$ = new AST::ProgramBodyNode($1,$2,$3,$4,$5); }
             ;


identifier_list : identifier_list COMMA ID { $$=$1; $$->insertId($3); }
                | ID { $$ = new AST::IdListNode(); $$->insertId($1); }
                ;

const_parts : CONST const_declarations { $$=$2; }
            | { $$ = new AST::ConstDeclsNode(); }
            ;          

const_declarations : const_declarations ID EQ const_variable SEMI { $$ = $1;
                    $$->insertConstDecl(new AST::ConstDeclNode(new AST::IdNode($2),$4)); }
                   | ID EQ const_variable SEMI { $$=new AST::ConstDeclsNode(); 
                                                 $$->insertConstDecl(new AST::ConstDeclNode(new AST::IdNode($1),$3)); }
                   ;

const_variable : PLUS ID { $$=new AST::ConstVarNode(AST::ConstVarNode::POSITIVE,new AST::IdNode($2)); }
               | MINUS ID { $$=new AST::ConstVarNode(AST::ConstVarNode::MINUS,new AST::IdNode($2)); }
               | ID { $$=new AST::ConstVarNode(AST::ConstVarNode::NONE,new AST::IdNode($1)); }
               | PLUS INTNUM { $$=new AST::ConstVarNode(AST::ConstVarNode::POSITIVE,new AST::IntegerNode($2)); }
               | MINUS INTNUM { $$=new AST::ConstVarNode(AST::ConstVarNode::MINUS,new AST::IntegerNode($2)); }
               | INTNUM { $$=new AST::ConstVarNode(AST::ConstVarNode::NONE,new AST::IntegerNode($1)); }
               | PLUS REALNUM { $$=new AST::ConstVarNode(AST::ConstVarNode::POSITIVE,new AST::RealNode($2)); }
               | MINUS REALNUM { $$=new AST::ConstVarNode(AST::ConstVarNode::MINUS,new AST::RealNode($2)); }
               | REALNUM { $$=new AST::ConstVarNode(AST::ConstVarNode::NONE,new AST::RealNode($1)); }
               | LETTER { $$=new AST::ConstVarNode(AST::ConstVarNode::NONE,new AST::CharNode($1)); }
               ;

type_parts : TYPE type_declarations { $$=$2; }
           | { $$ = new AST::TypeDeclsNode(); }
           ;   

type_declarations : type_declarations ID EQ type SEMI { $$ = $1; $$->insertTypeDecl(new AST::TypeDeclNode(new AST::IdNode($2),$4)); }
                  | ID EQ type SEMI { $$ = new AST::TypeDeclsNode(); $$->insertTypeDecl(new AST::TypeDeclNode(new AST::IdNode($1),$3)); } 
                  ;

type : standard_type { $$ = $1; }
     | array_type { $$ = $1; }
     ;

standard_type : INTEGER { $$ = new AST::StandardTypeNode(AST::StandardTypeNode::StandardType::integer); }
              | REAL    { $$ = new AST::StandardTypeNode(AST::StandardTypeNode::StandardType::real); }
              | CHAR    { $$ = new AST::StandardTypeNode(AST::StandardTypeNode::StandardType::character); }
              | BOOLEAN { $$ = new AST::StandardTypeNode(AST::StandardTypeNode::StandardType::boolean); }
              ;     

array_type : ARRAY OB periods CB OF type { $$ = new AST::ArrayTypeNode($3, $6); }
           ;            

periods : periods COMMA period { $$ = $1; $$->insertPeriod($3); }
        | period { $$ = new AST::PeriodsNode(); $$->insertPeriod($1); }
        ;

period : const_variable DOT DOT const_variable { $$ = new AST::PeriodNode($1, $4); }
       ;

var_parts : VAR var_declarations { $$ = $2; }
          | { $$ = new AST::VarDeclsNode(); }
          ;

var_declarations : var_declarations var_declaration { 
                    $$ = $1; 
                    $$->insertVarDecl($2); 
                }
                | var_declaration { 
                    $$ = new AST::VarDeclsNode(); 
                    $$->insertVarDecl($1); 
                }
                ;

var_declaration : identifier_list COLON type SEMI { $$ = new AST::VarDeclNode($1,$3); }
                ;

subprogram_declarations : subprogram_declarations subprogram_declaration SEMI { $$ = $1; $$->insertSubProgDecl($2); }
                        | { $$ = new AST::SubProgDeclsNode(); }
                        ;

subprogram_declaration : subprogram_head program_body { $$ = new AST::SubProgDeclNode($1, $2); }
                       ;

subprogram_head : FUNCTION ID parameter_parts COLON standard_type SEMI { $$ = new AST::FunctionHeadNode(new AST::IdNode($2),$3,$5); }
                | PROCEDURE ID parameter_parts SEMI { $$ = new AST::FunctionHeadNode(new AST::IdNode($2),$3,nullptr); }
                ;               

parameter_parts : OP parameter_lists CP { $$ = $2; }
                | { $$ = new AST::ParameterListsNode(); }
                ;

parameter_lists : parameter_lists SEMI parameter_list { $$ = $1; $$->insertParameterList($3); }
                | parameter_list { $$ = new AST::ParameterListsNode(); $$->insertParameterList($1); }
                ;

parameter_list : VAR identifier_list COLON standard_type { $$=new AST::VarParameterNode(new AST::ValueParameterNode($2,$4)); }
               | identifier_list COLON standard_type { $$=new AST::ValueParameterNode($1,$3); }
               ;

compound_statement : BEGINN statement_list END { $$ = new AST::CompoundStatNode($2); }
                   ;

statement_list : statement_list SEMI statement { $$ = $1; $$->insertStatement($3); }
               | statement { $$ = new AST::StatementListNode(); $$->insertStatement($1); }
               ;

statement : variable AS expression {
              $1->isLeft = true;
              $$ = new AST::AssignExprStatNode($1,$3); }
          | ID AS expression { 
              AST::VariableNode *variable = new AST::VariableNode(new AST::IdNode($1),nullptr);
              variable->isLeft = true;
              $$ = new AST::AssignExprStatNode(variable,$3); }
          | call_procedure_statement { $$ = $1; }
          | compound_statement { $$ = $1; }
          | IF expression THEN statement ELSE statement { $$ = new AST::IfThenStatNode($2, $4, $6); }
          | IF expression THEN statement { $$ = new AST::IfThenStatNode($2, $4, nullptr); }
          | CASE expression OF case_body END { $$ = new AST::CaseStatNode($2, $4); }
          | WHILE expression DO statement { $$ = new AST::WhileStatNode($2, $4); }
          | REPEAT statement_list UNTIL expression { $$ = new AST::RepeatStatNode($4, $2); }
          | FOR ID AS expression TO expression DO statement { $$ = new AST::ForStatNode(new AST::IdNode($2), $4, AST::ForStatNode::UpDownType::TO, $6, $8); }
          | FOR ID AS expression DOWNTO expression DO statement { $$ = new AST::ForStatNode(new AST::IdNode($2), $4, AST::ForStatNode::UpDownType::DOWNTO, $6, $8); }
          | { $$ = new AST::CompoundStatNode(new AST::StatementListNode()); }
          ;

variable : ID OB expression_list CB { 
    AST::IdVarPartsNode *id_varparts = new AST::IdVarPartsNode();
    AST::IdVarPartNode *id_varpart = new AST::IdVarPartNode($3);
    id_varparts->insertIdVarPart(id_varpart);
    $$ = new AST::VariableNode(new AST::IdNode($1), id_varparts);
};


case_body : branch_list { $$ = new AST::CaseBodyNode($1); }
          | { $$ = new AST::CaseBodyNode(new AST::BranchListNode()); }
          ;

branch_list : branch_list SEMI branch { $$ = $1; $$->insertBranch($3); }
            | branch { $$ = new AST::BranchListNode(); $$->insertBranch($1); }
            ;

branch : const_list COLON statement { $$ = new AST::BranchNode($1, $3); }
       ;

const_list : const_list COMMA const_variable { $$ = $1; $$->insertConstVar($3); }
           | const_variable { $$ = new AST::ConstListNode(); $$->insertConstVar($1); }
           ;

call_procedure_statement : ID { $$ = new AST::CallProceStatNode(new AST::IdNode($1),nullptr); }
                         | ID OP expression_list CP { $$ = new AST::CallProceStatNode(new AST::IdNode($1),$3); }
                         ;

expression_list : expression_list COMMA expression { $$ = $1; $$->insertExpression($3); }
                | expression { $$ = new AST::ExprListNode(); $$->insertExpression($1); }
                ;

                                  
expression : simple_expression EQ simple_expression { $$ = new AST::RelopExprNode($1, AST::RelopExprNode::RelopType::EQ, $3); }
           | simple_expression NE simple_expression { $$ = new AST::RelopExprNode($1, AST::RelopExprNode::RelopType::NE, $3); }
           | simple_expression LT simple_expression { $$ = new AST::RelopExprNode($1, AST::RelopExprNode::RelopType::LT, $3); }
           | simple_expression LE simple_expression { $$ = new AST::RelopExprNode($1, AST::RelopExprNode::RelopType::LE, $3); }
           | simple_expression GT simple_expression { $$ = new AST::RelopExprNode($1, AST::RelopExprNode::RelopType::GT, $3); }
           | simple_expression GE simple_expression { $$ = new AST::RelopExprNode($1, AST::RelopExprNode::RelopType::GE, $3); }
           | simple_expression { $$ = $1; }
           ;

simple_expression : term { $$ = $1; }
                  | PLUS term { $$ = new AST::SignedTermNode(AST::SignedTermNode::SignedType::POSITIVE, $2); } 
                  | MINUS term { $$ = new AST::SignedTermNode(AST::SignedTermNode::SignedType::MINUS, $2); }
                  | simple_expression PLUS term { $$ = new AST::SimpleAddopNode($1, AST::SimpleAddopNode::AddopType::PLUS, $3); }
                  | simple_expression MINUS term { $$ = new AST::SimpleAddopNode($1, AST::SimpleAddopNode::AddopType::MINUS, $3); }
                  | simple_expression OR term { $$ = new AST::SimpleAddopNode($1, AST::SimpleAddopNode::AddopType::OR, $3); }
                  ;

term : term MULTIPLY factor { $$ = new AST::UnsignedTermNode(AST::UnsignedTermNode::MulopType::MULT,$1,$3); }
     | term DIVIDE factor { $$ = new AST::UnsignedTermNode(AST::UnsignedTermNode::MulopType::DIV,$1,$3); }
     | term DIV factor { $$ = new AST::UnsignedTermNode(AST::UnsignedTermNode::MulopType::INTDIV,$1,$3); }
     | term MOD factor { $$ = new AST::UnsignedTermNode(AST::UnsignedTermNode::MulopType::MOD,$1,$3); }
     | term AND factor { $$ = new AST::UnsignedTermNode(AST::UnsignedTermNode::MulopType::AND,$1,$3); }
     | factor { $$ = new AST::UnsignedTermNode($1); }
     ;


factor : unsign_const_variable { $$ = $1; }
       | variable { $$ = $1; }
       | ID OP expression_list CP { $$ = new AST::CallProceStatNode(new AST::IdNode($1), $3); }
       | OP expression CP { $$ = $2; }
       | NOT factor { if($2->type == AST::NOT_FACTOR) { $$ = $2; ((AST::NotFactorNode *)$$)->increaseNotNums(); }
                      else { $$ = new AST::NotFactorNode($2); } }
       ;

unsign_const_variable : INTNUM { $$ = new AST::IntegerNode($1); }
                      | REALNUM    { $$ = new AST::RealNode($1); }
                      | ID { $$ = new AST::IdNode($1); }
                      | LETTER { $$ = new AST::CharNode($1); }
                      | TRUE { $$ = new AST::BooleanNode(true); }
                      | FALSE { $$ = new AST::BooleanNode(false); }
                      ;





%%

void yyerror(char *msg){
    printError(msg);
	parseError = 1;
}