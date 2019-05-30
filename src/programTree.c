#include <stdio.h>
#include <stdlib.h>

#include "programTree.h"

#include "programTreePrinter.h"
#include "linkedlist.h"

int yeet = 42;
void yyerror(const char *msg);

/*
__   _______ _____ _   _   ___  _    _                 
\ \ / /  ___|  ___| | | | / _ \| |  | |                
 \ V /| |__ | |__ | |_| |/ /_\ \ |  | |                
  \ / |  __||  __||  _  ||  _  | |/\| |                
  | | | |___| |___| | | || | | \  /\  /                
  \_/ \____/\____/\_| |_/\_| |_/\/  \/                 
                                                       
                                                       
 _____ _____ _ _____    _____    _____ ___________ _____ 
|_   _|_   _( )  ___|  /  __ \  /  __ \  _  |  _  \  ___|
  | |   | | |/\ `--.   | /  \/  | /  \/ | | | | | | |__  
  | |   | |    `--. \  | |      | |   | | | | | | |  __| 
 _| |_  | |   /\__/ /  | \__/\  | \__/\ \_/ / |/ /| |___ 
 \___/  \_/   \____/    \____/   \____/\___/|___/ \____/ 
                                                       
*/

/**
 * hello
 * i am a comment
 * 
 * why did the chicken cross the road?
 */

/*
*   For every grammar rule, there is a function.
*/

/*****************************************************************************************************************************************
*   program
*/
nt_program *ntf_program_1(nt_program_element_list *program_element_list)
{
    nt_program *ret = malloc(sizeof(nt_program));
    ret->self = PROGRAM;
    ret->variable_declarations = program_element_list->variable_declarations;
    ret->function_declarations = program_element_list->function_declarations;
    ret->function_definitions = program_element_list->function_definitions;
    ret->entry_point = 0;
    if (print_st)
    {
        printf("################################################################################\n");
        printf("    SYMBOL TABLE\n");
        printf("################################################################################\n");
        print_symbol_table();
    }
    if (print_c)
    {
        printf("################################################################################\n");
        printf("    RECREATED CODE FROM SYNTAX TREE\n");
        printf("################################################################################\n");
        print_nt_program(0, ret);
        printf("################################################################################\n");
        printf("    end.\n");
        printf("################################################################################\n");
    }
    if (print_i)
    {
        print_inter_code(out_inter_code(ret));
    }
    return ret;
}

/*****************************************************************************************************************************************
*   program_element_list
*/
nt_program_element_list *ntf_program_element_list_1(nt_program_element_list *rest, nt_program_element *next)
{
    switch (next->represents)
    {
    case VARIABLE_DECLARATION:
        add_to_ll(rest->variable_declarations, next->content);
        break;
    case FUNCTION_DECLARATION:
        add_to_ll(rest->function_declarations, next->content);
        break;
    case FUNCTION_DEFINITION:
        add_to_ll(rest->function_definitions, next->content);
        break;
    default:
        break;
    }
    return rest;
}
nt_program_element_list *ntf_program_element_list_2(nt_program_element *next)
{
    nt_program_element_list *ret = malloc(sizeof(nt_program_element_list));
    ret->self = PROGRAM_ELEMENT_LIST;
    ret->variable_declarations = new_ll();
    ret->function_declarations = new_ll();
    ret->function_definitions = new_ll();
    switch (next->represents)
    {
    case VARIABLE_DECLARATION:
        add_to_ll(ret->variable_declarations, next->content);
        break;
    case FUNCTION_DECLARATION:
        add_to_ll(ret->function_declarations, next->content);
        break;
    case FUNCTION_DEFINITION:
        add_to_ll(ret->function_definitions, next->content);
        break;
    default:
        break;
    }
    return ret;
}

/*****************************************************************************************************************************************
*   program_element
*/
nt_program_element *ntf_program_element_1(nt_variable_declaration *var)
{
    list_element *current = var->identifiers->first;
    while (current)
    {
        nt_identifier_declaration *ident = current->data;
        if (get_variable_entry(ident->id))
        {
            yyerror("Variable already defined!");
            exit(1);
        }
        type type = var->type;
        if (type == INT_ && ident->arrayAize > 0)
            type = INTARRAY;
        add_global_variable(ident->id, type);
        current = current->next;
    }
    nt_program_element *ret = malloc(sizeof(nt_program_element));
    ret->self = PROGRAM_ELEMENT;
    ret->represents = VARIABLE_DECLARATION;
    ret->content = var;
    return ret;
}
nt_program_element *ntf_program_element_2(nt_function_declaration *fun)
{
    //TODO
    nt_program_element *ret = malloc(sizeof(nt_program_element));
    ret->self = PROGRAM_ELEMENT;
    ret->represents = FUNCTION_DECLARATION;
    ret->content = fun;
    return ret;
}
nt_program_element *ntf_program_element_3(nt_function_definition *fun)
{
    //TODO
    nt_program_element *ret = malloc(sizeof(nt_program_element));
    ret->self = PROGRAM_ELEMENT;
    ret->represents = FUNCTION_DEFINITION;
    ret->content = fun;
    return ret;
}

