#include <stdio.h>
#include <stdlib.h>

#include "programTreePrinter.h"


#define debuggo( ENUM ) { if(nt->self != ENUM) printf("Expected %d, got %d\n", ENUM, nt->self );}

void print_nt(int offset, void * nt)
{
    NT_TYPE type = *((NT_TYPE *)nt);
    switch(type)
    {
        case PROGRAM:                   print_nt_program(offset, nt); break;
        case PROGRAM_ELEMENT_LIST:      print_nt_program_element_list(offset, nt); break;
        case PROGRAM_ELEMENT:           print_nt_program_element(offset, nt); break;
        case VARIABLE_DECLARATION:      print_nt_variable_declaration(offset, nt); break;
        case IDENTIFIER_DECLARATION:    print_nt_identifier_declaration(offset, nt); break;
        case FUNCTION_DEFINITION:       print_nt_function_definition(offset, nt); break;
        case FUNCTION_DECLARATION:      print_nt_function_declaration(offset, nt); break;
        case FUNCTION_PARAMETER_LIST:   print_nt_function_parameter_list(offset, nt); break;
        case FUNCTION_PARAMETER:        print_nt_function_parameter(offset, nt); break;
        case STMT_LIST:                 print_nt_stmt_list(offset, nt); break;
        case STMT:                      print_nt_stmt(offset, nt); break;
        case EXPRESSION:                print_nt_expression(offset, nt); break;
        case STMT_CONDITIONAL:          print_nt_stmt_conditional(offset, nt); break;
        case STMT_LOOP:                 print_nt_stmt_loop(offset, nt); break;
        case PRIMARY:                   print_nt_primary(offset, nt); break;
        case FUNCTION_CALL_PARAMETERS:  print_nt_function_call_parameters(offset, nt); break;
        case FUNCTION_CALL:             print_nt_function_call(offset, nt); break;
        default: break;
    }
}

void print_whitespace(int n)
{
    for(int i = 0; i<n; i++)
    {
        printf(" ");
    }
}


void print_nt_primary(int o, nt_primary * nt)
{
    debuggo(PRIMARY);
    if(nt->identifier)
    {
        printf("%s", nt->identifier);
    } else {
        printf("%d", nt->numeric_value);
    }
}


void print_nt_identifier_declaration(int o, nt_identifier_declaration * nt)
{
    debuggo(IDENTIFIER_DECLARATION);
    printf("%s", nt->id);
    if(nt->arrayAize)
    {
        printf("[%d]", nt->arrayAize);
    }
}


void print_nt_variable_declaration(int o, nt_variable_declaration * nt)
{
    debuggo(VARIABLE_DECLARATION);
    switch(nt->type)
    {
        case VOID_: printf("void "); break;
        case INT_: printf("int "); break;
        case INTARRAY: printf("int "); break;
        default: break;
    }
    list_element * current = nt->identifiers->first;
    while(current)
    {
        print_nt_identifier_declaration(o, current->data);
        current=current->next;
        if(current) printf(", ");
    }
}

void print_nt_function_call_parameters(int o, nt_function_call_parameters * nt)
{
    debuggo(FUNCTION_CALL_PARAMETERS);
    list_element * current = nt->expressions->first;
    while(current)
    {
        print_nt_expression(o, current->data);
        current=current->next;
        if(current) printf(", ");
    }
}

void print_nt_function_call(int o, nt_function_call * nt)
{
    debuggo(FUNCTION_CALL);
    printf("%s(", nt->func_id);
    if(nt->params)print_nt_function_call_parameters(o, nt->params);
    printf(")");
}

