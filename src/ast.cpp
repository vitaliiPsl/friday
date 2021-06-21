#include "include/ast.hpp"

Ast_f::Ast_f(Ast_type ast_t) : m_type(ast_t){
//node scope
    scope = nullptr;
    
//compound
    compound_size = 0;
    compound_value = nullptr;

//expression
    expression_tree_root = nullptr;

//variable definition
    variable_definition_name = nullptr;
    variable_definition_value = nullptr;

// variable
    variable_name = nullptr;

//variable assignment
    variable_assignment_name = nullptr;
    variable_assignment_value = nullptr;

//function definition
    function_definition_name = nullptr;
    function_definition_body = nullptr;
    function_definition_arguments = nullptr;

//function
    function_name = nullptr;
    function_arguments = nullptr;

//arguments list
    arguments_list_size = 0;
    arguments_list_value = nullptr;

//argument
    argument_name = nullptr;
    argument_value = nullptr;

//string 
    string_value = nullptr;

//number 
    number_value = nullptr;

//integer
    integer_value = 0;

//float
    float_value = 0;

//ni
    ni = nullptr;

//operators
    operator_priority = 0;

//operations
    bin_op_left = nullptr;
    bin_op_right = nullptr;
    bin_op_operator = nullptr;
}

Ast_type Ast_f::get_type() const{
    return m_type;
}

