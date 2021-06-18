#if !defined(TOKEN_H)
#define TOKEN_H


enum Token_type{
    ID_TOKEN, 
    STRING_TOKEN,
    NUMBER_TOKEN,

    ASSIGNMENT_TOKEN,
    SEMI_TOKEN,
    COME_TOKEN,

    LPAREN_TOKEN,
    RPAREN_TOKEN,
    LBRACE_TOKEN,
    RBRACE_TOKEN,

    ARITHMETIC_OPERATOR_TOKEN,

    EOF_TOKEN
};

class Token_f{
    Token_type m_type;
    char* m_value;

public:
    Token_f(Token_type token_t, char* token_v);
    Token_type get_type() const;
    char* get_value() const;
};

#endif // TOKEN_H
