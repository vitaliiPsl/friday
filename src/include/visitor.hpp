#if !defined(VISITOR_H)
#define VISITOR_H

#include "ast.hpp"
#include "scope.hpp"

class Visitor_f{
    Scope_f* m_scope;

public:
    Visitor_f(Scope_f* scope);
    Ast_f* visit(Ast_f* node);

private:
    Ast_f* visit_compound(Ast_f* node);
    Ast_f* visit_variable_definition(Ast_f* node);
    Ast_f* visit_variable(Ast_f* node);
    Ast_f* visit_variable_assignment(Ast_f* node);
    Ast_f* visit_function_definition(Ast_f* node);
    Ast_f* visit_function(Ast_f* node);
    Ast_f* visit_string(Ast_f* node);
    Ast_f* visit_number(Ast_f* node);
    // Ast_f* visit_integer(Ast_f* node);
    // Ast_f* visit_float(Ast_f* node);
    Ast_f* visit_ni(Ast_f* node);
    Ast_f* visit_expression(Ast_f* node);

    void std_println(Ast_f** argv, size_t args);
};

#endif // VISITOR_H
