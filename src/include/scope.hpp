#if !defined(SCOPE_H)
#define SCOPE_H

#include "ast.hpp"

class Scope_f{
//0 - global, 1 - local scope
    bool m_type;

    size_t m_variable_definition_size;
    Ast_f** m_variable_definitions;

    size_t m_function_definition_size;
    Ast_f** m_function_definitions;

public:
    Scope_f(bool scope_t = 0);

    void add_variable_definition(Ast_f* variable_definition);
    Ast_f* get_variable_definition(char* variable_name);

    void add_function_definition(Ast_f* function_definition);
    Ast_f* get_function_definition(char* function_name);

    bool get_type() const;
};

#endif // SCOPE_H
