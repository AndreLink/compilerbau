#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "symbol_table.h"

//#define DEBUGGGGGG

void yyerror (const char *msg);


linked_list * variables = 0;
linked_list * functions = 0;
linked_list * all_entries = 0;

function_entry * functionBuffer = 0;



int has_same_param_types(linked_list * listA, linked_list * listB)
{
    list_element * curA = listA->first;
    list_element * curB = listB->first;
    while(curA && curB)
    {
        variable_entry * paramA = curA->data;
        variable_entry * paramB = curB->data;
        if(paramA->type != paramB->type)
        {
            return 0;
        }
        if(paramA->type_arraylength != paramB->type_arraylength)
        {
            return 0;
        }
        curA = curA->next;
        curB = curB->next;
    }
    if(curA || curB)
    {
        return 0;
    }
    return 1;
}


int init_variables()
{
    #ifdef DEBUGGGGGG
    printf("init_variables();\n");
    #endif // DEBUGGGGGG

    variables = new_ll();
}

int init_functions()
{
    #ifdef DEBUGGGGGG
    printf("init_functions();\n");
    #endif // DEBUGGGGGG

    functions = new_ll();
}

int init_functionBuffer()
{
    #ifdef DEBUGGGGGG
    printf("init_functionBuffer();\n");
    #endif // DEBUGGGGGG

    functionBuffer = malloc(sizeof(function_entry));
    functionBuffer->isFunction = 1;
    functionBuffer->scope = GLOBAL;
    functionBuffer->type = UNKNOWN_TYPE;
    functionBuffer->functionParameters = new_ll();
    functionBuffer->functionVariables = new_ll();
}

variable_entry * new_variable_entry(char * id, type type, scope scope, int arraylength)
{
    #ifdef DEBUGGGGGG
    printf("new_variable_entry(%s, %d, %d);\n", id, type, scope);
    #endif // DEBUGGGGGG

	variable_entry * var = malloc(sizeof(variable_entry));

	var->id = id;
	var->isFunction = 0;
	var->scope = scope;
	var->type = type;
    var->type_arraylength = arraylength;
	var->functionParameters = 0;
	var->functionVariables = 0;
}

int add_global_variable(char * id, type type)
{
    add_global_variable_a(id, type, 0);
}
int add_global_variable_a(char * id, type type, int arraylength)
{
    #ifdef DEBUGGGGGG
    printf("add_global_variable(%s, %d);\n", id, type);
    #endif // DEBUGGGGGG

    if(get_function_entry(id))
    {
        yyerror("Identifier already used as function.\n");
        exit(1);
    }

    if(variables == 0){init_variables();}
    variable_entry * var = new_variable_entry(id, type, GLOBAL, arraylength);
	add_to_ll(variables, var);
	return 0;
}

int add_parameter_to_function(char * param_id, type type)
{
    add_parameter_to_function_a(param_id, type, 0);
}
int add_parameter_to_function_a(char * param_id, type type, int arraylength)
{
    #ifdef DEBUGGGGGG
    printf("add_parameter_to_function(%s, %d);\n", param_id, type);
    #endif // DEBUGGGGGG

    if(get_function_entry(param_id))
    {
        yyerror("Identifier already used as function.\n");
        exit(1);
    }

    if(functionBuffer == 0){init_functionBuffer();}
    variable_entry * var = new_variable_entry(param_id, type, PARAM, arraylength);
    add_to_ll(functionBuffer->functionParameters, var);
    return 0;
}

int add_local_var_to_function(char * var_id, type type)
{
    add_local_var_to_function_a(var_id, type, 0);
}
int add_local_var_to_function_a(char * var_id, type type, int arraylength)
{
    #ifdef DEBUGGGGGG
    printf("add_local_var_to_function(%s, %d);\n", var_id, type);
    #endif // DEBUGGGGGG

    if(get_function_entry(var_id))
    {
        yyerror("Identifier already used as function.\n");
        exit(1);
    }

    if(functionBuffer == 0){init_functionBuffer();}
    variable_entry * var = new_variable_entry(var_id, type, LOCAL, arraylength);
    add_to_ll(functionBuffer->functionVariables, var);
    return 0;
}