/*****************************************************************************************************************************************
*   variable_declaration
*/
nt_variable_declaration *ntf_variable_declaration_1(nt_variable_declaration *rest, nt_identifier_declaration *next)
{
    add_to_ll(rest->identifiers, next);
    return rest;
}
nt_variable_declaration *ntf_variable_declaration_2(type type, nt_identifier_declaration *next)
{
    if (type == VOID_)
    {
        yyerror("Variables may not be of type VOID!");
        exit(1);
    }
    nt_variable_declaration *ret = malloc(sizeof(nt_variable_declaration));
    ret->self = VARIABLE_DECLARATION;
    ret->type = type;
    ret->identifiers = new_ll();
    add_to_ll(ret->identifiers, next);
    return ret;
}

/*****************************************************************************************************************************************
*   identifier_declaration
*/
nt_identifier_declaration *ntf_identifier_declaration_1(char *id, int size)
{
    if (size <= 0)
    {
        yyerror("");
        //good enough
        printf("Size of array %s must be greater than zero, is %d\n", id, size);
        exit(1);
    }
    nt_identifier_declaration *ret = malloc(sizeof(nt_identifier_declaration));
    ret->self = IDENTIFIER_DECLARATION;
    ret->id = id;
    ret->arrayAize = size;
    return ret;
}
nt_identifier_declaration *ntf_identifier_declaration_2(char *id)
{
    nt_identifier_declaration *ret = malloc(sizeof(nt_identifier_declaration));
    ret->self = IDENTIFIER_DECLARATION;
    ret->id = id;
    ret->arrayAize = 0;
    return ret;
}

/*****************************************************************************************************************************************
*   function_definition
*/
nt_function_definition *ntf_function_definition_1(type type, char *id, nt_stmt_list *stmt_list)
{
    nt_function_definition *ret = malloc(sizeof(nt_function_definition));
    ret->self = FUNCTION_DEFINITION;
    ret->id = id;
    ret->return_type = type;
    ret->params = new_ll();
    ret->statements = stmt_list->statements;
    ret->stmt_list = stmt_list;

    finish_function_definition(id, type);

    return ret;
}
nt_function_definition *ntf_function_definition_2(type type, char *id, nt_function_parameter_list *param_list, nt_stmt_list *stmt_list)
{
    nt_function_definition *ret = malloc(sizeof(nt_function_definition));
    ret->self = FUNCTION_DEFINITION;
    ret->id = id;
    ret->return_type = type;
    ret->params = param_list->params;
    ret->statements = stmt_list->statements;
    ret->stmt_list = stmt_list;

    finish_function_definition(id, type);

    return ret;
}

/*****************************************************************************************************************************************
*   function_declaration
*/
nt_function_declaration *ntf_function_declaration_1(type type, char *id)
{
    nt_function_declaration *ret = malloc(sizeof(nt_function_declaration));
    ret->self = FUNCTION_DECLARATION;
    ret->id = id;
    ret->return_type = type;
    ret->params = new_ll();

    finish_function_declaration(id, type);

    return ret;
}
nt_function_declaration *ntf_function_declaration_2(type type, char *id, nt_function_parameter_list *param_list)
{
    nt_function_declaration *ret = malloc(sizeof(nt_function_declaration));
    ret->self = FUNCTION_DECLARATION;
    ret->id = id;
    ret->return_type = type;
    ret->params = param_list->params;

    finish_function_declaration(id, type);

    return ret;
}

