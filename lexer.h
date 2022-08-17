#ifndef LANG_LEX
#define LANG_LEX

#include "parser.h"

enum TOKEN_TYPE {
	TOKEN_EOF,
	TOKEN_STRING, TOKEN_NUMBER, TOKEN_IDENTIFIER, TOKEN_KEYWORD, TOKEN_TYPE, TOKEN_SEMICOLON,
	TOKEN_LARROW, TOKEN_RARROW,
	TOKEN_OPEN_PAREN, TOKEN_CLOSE_PAREN, TOKEN_OPEN_BRACE, TOKEN_CLOSE_BRACE, TOKEN_QUESTIONMARK, TOKEN_COLON, TOKEN_DOUBLE_COLON,
	TOKEN_ADD, TOKEN_SUB, TOKEN_MUL, TOKEN_DIV, TOKEN_MOD,
	TOKEN_SET, TOKEN_NOT,
	TOKEN_EQ, TOKEN_NOTEQ, TOKEN_GT, TOKEN_GE, TOKEN_LT, TOKEN_LE,
};

typedef struct {
	enum TOKEN_TYPE type;
	char* value;
} token;

typedef struct {
	token* tokens;
	long tokencount;
} lex_result;

lex_result lexer_lex(char*);
void lexer_preprocess(char*);

#endif