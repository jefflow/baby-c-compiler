/*
    Baby C Compiler
    Compiler written by Jeff Low

    This compiler was built for the CSC 151 Compiler Construction course
    at CSU, Sacramento. Instructor: Ghassan Shobaki
 */

#include "your_code.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *fp = NULL;

int labelNumber = 0;
int regNumber = 0;
int offset = 0;

ASTOp labelType;

ASTNode* CreateIdentNode(char* name) {
	struct Name *names = (struct Name*) malloc(sizeof(struct Name));
	names = hashSearch(name);
	
	if(names == NULL) {
		yyerror ("Ident not declared");
		exit(1);
	}

	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_IDENT;
	node->name = name;
	return node;
}

ASTNode* CreateAddNode(ASTNode* expr, ASTNode* term) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ARITH_OP;
	node->op = ADD_OP;
	node->left = expr;
	node->right = term;
	return node;
}

ASTNode* CreateSubNode(ASTNode* expr, ASTNode* term) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ARITH_OP;
	node->op = SUB_OP;
	node->left = expr;
	node->right = term;
	return node;
}

ASTNode* CreateMultNode(ASTNode* term, ASTNode* factor) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ARITH_OP;
	node->op = MUL_OP;
	node->left = term;
	node->right = factor;
	return node;
}

ASTNode* CreateDivNode(ASTNode* term, ASTNode* factor) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ARITH_OP;
	node->op = DIV_OP;
	node->left = term;
	node->right = factor;
	return node;
}

ASTNode* CreateNumNode(int num) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_NUM;
	node->num = num;
	return node;
}

ASTNode* CreateIfNode(ASTNode* condList, ASTNode* statList) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_IF;
	node->left = condList;
	node->right = statList;
	return node;
}

ASTNode* CreateIfElseNode(ASTNode* condList, ASTNode* statList, ASTNode* elseStatList) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_IF_ELSE;
	node->left = CreateIfNode(condList, statList);
	node->right = elseStatList;
	return node;
}

ASTNode* CreateConditionListNode(ASTNode* cond, ASTNode* condList){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_LOGIC_OP;
	node->op = OR_OP;
	node->left = cond;
	node->right = condList;
	return node;
}

ASTNode* CreateConditionNode(ASTNode* comp, ASTNode* cond){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_LOGIC_OP;
	node->op = AND_OP;
	node->left = comp;
	node->right = cond;
	return node;
}
//
ASTNode* CreateEQNode(ASTNode* exprLeft, ASTNode* exprRight){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_COMPARE;
	node->op = EQ_OP;
	node->left = exprLeft;
	node->right = exprRight;
	return node;
}

ASTNode* CreateNENode(ASTNode* exprLeft, ASTNode* exprRight){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_COMPARE;
	node->op = NE_OP;
	node->left = exprLeft;
	node->right = exprRight;
	return node;
}

ASTNode* CreateGTNode(ASTNode* exprLeft, ASTNode* exprRight){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_COMPARE;
	node->op = GT_OP;
	node->left = exprLeft;
	node->right = exprRight;
	return node;
}

ASTNode* CreateLTNode(ASTNode* exprLeft, ASTNode* exprRight){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_COMPARE;
	node->op = LT_OP;
	node->left = exprLeft;
	node->right = exprRight;
	return node;
}

ASTNode* CreateLENode(ASTNode* exprLeft, ASTNode* exprRight){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_COMPARE;
	node->op = LE_OP;
	node->left = exprLeft;
	node->right = exprRight;
	return node;
}

ASTNode* CreateGENode(ASTNode* exprLeft, ASTNode* exprRight){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_COMPARE;
	node->op = GE_OP;
	node->left = exprLeft;
	node->right = exprRight;
	return node;
}

ASTNode* CreateWhileNode(ASTNode* cond, ASTNode* statList){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_WHILE;
	node->left = cond;
	node->right = statList;
	return node;
}

ASTNode* CreateDeclarationListNode(ASTNode* dc, ASTNode* dcList) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_DECLARATION_LIST;
	node->left = dc;
	node->right = dcList;
	return node;
}

