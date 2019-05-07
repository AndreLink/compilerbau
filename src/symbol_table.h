#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include "linkedlist.h"

typedef enum type {  UNKNOWN_TYPE = 0, INT_, CHAR, BOOLEAN, VOID_, INTARRAY } type;
typedef enum scope {  UNKNOWN_SCOPE = 0, GLOBAL, LOCAL, PARAM } scope;

typedef struct {
    char* id;
    int isFunction;
    int functionDefined;
    scope scope;
    type type;
    int type_arraylength;
    linked_list * functionParameters;
    linked_list * functionVariables;
} symbol_table_entry;

typedef symbol_table_entry variable_entry;
typedef symbol_table_entry function_entry;


int add_global_variable(char * id, type type);
int add_global_variable_a(char * id, type type, int arraylength);

int add_parameter_to_function(char * param_id, type type);
int add_parameter_to_function_a(char * param_id, type type, int arraylength);

int add_local_var_to_function(char * var_id, type type);
int add_local_var_to_function_a(char * var_id, type type, int arraylength);

int add_return_statement_to_function(type type);

int finish_function_declaration(char * func_id,  type return_type);
int finish_function_definition(char * func_id,  type return_type);

function_entry * get_function_entry(char * func_id);
variable_entry * get_variable_entry(char * id);



void print_symbol_table();

#endif
