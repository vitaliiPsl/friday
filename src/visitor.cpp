#include "include/visitor.hpp"
#include <iostream>
#include <cstring>

Visitor_f::Visitor_f(Scope_f* scope) : m_scope(scope){}

void Visitor_f::std_println(Ast_f** argv, size_t args){
    // puts(__func__);

    for(int i = 0; i < args; i++){
        Ast_f* arg_data = visit(argv[i]->argument_value);

        switch(arg_data->get_type()){
            case Ast_type::STRING_AST:
                printf("%s ", arg_data->string_value);
                break;
            case Ast_type::NUMBER_AST:
                printf("%s ", arg_data->number_value);
                break;
            
            case Ast_type::NI_AST:
                printf("%s ", arg_data->ni);
                break;
            default:
                std::cout << "Unknown value" << std::endl;    
        }
    }
    puts("");
}

Ast_f* Visitor_f::visit(Ast_f* node){
    // puts(__func__);

    switch(node->get_type()){
        case Ast_type::COMPOUND_AST:
            return visit_compound(node);
        case Ast_type::VARIABLE_DEFINITION_AST:
            return visit_variable_definition(node);
        case Ast_type::VARIABLE_AST:
            return visit_variable(node);
        case Ast_type::VARIABLE_ASSIGNMENT_AST:
            return visit_variable_assignment(node);
        case Ast_type::FUNCTION_DEFINITION_AST:
            return visit_function_definition(node);
        case Ast_type::FUNCTION_AST:
            return visit_function(node);
        case Ast_type::STRING_AST:
            return visit_string(node);
        case Ast_type::NUMBER_AST:
            return visit_number(node);
        case Ast_type::NI_AST:
            return visit_ni(node);
        case Ast_type::EXPRESSION_AST:
            return visit_expression(node);
        case Ast_type::END_AST: 
            return node;
        default:
            break;
    }
    std::cout << "Unknown statement of type: " << node->get_type() << std::endl;
    exit(1);
}

Ast_f* Visitor_f::visit_compound(Ast_f* node){
    // puts(__func__);

    for(int i = 0; i < node->compound_size; i++){
        visit(node->compound_value[i]);
    }

    return node;
}

Ast_f* Visitor_f::visit_variable_definition(Ast_f* node){
    // puts(__func__);

    if(node->scope->get_type() == 0)
        m_scope->add_variable_definition(node);
    else    
        node->scope->add_variable_definition(node);
    return node;
}

Ast_f* Visitor_f::visit_variable(Ast_f* node){
    // puts(__func__);

    Ast_f* variable_definition = nullptr;

    // if(node->scope->get_type() == 0)
    //     variable_definition = m_scope->get_variable_definition(node->variable_name);
    // else    
    //     variable_definition = node->scope->get_variable_definition(node->variable_name);

    variable_definition = node->scope->get_variable_definition(node->variable_name);
    if(!variable_definition)
        variable_definition = m_scope->get_variable_definition(node->variable_name);
    
    if(variable_definition)
        return visit(variable_definition->variable_definition_value);

    std::cout << "Undefined variable: " << node->variable_name << std::endl;
    exit(1);
}

Ast_f* Visitor_f::visit_variable_assignment(Ast_f* node){
    // puts(__func__);

    Ast_f* variable_definition = node->scope->get_variable_definition(node->variable_assignment_name);
    if(!variable_definition)
        variable_definition = m_scope->get_variable_definition(node->variable_assignment_name);
    if(variable_definition)
        variable_definition->variable_definition_value = node->variable_assignment_value;
        return node;
    
    std::cout << "Undefined variable: " << node->variable_assignment_name << std::endl;
    exit(1);
}

Ast_f* Visitor_f::visit_function_definition(Ast_f* node){
    // puts(__func__);

    m_scope->add_function_definition(node);

    return node;
}

Ast_f* Visitor_f::visit_function(Ast_f* node){
    // puts(__func__);
    
    if(strcmp(node->function_name, "println") == 0){
        std_println(node->function_arguments->arguments_list_value, node->function_arguments->arguments_list_size);

        return node;
    }


    Ast_f* function_definition = m_scope->get_function_definition(node->function_name);

    if(function_definition){
        if(node->function_arguments->arguments_list_size != function_definition->function_definition_arguments->arguments_list_size){
            std::cout << "In function - " << node->function_name << ": " << std::endl;
            std::cout << "Wrong number of arguments. Expected: " << function_definition->function_arguments->arguments_list_size << " arguments" << std::endl;
            exit(1);
        }
        for(int i = 0; i < node->function_arguments->arguments_list_size; i++){
            Ast_f* variable_definition = new Ast_f(Ast_type::VARIABLE_DEFINITION_AST);
            variable_definition->variable_definition_name = function_definition->function_definition_arguments->arguments_list_value[i]->argument_name;
            variable_definition->variable_definition_value = node->function_arguments->arguments_list_value[i]->argument_value;
            
            function_definition->function_definition_body->scope->add_variable_definition(variable_definition);
        }

        return visit(function_definition->function_definition_body);
    }

    std::cout << "Undefined function with name: " << node->function_name << std::endl;
    exit(1);
}

Ast_f* Visitor_f::visit_string(Ast_f* node){
    // puts(__func__);

    return node;
}

Ast_f* Visitor_f::visit_number(Ast_f* node){
    // puts(__func__);

    return node;
}

Ast_f* Visitor_f::visit_ni(Ast_f* node){
    // puts(__func__);

    return node;
}

Ast_f* Visitor_f::visit_expression(Ast_f* node){
    // puts(__func__);
    
}
