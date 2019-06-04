#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mipsPrinter.h"

#define debuggo( ENUM ) { if(nt->self != ENUM) printf("Expected %d, got %d\n", ENUM, nt->self );}

int t_depth = 1;
int e_lab = 0;

void print_mips_nt(int offset, void * nt)
{
    NT_TYPE type = *((NT_TYPE *)nt);
    switch(type)
    {
        case PROGRAM:                   print_mips_nt_program(offset, nt); break;
        case PROGRAM_ELEMENT_LIST:      print_mips_nt_program_element_list(offset, nt); break;
        case PROGRAM_ELEMENT:           print_mips_nt_program_element(offset, nt); break;
        case VARIABLE_DECLARATION:      print_mips_nt_variable_declaration(offset, nt); break;
        case IDENTIFIER_DECLARATION:    print_mips_nt_identifier_declaration(offset, nt); break;
        case FUNCTION_DEFINITION:       print_mips_nt_function_definition(offset, nt); break;
        case FUNCTION_DECLARATION:      print_mips_nt_function_declaration(offset, nt); break;
        case FUNCTION_PARAMETER_LIST:   print_mips_nt_function_parameter_list(offset, nt); break;
        case FUNCTION_PARAMETER:        print_mips_nt_function_parameter(offset, nt, -77); break;
        case STMT_LIST:                 print_mips_nt_stmt_list(offset, nt); break;
        case STMT:                      print_mips_nt_stmt(offset, nt); break;
        case EXPRESSION:                print_mips_nt_expression(offset, nt, 0); break;
        case STMT_CONDITIONAL:          print_mips_nt_stmt_conditional(offset, nt); break;
        case STMT_LOOP:                 print_mips_nt_stmt_loop(offset, nt); break;
        case PRIMARY:                   print_mips_nt_primary(offset, nt, 0); break;
        case FUNCTION_CALL_PARAMETERS:  print_mips_nt_function_call_parameters(offset, nt); break;
        case FUNCTION_CALL:             print_mips_nt_function_call(offset, nt); break;
        default: break;
    }
}


void print_mips_nt_primary(int o, nt_primary * nt, int pre_process)
{
    debuggo(PRIMARY);
    if(nt->identifier)
    {
        if (pre_process)
        {
            //printf(".text\n");
            print_whitespace(o); printf("lw\t$t%d, ", pre_process);
            printf("%s", nt->identifier);
            printf("\n");
        }
        else
        {
            printf("%s", nt->identifier);
        }
    } else {
        if (pre_process)
        {
            //printf(".text\n");
            print_whitespace(o); printf("li\t$t%d, ", pre_process);
            printf("%d", nt->numeric_value);
            printf("\n");
        }
        else {
            printf("%d", nt->numeric_value * 4);
        }
    }
}


void print_mips_nt_identifier_declaration(int o, nt_identifier_declaration * nt)
{
    debuggo(IDENTIFIER_DECLARATION);

    printf("%s:\t", nt->id);
    if(nt->arrayAize)
    {
        printf(".space %d", nt->arrayAize * 4);
    }
    else
    {
        printf(".word 0");
    }
    
}


void print_mips_nt_variable_declaration(int o, nt_variable_declaration * nt)
{
    debuggo(VARIABLE_DECLARATION);
    switch(nt->type)
    {
        case VOID_: printf("void "); break;
        case INT_: printf(".data\n"); print_whitespace(o + 4); break;
        case INTARRAY: printf("int "); break;
        default: break;
    }
    list_element * current = nt->identifiers->first;
    while(current)
    {
        print_mips_nt_identifier_declaration(o, current->data);
        current=current->next;
        if(current) printf(", ");
    }
}

void print_mips_nt_function_call_parameters(int o, nt_function_call_parameters * nt)
{
    debuggo(FUNCTION_CALL_PARAMETERS);
    int cnt = 0;
    list_element * current = nt->expressions->first;
    while(current)
    {
        print_whitespace(o);
        printf("lw\t$a%d, ", cnt++);
        print_mips_nt_expression(o, current->data, 0);
        current=current->next;
        //if(current) printf(", ");
        printf("\n");
    }
}