void print_nt_expression(int o, nt_expression * nt)
{
    debuggo(EXPRESSION);
    switch(nt->operator)
    {
        case _UNKNOWN_OPERATOR: break;
        case _ASSIGN:           print_nt_expression(o, nt->a); printf("=" ); print_nt_expression(o, nt->b); break;
        case _LOGICAL_OR:       print_nt_expression(o, nt->a); printf("||"); print_nt_expression(o, nt->b); break;
        case _LOGICAL_AND:      print_nt_expression(o, nt->a); printf("&&"); print_nt_expression(o, nt->b); break;
        case _LOGICAL_NOT:      printf("!"); print_nt_expression(o, nt->a); break;
        case  _EQ:              print_nt_expression(o, nt->a); printf("=="); print_nt_expression(o, nt->b); break;
        case _NE:               print_nt_expression(o, nt->a); printf("!="); print_nt_expression(o, nt->b); break;
        case _LS:               print_nt_expression(o, nt->a); printf("<" ); print_nt_expression(o, nt->b); break;
        case _LSEQ:             print_nt_expression(o, nt->a); printf("<="); print_nt_expression(o, nt->b); break;
        case _GT:               print_nt_expression(o, nt->a); printf(">" ); print_nt_expression(o, nt->b); break;
        case _GTEQ:             print_nt_expression(o, nt->a); printf(">="); print_nt_expression(o, nt->b); break;
        case _PLUS:             print_nt_expression(o, nt->a); printf("+" ); print_nt_expression(o, nt->b); break;
        case _MINUS:            print_nt_expression(o, nt->a); printf("-" ); print_nt_expression(o, nt->b); break;
        case _UNARY_PLUS:       printf("+"); print_nt_expression(o, nt->a); break;
        case _UNARY_MINUS:      printf("-"); print_nt_expression(o, nt->a); break;      
        case _SHIFT_LEFT:       print_nt_expression(o, nt->a); printf("<<"); print_nt_expression(o, nt->b); break;
        case _SHIFT_RIGHT:      print_nt_expression(o, nt->a); printf(">>"); print_nt_expression(o, nt->b); break;
        case _MUL:              print_nt_expression(o, nt->a); printf("*" ); print_nt_expression(o, nt->b); break;
        case _DIV:              print_nt_expression(o, nt->a); printf("/" ); print_nt_expression(o, nt->b); break;
        case _ARRAY_ACCESS:     printf("%s", (char*) nt->a); printf("[ "); print_nt_primary(o, nt->b); printf(" ]"); break;
        case _FUNCTION_CALL:    print_nt_function_call(o, nt->a); break;
        case _PRIMARY:          print_nt_primary(0, nt->a); break;
        case _PARENTHESIS:      printf("( "); print_nt_expression(o, nt->a); printf(" )"); break;  
        default: break;
    }
}

void print_nt_stmt (int o, nt_stmt * nt)
{
    debuggo(STMT);
    print_whitespace(o);
    switch(nt->represents)
    {
        case STMT_BLOCK:                print_nt_stmt_block(o, nt->data); break;
        case VARIABLE_DECLARATION:      print_nt_variable_declaration(o, nt->data); printf(";\n"); break;
        case EXPRESSION:                print_nt_expression(o, nt->data); printf(";\n"); break;
        case STMT_CONDITIONAL:          print_nt_stmt_conditional(o, nt->data); printf("\n"); break;
        case STMT_LOOP:                 print_nt_stmt_loop(o, nt->data); printf("\n"); break;
        case STMT_RETURN_EXPRESSION:    printf("return "); print_nt_expression(o, nt->data); printf(";\n"); break;
        case STMT_RETURN:               printf("return;\n");break;
        case STMT_EMPTY:                printf(";\n"); break;
        case STMT_GOTO:                 print_nt_stmt_goto(o, nt->data); printf("\n"); break;
        case STMT_LABEL:                print_nt_stmt_label(o, nt->data); printf("\n"); break;
        default:    printf("%d not found", nt->represents); break;
    }
}

void print_nt_stmt_block (int o, nt_stmt * nt)
{
    debuggo(STMT_BLOCK);
    printf("{\n");
    print_nt_stmt_list(o+4, nt->data);
    print_whitespace(o);
    printf("}\n");
}

void print_nt_stmt_list (int o, nt_stmt_list * nt)
{
    debuggo(STMT_LIST);
    list_element * current = nt->statements->first;
    while(current)
    {
        print_nt_stmt(o, current->data);
        current=current->next;
    }
}

void print_nt_stmt_conditional(int o, nt_stmt_conditional * nt)
{
    debuggo(STMT_CONDITIONAL);
    printf("if( ");
    print_nt_expression(o, nt->condition);
    printf(" )\n");
    print_nt_stmt(o+4, nt->body);
    if(nt->body_else)
    {
        printf(" else \n");
        print_nt_stmt(o, nt->body_else);
    }
}


void print_nt_stmt_loop(int o, nt_stmt_loop * nt)
{
    debuggo(STMT_LOOP);
    if(nt->loop_type == WHILE_LOOP)
    {
        printf("while( ");
        print_nt_expression(o, nt->condition);
        printf(" )\n");
        print_nt_stmt(o, nt->body);
    }
    if(nt->loop_type == DO_WHILE_LOOP)
    {
        printf("do\n");
        print_nt_stmt(o, nt->body);
        printf("while( ");
        print_nt_expression(o, nt->condition);
        printf(" )");
    }
}

