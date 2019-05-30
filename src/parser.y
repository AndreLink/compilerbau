/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%{	
	// Project-specific includes
	#include "diag.h"
     #include "symbol_table.h"
%}

%code top{
     //#include "symbol_table.h"
}

%code requires{
     #include "symbol_table.h"
     #include "programTree.h"
}

%{
     int yylex();
     void yyerror(const char*);
     int yyparse();
%}

%union {
  int i;
  char *id;
  type type_;
  nt_program *nt_program_;
  nt_program_element_list *nt_program_element_list_;
  nt_program_element *nt_program_element_;
  nt_variable_declaration *nt_variable_declaration_;
  nt_identifier_declaration *nt_identifier_declaration_;
  nt_function_definition *nt_function_definition_;
  nt_function_declaration *nt_function_declaration_;
  nt_function_parameter_list *nt_function_parameter_list_;
  nt_function_parameter *nt_function_parameter_;
  nt_stmt_list *nt_stmt_list_;
  nt_stmt *nt_stmt_;
  nt_stmt_block *nt_stmt_block_;
  nt_stmt_conditional *nt_stmt_conditional_;
  nt_stmt_loop *nt_stmt_loop_;
  nt_expression *nt_expression_;
  nt_primary *nt_primary_;
  nt_function_call *nt_function_call_;
  nt_function_call_parameters *nt_function_call_parameters_;
}
 
// Verbose error messages
%error-verbose

%locations
%start program

// Compiler in debugging information
%debug

// Enable debug output
%initial-action
{
	yydebug = 0;
};

/*
 * One shift/reduce conflict is expected for the "dangling-else" problem. This
 * conflict however is solved by the default behavior of bison for shift/reduce 
 * conflicts (shift action). The default behavior of bison corresponds to what
 * we want bison to do: SHIFT if the lookahead is 'ELSE' in order to bind the 'ELSE' to
 * the last open if-clause. 
 */

%type<id> ID;
%type<i> INT;
%type<i> NUM;
%type<type_> type;
%type<nt_program_> program;
%type<nt_program_element_list_> program_element_list;
%type<nt_program_element_> program_element;
%type<nt_variable_declaration_> variable_declaration;
%type<nt_identifier_declaration_> identifier_declaration;
%type<nt_function_definition_> function_definition;
%type<nt_function_declaration_> function_declaration;
%type<nt_function_parameter_list_> function_parameter_list;
%type<nt_function_parameter_> function_parameter;
%type<nt_stmt_list_> stmt_list;
%type<nt_stmt_> stmt;
%type<nt_stmt_block_> stmt_block;
%type<nt_stmt_conditional_> stmt_conditional;
%type<nt_stmt_loop_> stmt_loop;
%type<nt_expression_> expression;
%type<nt_primary_> primary;
%type<nt_function_call_> function_call;
%type<nt_function_call_parameters_> function_call_parameters;

%expect 1

%token DO WHILE
%token IF ELSE
%token INT VOID
%token RETURN

%token COLON COMMA SEMICOLON

%token BRACE_OPEN BRACE_CLOSE
%token BRACKET_OPEN BRACKET_CLOSE
%token PARA_OPEN PARA_CLOSE

%token ID
%token NUM

%token ASSIGN 
%token LOGICAL_OR
%token LOGICAL_AND
%token EQ NE     
%token LS LSEQ GTEQ GT 
%token SHIFT_LEFT SHIFT_RIGHT
%token PLUS MINUS     
%token MUL DIV MOD
%token LOGICAL_NOT UNARY_MINUS UNARY_PLUS

%left SHIFT_LEFT SHIFT_RIGHT

%right ASSIGN
%left PLUS MINUS
%left MUL DIV

%right UNARY_PLUS UNARY_MINUS

%left EQ NE LS LSEQ GTEQ GT
%left LOGICAL_OR
%left LOGICAL_AND
%right LOGICAL_NOT

%%

program
     : program_element_list                                                          { $$ = ntf_program_1($1); }
     ;

program_element_list
     : program_element_list program_element                                          { $$ = ntf_program_element_list_1($1, $2); }
     | program_element                                                               { $$ = ntf_program_element_list_2($1); }
     ;

program_element
     : variable_declaration SEMICOLON                                                { $$ = ntf_program_element_1($1); }
     | function_declaration SEMICOLON                                                { $$ = ntf_program_element_2($1); }
     | function_definition                                                           { $$ = ntf_program_element_3($1); }
     | SEMICOLON                                                                     { $$ = 0; }
     ;
									
type
     : INT                                                                           { $$ = INT_; }
     | VOID                                                                          { $$ = VOID_; }
     ;

variable_declaration
     : variable_declaration COMMA identifier_declaration                             { $$ = ntf_variable_declaration_1($1, $3); }
     | type identifier_declaration                                                   { $$ = ntf_variable_declaration_2($1, $2); }
     ;

identifier_declaration
     : ID BRACKET_OPEN NUM BRACKET_CLOSE                                             { $$ = ntf_identifier_declaration_1($1, $3); }
     | ID                                                                            { $$ = ntf_identifier_declaration_2($1); }
     ;

function_definition
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN stmt_list BRACE_CLOSE                 { $$ = ntf_function_definition_1($1, $2, $6); }
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE BRACE_OPEN stmt_list BRACE_CLOSE   { $$ = ntf_function_definition_2($1, $2, $4, $7); }
     ;

