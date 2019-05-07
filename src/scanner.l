/* 
 * scanner.l - Scanner utility for the DHBW compiler
 */

%{
	#include "parser.y.h"
%}

%{
	int column = 0;

	#define YY_USER_ACTION {	\
		yylloc.first_line = yylineno;	\
		yylloc.last_line = yylineno;	\
		yylloc.first_column = column;	\
		yylloc.last_column = column + yyleng - 1;	\
		column += yyleng;	\
	}
%}

/* 
 * Default setting for flex which indicates that yytext, which is one of the main connections
 * between the scanner and parser, is handled as pointer of char instead of an array of char. 
*/
%pointer	

/* save the line number of terminals */
%option yylineno

/* finish after scanning the provided file */
%option noyywrap		

%%

	/* Datentypen */
int { return INT; }
void { return VOID; }

	/* Zuweisung */
\= { return ASSIGN; }

	/* Binäre arithmetische Operatoren */
\+ { return PLUS; }
\- { return MINUS; }
\* { return MUL; }
\/ { return DIV; }
\% { return MOD; }
\<\< { return SHIFT_LEFT; }
\>\> { return SHIFT_RIGHT; }

	/* Binäre Vergleichsoperatoren */
\>\= { return LSEQ; }
\> { return LS; }
\<\= { return GTEQ; }
\< { return GT; }
\=\= { return EQ; }
\!\= { return NE; }

	/* Binäre Logische Operatoren */
\&\& { return LOGICAL_AND; }
\|\| { return LOGICAL_OR; }

	/* Unäre Logische Operatoren */
\! { return LOGICAL_NOT; }

	/* Kontrollstrukturen */
do { return DO; }
while { return WHILE; }
if { return IF; }
else { return ELSE; }
return { return RETURN; }
\; { return SEMICOLON; }
\: { return COLON; }
\, { return COMMA; }

	/* Klammern */
\{ { return BRACE_OPEN; }
\} { return BRACE_CLOSE; }
\[ { return BRACKET_OPEN; }
\] { return BRACKET_CLOSE; }
\( { return PARA_OPEN; }
\) { return PARA_CLOSE; }

	/* Bezeichner */
[_a-zA-Z][_\-a-zA-Z0-9]* { yylval.id = strdup(yytext); return ID; }
0|[1-9][0-9]* { yylval.i = atoi(yytext); return NUM; }

	/* Error Handling */
\n	{ column = 0; }
