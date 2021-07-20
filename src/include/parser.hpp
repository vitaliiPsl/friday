#if !defined(PARSER_H)
#define PARSER_H

#include "lexer.hpp"
#include "ast.hpp"
#include "scope.hpp"

class Parser_f{
    Lexer_f* m_lexer;
    Token_f* m_curr_token;
    Token_f* m_prev_token;
    Scope_f* m_scope;

public:
    Parser_f(Lexer_f* lexer, Scope_f* scope);
    ~Parser_f();
    Ast_f* parse();

private:
    void next_token(Token_type token_t);
    void set_scope(Scope_f* scope);

    Ast_f* parse_compound();
    Ast_f* parse_statement();

    Ast_f* parse_id();

    Ast_f* parse_variable_definition();
    Ast_f* parse_variable();
    Ast_f* parse_variable_assignment();
    
    Ast_f* parse_function_definition();
    Ast_f* parse_function();

    Ast_f* parse_arguments_list();
    Ast_f* parse_argument();

//contorl flow
    Ast_f* parse_repeat();
    Ast_f* parse_while();

    
    Ast_f* parse_string();
    Ast_f* parse_number();
    Ast_f* parse_integer();
    Ast_f* parse_float();
    Ast_f* parse_bool();
    Ast_f* parse_ni();
    Ast_f* parse_operator();

    Ast_f* parse_expression();
    Ast_f* expr();
    Ast_f* term();
    Ast_f* factor();

    Ast_f* parse_logical_expression();
    Ast_f* logical_expr();
    Ast_f* logical_term();
    Ast_f* logical_factor();

    void traverse(Ast_f* root);

};

#endif // PARSER_H