void AddDeclaration(char* name) {
	struct Name *names = (struct Name*) malloc(sizeof(struct Name));
	names = hashSearch(name);
	if(names != NULL) {
		char temp[(strlen("Multiple declarations of ") + strlen(name) + 2)];
		strcpy(temp, "Multiple declarations of ");
	} else {
		Insert(name);
	}
}

ASTNode* CreateStatementListNode(ASTNode* st, ASTNode* stList) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_STATEMENT_LIST;
	node->left = st;
	node->next = stList;
	return node;
}

ASTNode* CreateAssignmentNode(ASTNode* ident, ASTNode* expr) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ASSIGN;
	node->left = ident;
	node->right = expr;
	return node;
}

// Take a statement node and a statement list node and connect them together
// to form a bigger statement list node (add the statement to the statement list).
// Return a pointer to the bigger list that resulted from this linking
/*
   Hash Table
 */
void Insert(char *name){
	struct Name *ident = (struct Name*) malloc(sizeof(struct Name));
	ident->name = name;
	ident->offset = GetNextOffset();
	int index = hashIndex(name);
	while(hash[index] != NULL) {
		++index;
		index %= SIZE;
	}
	hash[index] = ident;
}

int hashIndex(char *name) {
	int i;
	int key = 0;
	for(i = 0; i < strlen(name); i++) {
		key += (name[i] - '0');
	}
	int index = key % SIZE;
	return index;
}

struct Name *hashSearch(char* name) {
	int index = hashIndex(name);
	while(hash[index] != NULL) {
		if(0 == strcmp(hash[index]->name, name)) {return hash[index];}
		++index;
		index %= SIZE;
	}
	return NULL;
}

char *SearchOffset(char* name) {
	int index = hashIndex(name);
	while(hash[index] != NULL) {
		if(0 == strcmp(hash[index]->name, name)) {return hash[index]->offset;}
		++index;
		index %= SIZE;
	}
	return NULL;
}

void GenerateILOC(ASTNode* node) {
	switch(node->type) {
	case ASTNODE_DECLARATION_LIST:
		break;
	case ASTNODE_ARITH_OP:
		GenerateILOC(node->left);
		GenerateILOC(node->right);
		node->regNum = GetNextRegister();
		Emit(node->op, node->left->regNum, node->right->regNum, node->regNum);
		break;
	case ASTNODE_IDENT:
		if(node->offset == NULL) {node->offset = SearchOffset(node->name);}
		node->regNum = GetNextRegister();
		Emit(LOAD_AI, "rarp", node->offset, node->regNum);
		break;
	case ASTNODE_NUM:
		node->regNum = GetNextRegister();
		char *nodeNum = malloc(sizeof(char) * 5);
		sprintf(nodeNum, "%d", node->num);
		Emit(LOAD_I, nodeNum, NULL, node->regNum);
		break;
	case ASTNODE_ASSIGN:
		GenerateILOC(node->right);
		if(node->left->offset == NULL) {node->left->offset = SearchOffset(node->left->name);}
		Emit(STORE_AI, node->right->regNum, NULL, node->left->offset);
		break;
	case ASTNODE_STATEMENT_LIST:
		GenerateILOC(node->left);
		if(node->next != NULL) {GenerateILOC(node->next);}
		break;
	case ASTNODE_WHILE:
		node->labelNum = GetNextLabel();
		Emit(LABEL, node->labelNum, "_C:",NULL);
		GenerateILOC(node->left);
		Emit(CBR, node->labelNum, "_B_O", node->left->regNum);
		Emit(LABEL, node->labelNum, "_B:",NULL);
		GenerateILOC(node->right);
		Emit(JMP, node->labelNum, "_C",NULL);
		Emit(LABEL, node->labelNum, "_O:",NULL);
		break;
	case ASTNODE_COMPARE:
		GenerateILOC(node->left);
		GenerateILOC(node->right);
		node->regNum = GetNextRegister();
		Emit(node->op, node->left->regNum, node->right->regNum, node->regNum);
		break;
	case ASTNODE_LOGIC_OP:
		node->labelNum = GetCurrentLabel();
		GenerateILOC(node->left);
		GenerateILOC(node->right);
		node->regNum = GetNextRegister();
		Emit(node->op, node->left->regNum, node->right->regNum, node->regNum);
		break;
	case ASTNODE_IF:
		node->labelNum = GetNextLabel();
		GenerateILOC(node->left);
		Emit(CBR, node->labelNum, "_T_M", node->left->regNum);
		Emit(LABEL, node->labelNum, "_T:",NULL);
		GenerateILOC(node->right);
		Emit(LABEL, node->labelNum, "_M:",NULL);
		break;
	case ASTNODE_IF_ELSE:
		node->labelNum = GetNextLabel();
		GenerateILOC(node->left->left);
		Emit(CBR, node->labelNum, "_T_E", node->left->left->regNum);
		Emit(LABEL, node->labelNum, "_T:",NULL);
		GenerateILOC(node->left->right);
		Emit(JMP, node->labelNum, "_M",NULL);
		Emit(LABEL, node->labelNum, "_E:",NULL);
		GenerateILOC(node->right);
		Emit(LABEL, node->labelNum, "_M:",NULL);
		break;
	default:
		if (node->left != NULL) {GenerateILOC(node->left);}
		if (node->right != NULL) {GenerateILOC(node->right);}
		break;
	}
}