void print_nt_stmt_goto (int o, nt_stmt_goto * nt)
{
    debuggo(STMT_GOTO);
    printf("if( ");
    print_nt_expression(o, nt->condition);
    printf(" ) { GOTO %s; }", nt->label);
}

void print_nt_stmt_label (int o, nt_stmt_label* nt)
{
    debuggo(STMT_LABEL);
    printf("%s:", nt->id);
}

void print_nt_function_parameter(int o, nt_function_parameter * nt)
{
    debuggo(FUNCTION_PARAMETER);
    switch(nt->type)
    {
        case VOID_: printf("void "); break;
        case INT_: printf("int "); break;
        case INTARRAY: printf("int "); break;
        default: break;
    }
    printf("%s", nt->id);
    if(nt->arraySize)
    {
        printf("[%d]", nt->arraySize);
    }
}


void print_nt_function_parameter_list(int o, nt_function_parameter_list * nt)
{
    debuggo(FUNCTION_PARAMETER_LIST);
    list_element * current = nt->params->first;
    while(current)
    {
        print_nt_stmt(o, current->data);
        current=current->next;
        if(current) printf(", ");
    }
}


void print_nt_function_declaration (int o, nt_function_declaration * nt)
{
    debuggo(FUNCTION_DECLARATION);
    switch(nt->return_type)
    {
        case VOID_: printf("void "); break;
        case INT_: printf("int "); break;
        case INTARRAY: printf("int "); break;
        default: break;
    }
    if(nt->params->first)
    {
        printf("%s( ", nt->id);
        list_element * current = nt->params->first;
        while(current)
        {
            print_nt_function_parameter(o, current->data);
            current=current->next;
            if(current) printf(", ");
        }
        printf(" )");
    } else printf("%s()", nt->id);
}


void print_nt_function_definition(int o, nt_function_definition * nt)
{
    debuggo(FUNCTION_DEFINITION);
    switch(nt->return_type)
    {
        case VOID_: printf("void "); break;
        case INT_: printf("int "); break;
        case INTARRAY: printf("int "); break;
        default: break;
    }
    if(nt->params->first)
    {
        printf("%s( ", nt->id);
        list_element * current = nt->params->first;
        while(current)
        {
            print_nt_function_parameter(o, current->data);
            current=current->next;
            if(current) printf(", ");
        }
        printf(" )");
    } else printf("%s()", nt->id);
    printf("\n{\n");
    print_nt_stmt_list(o+4, nt->stmt_list);
    printf("}\n\n");
}


void print_nt_program_element(int o, nt_program_element * nt)
{
    debuggo(PROGRAM_ELEMENT);
    switch(nt->represents)
    {
        case VARIABLE_DECLARATION: print_nt_variable_declaration(o, nt->content); break;
        case FUNCTION_DECLARATION: print_nt_function_declaration(o, nt->content); break;
        case FUNCTION_DEFINITION: print_nt_function_definition(o, nt->content); break;
        default: break;
    }
}


void print_nt_program_element_list(int o, nt_program_element_list * nt)
{
    debuggo(PROGRAM_ELEMENT_LIST);
    list_element * current = nt->variable_declarations->first;
    while(current)
    {
        print_nt_variable_declaration(o, current->data); printf(";\n");
        current=current->next;
    }
    printf("\n");
    current = nt->function_declarations->first;
    while(current)
    {
        print_nt_function_declaration(o, current->data); printf(";\n");
        current=current->next;
    }
    printf("\n");
    current = nt->function_definitions->first;
    while(current)
    {
        print_nt_function_definition(o, current->data);
        current=current->next;
    }
}


void print_nt_program(int o, nt_program * nt)
{
    debuggo(PROGRAM);
    printf("//Automatically generated code\n");
    printf("//Global variables:\n");
    list_element * current = nt->variable_declarations->first;
    while(current)
    {
        print_nt_variable_declaration(o, current->data); printf(";\n");
        current=current->next;
    }
    printf("\n");
    printf("//Function declarations:\n");
    current = nt->function_declarations->first;
    while(current)
    {
        print_nt_function_declaration(o, current->data); printf(";\n");
        current=current->next;
    }
    printf("\n");
    printf("//Function definitions:\n");
    current = nt->function_definitions->first;
    while(current)
    {
        print_nt_function_definition(o, current->data);
        current=current->next;
    }
}
