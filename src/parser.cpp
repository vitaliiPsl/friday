#include "include/parser.hpp"
#include <iostream>
#include <cstring>

Parser_f::Parser_f(Lexer_f* lexer, Scope_f* scope){
    // puts(__func__);

    m_lexer = lexer;
    m_curr_token = lexer->get_token();
    m_prev_token = m_curr_token;
    m_scope = scope;
}
Parser_f::~Parser_f(){
    delete m_lexer;
    m_lexer = nullptr;

    delete m_curr_token;
    m_curr_token = nullptr;

    delete m_prev_token;
    m_prev_token = nullptr;

    delete m_scope;
    m_scope = nullptr;
}

void Parser_f::next_token(Token_type token_t){
    // puts(__func__);

    if(m_curr_token->get_type() == token_t){
        delete m_prev_token;
        m_prev_token = m_curr_token;
        m_curr_token = m_lexer->get_token();
    }
    else{
        std::cout << "Unexpected token: '" << m_curr_token->get_value() << "' with type: " << m_curr_token->get_type() << std::endl;
        std::cout << "Expected token type: " << token_t << std::endl;
        exit(1);
    }
}

void Parser_f::set_scope(Scope_f* scope){
    // puts(__func__);

    m_scope = scope;
}


Ast_f* Parser_f::parse(){
    // puts(__func__);

    return parse_compound();
}

Ast_f* Parser_f::parse_compound(){
    // puts(__func__);

    Ast_f* ast_compound = new Ast_f(Ast_type::COMPOUND_AST);
    ast_compound->scope = m_scope;

    Ast_f* statement = parse_statement();
    statement->scope = m_scope;

    ast_compound->compound_size++;
    ast_compound->compound_value = new Ast_f*[ast_compound->compound_size];

    ast_compound->compound_value[ast_compound->compound_size - 1] = statement;

    while(m_curr_token->get_type() == Token_type::ENDL_TOKEN){
        next_token(Token_type::ENDL_TOKEN);

        statement = parse_statement();
        statement->scope = m_scope;

        ast_compound->compound_size++;
        ast_compound->compound_value = (Ast_f**)realloc(ast_compound->compound_value, ast_compound->compound_size * sizeof(Ast_f));
        ast_compound->compound_value[ast_compound->compound_size - 1] = statement;
        // std::cout << "Type: " << m_curr_token->get_type() << std::endl;

    }

    return ast_compound;
}

Ast_f* Parser_f::parse_statement(){
    // puts(__func__);

    if(m_curr_token->get_type() == Token_type::ID_TOKEN)
        return parse_id();
    
    return (new Ast_f(Ast_type::END_AST));
}

Ast_f* Parser_f::parse_id(){
    // puts(__func__);

    if(strcmp(m_curr_token->get_value(), "var") == 0 || m_curr_token->get_value()[0] == '_')
        return parse_variable_definition();
    if(strcmp(m_curr_token->get_value(), "fun") == 0 || m_curr_token->get_value()[0] == ':')
        return parse_function_definition();
    else 
        return parse_variable();
    
}

Ast_f* Parser_f::parse_variable_definition(){
    // puts(__func__);

    Ast_f* ast_variable_definition = new Ast_f(Ast_type::VARIABLE_DEFINITION_AST);
    ast_variable_definition->scope = m_scope;

    if(strcmp(m_curr_token->get_value(), "var") == 0){
        next_token(Token_type::ID_TOKEN); // skip 'var'
        ast_variable_definition->variable_definition_name = new char[(strlen(m_curr_token->get_value()) +1) * sizeof(char)];
        strcpy(ast_variable_definition->variable_definition_name, m_curr_token->get_value());
    }
    else{
        ast_variable_definition->variable_definition_name = new char[strlen(m_curr_token->get_value()) * sizeof(char)];
        strcpy(ast_variable_definition->variable_definition_name, m_curr_token->get_value() + 1);
    }
    next_token(Token_type::ID_TOKEN);//skip variable name

    if(m_curr_token->get_type() == Token_type::ENDL_TOKEN){
        ast_variable_definition->variable_definition_value = parse_ni();
        return ast_variable_definition;
    }

    next_token(Token_type::ASSIGNMENT_TOKEN);
    ast_variable_definition->variable_definition_value = parse_expression();
    
    return ast_variable_definition;
}