void Emit(ASTOp operation, char *source_1, char *source_2, char *destination){
	char *opcode;
	switch(operation) {
	case ADD_OP:
		opcode = "add";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case SUB_OP:
		opcode = "sub";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case MUL_OP:
		opcode = "mult";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case DIV_OP:
		opcode = "div";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case LT_OP:
		opcode = "cmp_LT";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case LE_OP:
		opcode = "cmp_LE";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case GT_OP:
		opcode = "cmp_GT";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case GE_OP:
		opcode = "cmp_GE";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case EQ_OP:
		opcode = "cmp_EQ";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case NE_OP:
		opcode = "cmp_NE";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case AND_OP:
		opcode = "and";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case OR_OP:
		opcode = "or";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case JMP:
		opcode = "jumpi";
		fprintf(fp, "	%s -> %s%s\n", opcode, source_1, source_2);
		break;
	case CBR:
		opcode = "cbr";
		fprintf(fp, "	%s %s -> %s%c%c, %s%c%c\n", opcode, destination, source_1, source_2[0], source_2[1], source_1, source_2[2], source_2[3]);
		break;
	case LOAD_I:
		opcode = "loadi";
		fprintf(fp, "	%s %s -> %s\n", opcode, source_1, destination);
		break;
	case LOAD_AI:
		opcode = "loadAI";
		fprintf(fp, "	%s %s, %s -> %s\n", opcode, source_1, source_2, destination);
		break;
	case STORE_AI:
		opcode = "storeAI";
		fprintf(fp, "	%s %s -> rarp, %s\n", opcode, source_1, destination);
		break;
	case LABEL:
		opcode = "label";
		fprintf(fp, "%s%s\n",source_1, source_2);
		break;
	default:
		printf("ERROR\n");
		exit(1);
		break;
	}
}

char *GetNextLabel() {
	labelNumber++;
	char *temp = malloc(sizeof(char) * 5);
	sprintf(temp, "L%d", labelNumber);
	return temp;
}

char *GetCurrentLabel() {
	char *temp = malloc(sizeof(char) * 5);
	sprintf(temp, "L%d", labelNumber);
	return temp;
}

ASTOp GetCurrentLabelType() {
	return labelType;
}

void SetCurrentLabelType(ASTOp type){
	labelType = type;
	return;
}

char *GetNextOffset(){
	char *temp = malloc(sizeof(char) * 5);
	sprintf(temp, "%d", offset);
	offset += 4;
	return temp;
}

char *GetNextRegister() {
	regNumber++;
	char *temp = malloc(sizeof(char) * 5);
	sprintf(temp, "r%d", regNumber);
	return temp;
}
