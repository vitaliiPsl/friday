#include "include/visitor.hpp"
#include <iostream>
#include <cstring>

Visitor_f::Visitor_f(Scope_f* scope) : m_scope(scope){}

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
       
        case Ast_type::REPEAT_AST:
            return visit_repeat(node);
        case Ast_type::WHILE_AST:
            return visit_while(node);
                
        case Ast_type::STRING_AST:
            return visit_string(node);
        case Ast_type::INTEGER_AST:
            return visit_number(node);
        case Ast_type::FLOAT_AST:
            return visit_number(node);
        case Ast_type::NI_AST:
            return visit_ni(node);
        case Ast_type::EXPRESSION_AST:{
            return visit_expression(node->expression_tree_root);
        }
        case Ast_type::LOGICAL_EXPRESSION_AST:{
            return visit_logical_expression(node->expression_tree_root);
        }
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
    
    if(variable_definition){
        Ast_f* var_definition_value = visit(variable_definition->variable_definition_value);
        return visit(variable_definition->variable_definition_value);
    }

    std::cout << "Undefined variable: " << node->variable_name << std::endl;
    exit(1);
}

Ast_f* Visitor_f::visit_variable_assignment(Ast_f* node){
    // puts(__func__);

    Ast_f* variable_definition = node->scope->get_variable_definition(node->variable_assignment_name);
    if(!variable_definition)
        variable_definition = m_scope->get_variable_definition(node->variable_assignment_name);
    if(variable_definition){
        Ast_f* var_assignment_value = visit(node->variable_assignment_value);
        variable_definition->variable_definition_value = var_assignment_value;
        return node;
    }

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
    
    if(strcmp(node->function_name, "print") == 0){
        std_print(node->function_arguments->arguments_list_value, node->function_arguments->arguments_list_size);

        return node;
    }
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

Ast_f* Visitor_f::visit_repeat(Ast_f* node){
    // puts(__func__);

    node->repeat_argument->argument_value = visit_expression(node->repeat_argument->argument_value);
    
    if(node->repeat_argument->argument_value->get_type() != Ast_type::INTEGER_AST){
        std::cout << "Repeat loop argument should be integer" << std::endl;
        exit(1);
    }    
    
    for(int i = 0; i < node->repeat_argument->argument_value->integer_value; i++){
        visit(node->control_flow_body);
    }

    return node;
}

Ast_f* Visitor_f::visit_while(Ast_f* node){
    // puts(__func__);

    Ast_f* condition = visit(node->while_expression);
    while(condition->bool_value){
        visit(node->control_flow_body);
        condition = visit(node->while_expression);
    }

    return node;
}

Ast_f* Visitor_f::visit_string(Ast_f* node){
    // puts(__func__);

    return node;
}

Ast_f* Visitor_f::visit_number(Ast_f* node){
    // puts(__func__);

    return node;
}

Ast_f* Visitor_f::visit_bool(Ast_f* node){
    // puts(__func__);

    return node;
}

Ast_f* Visitor_f::visit_ni(Ast_f* node){
    // puts(__func__);

    return node;
}

Ast_f* Visitor_f::visit_expression(Ast_f* node){
    // puts(__func__);
    
    if(node->get_type() != Ast_type::BINARY_OPERATION_AST)
        return visit(node);
    
    Ast_f* left, *right, *b_operator;
    // std::cout << "Left: " << std::endl; 
    // std::cout << node->bin_op_left->get_type() << std::endl;
    left = visit_expression(node->bin_op_left);
    // std::cout << "Right: " << std::endl;
    // std::cout << node->bin_op_right->get_type() << std::endl;
    right = visit_expression(node->bin_op_right);
    
    if(left->get_type() == Ast_type::STRING_AST || right->get_type() == Ast_type::STRING_AST){
        std::cout << "It is forbiden to use string in expression!" << std::endl;
        exit(1);
    }
    // std::cout << "Operator: " << node->bin_op_operator->get_type() << std::endl;
    b_operator = node->bin_op_operator;

    return visitor_evaluate_binary_operation(left, right, b_operator);
}

Ast_f* Visitor_f::visitor_evaluate_binary_operation(Ast_f* left, Ast_f* right, Ast_f* b_operator){
    // puts(__func__);

    if(left->get_type() == Ast_type::FLOAT_AST && right->get_type() == Ast_type::FLOAT_AST){
        double d_left = left->float_value;
        double d_right = right->float_value;
        double result;

        switch(b_operator->get_type()){
            case Ast_type::OPERATOR_AR_ADDITION_AST:
                result = d_left + d_right;
                break;
            case Ast_type::OPERATOR_AR_SUBTRACTION_AST:
                result = d_left - d_right;
                break;
            case Ast_type::OPERATOR_AR_MULTIPLICATION_AST:
                result = d_left * d_right;
                break;
            case Ast_type::OPERATOR_AR_DIVISION_AST:
                result = d_left / d_right;
                break;
            case Ast_type::OPERATOR_AR_MODULO_AST:
                std::cout << "It is forbiden to use modulo operator with float number!" << std::endl;
                exit(1);
            default:
                break;
        }

        Ast_f* float_number = new Ast_f(Ast_type::FLOAT_AST);
        float_number->float_value = result;
        return float_number;
    }
    if(left->get_type() == Ast_type::INTEGER_AST && right->get_type() == Ast_type::INTEGER_AST){
        long i_left = left->integer_value;
        long i_right = right->integer_value;
        long result;

        switch(b_operator->get_type()){
            case Ast_type::OPERATOR_AR_ADDITION_AST:
                result = i_left + i_right;
                break;
            case Ast_type::OPERATOR_AR_SUBTRACTION_AST:
                result = i_left - i_right;
                break;
            case Ast_type::OPERATOR_AR_MULTIPLICATION_AST:
                result = i_left * i_right;
                break;
            case Ast_type::OPERATOR_AR_DIVISION_AST:
                result = i_left / i_right;
                break;
            case Ast_type::OPERATOR_AR_MODULO_AST:
                result = i_left % i_right;
                break;
            default:
                break;
        }

        Ast_f* int_number = new Ast_f(Ast_type::INTEGER_AST);
        int_number->integer_value = result;
        return int_number;
    }
    else{
        if(left->get_type() == Ast_type::FLOAT_AST){
            double d_left = left->float_value;
            double d_right = right->integer_value;
            double result;

            switch(b_operator->get_type()){
                case Ast_type::OPERATOR_AR_ADDITION_AST:
                    result = d_left + d_right;
                    break;
                case Ast_type::OPERATOR_AR_SUBTRACTION_AST:
                    result = d_left - d_right;
                    break;
                case Ast_type::OPERATOR_AR_MULTIPLICATION_AST:
                    result = d_left * d_right;
                    break;
                case Ast_type::OPERATOR_AR_DIVISION_AST:
                    result = d_left / d_right;
                    break;
                case Ast_type::OPERATOR_AR_MODULO_AST:
                    std::cout << "It is forbiden to use modulo operator with float number!" << std::endl;
                    exit(1);
                default:
                    break;
            }

            Ast_f* float_number = new Ast_f(Ast_type::FLOAT_AST);
            float_number->float_value = result;
            return float_number;
        }
        else{
            double d_left = left->integer_value;
            double d_right = right->float_value;
            double result;

            switch(b_operator->get_type()){
                case Ast_type::OPERATOR_AR_ADDITION_AST:
                    result = d_left + d_right;
                    break;
                case Ast_type::OPERATOR_AR_SUBTRACTION_AST:
                    result = d_left - d_right;
                    break;
                case Ast_type::OPERATOR_AR_MULTIPLICATION_AST:
                    result = d_left * d_right;
                    break;
                case Ast_type::OPERATOR_AR_DIVISION_AST:
                    result = d_left / d_right;
                    break;
                case Ast_type::OPERATOR_AR_MODULO_AST:
                    std::cout << "It is forbiden to use modulo operator with float number!" << std::endl;
                    exit(1);
                default:
                    break;
            }

            Ast_f* float_number = new Ast_f(Ast_type::FLOAT_AST);
            float_number->float_value = result;
            return float_number;
        }
    }
}

Ast_f* Visitor_f::visit_logical_expression(Ast_f* node){
    // puts(__func__);

    if(node->get_type() != Ast_type::BINARY_LOGICAL_OPERATION_AST)
        return visit(node);
    
    Ast_f* left, *right, *b_operator;
    
    if(node->bin_op_left->get_type() == Ast_type::EXPRESSION_AST)
        left = visit_expression(node->bin_op_left);
    else
        left = visit_logical_expression(node->bin_op_left);
    if(node->bin_op_right->get_type() == Ast_type::EXPRESSION_AST)
        right = visit_expression(node->bin_op_right);
    else
        right = visit_logical_expression(node->bin_op_right);

    b_operator = node->bin_op_operator;


    return visitor_evaluate_logical_binary_operation(left, right, b_operator);
}

Ast_f* Visitor_f::visitor_evaluate_logical_binary_operation(Ast_f* left, Ast_f* right, Ast_f* b_operator){
    // puts(__func__);

    Ast_f* ast_bool = new Ast_f(Ast_type::BOOL_AST);
    ast_bool->scope = m_scope;

    bool result;

    switch(b_operator->get_type()){
        case Ast_type::OPERATOR_COMP_GREATER_AST:
            ast_bool->bool_value = left->integer_value > right->integer_value;
            return ast_bool;
        case Ast_type::OPERATOR_COMP_LESS_AST:
            ast_bool->bool_value = left->integer_value < right->integer_value;
            return ast_bool;
        case Ast_type::OPERATOR_COMP_GREATER_EQUAL_AST:
            ast_bool->bool_value = left->integer_value >= right->integer_value;
            return ast_bool;
        case Ast_type::OPERATOR_COMP_LESS_EQUAL_AST:
            ast_bool->bool_value = left->integer_value <= right->integer_value;
            return ast_bool;
        case Ast_type::OPERATOR_COMP_EQUAL_AST:
            ast_bool->bool_value = left->integer_value == right->integer_value;
            return ast_bool;
        case Ast_type::OPERATOR_COMP_NOT_EQUAL_AST:
            ast_bool->bool_value = left->integer_value != right->integer_value;
            return ast_bool;
        case Ast_type::OPERATOR_LOG_AND_AST:
            ast_bool->bool_value = left->integer_value  && right->integer_value;
            return ast_bool;
        case Ast_type::OPERATOR_LOG_OR_AST:
            ast_bool->bool_value = left->integer_value  || right->integer_value;
            return ast_bool;
        default:
            printf("Logical operation error\n");
            exit(1);

        
    }
}

void Visitor_f::std_print(Ast_f** argv, size_t args){
    // puts(__func__);

    for(int i = 0; i < args; i++){
        Ast_f* arg_data = visit(argv[i]->argument_value);

        switch(arg_data->get_type()){
            case Ast_type::STRING_AST:
                printf("%s", arg_data->string_value);
                break;
            case Ast_type::INTEGER_AST:
                printf("%ld", arg_data->integer_value);
                break;
            case Ast_type::FLOAT_AST:
                printf("%f", arg_data->float_value);
                break;
            case Ast_type::NI_AST:
                printf("%s", arg_data->ni);
                break;
            default:
                std::cout << "Unknown value" << std::endl;    
        }
    }
}

void Visitor_f::std_println(Ast_f** argv, size_t args){
    // puts(__func__);

    std_print(argv, args);
    puts("");
}