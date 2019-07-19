/*
	Baby C Compiler
	Compiler written by Jeff Low

	This compiler was built for the CSC 151 Compiler Construction course
	at CSU, Sacramento. Instructor: Ghassan Shobaki
*/

#ifndef YOUR_CODE_H
#define YOUR_CODE_H

#define SIZE 100

typedef enum{ASTNODE_ARITH_OP, ASTNODE_LOGIC_OP, ASTNODE_COMPARE, ASTNODE_ASSIGN,
ASTNODE_IDENT, ASTNODE_NUM, ASTNODE_IF, ASTNODE_IF_ELSE, ASTNODE_WHILE,
		ASTNODE_DECLARATION_LIST, ASTNODE_STATEMENT_LIST} ASTNodeType;

// Define all operation types (add, mult, etc) here.
typedef enum{ADD_OP, MUL_OP, SUB_OP, DIV_OP, OR_OP, AND_OP, EQ_OP, NE_OP, GT_OP, LT_OP, LE_OP, GE_OP,
		JMP, CBR, LOAD_I, LOAD_AI, STORE_AI, LABEL} ASTOp;

typedef struct ASTNode ASTNode;
struct ASTNode{
	ASTNodeType type;
  ASTOp op;        // The actual operation (add, mult, etc)
	int num;         // Need to store the actual value for number nodes
	char *name;      // Need to store the actual variable name for ident nodes
	ASTNode* left;   // Left child
	ASTNode* right;  // Right child
	ASTNode* next;   // a pointer used to link statement nodes together in a statement list
	char *regNum;
	char *labelNum;
	char *offset;
	ASTOp labelType;
};

/*
	Fields for the hash table.
*/
struct Name{
	char* name;
	char* offset;
};


struct Name* hash[SIZE];

void Insert(char *name);
struct Name *hashSearch(char *name);
int hashIndex(char * name);

// Add functions to create the different kinds of ASTNodes
// These functions are called by the code embedded in the grammar.
// Here are some samples:
ASTNode* CreateNumNode(int num);
ASTNode* CreateIdentNode(char* name);
ASTNode* CreateAddNode(ASTNode* expr, ASTNode* term);
ASTNode* CreateSubNode(ASTNode* expr, ASTNode* term);
ASTNode* CreateMultNode(ASTNode* term, ASTNode* factor);
ASTNode* CreateDivNode(ASTNode* term, ASTNode* factor);
ASTNode* CreateIfNode(ASTNode* condList, ASTNode* statList);
ASTNode* CreateIfElseNode(ASTNode* condList, ASTNode* statList, ASTNode* elseStatList);
ASTNode* CreateConditionListNode(ASTNode* cond, ASTNode* condList);
ASTNode* CreateConditionNode(ASTNode* comp, ASTNode* cond);
ASTNode* CreateCompareNode(ASTNode* exprLeft, ASTNode* exprRight);
ASTNode* CreateWhileNode(ASTNode* cond, ASTNode* statList);
ASTNode* CreateStatementListNode(ASTNode* st, ASTNode* stList);
ASTNode* CreateAssignmentNode(ASTNode* ident, ASTNode* expr);
ASTNode* CreateDeclarationListNode(ASTNode* dc, ASTNode* dcList);

// Need a function to add a declaration to your symbol table
void AddDeclaration(char* name);

// This is the function that generates ILOC code after the construction of the AST
void GenerateILOC(ASTNode* node);

void Emit(ASTOp operation, char *source_1, char *source_2, char *destination);
char *GetNextRegister();
char *GetNextLabel();
char *GetCurrentLabel();
ASTOp GetCurrentLabelType();
void SetCurrentLabelType(ASTOp type);
char *GetNextOffset();
#endif
