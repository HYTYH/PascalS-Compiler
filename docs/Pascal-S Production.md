## Pascal-S Production

```
program → program_head program_body.

program_head → program id (identifier_list);

program_body → const_declarations
               type_declarations
               var_declarations
               subprogram_declarations
               compound_statement
               
identifier_list → identifier_list,id | id

const_declarations → const const_declaration; | ε

const_declaration → const_declaration; id = const_variable 
                   | id = const_variable

const_variable → +id | -id | id |
                 +num | -num | num | 
                 'letter'

type_declarations → type type_declaration; | ε

type_declaration → type_declaration; id = type | id = type

type → standard_type
              | record record_body end
              | array [periods] of type
              
standard_type → integer | real | boolean | char

record_body → var_declaration 
            | ε

periods → periods,period | period

period → const_variable .. const_variable

var_declarations → var var_declaration; 
                  | ε

var_declaration → var_declaration; identifier_list: type 
                 | identifier_list: type

subprogram_declarations → subprogram_declarations subprogram_declaration; 
                        |  ε
                        
subprogram_declaration → subprogram_head program_body

subprogram_head → function id formal_parameter: standard_type; 
                 | procedure id formal_parameter;
                 
formal_parameter → ( parameter_lists ) 
                  | ε
                  
parameter_lists → parameter_lists; parameter_list | parameter_list

parameter_list → var_parameter | value_parameter

var_parameter → var value_parameter

value_parameter → identifier_list : standard_type

compound_statement → begin statement_list end

statement_list → statement_list; statement 
                | statement
                
statement → variable assignop expression
            | call_procedure_statement
            | compound_statement
            | if expression then statement else_part
            | case expression of case_body end
            | while expression do statement
            | repeat statement_list until expression
            | for id assignop expression updwon expression do statement 
            | ε

variable → id id_varparts

id_varparts → id_varparts id_varpart | ε

id_varpart → [ expression_list ] | .id

else_part → else statement | ε

case_body → branch_list | ε

branch_list → branch_list;branch | branch

branch → const_list : statement

const_list → const_list,const_variable | const_variable

updown → to | downto

call_procedure_statement → id | id(expression_list) 

expression_list → expression_list, expression | expression

expression → simple_expression relop simple_expression | simple_expression

simple_expression → term | +term | -term | simple_expression addop term

term → term mulop factor | factor

factor → unsign_const_variable
        | variable
        | id (expression_list)
        | (expression)
        | not factor
        
unsign_const_variable → id | num | 'letter' 
```