int finish_function_declaration(char * func_id,  type return_type)
{
    #ifdef DEBUGGGGGG
    printf("finish_function_declaration(%s, %d);\n", func_id, return_type);
    #endif // DEBUGGGGGG

    if(get_variable_entry(func_id))
    {
        yyerror("Identifier already used as variable.\n");
        exit(1);
    }

    if(functions == 0){init_functions();}
    if(functionBuffer == 0){init_functionBuffer();}

    //check if function is already known
    list_element * current = functions->first;
    while(current)
    {
        function_entry * func = current->data;
        if(strcmp(func_id, func->id) == 0)
        {
            printf("ERROR: Redeclaration of function %s\n", func_id);
            yyerror("Function redeclared\n");
            exit(1);
        }
        current=current->next;
    }

    functionBuffer->id = func_id;
    functionBuffer->type = return_type;
    functionBuffer->functionDefined = 0;

    add_to_ll(functions, functionBuffer);
    functionBuffer = 0;
    return 0;
}

int finish_function_definition(char * func_id,  type return_type)
{
    #ifdef DEBUGGGGGG
    printf("finish_function_definition(%s, %d);\n", func_id, return_type);
    #endif // DEBUGGGGGG

    if(get_variable_entry(func_id))
    {
        yyerror("Identifier already used as variable.\n");
        exit(1);
    }

    if(functions == 0){init_functions();}
    if(functionBuffer == 0){init_functionBuffer();}

    // ->type may be set to the type of a return statement in the function, needs to match function type!
    if(functionBuffer->type == UNKNOWN_TYPE)
    {
        if(return_type == VOID_)
        {
            //all good; void function doesnt need a return statement
        }
        else
        {
            yyerror("Function needs to return a value!");
            exit(1);
        }
    }
    else if(functionBuffer->type != return_type)
    {
        yyerror("Conflicting return types!");
         exit(1);
    }

    //check if function is already known
    list_element * current = functions->first;
    int index = 0;
    while(current)
    {
        function_entry * func = current->data;
        if(strcmp(func_id, func->id) == 0)
        {
            if( func->functionDefined )
            {
                printf("ERROR: Function %s is defined twice!\n", func_id);
                yyerror("Function redefined\n");
                exit(1);
            } 
            else if(return_type != func->type)
            {
                printf("ERROR: Function %s has inconsistent return types!\n", func_id);
                yyerror("Function inconsistent\n");
                exit(1);
            }
            else
            {
                 if(has_same_param_types(func->functionParameters, functionBuffer->functionParameters))
                {
                    delete_element_ll(functions, index);
                } 
                else 
                {
                    printf("ERROR: Function %s is redeclared with different parameter types!\n", func_id);
                    yyerror("Wrong function parameters\n");
                    exit(1);
                }
            }
        }
        current=current->next;
        index++;
    }

    functionBuffer->id = func_id;
    functionBuffer->type = return_type;
    functionBuffer->functionDefined = 1;

    add_to_ll(functions, functionBuffer);
    functionBuffer = 0;
    return 0;
}


/*
//scope unknown at this point
variable_entry * new_variable(char * id, type type)
{
    #ifdef DEBUGGGGGG
    printf("new_variable(%s, %d, %d);\n", id, type, scope);
    #endif // DEBUGGGGGG

	variable_entry * var = malloc(sizeof(variable_entry));

	var->id = id;
	var->isFunction = 0;
    var->scope = UNKNOWN_SCOPE;
	var->type = type;
	var->functionParameters = 0;
	var->functionVariables = 0;
}
*/

/*
int to_global_variables(variable_entry * var)
{
    #ifdef DEBUGGGGGG
    printf("to_global_variables(%p);\n", var);
    #endif // DEBUGGGGGG

    if(variables == 0){init_variables();}
    var->scope = GLOBAL;
	add_to_ll(variables, var);
	return 0;
}
*/

/*
int to_local_variables(variable_entry * var)
{
    #ifdef DEBUGGGGGG
    printf("to_local_variables(p);\n", var);
    #endif // DEBUGGGGGG

    if(functionBuffer == 0){init_functionBuffer();}
    var->scope = LOCAL;
    add_to_ll(functionBuffer->functionVariables, var);
    return 0;
}
*/

/*
int to_param_variables(variable_entry * var)
{
    #ifdef DEBUGGGGGG
    printf("to_param_variables(p);\n", var);
    #endif // DEBUGGGGGG

    if(functionBuffer == 0){init_functionBuffer();}
    var->scope = PARAM;
    add_to_ll(functionBuffer->functionParameters, var);
    return 0;
}
*/