Ast_f* Parser_f::parse_variable(){
    // puts(__func__);

    next_token(Token_type::ID_TOKEN); // skip variable name

    if(m_curr_token->get_type() == Token_type::LPAREN_TOKEN){
        return parse_function();
    }
    
    if(m_curr_token->get_type() == Token_type::ASSIGNMENT_TOKEN){
        return parse_variable_assignment();
    }

    Ast_f* ast_variable = new Ast_f(Ast_type::VARIABLE_AST);
    ast_variable->scope = m_scope;

    ast_variable->variable_name = new char[(strlen(m_prev_token->get_value()) + 1) * sizeof(char)];
    strcpy(ast_variable->variable_name, m_prev_token->get_value());

    return ast_variable;
}

Ast_f* Parser_f::parse_variable_assignment(){
    // puts(__func__);

    Ast_f* ast_variable_assignment = new Ast_f(Ast_type::VARIABLE_ASSIGNMENT_AST);
    ast_variable_assignment->scope = m_scope;

    ast_variable_assignment->variable_assignment_name = new char[(strlen(m_prev_token->get_value()) + 1) * sizeof(char)];
    strcpy(ast_variable_assignment->variable_assignment_name, m_prev_token->get_value());

    next_token(Token_type::ASSIGNMENT_TOKEN); //skip '='
    ast_variable_assignment->variable_assignment_value = parse_expression();

    return ast_variable_assignment;
}

Ast_f* Parser_f::parse_function_definition(){
    // puts(__func__);

    Ast_f* ast_function_definition = new Ast_f(Ast_type::FUNCTION_DEFINITION_AST);
    ast_function_definition->scope = m_scope;

//function can be declared in two ways: with keyword 'fun' and with ':' as first name's symbol
    if(strcmp(m_curr_token->get_value(), "fun") == 0){
        next_token(Token_type::ID_TOKEN); //skip fun
        ast_function_definition->function_definition_name = new char[(strlen(m_curr_token->get_value()) + 1) * sizeof(char)];
        strcpy(ast_function_definition->function_definition_name, m_curr_token->get_value());
    }
    else{
        ast_function_definition->function_definition_name = new char[strlen(m_curr_token->get_value()) * sizeof(char)];
        strcpy(ast_function_definition->function_definition_name, m_curr_token->get_value() + 1);
    }

    next_token(Token_type::ID_TOKEN); //skip function name
    next_token(Token_type::LPAREN_TOKEN); //skip '('

    if(m_curr_token->get_type() != Token_type::RPAREN_TOKEN)
        ast_function_definition->function_definition_arguments = parse_arguments_list();
    
    next_token(Token_type::RPAREN_TOKEN); // skip ')'
    next_token(Token_type::LBRACE_TOKEN); //skip '{'
    
//In order to parse funtion definition body we need to init new scope with local type, chage parser's scope, 
//and parse compound with new scope. So all statements in function body will have local scope
    ast_function_definition->function_definition_body = new Ast_f(Ast_type::COMPOUND_AST);
    ast_function_definition->function_definition_body->scope = new Scope_f(1);
    
    Scope_f* parser_scope = m_scope;
    set_scope(ast_function_definition->function_definition_body->scope);

    ast_function_definition->function_definition_body = parse_compound();

    set_scope(parser_scope);

    next_token(Token_type::RBRACE_TOKEN);

    return ast_function_definition;
}