void print_mips_nt_function_call(int o, nt_function_call * nt)
{
    debuggo(FUNCTION_CALL);
    //printf("TEMP!!!%s(", nt->func_id);
    if(nt->params)print_mips_nt_function_call_parameters(o, nt->params);
    //printf(")");
    print_whitespace(o); printf("jal\t%s\n", nt->func_id);
}

void print_mips_nt_expression(int o, nt_expression * nt, int pre_process)
{
    debuggo(EXPRESSION);
    switch(nt->operator)
    {
        case _UNKNOWN_OPERATOR: break;
        case _ASSIGN:           if (pre_process == 0)
                                {
                                    printf(".text\n");
                                    print_mips_nt_expression(o + 4, nt->b, 1);
                                    print_whitespace(o + 4); printf("sw\t$t1, ");
                                    print_mips_nt_expression(o, nt->a, 0);
                                } break;
        case _LOGICAL_OR:       if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o); printf("bne\t$t%d, $zero, __el%d\n", pre_process + 1, e_lab);
                                    print_whitespace(o); printf("bne\t$t%d, $zero, __el%d\n", pre_process + 2, e_lab);
                                    print_whitespace(o); printf("li\t$t%d, 0\n", pre_process);
                                    print_whitespace(o); printf("j\t__el%d\n", e_lab + 1);
                                    print_whitespace(o - 2); printf("__el%d:\n", e_lab);
                                    print_whitespace(o); printf("li\t$t%d, 1\n", pre_process);
                                    print_whitespace(o - 2); printf("__el%d:\n", e_lab + 1);
                                    e_lab = e_lab + 2;
                                } break;
        case _LOGICAL_AND:      if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o); printf("beq\t$t%d, $zero, __el%d\n", pre_process + 1, e_lab);
                                    print_whitespace(o); printf("beq\t$t%d, $zero, __el%d\n", pre_process + 2, e_lab);
                                    print_whitespace(o); printf("li\t$t%d, 1\n", pre_process);
                                    print_whitespace(o); printf("j\t__el%d\n", e_lab + 1);
                                    print_whitespace(o - 2); printf("__el%d:\n", e_lab);
                                    print_whitespace(o); printf("li\t$t%d, 0\n", pre_process);
                                    print_whitespace(o - 2); printf("__el%d:\n", e_lab + 1);
                                    e_lab = e_lab + 2;
                                } break;
        case _LOGICAL_NOT:      if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_whitespace(o); printf("beq\t$t%d, $zero, __el%d\n", pre_process + 1, e_lab);
                                    print_whitespace(o); printf("li\t$t%d, 1\n", pre_process);
                                    print_whitespace(o); printf("j\t__el%d\n", e_lab + 1);
                                    print_whitespace(o - 2); printf("__el%d:\n", e_lab);
                                    print_whitespace(o); printf("li\t$t%d, 0\n", pre_process);
                                    print_whitespace(o - 2); printf("__el%d:\n", e_lab + 1);
                                    e_lab = e_lab + 2;
                                } break;
        case  _EQ:              if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o); printf("beq\t$t%d, $t%d, __el%d\n", pre_process + 1, pre_process + 2, e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 0\n", pre_process);
                                    print_whitespace(o);
                                    printf("j\t__el%d\n", e_lab + 1);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 1\n", pre_process);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab + 1);
                                    e_lab = e_lab + 2;
                                } break;
        case _NE:               if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o); printf("bne\t$t%d, $t%d, __el%d\n", pre_process + 1, pre_process + 2, e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 0\n", pre_process);
                                    print_whitespace(o);
                                    printf("j\t__el%d\n", e_lab + 1);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 1\n", pre_process);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab + 1);
                                    e_lab = e_lab + 2;
                                } break;
        case _LS:               if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o); printf("sub\t$t%d, $t%d, $t%d\n", pre_process + 1, pre_process + 2, pre_process + 1);
                                    print_whitespace(o); printf("bgtz\t$t%d, __el%d\n", pre_process + 1, e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 0\n", pre_process);
                                    print_whitespace(o);
                                    printf("j\t__el%d\n", e_lab + 1);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 1\n", pre_process);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab + 1);
                                    e_lab = e_lab + 2;
                                } break;
        case _LSEQ:             if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o); printf("sub\t$t%d, $t%d, $t%d\n", pre_process + 1, pre_process + 1, pre_process + 2);
                                    print_whitespace(o); printf("blez\t$t%d, __el%d\n", pre_process + 1, e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 0\n", pre_process);
                                    print_whitespace(o);
                                    printf("j\t__el%d\n", e_lab + 1);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 1\n", pre_process);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab + 1);
                                    e_lab = e_lab + 2;
                                } break;
        case _GT:               if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o); printf("sub\t$t%d, $t%d, $t%d\n", pre_process + 1, pre_process + 1, pre_process + 2);
                                    print_whitespace(o); printf("bgtz\t$t%d, __el%d\n", pre_process + 1, e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 0\n", pre_process);
                                    print_whitespace(o);
                                    printf("j\t__el%d\n", e_lab + 1);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 1\n", pre_process);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab + 1);
                                    e_lab = e_lab + 2;
                                } break;
        case _GTEQ:             if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o); printf("sub\t$t%d, $t%d, $t%d\n", pre_process + 1, pre_process + 2, pre_process + 1);
                                    print_whitespace(o); printf("blez\t$t%d, __el%d\n", pre_process + 1, e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 0\n", pre_process);
                                    print_whitespace(o);
                                    printf("j\t__el%d\n", e_lab + 1);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab);
                                    print_whitespace(o);
                                    printf("li\t$t%d, 1\n", pre_process);
                                    print_whitespace(o - 2);
                                    printf("__el%d:\n", e_lab + 1);
                                    e_lab = e_lab + 2;
                                } break;
        case _PLUS:             if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o + 4); printf("add\t$t%d, $t%d, $t%d", pre_process, pre_process + 1, pre_process + 2); printf("\n");
                                } break;
        case _MINUS:            if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o + 4); printf("sub\t$t%d, $t%d, $t%d", pre_process, pre_process + 1, pre_process + 2); printf("\n");
                                } break;
        case _UNARY_PLUS:       if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process);
                                } break;
        case _UNARY_MINUS:      if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_whitespace(o + 4); printf("sub\t$t%d, $zero, $t%d", pre_process, pre_process + 1); printf("\n");
                                } break;
        case _SHIFT_LEFT:       if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o + 4); printf("sllv\t$t%d, $t%d, $t%d", pre_process, pre_process + 1, pre_process + 2); printf("\n");
                                } break;
        case _SHIFT_RIGHT:      if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o + 4); printf("srlv\t$t%d, $t%d, $t%d", pre_process, pre_process + 1, pre_process + 2); printf("\n");
                                } break;
        case _MUL:              if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o + 4); printf("mul\t$t%d, $t%d, $t%d", pre_process, pre_process + 1, pre_process + 2); printf("\n");
                                } break;
        case _DIV:              if (pre_process)
                                {
                                    print_mips_nt_expression(o, nt->a, pre_process + 1);
                                    print_mips_nt_expression(o, nt->b, pre_process + 2);
                                    print_whitespace(o + 4); printf("div\t$t%d, $t%d, $t%d", pre_process, pre_process + 1, pre_process + 2); printf("\n");
                                } break;
        case _ARRAY_ACCESS:     if (pre_process)
                                {
                                    print_whitespace(o);
                                    printf("lw\t$t%d, %s", pre_process, (char*) nt->a);
                                    printf("( ");
                                    print_mips_nt_primary(o, nt->b, 0);
                                    printf(" )\n");
                                }
                                else
                                {
                                    printf("%s", (char*) nt->a);
                                    printf("( ");
                                    print_mips_nt_primary(o, nt->b, 0);
                                    printf(" )\n");
                                } break;
        case _FUNCTION_CALL:    print_mips_nt_function_call(o, nt->a); break;
        case _PRIMARY:          print_mips_nt_primary(o, nt->a, pre_process); break;
        case _PARENTHESIS:      print_mips_nt_expression(o, nt->a, pre_process); break;  
        default: break;
    }
}