function_entry * get_function_entry(char * func_id)
{
    if(functions == 0){ init_functions();}

    list_element * current = functions->first;
    while(current)
    {
        function_entry * fun = current->data;
        if(strcmp(func_id, fun->id) == 0)
        {
            return fun;
        }
        current=current->next;
    }
    return 0;
}


variable_entry * get_variable_entry(char * id)
{
    if(variables == 0) { init_variables(); }

    list_element * current = variables->first;
    while(current)
    {
        variable_entry * var = current->data;
        if(strcmp(id, var->id) == 0)
        {
            return var;
        }
        current=current->next;
    }


    if(functionBuffer == 0)
    {
        init_functionBuffer();
        return 0;
    }

    current = functionBuffer->functionParameters->first;
    while(current)
    {
        variable_entry * var = current->data;
        if(strcmp(id, var->id) == 0)
        {
            return var;
        }
        current=current->next;
    }

    current = functionBuffer->functionVariables->first;
    while(current)
    {
        variable_entry * var = current->data;
        if(strcmp(id, var->id) == 0)
        {
            return var;
        }
        current=current->next;
    }
    return 0;
}


int add_return_statement_to_function(type type)
{
    if(functionBuffer == 0){ init_functionBuffer(); }
    if(functionBuffer->type == UNKNOWN_TYPE)
    {
        functionBuffer->type = type;
    }
    else if(functionBuffer->type == type)
    {
        //all good
    }
    else
    {
        yyerror("Conflicting return types!\n");
        exit(1);
    }

    return 0;
}






















void print_variables(linked_list * vars)
{
    #ifdef DEBUGGGGGG
    printf("print_variables(%p);\n", vars);
    #endif // DEBUGGGGGG
    if(vars == 0) return;

    list_element * current = vars->first;
    while(current)
    {
        variable_entry * var = current->data;
        print_variable(var);
        current=current->next;
    }
}


void print_functions(linked_list * funs)
{
    #ifdef DEBUGGGGGG
    printf("print_functions(%p);\n", funs);
    #endif // DEBUGGGGGG
    if(funs == 0) return;

    list_element * current = funs->first;
    while(current)
    {
        function_entry * fun = current->data;
        print_function(fun);
        current=current->next;
    }
}


void print_variable(variable_entry * var)
{
    #ifdef DEBUGGGGGG
    printf("print_variable(%p);\n", var);
    #endif // DEBUGGGGGG
    if(var == 0) return;

	printf("%32s ", var->id);
    switch(var->type)
    {
        case INT_:		printf("| INT  "); break;
        case CHAR:		printf("| CHAR "); break;
        case BOOLEAN:	printf("| BOOL "); break;
        case VOID_:	    printf("| VOID "); break;
        case INTARRAY:  printf("| IARR "); break;
        default:		printf("| err  ");
    }
    switch(var->scope)
    {
        case GLOBAL:		printf("| GLOBAL "); break;
        case LOCAL:			printf("| LOCAL  "); break;
        case PARAM:			printf("| PARAM  "); break;
        default:			printf("| err    ");
    }
    printf("\n");
}
void print_function(function_entry * fun)
{
    #ifdef DEBUGGGGGG
    printf("print_function(%p);\n", fun);
    #endif // DEBUGGGGGG
    if(fun == 0) return;

	printf("Function \"%s\": Returns ", fun->id);
    switch(fun->type)
    {
        case INT_:		printf("INT"); break;
        case CHAR:		printf("CHAR"); break;
        case BOOLEAN:	printf("BOOL"); break;
        case VOID_:	    printf("VOID"); break;
        case INTARRAY:  printf("IARR"); break;
        default:		printf("err");
    }
    printf("\nParameters:\n");
    if(fun->functionParameters->first) {print_variables(fun->functionParameters);}
    printf("Local variables:\n");
    if(fun->functionVariables->first) {print_variables(fun->functionVariables);}
    printf("\n");
}

void print_symbol_table()
{
    #ifdef DEBUGGGGGG
    printf("print_symbol_table();\n");
    #endif // DEBUGGGGGG

	printf("--GLOBAL VARIABLES--        NAME | TYPE | SCOPE\n");
    print_variables(variables);
    printf("\n");
    print_functions(functions);
}