/*****************************************************************************************************************************************
*   function_parameter_list
*/
nt_function_parameter_list *ntf_function_parameter_list_1(nt_function_parameter_list *rest, nt_function_parameter *next)
{
    add_to_ll(rest->params, next);
    add_parameter_to_function(next->id, next->type);
    return rest;
}
nt_function_parameter_list *ntf_function_parameter_list_2(nt_function_parameter *next)
{
    nt_function_parameter_list *ret = malloc(sizeof(nt_function_parameter_list));
    ret->self = FUNCTION_PARAMETER_LIST;
    ret->params = new_ll();
    add_to_ll(ret->params, next);
    add_parameter_to_function(next->id, next->type);
    return ret;
}

/*****************************************************************************************************************************************
*   function_parameter
*/
nt_function_parameter *ntf_function_parameter_1(type type, nt_identifier_declaration *identifier)
{
    nt_function_parameter *ret = malloc(sizeof(nt_function_parameter));
    ret->self = FUNCTION_PARAMETER;
    ret->type = type;
    if (ret->type == INT_ && identifier->arrayAize > 0)
        ret->type = INTARRAY;
    ret->id = identifier->id;
    ret->arraySize = identifier->arrayAize;
    return ret;
}

/*****************************************************************************************************************************************
*   stmt_list
*/
nt_stmt_list *ntf_stmt_list_1()
{
    nt_stmt_list *ret = malloc(sizeof(nt_stmt_list));
    ret->self = STMT_LIST;
    ret->statements = new_ll();
    return ret;
}
nt_stmt_list *ntf_stmt_list_2(nt_stmt_list *rest, nt_stmt *stmt)
{
    add_to_ll(rest->statements, stmt);
    if (stmt->represents == VARIABLE_DECLARATION)
    {
        nt_variable_declaration *var_dec = stmt->data;
        list_element *current = var_dec->identifiers->first;
        while (current)
        {
            nt_identifier_declaration *ident = current->data;
            type type = var_dec->type;
            if (type == INT_ && ident->arrayAize > 0)
                type = INTARRAY;
            add_local_var_to_function(ident->id, type);
            current = current->next;
        }
    }
    return rest;
}

/*****************************************************************************************************************************************
*   stmt
*/
nt_stmt *ntf_stmt_1(nt_stmt_block *block)
{
    nt_stmt *ret = malloc(sizeof(nt_stmt));
    ret->self = STMT;
    ret->represents = STMT_BLOCK;
    ret->data = block;
    return ret;
}
nt_stmt *ntf_stmt_2(nt_variable_declaration *var_dec)
{
    nt_stmt *ret = malloc(sizeof(nt_stmt));
    ret->self = STMT;
    ret->represents = VARIABLE_DECLARATION;
    ret->data = var_dec;
    return ret;
}
nt_stmt *ntf_stmt_3(nt_expression *expression)
{
    nt_stmt *ret = malloc(sizeof(nt_stmt));
    ret->self = STMT;
    ret->represents = EXPRESSION;
    ret->data = expression;
    return ret;
}
nt_stmt *ntf_stmt_4(nt_stmt_conditional *stmt)
{
    nt_stmt *ret = malloc(sizeof(nt_stmt));
    ret->self = STMT;
    ret->represents = STMT_CONDITIONAL;
    ret->data = stmt;
    return ret;
}
nt_stmt *ntf_stmt_5(nt_stmt_loop *stmt)
{
    nt_stmt *ret = malloc(sizeof(nt_stmt));
    ret->self = STMT;
    ret->represents = STMT_LOOP;
    ret->data = stmt;
    return ret;
}
nt_stmt *ntf_stmt_6(nt_expression *return_expression)
{
    nt_stmt *ret = malloc(sizeof(nt_stmt));
    ret->self = STMT;
    ret->represents = STMT_RETURN_EXPRESSION;
    ret->data = return_expression;
    add_return_statement_to_function(return_expression->type);
    return ret;
}
nt_stmt *ntf_stmt_7()
{
    nt_stmt *ret = malloc(sizeof(nt_stmt));
    ret->self = STMT;
    ret->represents = STMT_RETURN;
    ret->data = 0;
    add_return_statement_to_function(VOID_);
    return ret;
}
nt_stmt *ntf_stmt_8()
{
    nt_stmt *ret = malloc(sizeof(nt_stmt));
    ret->self = STMT;
    ret->represents = STMT_EMPTY;
    ret->data = 0;
    return ret;
}

/*****************************************************************************************************************************************
*   stmt_block
*/
nt_stmt_block *ntf_stmt_block(nt_stmt_list *stmts)
{
    nt_stmt_block *ret = malloc(sizeof(nt_stmt_block));
    ret->self = STMT_BLOCK;
    ret->stmts = stmts;
    return ret;
}

