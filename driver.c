/*
    Baby C Compiler
    Compiler written by Jeff Low

    This compiler was built for the CSC 151 Compiler Construction course
    at CSU, Sacramento. Instructor: Ghassan Shobaki
 */

#include <stdio.h>
#include <stdlib.h>
#include "your_code.h"

extern FILE* yyin;
extern int yyparse();
FILE *fp;

// A global variable that holds a pointer to the AST root
ASTNode* gASTRoot;

int main(int argc, char**argv) {
	if (argc != 2) {
		printf("ERROR: Invalid number of command-line arguments. Usage: bcc File_Name.bc\n");
		exit(1);
	}
	yyin = fopen(argv[1], "r" );
	if (yyin == NULL) {
		printf("ERROR: Failed to open the input file\n");
		exit(1);
	}

	// Call the parser.
	// Add embedded actions to the parser (in BabyC.y) to construct the AST and store its root in gASTRoot.
	yyparse();
	fclose(yyin);
	//fp = fopen("Output.txt", "w");
	char *fileName = argv[1];
	char out[64];
	char temp = fileName[4];
	sprintf(out, "test%c.iloc",temp);
	fp = fopen(out, "w+");

	// Now that the AST has been constructed, pass its root to the function that traverses it and generates the ILOC code.
	GenerateILOC(gASTRoot);
	fclose(fp);
}
