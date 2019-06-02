#ifndef programTree
#define programTree

#include "symbol_table.h"
#include "linkedlist.h"

#define print_st 0
#define print_c 0
#define print_i 1

typedef enum NT_TYPE
{
    UNKNOWN_NT_TYPE = 0,
    PROGRAM,
    PROGRAM_ELEMENT_LIST,
    PROGRAM_ELEMENT,
    TYPE,
    VARIABLE_DECLARATION,
    IDENTIFIER_DECLARATION,
    FUNCTION_DEFINITION,
    FUNCTION_DECLARATION,
    FUNCTION_PARAMETER_LIST,
    FUNCTION_PARAMETER,
    STMT_LIST,
    STMT,
    STMT_BLOCK,
    STMT_CONDITIONAL,
    STMT_LOOP,
    STMT_EMPTY,
    STMT_RETURN,
    STMT_RETURN_EXPRESSION,
    STMT_GOTO,
    STMT_LABEL,
    EXPRESSION,
    PRIMARY,
    FUNCTION_CALL,
    FUNCTION_CALL_PARAMETERS,
} NT_TYPE;

typedef enum OPERATOR
{                          //pseudocode    If nothing else is stated, assume that a and b are of type nt_expression*
    _UNKNOWN_OPERATOR = 0, //fail          a, b unused
    _ASSIGN,               //a = b
    _LOGICAL_OR,           //a || b
    _LOGICAL_AND,          //a && b
    _LOGICAL_NOT,          //!a            b unused
    _EQ,                   //a == b
    _NE,                   //a != b
    _LS,                   //a < b
    _LSEQ,                 //a <= b
    _GT,                   //a > b
    _GTEQ,                 //a >= b
    _PLUS,                 //a + b
    _MINUS,                //a - b
    _UNARY_PLUS,           //a             b unused
    _UNARY_MINUS,          //-a            b unused
    _SHIFT_LEFT,           //a << b
    _SHIFT_RIGHT,          //a >> b
    _MUL,                  //a * b
    _DIV,                  //a / b
    _ARRAY_ACCESS,         //a[b]          a is of type char*, b is of type nt_primary*
    _FUNCTION_CALL,        // a()          a is of type nt_function_call*, b is unused
    _PRIMARY,              // a            a is of type nt_primary*, b is unused
    _PARENTHESIS,          //(a)           b is unused
} OPERATOR;

typedef enum LOOPTYPE
{
    WHILE_LOOP,
    DO_WHILE_LOOP,
} LOOPTYPE;

typedef struct nt_program
{
    NT_TYPE self;
    linked_list *variable_declarations;
    linked_list *function_declarations;
    linked_list *function_definitions;
    struct nt_function_definition *entry_point;
} nt_program;

typedef struct nt_program_element_list
{
    NT_TYPE self;
    linked_list *variable_declarations;
    linked_list *function_declarations;
    linked_list *function_definitions;
} nt_program_element_list;

typedef struct nt_program_element
{
    NT_TYPE self;
    NT_TYPE represents;
    void *content;
} nt_program_element;

typedef struct nt_variable_declaration
{
    NT_TYPE self;
    type type;
    linked_list *identifiers;
} nt_variable_declaration;

typedef struct nt_identifier_declaration
{
    NT_TYPE self;
    char *id;
    int arrayAize; //0 for non-array types
} nt_identifier_declaration;

typedef struct nt_function_definition
{
    NT_TYPE self;
    char *id;
    type return_type;
    linked_list *params;
    linked_list *statements;
    struct nt_stmt_list *stmt_list;
} nt_function_definition;

typedef struct nt_function_declaration
{
    NT_TYPE self;
    char *id;
    type return_type;
    linked_list *params;
} nt_function_declaration;

typedef struct nt_function_parameter_list
{
    NT_TYPE self;
    linked_list *params;
} nt_function_parameter_list;

typedef struct nt_function_parameter
{
    NT_TYPE self;
    char *id;
    type type;
    int arraySize;
} nt_function_parameter;

typedef struct nt_stmt_list
{
    NT_TYPE self;
    linked_list *statements;
} nt_stmt_list;

typedef struct nt_stmt
{
    NT_TYPE self;
    NT_TYPE represents;
    void *data;
} nt_stmt;

typedef struct nt_expression
{
    NT_TYPE self;
    OPERATOR operator;
    void *a; //depending on operator, may have different types or not be set at all
    void *b; //depending on operator, may have different types or not be set at all
    type type;
    int arraylength;
} nt_expression;

typedef struct nt_stmt_block
{
    NT_TYPE self;
    nt_stmt_list *stmts;
} nt_stmt_block;

typedef struct nt_stmt_conditional
{
    NT_TYPE self;
    nt_expression *condition;
    nt_stmt *body;
    nt_stmt *body_else;
} nt_stmt_conditional;

typedef struct nt_stmt_loop
{
    NT_TYPE self;
    nt_expression *condition;
    nt_stmt *body;
    LOOPTYPE loop_type;
} nt_stmt_loop;

typedef struct nt_stmt_goto
{
    NT_TYPE self;
    char *label;
    nt_expression *condition;

} nt_stmt_goto;

typedef struct nt_stmt_label
{
    NT_TYPE self;
    char *id;
} nt_stmt_label;

typedef struct nt_primary
{
    NT_TYPE self;
    char *identifier; //If nullptr, the primary is a constant expression with value numeric_value
    int numeric_value;
    type type;
} nt_primary;

typedef struct nt_function_call_parameters
{
    NT_TYPE self;
    linked_list *expressions;
} nt_function_call_parameters;