void print_mips_nt_stmt (int o, nt_stmt * nt)
{
    debuggo(STMT);
    print_whitespace(o);
    switch(nt->represents)
    {
        case STMT_BLOCK:                print_mips_nt_stmt_block(o, nt->data); break;
        case VARIABLE_DECLARATION:      print_mips_nt_variable_declaration(o, nt->data); printf("\n"); break;
        case EXPRESSION:                print_mips_nt_expression(o, nt->data, 0); printf("\n"); break;
        case STMT_CONDITIONAL:          print_mips_nt_stmt_conditional(o, nt->data); printf("\n"); break;
        case STMT_LOOP:                 print_mips_nt_stmt_loop(o, nt->data); printf("\n"); break;
        case STMT_RETURN_EXPRESSION:    printf("lw\t$v1,  "); print_mips_nt_expression(o, nt->data, 0); printf("\n");
                                        print_whitespace(o); printf("jr\t$ra\n"); break;
        case STMT_RETURN:               print_whitespace(o); printf("jr\t$ra\n"); break;
        case STMT_EMPTY:                printf(";\n"); break;
        case STMT_GOTO:                 print_mips_nt_stmt_goto(o, nt->data); printf("\n"); break;
        case STMT_LABEL:                print_mips_nt_stmt_label(o, nt->data); printf("\n"); break;
        default:    printf("%d not found", nt->represents); break;
    }
}

