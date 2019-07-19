
//    Baby C Compiler
//    Compiler written by Jeff Low

//    This compiler was built for the CSC 151 Compiler Construction course
//    at CSU, Sacramento. Instructor: Ghassan Shobaki

%{
	#include <stdio.h>
	#include "your_code.h"

// The parser needs to call the scanner to get the next token
	extern int yylex();

// The error function
	extern int yyerror(const char *);

// The ASTNode root
    extern ASTNode* gASTRoot;
%}

//Put any initialization code here
%initial-action
{

}

%token LE "<="
%token GE ">="
%token EQ "=="
%token NE "!="
%token OR "||"
%token AND "&&"
%token MAIN "main"
%token INT "int"
%token IF "if"
%token ELSE "else"
%token WHILE "while"

//Define the types for the grammar attributes ($$, $1, $2, ...)
%union
{
	struct ASTNode* node; // Most $$ values will be ASTNodes
	int num; // Integer numbers
	char* string; // Strings for identifiers
}

%token <string> IDENT
%token <num> NUM

//Specify the type for each non-terminal in the grammar. Here are some samples:

%type <node> Statement
%type <node> Assignment
%type <node> Expr
%type <node> Term
%type <node> Factor
%type <node> If
%type <node> While
%type <node> Compare
%type <node> Condition
%type <node> StatementList
%type <node> LHS
%type <node> ConditionList
%type <node> DeclarationList
%type <node> Declaration
%%

// Write the grammar for BabyC, and write an embedded action for each production. Here are some samples for you:

Goal: "main" '(' ')' '{' DeclarationList StatementList '}'	{gASTRoot=$6;} // Store the AST root node in gASTRoot
;

DeclarationList: {$$ = NULL;}
		 | Declaration DeclarationList 	{$$ = CreateDeclarationListNode($1,$2);}// Note that a DeclarationList may be empty
;

Declaration: "int" IDENT ';' {AddDeclaration($2); printf("Processing declaration of %s\n", $2);}
;
Factor: IDENT 		{$$ = CreateIdentNode($1); printf("Creating IDENT node for %s\n", $1);}
	| NUM 		{$$ = CreateNumNode($1); printf("Creating NUM node for %d\n", $1);}
	| '('Expr')'	{$$ = $2; printf("Creating Expression node in parentheses\n");}
;

Statement:  Assignment {$$ = $1;}
	| If 	{$$ = $1;}
	| While	{$$ = $1;}
;

Assignment: LHS '=' Expr ';'	{$$ = CreateAssignmentNode($1,$3); printf("Creating Assignment node\n");}
;

LHS: IDENT { $$ = CreateIdentNode($1); printf("Creating left-hand IDENT node for %s\n", $1);}
;

Expr: Term 		{$$ = $1;}
	| Expr '+' Term	{$$ = CreateAddNode($1, $3); printf("Creating Addition node\n");}
	| Expr '-' Term {$$ = CreateSubNode($1, $3); printf("Creating Subtraction node\n");}
;

Term: 	Factor			{$$ = $1}
	| Term '*' Factor  	{$$ = CreateMultNode($1, $3); printf("Creating Multiplication node\n");}
	| Term '/' Factor	{$$ = CreateDivNode($1, $3); printf("Creating Division node\n");}
;

If:   "if" '(' ConditionList ')' '{' StatementList '}' 									{$$ = CreateIfNode($3, $6); printf("Creating if Statement node\n");}
	| "if" '(' ConditionList ')' '{' StatementList '}' "else" '{' StatementList '}' 	{$$ = CreateIfElseNode($3, $6, $10); printf("Creating if-else Statement node\n");}
;

ConditionList: 	Condition 				{$$ = $1}
		| Condition "||" ConditionList	{$$ = CreateConditionListNode($1, $3); printf("Creating OR node\n");}
;

Condition:	Compare						{$$ = $1}
		| Compare "&&" Condition		{$$ = CreateConditionNode($1, $3); printf("Creating AND node\n");}
;

Compare:  Expr "==" Expr	  {$$ = (ASTNode*) CreateEQNode($1, $3); printf("Creating Compare node\n");}
		| Expr "!=" Expr				{$$ = (ASTNode*) CreateNENode($1, $3); printf("Creating Compare node\n");}
		| Expr '>' Expr					{$$ = (ASTNode*) CreateGTNode($1, $3); printf("Creating Compare node\n");}
		| Expr '<' Expr					{$$ = (ASTNode*) CreateLTNode($1, $3); printf("Creating Compare node\n");}
		| Expr "<=" Expr				{$$ = (ASTNode*) CreateLENode($1, $3); printf("Creating Compare node\n");}
		| Expr ">=" Expr				{$$ = (ASTNode*) CreateGENode($1, $3); printf("Creating Compare node\n");}
;

While:		"while" '(' ConditionList ')' '{'  StatementList '}'	{$$ = CreateWhileNode($3, $6); printf("Creating while loop node\n");}
;

StatementList: {$$ = NULL;}
    | Statement StatementList	{$$ = CreateStatementListNode($1,$2); printf("Adding a Statement to a Statement list \n");}
;
%%