function_declaration
     : type ID PARA_OPEN PARA_CLOSE                                                  { $$ = ntf_function_declaration_1($1, $2); }
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE                          { $$ = ntf_function_declaration_2($1, $2, $4); }
     ;

function_parameter_list
     : function_parameter                                                            { $$ = ntf_function_parameter_list_2($1); }
     | function_parameter_list COMMA function_parameter                              { $$ = ntf_function_parameter_list_1($1, $3); }
     ;
	
function_parameter
     : type identifier_declaration                                                    { $$ = ntf_function_parameter_1($1, $2); }
     ;
									
stmt_list
     : /* empty: epsilon */                                                           { $$ = ntf_stmt_list_1(); }
     | stmt_list stmt                                                                 { $$ = ntf_stmt_list_2($1, $2); }                                                                 
     ;

stmt
     : stmt_block                                                                     { $$ = ntf_stmt_1($1); }
     | variable_declaration SEMICOLON                                                 { $$ = ntf_stmt_2($1); }
     | expression SEMICOLON                                                           { $$ = ntf_stmt_3($1); }
     | stmt_conditional                                                               { $$ = ntf_stmt_4($1); }
     | stmt_loop                                                                      { $$ = ntf_stmt_5($1); }
     | RETURN expression SEMICOLON                                                    { $$ = ntf_stmt_6($2); }
     | RETURN SEMICOLON                                                               { $$ = ntf_stmt_7(); }
     | SEMICOLON /* empty statement */                                                { $$ = ntf_stmt_8(); }    
     ;

stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE                                               { $$ = ntf_stmt_block($2); }
     ;
	
stmt_conditional
     : IF PARA_OPEN expression PARA_CLOSE stmt                                        { $$ = ntf_stmt_conditional_1($3, $5); }
     | IF PARA_OPEN expression PARA_CLOSE stmt ELSE stmt                              { $$ = ntf_stmt_conditional_2($3, $5, $7); }
     ;
									
stmt_loop
     : WHILE PARA_OPEN expression PARA_CLOSE stmt                                     { $$ = ntf_stmt_loop_1($3, $5); }
     | DO stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON                        { $$ = ntf_stmt_loop_2($2, $5); }
     ;
									
expression
     : expression ASSIGN expression                                                   { $$ = ntf_expression_assignment($1, $3); }
     | expression LOGICAL_OR expression                                               { $$ = ntf_expression($1, _LOGICAL_OR, $3); }
     | expression LOGICAL_AND expression                                              { $$ = ntf_expression($1, _LOGICAL_AND, $3); }
     | LOGICAL_NOT expression                                                         { $$ = ntf_expression_negation($2); }
     | expression EQ expression                                                       { $$ = ntf_expression($1, _EQ, $3); }
     | expression NE expression                                                       { $$ = ntf_expression($1, _NE, $3); }
     | expression LS expression                                                       { $$ = ntf_expression($1, _LS, $3); }
     | expression LSEQ expression                                                     { $$ = ntf_expression($1, _LSEQ, $3); }
     | expression GTEQ expression                                                     { $$ = ntf_expression($1, _GTEQ, $3); }
     | expression GT expression                                                       { $$ = ntf_expression($1, _GT, $3); }
     | expression PLUS expression                                                     { $$ = ntf_expression($1, _PLUS, $3); }
     | expression MINUS expression                                                    { $$ = ntf_expression($1, _MINUS, $3); }
     | expression SHIFT_LEFT expression                                               { $$ = ntf_expression($1, _SHIFT_LEFT, $3); }
     | expression SHIFT_RIGHT expression                                              { $$ = ntf_expression($1, _SHIFT_RIGHT, $3); }
     | expression MUL expression                                                      { $$ = ntf_expression($1, _MUL, $3); }
     | expression DIV expression                                                      { $$ = ntf_expression($1, _DIV, $3); }
     | MINUS expression %prec UNARY_MINUS                                             { $$ = ntf_expression_unary_minus($2); }
     | PLUS expression %prec UNARY_PLUS                                               { $$ = ntf_expression_unary_plus($2); }
     | ID BRACKET_OPEN primary BRACKET_CLOSE                                          { $$ = ntf_expression_array($1, $3); }
     | PARA_OPEN expression PARA_CLOSE                                                { $$ = ntf_expression_parenthesis($2); }
     | function_call                                                                  { $$ = ntf_expression_function_call($1); }
     | primary                                                                        { $$ = ntf_expression_primary($1); }
     ;

primary
     : NUM                                                                            { $$ = ntf_primary_1($1); }
     | ID                                                                             { $$ = ntf_primary_2($1); }
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE                                                        { $$ = ntf_function_call_1($1); }
      | ID PARA_OPEN function_call_parameters PARA_CLOSE                               { $$ = ntf_function_call_2($1, $3); }
      ;

function_call_parameters
     : function_call_parameters COMMA expression                                       { $$ = ntf_function_call_parameters_1($1, $3); }
     | expression                                                                      { $$ = ntf_function_call_parameters_2($1); }
     ;

%%

void yyerror (const char *msg)
{
	FATAL_COMPILER_ERROR(INVALID_SYNTAX, 0, "(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}