void print_mips_nt_stmt_block (int o, nt_stmt * nt)
{
    debuggo(STMT_BLOCK);
    //printf("{\n");
    print_mips_nt_stmt_list(o, nt->data);
    //print_whitespace(o);
    //printf("}\n");
}

void print_mips_nt_stmt_list (int o, nt_stmt_list * nt)
{
    debuggo(STMT_LIST);
    list_element * current = nt->statements->first;
    while(current)
    {
        print_mips_nt_stmt(o, current->data);
        current=current->next;
    }
}

void print_mips_nt_stmt_conditional(int o, nt_stmt_conditional * nt)
{
    debuggo(STMT_CONDITIONAL);
    printf("if( ");
    print_mips_nt_expression(o, nt->condition, 0);
    printf(" )\n");
    print_mips_nt_stmt(o+4, nt->body);
    if(nt->body_else)
    {
        printf(" else \n");
        print_mips_nt_stmt(o, nt->body_else);
    }
}


void print_mips_nt_stmt_loop(int o, nt_stmt_loop * nt)
{
    debuggo(STMT_LOOP);
    if(nt->loop_type == WHILE_LOOP)
    {
        printf("while( ");
        print_mips_nt_expression(o, nt->condition, 0);
        printf(" )\n");
        print_mips_nt_stmt(o, nt->body);
    }
    if(nt->loop_type == DO_WHILE_LOOP)
    {
        printf("do\n");
        print_mips_nt_stmt(o, nt->body);
        printf("while( ");
        print_mips_nt_expression(o, nt->condition, 0);
        printf(" )");
    }
}

void print_mips_nt_stmt_goto (int o, nt_stmt_goto * nt)
{
    debuggo(STMT_GOTO);
    //printf(".text\n");
    print_mips_nt_expression(o, nt->condition, 1);
    print_whitespace(o + 4);
    printf("bnez\t$t1, %s", nt->label);
}

void print_mips_nt_stmt_label (int o, nt_stmt_label* nt)
{
    debuggo(STMT_LABEL);
    printf("%s:", nt->id);
}

void print_mips_nt_function_parameter(int o, nt_function_parameter * nt, int p_cnt)
{
    debuggo(FUNCTION_PARAMETER);
    //switch(nt->type)
    //{
    //    case VOID_: printf("void "); break;
    //    case INT_: printf("int "); break;
    //    case INTARRAY: printf("int "); break;
    //    default: break;
    //}
    print_whitespace(o);
    printf(".data\n");
    print_whitespace(o + 4);
    printf("%s:\t.word, 0\n", nt->id);
    print_whitespace(o);
    printf(".text\n");
    print_whitespace(o + 4);
    printf("sw\t$a%d, %s", p_cnt, nt->id);

    if(nt->arraySize)
    {
        printf("[%d]", nt->arraySize);
    }
}


