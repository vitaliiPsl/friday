#include "include/lexer.hpp"

#include <iostream>
#include <cstring>

Lexer_f::Lexer_f(char* content) : m_content(content), m_index(0){
    m_current = m_content[m_index];
    m_content_size = strlen(m_content);
}

Lexer_f::~Lexer_f(){
    delete[] m_content;
    m_content = nullptr;
}

void Lexer_f::skip_current(){
    if(m_index < m_content_size && m_current != '\0'){
        m_index++;
        m_current = m_content[m_index];
    }
    else{
        std::cout << "End of the file" << std::endl;
    }
}
void Lexer_f::skip_spaces(){
    while(m_current == ' ')
        skip_current();
}
char* Lexer_f::get_current(){
    char* curr_c = new char[2];
    curr_c[0] = m_current;
    curr_c[1] = '\0';

    return curr_c;
}

Token_f* Lexer_f::get_token(){
    while(m_index < m_content_size && m_current != '0'){

        if(m_current == ' ')
            skip_spaces();
        if(isalpha(m_current) || m_current == '_' || m_current == ':')
            return get_id();
        if(m_current == '"')
            return get_string();
        if(isdigit(m_current))
            return get_number();
        
        switch(m_current){
            case '=':
                return get_next_token((new Token_f(Token_type::ASSIGNMENT_TOKEN, get_current())));
            case ';':
                return get_next_token((new Token_f(Token_type::SEMI_TOKEN, get_current())));
            case '\n':
                return get_next_token((new Token_f(Token_type::ENDL_TOKEN, get_current())));
            case ',':
                return get_next_token((new Token_f(Token_type::COMA_TOKEN, get_current())));
            case '(':
                return get_next_token((new Token_f(Token_type::LPAREN_TOKEN, get_current())));
            case ')':
                return get_next_token((new Token_f(Token_type::RPAREN_TOKEN, get_current())));
            case '{':
                return get_next_token((new Token_f(Token_type::LBRACE_TOKEN, get_current())));
            case '}':
                return get_next_token((new Token_f(Token_type::RBRACE_TOKEN, get_current())));
            case '+':
                return get_next_token((new Token_f(Token_type::ARITHMETIC_OPERATOR_TOKEN, get_current())));
            case '-':
                return get_next_token((new Token_f(Token_type::ARITHMETIC_OPERATOR_TOKEN, get_current())));
            case '*':
                return get_next_token((new Token_f(Token_type::ARITHMETIC_OPERATOR_TOKEN, get_current())));
            case '/':
                return get_next_token((new Token_f(Token_type::ARITHMETIC_OPERATOR_TOKEN, get_current())));
            case '%':
                return get_next_token((new Token_f(Token_type::ARITHMETIC_OPERATOR_TOKEN, get_current())));
            default:
                break;
        }
    }

    return (new Token_f(Token_type::EOF_TOKEN, "\0"));
}
Token_f* Lexer_f::get_id(){
    char* id_value = new char[1];
    id_value[0] = '\0';
    char* curr_c = get_current();
    id_value = (char*)realloc(id_value, (strlen(id_value) + strlen(curr_c) + 1) * sizeof(char));

    strcat(id_value, curr_c);
    skip_current();

    while(isalnum(m_current) || m_current == '_'){
        char* curr_c = get_current();
        id_value = (char*)realloc(id_value, (strlen(id_value) + strlen(curr_c) + 1) * sizeof(char));

        strcat(id_value, curr_c);
        skip_current();
    } 

    return (new Token_f(Token_type::ID_TOKEN, id_value));
}
Token_f* Lexer_f::get_string(){
    skip_current();

    char* string_value = new char[1];
    string_value[0] = '\0';

    while(m_current != '"'){
        char* curr_c = get_current();
        string_value = (char*)realloc(string_value, (strlen(string_value) + strlen(curr_c) + 1) * sizeof(char));
        strcat(string_value, curr_c);

        skip_current();
    } 

    skip_current(); //skip '"'

    return (new Token_f(Token_type::STRING_TOKEN, string_value));
}
Token_f* Lexer_f::get_number(){

    char* number_value = new char[1];
    number_value[0] = '\0';

    while(isdigit(m_current)){
        char* curr_c = get_current();
        number_value = (char*)realloc(number_value, (strlen(number_value) + strlen(curr_c) + 1) * sizeof(char));
        strcat(number_value, curr_c);

        skip_current();
    }

    if(m_current == '.'){
        char* curr_c = get_current();
        number_value = (char*)realloc(number_value, (strlen(number_value) + strlen(curr_c) + 1) * sizeof(char));
        strcat(number_value, curr_c);

        skip_current();
        if(isdigit(m_current)){
            while(isdigit(m_current)){
                char* curr_c = get_current();
                number_value = (char*)realloc(number_value, (strlen(number_value) + strlen(curr_c) + 1) * sizeof(char));
                strcat(number_value, curr_c);

                skip_current();
            }
            
            if(m_current != ' ' && m_current != '\n' && m_current != ';'){
                std::cout << "Unexpected character '" << m_current << "' in number literal" << std::endl;
                exit(1);
            }
        }

        return (new Token_f(Token_type::FLOAT_TOKEN, number_value));
    } 

    

    return (new Token_f(Token_type::INTEGER_TOKEN, number_value));
}

Token_f* Lexer_f::get_next_token(Token_f* token){
    skip_current();
    return token;
}