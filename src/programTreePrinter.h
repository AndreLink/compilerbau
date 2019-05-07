#ifndef programTreePrinter
#define programTreePrinter

#include "programTree.h"
#include "linkedlist.h"

void print_nt(int offset, void * nt);


void print_nt_program(int o, nt_program * nt);
void print_nt_program_element_list(int o, nt_program_element_list * nt);
void print_nt_program_element(int o, nt_program_element * nt);
void print_nt_variable_declaration(int o, nt_variable_declaration * nt);
void print_nt_identifier_declaration(int o, nt_identifier_declaration * nt);
void print_nt_function_definition(int o, nt_function_definition * nt);
void print_nt_function_declaration (int o, nt_function_declaration * nt);
void print_nt_function_parameter_list(int o, nt_function_parameter_list * nt);
void print_nt_function_parameter(int o, nt_function_parameter * nt);
void print_nt_stmt_list (int o, nt_stmt_list * nt);
void print_nt_stmt (int o, nt_stmt * nt);
void print_nt_stmt_block (int o, nt_stmt * nt);
void print_nt_stmt_goto (int o, nt_stmt_goto * nt);
void print_nt_stmt_label (int o, nt_stmt_label* nt);
void print_nt_expression(int o, nt_expression * nt);
void print_nt_stmt_conditional(int o, nt_stmt_conditional * nt);
void print_nt_stmt_loop(int o, nt_stmt_loop * nt);
void print_nt_primary(int o, nt_primary * nt);
void print_nt_function_call_parameters(int o, nt_function_call_parameters * nt);
void print_nt_function_call(int o, nt_function_call * nt);





#endif