typedef struct nt_function_call
{
    NT_TYPE self;
    char *func_id;
    nt_function_call_parameters *params;
    type type;
} nt_function_call;

nt_program *ntf_program_1(nt_program_element_list *program_element_list);

nt_program_element_list *ntf_program_element_list_1(nt_program_element_list *rest, nt_program_element *next);
nt_program_element_list *ntf_program_element_list_2(nt_program_element *next);

nt_program_element *ntf_program_element_1(nt_variable_declaration *var);
nt_program_element *ntf_program_element_2(nt_function_declaration *fun);
nt_program_element *ntf_program_element_3(nt_function_definition *fun);

nt_variable_declaration *ntf_variable_declaration_1(nt_variable_declaration *rest, nt_identifier_declaration *next);
nt_variable_declaration *ntf_variable_declaration_2(type type, nt_identifier_declaration *next);

nt_identifier_declaration *ntf_identifier_declaration_1(char *id, int size);
nt_identifier_declaration *ntf_identifier_declaration_2(char *id);

nt_function_definition *ntf_function_definition_1(type type, char *id, nt_stmt_list *stmt_list);
nt_function_definition *ntf_function_definition_2(type type, char *id, nt_function_parameter_list *param_list, nt_stmt_list *stmt_list);

nt_function_declaration *ntf_function_declaration_1(type type, char *id);
nt_function_declaration *ntf_function_declaration_2(type type, char *id, nt_function_parameter_list *param_list);

nt_function_parameter_list *ntf_function_parameter_list_1(nt_function_parameter_list *rest, nt_function_parameter *next);
nt_function_parameter_list *ntf_function_parameter_list_2(nt_function_parameter *next);

nt_function_parameter *ntf_function_parameter_1(type type, nt_identifier_declaration *identifier);

nt_stmt_list *ntf_stmt_list_1();
nt_stmt_list *ntf_stmt_list_2(nt_stmt_list *rest, nt_stmt *stmt);

nt_stmt *ntf_stmt_1(nt_stmt_block *block);
nt_stmt *ntf_stmt_2(nt_variable_declaration *var_dec);
nt_stmt *ntf_stmt_3(nt_expression *expression);
nt_stmt *ntf_stmt_4(nt_stmt_conditional *stmt);
nt_stmt *ntf_stmt_5(nt_stmt_loop *stmt);
nt_stmt *ntf_stmt_6(nt_expression *return_expression);
nt_stmt *ntf_stmt_7();
nt_stmt *ntf_stmt_8();

nt_stmt_block *ntf_stmt_block(nt_stmt_list *ntf_stmt_list_1);

nt_stmt_conditional *ntf_stmt_conditional_1(nt_expression *expression, nt_stmt *stmt);
nt_stmt_conditional *ntf_stmt_conditional_2(nt_expression *expression, nt_stmt *stmt, nt_stmt *stmt_else);

nt_stmt_loop *ntf_stmt_loop_1(nt_expression *expression, nt_stmt *stmt);
nt_stmt_loop *ntf_stmt_loop_2(nt_stmt *stmt, nt_expression *expression);

nt_expression *ntf_expression(nt_expression *expression_left, OPERATOR op, nt_expression *expression_right);
nt_expression *ntf_expression_assignment(nt_expression *expression_left, nt_expression *expression_right);
nt_expression *ntf_expression_negation(nt_expression *expression);
nt_expression *ntf_expression_unary_minus(nt_expression *expression);
nt_expression *ntf_expression_unary_plus(nt_expression *expression);
nt_expression *ntf_expression_array(char *id, nt_primary *primary);
nt_expression *ntf_expression_parenthesis(nt_expression *expression);
nt_expression *ntf_expression_function_call(nt_function_call *fun);
nt_expression *ntf_expression_primary(nt_primary *primary);

nt_primary *ntf_primary_1(int value);
nt_primary *ntf_primary_2(char *id);

nt_function_call *ntf_function_call_1(char *id);
nt_function_call *ntf_function_call_2(char *id, nt_function_call_parameters *params);

nt_function_call_parameters *ntf_function_call_parameters_1(nt_function_call_parameters *rest, nt_expression *next);
nt_function_call_parameters *ntf_function_call_parameters_2(nt_expression *next);

linked_list * out_inter_code(nt_program *nt);
void generate_variable_declarations(linked_list *, linked_list *);
void generate_function_declarations(linked_list *, linked_list *);
void generate_function_definitions(linked_list *, linked_list *);
void print_inter_code(linked_list *ic_ll);
void print_function_declaration(type ret_type, char *id, linked_list *parameter_list);
void print_function_declaration_statement_list(nt_stmt_list *statement_list);
nt_stmt_list *handle_statement_list(nt_stmt_list *stmt_list);
void handle_single_statement(nt_stmt *current_statement, nt_stmt_list *new_statement_list);
void merge_statement_lists(nt_stmt_list *stmt_list_dest, nt_stmt_list *stmt_list_source);
int handle_expression(nt_stmt_list *new_statement_list, nt_expression *expr);
int handle_operators(nt_stmt_list *new_statement_list, OPERATOR op, nt_expression *expr);
nt_expression *primary_helper_expression(int helper_var_index, type t, nt_stmt_list *declarations);
void add_helper_assign_expression_to_list(nt_stmt_list *new_statement_list, nt_expression *primary_helper, nt_expression *right_side, nt_expression *expr);
nt_stmt *create_goto(int goto_label, nt_expression *condition);
nt_stmt *create_label(int label);

#endif