void print_mips_nt_function_parameter_list(int o, nt_function_parameter_list * nt)
{
    debuggo(FUNCTION_PARAMETER_LIST);
    list_element * current = nt->params->first;
    while(current)
    {
        print_mips_nt_stmt(o, current->data);
        current=current->next;
        if(current) printf(", ");
    }
}


void print_mips_nt_function_declaration (int o, nt_function_declaration * nt)
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
            print_mips_nt_function_parameter(o, current->data, -99);
            current=current->next;
            if(current) printf(", ");
        }
        printf(" )");
    } else printf("%s()", nt->id);
}


void print_mips_nt_function_definition(int o, nt_function_definition * nt)
{
    debuggo(FUNCTION_DEFINITION);
    //switch(nt->return_type)
    //{
    //    case VOID_: printf("void "); break;
    //    case INT_: printf("int "); break;
    //    case INTARRAY: printf("int "); break;
    //    default: break;
    //}
    int p_cnt = 0;

    if(nt->params->first)
    {
        printf("%s:\n", nt->id);
        list_element * current = nt->params->first;
        while(current)
        {
            print_mips_nt_function_parameter(o + 4, current->data, p_cnt);
            current=current->next;
            //if(current) printf(", ");
            printf("\n");
            p_cnt = p_cnt + 1;
        }
        //print_whitespace(o + 4);
        //printf(".text\n");
    } else printf("%s:\n", nt->id);
    //printf("\n{\n");
    print_mips_nt_stmt_list(o+4, nt->stmt_list);
    //printf("}\n");
    
    //if (strcmp(nt->id, "main") == 0)
    //{
    //    printf("li $v0, 10\nsyscall\n");
    //} else 
    {
        print_whitespace(o + 4); printf("jr\t$ra");
    }
}


void print_mips_nt_program_element(int o, nt_program_element * nt)
{
    debuggo(PROGRAM_ELEMENT);
    switch(nt->represents)
    {
        case VARIABLE_DECLARATION: print_mips_nt_variable_declaration(o, nt->content); break;
        case FUNCTION_DECLARATION: print_mips_nt_function_declaration(o, nt->content); break;
        case FUNCTION_DEFINITION: print_mips_nt_function_definition(o, nt->content); break;
        default: break;
    }
}


void print_mips_nt_program_element_list(int o, nt_program_element_list * nt)
{
    debuggo(PROGRAM_ELEMENT_LIST);
    list_element * current = nt->variable_declarations->first;
    while(current)
    {
        print_mips_nt_variable_declaration(o, current->data); printf(";\n");
        current=current->next;
    }
    printf("\n");
    current = nt->function_declarations->first;
    while(current)
    {
        print_mips_nt_function_declaration(o, current->data); printf(";\n");
        current=current->next;
    }
    printf("\n");
    current = nt->function_definitions->first;
    while(current)
    {
        print_mips_nt_function_definition(o, current->data);
        current=current->next;
    }
}


void print_mips_nt_program(int o, nt_program * nt)
{
    debuggo(PROGRAM);
    printf("//Automatically generated code\n");
    printf("//Global variables:\n");
    list_element * current = nt->variable_declarations->first;
    while(current)
    {
        print_mips_nt_variable_declaration(o, current->data); printf(";\n");
        current=current->next;
    }
    printf("\n");
    printf("//Function declarations:\n");
    current = nt->function_declarations->first;
    while(current)
    {
        print_mips_nt_function_declaration(o, current->data); printf(";\n");
        current=current->next;
    }
    printf("\n");
    printf("//Function definitions:\n");
    current = nt->function_definitions->first;
    while(current)
    {
        print_mips_nt_function_definition(o, current->data);
        current=current->next;
    }
}