Ast_f* Parser_f::parse_function(){
    // puts(__func__);

    Ast_f* ast_function = new Ast_f(Ast_type::FUNCTION_AST);
    ast_function->scope = m_scope;

    ast_function->function_name = new char[(strlen(m_prev_token->get_value()) + 1) * sizeof(char)];
    strcpy(ast_function->function_name, m_prev_token->get_value());

    next_token(Token_type::LPAREN_TOKEN); // skip '('

    if(m_curr_token->get_type() != Token_type::RPAREN_TOKEN)
        ast_function->function_arguments = parse_arguments_list();

    next_token(Token_type::RPAREN_TOKEN);//skip ')'

    return ast_function;
}

Ast_f* Parser_f::parse_arguments_list(){
    // puts(__func__);
    
    Ast_f* ast_arguments_list = new Ast_f(Ast_type::ARGUMENTS_LIST_AST);
    ast_arguments_list->scope = m_scope;

    ast_arguments_list->arguments_list_size++;
    ast_arguments_list->arguments_list_value = new Ast_f*[ast_arguments_list->arguments_list_size];
    ast_arguments_list->arguments_list_value[ast_arguments_list->arguments_list_size - 1] = parse_argument();

    while(m_curr_token->get_type() != Token_type::RPAREN_TOKEN){
        next_token(Token_type::COMA_TOKEN); //skip ','

        ast_arguments_list->arguments_list_size++;
        ast_arguments_list->arguments_list_value =(Ast_f**)realloc(ast_arguments_list->arguments_list_value, ast_arguments_list->arguments_list_size * sizeof(Ast_f*));
        ast_arguments_list->arguments_list_value[ast_arguments_list->arguments_list_size - 1] = parse_argument();
    }

    return ast_arguments_list;
}

Ast_f* Parser_f::parse_argument(){
    // puts(__func__);
    
    Ast_f* ast_argument = new Ast_f(Ast_type::ARGUMENT_AST);
    ast_argument->scope = m_scope;

    ast_argument->argument_name = new char[(strlen(m_curr_token->get_value()) + 1) * sizeof(char)];
    strcpy(ast_argument->argument_name, m_curr_token->get_value());

    ast_argument->argument_value = parse_expression();

    return ast_argument;
}

Ast_f* Parser_f::parse_string(){
    // puts(__func__);
    
    Ast_f* ast_string = new Ast_f(Ast_type::STRING_AST);
    ast_string->scope = m_scope;

    ast_string->string_value = new char[(strlen(m_curr_token->get_value()) + 1) * sizeof(char)];
    strcpy(ast_string->string_value, m_curr_token->get_value());

    next_token(Token_type::STRING_TOKEN);

    return ast_string;
}

Ast_f* Parser_f::parse_number(){
    // puts(__func__);
    
    Ast_f* ast_number = new Ast_f(Ast_type::NUMBER_AST);
    ast_number->scope = m_scope;

    ast_number->number_value = new char[(strlen(m_curr_token->get_value()) + 1) * sizeof(char)];
    strcpy(ast_number->number_value, m_curr_token->get_value());

    next_token(Token_type::NUMBER_TOKEN);

    return ast_number;
}

Ast_f* Parser_f::parse_integer(){
    // puts(__func__);
    
    Ast_f* ast_integer = new Ast_f(Ast_type::INTEGER_AST);
    ast_integer->scope = m_scope;

    ast_integer->integer_value = atoi(m_curr_token->get_value());

    next_token(Token_type::INTEGER_TOKEN);

    return ast_integer;
}

Ast_f* Parser_f::parse_float(){
    // puts(__func__);
    
    Ast_f* ast_float = new Ast_f(Ast_type::FLOAT_AST);
    ast_float->scope = m_scope;

    ast_float->float_value = atof(m_curr_token->get_value());

    next_token(Token_type::INTEGER_TOKEN);

    return ast_float;
}

Ast_f* Parser_f::parse_ni(){
    // puts(__func__);
    
    Ast_f* ast_ni = new Ast_f(Ast_type::NI_AST);
    ast_ni->scope = m_scope;

    ast_ni->ni = "Not initialised";

    return ast_ni;
}