/*****************************************************************************************************************************************
*   stmt_conditional
*/
nt_stmt_conditional *ntf_stmt_conditional_1(nt_expression *expression, nt_stmt *stmt)
{
    if (expression->type != INT_)
    {
        yyerror("Conditional condition must be of type INT!");
        exit(1);
    }
    nt_stmt_conditional *ret = malloc(sizeof(nt_stmt_conditional));
    ret->self = STMT_CONDITIONAL;
    ret->condition = expression;
    ret->body = stmt;
    ret->body_else = 0;
    return ret;
}
nt_stmt_conditional *ntf_stmt_conditional_2(nt_expression *expression, nt_stmt *stmt, nt_stmt *stmt_else)
{
    if (expression->type != INT_)
    {
        yyerror("Conditional condition must be of type INT!");
        exit(1);
    }
    nt_stmt_conditional *ret = malloc(sizeof(nt_stmt_conditional));
    ret->self = STMT_CONDITIONAL;
    ret->condition = expression;
    ret->body = stmt;
    ret->body_else = stmt_else;
    return ret;
}

/*****************************************************************************************************************************************
*   stmt_loop
*/
nt_stmt_loop *ntf_stmt_loop_1(nt_expression *expression, nt_stmt *stmt)
{
    if (expression->type != INT_)
    {
        yyerror("Loop condition must be of type INT!");
        exit(1);
    }
    nt_stmt_loop *ret = malloc(sizeof(nt_stmt_loop));
    ret->self = STMT_LOOP;
    ret->condition = expression;
    ret->body = stmt;
    ret->loop_type = WHILE_LOOP;
    return ret;
}
nt_stmt_loop *ntf_stmt_loop_2(nt_stmt *stmt, nt_expression *expression)
{
    if (expression->type != INT_)
    {
        yyerror("Loop condition must be of type INT!");
        exit(1);
    }
    nt_stmt_loop *ret = malloc(sizeof(nt_stmt_loop));
    ret->self = STMT_LOOP;
    ret->condition = expression;
    ret->body = stmt;
    ret->loop_type = DO_WHILE_LOOP;
    return ret;
}

