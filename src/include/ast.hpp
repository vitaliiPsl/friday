#if !defined(AST_H)
#define AST_H

#include <cstdio>

enum Ast_type{
    COMPOUND_AST,
    EXPRESSION_AST,

    VARIABLE_DEFINITION_AST,
    VARIABLE_AST,
    VARIABLE_ASSIGNMENT_AST,

    FUNCTION_DEFINITION_AST,
    FUNCTION_AST,

    ARGUMENTS_LIST_AST,
    ARGUMENT_AST,

    STRING_AST,
    NUMBER_AST,
    NI_AST,

    END_AST, //END OF OPERATIONS

//OPERATOR_AR - arithmetic operators
    OPERATOR_AR_ADDITION_AST,
    OPERATOR_AR_SUBTRACTION_AST,
    OPERATOR_AR_DIVISION_AST,
    OPERATOR_AR_MULTIPLICATION_AST,
    OPERATOR_AR_MODULO_AST,

//operation type
    BINARY_OPERATION_AST,
    UNARY_OPERATION_AST,
};

class Scope_f;


class Ast_f{
    Ast_type m_type;

public:
//scope
    Scope_f* scope;
    
//compound
    size_t compound_size;
    Ast_f** compound_value;

//expression
    Ast_f** expression;
    size_t expression_size;
    Ast_f* expression_tree_root;

//variable definition
    char* variable_definition_name;
    Ast_f* variable_definition_value;

// variable
    char* variable_name;

//variable assignment
    char* variable_assignment_name;
    Ast_f* variable_assignment_value;

//function definition
    char* function_definition_name;
    Ast_f* function_definition_body;
    Ast_f* function_definition_arguments;

//function
    char* function_name;
    Ast_f* function_arguments;

//arguments list
    size_t arguments_list_size;
    Ast_f** arguments_list_value;

//argument
    char* argument_name;
    Ast_f* argument_value;

//string 
    char* string_value;

//number 
    char* number_value;

//ni
    char* ni;

//operators
    size_t operator_priority;

//operations
    Ast_f* bin_op_left;
    Ast_f* bin_op_right;
    Ast_f* bin_op_operator;

public:
    Ast_f(Ast_type ast_t);
    Ast_type get_type() const;
};

#endif // AST_H