Ast_f* Parser_f::parse_operator(){
    Ast_f* ast_operator;
    switch(m_curr_token->get_value()[0]){
        case '+':
            ast_operator = new Ast_f(Ast_type::OPERATOR_AR_ADDITION_AST);
            ast_operator->operator_priority = 5;
            ast_operator->scope = m_scope;
            break;
        case '-':
            ast_operator = new Ast_f(Ast_type::OPERATOR_AR_SUBTRACTION_AST);
            ast_operator->operator_priority = 5;
            ast_operator->scope = m_scope;
            break;
        case '*':
            ast_operator = new Ast_f(Ast_type::OPERATOR_AR_MULTIPLICATION_AST);
            ast_operator->operator_priority = 5;
            ast_operator->scope = m_scope;
            break;
        case '/':
            ast_operator = new Ast_f(Ast_type::OPERATOR_AR_DIVISION_AST);
            ast_operator->operator_priority = 5;
            ast_operator->scope = m_scope;
            break;
        case '%':
            ast_operator = new Ast_f(Ast_type::OPERATOR_AR_MODULO_AST);
            ast_operator->operator_priority = 5;
            ast_operator->scope = m_scope;
            break;
        default:
            std::cout << "Unexpected token: '" << m_curr_token->get_value() << "' with type: " << m_curr_token->get_type() << std::endl;
            exit(1);
    }
    next_token(Token_type::ARITHMETIC_OPERATOR_TOKEN);

    return ast_operator;
}

Ast_f* Parser_f::parse_expression(){
    // puts(__func__);
    
    Ast_f* ast_expression = new Ast_f(Ast_type::EXPRESSION_AST);
    ast_expression->scope = m_scope;

    ast_expression->expression_tree_root = expr();

    // std::cout << ast_expression->expression_tree_root->get_type() << std::endl;

    return ast_expression;
}



Ast_f* Parser_f::expr(){
    // puts(__func__);
    Ast_f* left = term();
    if(m_curr_token->get_type() == Token_type::ARITHMETIC_OPERATOR_TOKEN){
        if(m_curr_token->get_value()[0] == '+' || m_curr_token->get_value()[0] == '-'){
            Ast_f* e_operator = parse_operator();
            Ast_f* node = new Ast_f(Ast_type::BINARY_OPERATION_AST);
            node->bin_op_left = left;
            node->bin_op_operator = e_operator;
            node->bin_op_left = expr();
            return node;
        }
    }

    return left;
}

Ast_f* Parser_f::term(){
    // puts(__func__);
    Ast_f* left = factor();

    if(m_curr_token->get_type() == Token_type::ARITHMETIC_OPERATOR_TOKEN){
        if(m_curr_token->get_value()[0] == '*' || m_curr_token->get_value()[0] == '/'){
            Ast_f* t_operator = parse_operator();
            Ast_f* node = new Ast_f(Ast_type::BINARY_OPERATION_AST);
            node->bin_op_left = left;
            node->bin_op_operator = t_operator;
            node->bin_op_right = term();
            return node;
        }
    }

    return left;
}

Ast_f* Parser_f::factor(){
    // puts(__func__);
    
    if(m_curr_token->get_type() == Token_type::INTEGER_TOKEN)
        return parse_integer();
    if(m_curr_token->get_type() == Token_type::FLOAT_TOKEN)
        return parse_integer();
    if(m_curr_token->get_type() == Token_type::ID_TOKEN)
        return parse_integer();
    if(m_curr_token->get_type() == Token_type::STRING_TOKEN)
        return parse_integer();
    if(m_curr_token->get_type() == Token_type::LPAREN_TOKEN){
        next_token(Token_type::LPAREN_TOKEN);
        Ast_f* expr = parse_expression();
        next_token(Token_type::RPAREN_TOKEN);
        return expr;
    }

    std::cout << "Unexpected token: '" << m_curr_token->get_value() << "' with type: " << m_curr_token->get_type() << std::endl;
    exit(1);
}