/*****************************************************************************************************************************************
*   expression
*/
nt_expression *ntf_expression(nt_expression *expression_left, OPERATOR op, nt_expression *expression_right)
{
    //printf("ntf_expression()\n");
    if (expression_left->type != INT_)
    {
        yyerror("Operator requires argument of type INT!");
        exit(1);
    }
    if (expression_right->type != INT_)
    {
        yyerror("Operator requires argument of type INT!");
        exit(1);
    }
    nt_expression *ret = malloc(sizeof(nt_expression));
    ret->self = EXPRESSION;
    ret->operator= op;
    ret->a = expression_left;
    ret->b = expression_right;
    ret->type = INT_;
    return ret;
}
nt_expression *ntf_expression_assignment(nt_expression *expression_left, nt_expression *expression_right)
{
    //printf("ntf_expression_assignment()\n");
    if (expression_left->operator== _ARRAY_ACCESS)
    {
        //all good
    }
    else if (expression_left->operator== _PRIMARY)
    {
        nt_primary *primary = expression_left->a;
        if (primary->identifier == 0)
        {
            yyerror("Can't assign to numeric constant!");
            exit(1);
        }
        else
        {
            variable_entry *var = get_variable_entry(primary->identifier);
            if (var->type != expression_right->type)
            {
                yyerror("Assignment to wrong type!");
                exit(1);
            }
        }
    }
    else
    {
        yyerror("Assginment to non-assignable expression!");
        exit(1);
    }
    if (expression_right->type != INT_)
    {
        yyerror("Assigned value must be of type INT!");
        exit(1);
    }
    nt_expression *ret = malloc(sizeof(nt_expression));
    ret->self = EXPRESSION;
    ret->operator= _ASSIGN;
    ret->a = expression_left;
    ret->b = expression_right;
    ret->type = INT_;
    return ret;
}
nt_expression *ntf_expression_negation(nt_expression *expression)
{
    //printf("ntf_expression_negation()\n");
    if (expression->type != INT_)
    {
        yyerror("Operator requires argument of type INT!");
        exit(1);
    }
    nt_expression *ret = malloc(sizeof(nt_expression));
    ret->self = EXPRESSION;
    ret->operator= _LOGICAL_NOT;
    ret->a = expression;
    ret->b = 0;
    ret->type = INT_;
    return ret;
}
nt_expression *ntf_expression_unary_minus(nt_expression *expression)
{
    //printf("ntf_expression_unary_minus()\n");
    if (expression->type != INT_)
    {
        yyerror("Operator requires argument of type INT!");
        exit(1);
    }
    nt_expression *ret = malloc(sizeof(nt_expression));
    ret->operator= _UNARY_MINUS;
    ret->a = expression;
    ret->b = 0;
    ret->self = EXPRESSION;
    ret->type = INT_;
    return ret;
}
nt_expression *ntf_expression_unary_plus(nt_expression *expression)
{
    //printf("ntf_expression_unary_plus()\n");
    if (expression->type != INT_)
    {
        yyerror("Operator requires argument of type INT!");
        exit(1);
    }
    nt_expression *ret = malloc(sizeof(nt_expression));
    ret->self = EXPRESSION;
    ret->operator= _UNARY_PLUS;
    ret->a = expression;
    ret->b = 0;
    ret->type = INT_;
    return ret;
}
nt_expression *ntf_expression_array(char *id, nt_primary *primary)
{
    //printf("ntf_expression_array()\n");
    variable_entry *st_entry = get_variable_entry(id);
    if (st_entry == 0)
    {
        yyerror("Array not declared!");
        printf("Array: %s\n", id);
        exit(1);
    }
    if (st_entry->type != INTARRAY)
    {
        yyerror("Identifier is not of type ARRAY!");
        exit(1);
    }
    nt_expression *ret = malloc(sizeof(nt_expression));
    ret->self = EXPRESSION;
    ret->operator= _ARRAY_ACCESS;
    ret->a = id;
    ret->b = primary;
    ret->type = INT_;
    return ret;
}
nt_expression *ntf_expression_parenthesis(nt_expression *expression)
{
    //printf("ntf_expression_parenthesis()\n");
    nt_expression *ret = malloc(sizeof(nt_expression));
    ret->self = EXPRESSION;
    ret->operator= _PARENTHESIS;
    ret->a = expression;
    ret->b = 0;
    ret->type = expression->type;
    return ret;
}
nt_expression *ntf_expression_function_call(nt_function_call *fun)
{
    //printf("ntf_expression_function_call()\n");
    nt_expression *ret = malloc(sizeof(nt_expression));
    ret->self = EXPRESSION;
    ret->operator= _FUNCTION_CALL;
    ret->a = fun;
    ret->b = 0;
    ret->type = fun->type;
    return ret;
}
nt_expression *ntf_expression_primary(nt_primary *primary)
{
    //printf("ntf_expression_primary()\n");
    type type_tmp = INT_;
    int arraylength_tmp = 0;
    if (primary->identifier)
    {
        variable_entry *st_entry = get_variable_entry(primary->identifier);
        if (st_entry == 0)
        {
            yyerror("Variable not declared!");
            printf("Variable: %s\n", primary->identifier);
            exit(1);
        }
        if (st_entry->type == VOID_)
        {
            yyerror("Identifier is of type VOID!");
            exit(1);
        }
        type_tmp = st_entry->type;
        arraylength_tmp = st_entry->type_arraylength;
    }
    nt_expression *ret = malloc(sizeof(nt_expression));
    ret->self = EXPRESSION;
    ret->operator= _PRIMARY;
    ret->a = primary;
    ret->type = type_tmp;
    ret->arraylength = arraylength_tmp;
    return ret;
}

/*****************************************************************************************************************************************
*   primary
*/
nt_primary *ntf_primary_1(int value)
{
    //printf("ntf_primary_1()\n");
    nt_primary *ret = malloc(sizeof(nt_primary));
    ret->self = PRIMARY;
    ret->identifier = 0;
    ret->numeric_value = value;
    return ret;
}
nt_primary *ntf_primary_2(char *id)
{
    //printf("ntf_primary_2()\n");
    nt_primary *ret = malloc(sizeof(nt_primary));
    ret->self = PRIMARY;
    ret->identifier = id;
    ret->numeric_value = 0;
    return ret;
}

/*****************************************************************************************************************************************
*   function_call
*/
nt_function_call *ntf_function_call_1(char *id)
{
    //printf("ntf_function_call_1()\n");
    function_entry *st_entry = get_function_entry(id);
    if (st_entry == 0)
    {
        yyerror("Function not declared!");
        printf("Function: %s\n", id);
        exit(1);
    }
    if (st_entry->functionParameters->first)
    {
        yyerror("Parameters required!");
        printf("Function: %s\n", id);
        exit(1);
    }
    nt_function_call *ret = malloc(sizeof(nt_function_call));
    ret->self = FUNCTION_CALL;
    ret->func_id = id;
    ret->params = 0;
    ret->type = st_entry->type;
    return ret;
}
nt_function_call *ntf_function_call_2(char *id, nt_function_call_parameters *params)
{
    //printf("ntf_function_call_2()\n");
    function_entry *st_entry = get_function_entry(id);
    if (st_entry == 0)
    {
        yyerror("Function not declared!");
        printf("Function: %s\n", id);
        exit(1);
    }
    list_element *current_l = params->expressions->first;
    list_element *current_st = st_entry->functionParameters->first;
    int i = 1;
    while (current_l && current_st)
    {
        nt_expression *param_l = current_l->data;
        variable_entry *param_st = current_st->data;
        if (param_l->type != param_st->type)
        {
            yyerror("Function parameter mismatch!");
            printf("Parameter %d: Expected %d, received %d\n", i, param_st->type, param_l->type);
            exit(1);
        }
        current_l = current_l->next;
        current_st = current_st->next;
        i++;
    }
    if (current_l || current_st)
    {
        yyerror("Wrong function parameter count!");
        exit(1);
    }
    nt_function_call *ret = malloc(sizeof(nt_function_call));
    ret->self = FUNCTION_CALL;
    ret->func_id = id;
    ret->params = params;
    ret->type = st_entry->type;
    return ret;
}

/*****************************************************************************************************************************************
*   function_call_parameters
*/
nt_function_call_parameters *ntf_function_call_parameters_1(nt_function_call_parameters *rest, nt_expression *next)
{
    //printf("ntf_function_call_parameters_1()\n");
    add_to_ll(rest->expressions, next);
    return rest;
}
nt_function_call_parameters *ntf_function_call_parameters_2(nt_expression *next)
{
    //printf("ntf_function_call_parameters_2()\n");
    nt_function_call_parameters *ret = malloc(sizeof(nt_function_call_parameters));
    ret->self = FUNCTION_CALL_PARAMETERS;
    ret->expressions = new_ll();
    add_to_ll(ret->expressions, next);
    return ret;
}

/*****************************************************************************************************************************************
*   Zwischencodeerzeugung
*/

linked_list *out_inter_code(nt_program *nt)
{
    linked_list *ic_ll = new_ll();
    generate_variable_declarations(nt->variable_declarations, ic_ll);
    generate_function_declarations(nt->function_declarations, ic_ll);
    generate_function_definitions(nt->function_definitions, ic_ll);
    return ic_ll;
}

void generate_variable_declarations(linked_list *variable_declarations, linked_list *ic_ll)
{
    list_element *current_declaration_list_element = variable_declarations->first;
    while (current_declaration_list_element)
    {
        nt_variable_declaration *current_declaration = (nt_variable_declaration *)(current_declaration_list_element->data);
        list_element *current_identifier_list_element = current_declaration->identifiers->first;
        while (current_identifier_list_element)
        {
            nt_identifier_declaration *identifier = (nt_identifier_declaration *)current_identifier_list_element->data;
            add_to_ll(ic_ll, ntf_variable_declaration_2(current_declaration->type, identifier));
            current_identifier_list_element = current_identifier_list_element->next;
        }
        current_declaration_list_element = current_declaration_list_element->next;
    }
}

void generate_function_declarations(linked_list *function_declarations, linked_list *ic_ll)
{
    list_element *current_declaration_list_element = function_declarations->first;
    while (current_declaration_list_element)
    {
        nt_function_declaration *current_declaration = (nt_function_declaration *)(current_declaration_list_element->data);
        add_to_ll(ic_ll, current_declaration);
        current_declaration_list_element = current_declaration_list_element->next;
    }
}

void generate_function_definitions(linked_list *function_definitions, linked_list *ic_ll)
{
    list_element *current_definition_list_element = function_definitions->first;
    while (current_definition_list_element)
    {
        nt_function_definition *current_definition = (nt_function_definition *)(current_definition_list_element->data);
        add_to_ll(ic_ll, current_definition);
        current_definition_list_element = current_definition_list_element->next;
    }
}

void print_inter_code(linked_list *ic_ll)
{
    list_element *current = ic_ll->first;
    while (current)
    {
        print_nt(0, current->data);
        printf("\n");
        current = current->next;
    }
}
