#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

/*bool is_alphabet(char character) {
	if (character >= 'A' && character<='z' && !(character <= '`' && character >= '[')) {
		return true;
	}
}*/

void lexer_preprocess(char* inputstr) {
	char* inputorigin = inputstr;
	char* writeorigin = inputstr;
	while (*inputstr) {
		switch (*inputstr) {
			case '/':
				if (*(inputstr + 1) == '/') {
					while (*(inputstr) && *(inputstr) != '\n') {inputstr++;}
				}
				break;
			case '\\':
				inputstr++;
				break;
			default:
				*(writeorigin++) = *inputstr;
				break;
		}
		inputstr++;
	}
	*(writeorigin) = '\0';
	
	inputstr = realloc(inputorigin, writeorigin - inputorigin);
}

lex_result lexer_lex(char* input) {
	
	lexer_preprocess(input);

	char* scanptr = input;
	
	lex_result result = {malloc(3 * sizeof(token)), 0};
	
	while (*scanptr) {
		if (isalpha(*scanptr)) {
			char* strprescan = scanptr;
			
			while (isalnum(*(scanptr))) {
				scanptr++;
			} // scan to end of token

			token* newtoken = &(result.tokens[result.tokencount]);

			newtoken->type = TOKEN_IDENTIFIER;
			newtoken->value = malloc(scanptr - strprescan);
			
			strncpy(newtoken->value, strprescan, scanptr - strprescan);

			for (int keywordindex = 0; keywordindex < KEYWORD_COUNT; keywordindex++) {
				if (strcmp(newtoken->value, keywords[keywordindex]) == 0) {
					newtoken->type = TOKEN_KEYWORD;
					break;
				} 
			}
			
			scanptr--;
			
			result.tokencount++;
		} else if(isdigit(*scanptr)) {
			char* strprescan = scanptr;
			
			while (isdigit(*scanptr) || *scanptr == '.') {scanptr++;} // scan to end of token

			//puts(strprescan);
			result.tokens[result.tokencount] = (token) {TOKEN_NUMBER, malloc(scanptr - strprescan)};
			strncpy(result.tokens[result.tokencount].value, strprescan, scanptr - strprescan);

			scanptr--;
			
			result.tokencount++;
		} else {
			switch (*scanptr) {
				case '"': {
					char* strprescan = scanptr;

					scanptr++;

					while (*scanptr != '"') {scanptr++;} // scan to end of token
					//
					result.tokens[result.tokencount] = (token) {TOKEN_STRING, malloc(scanptr - strprescan + 1)};
					strncpy(result.tokens[result.tokencount].value, strprescan, scanptr - strprescan + 1);

					result.tokencount++;
					break;
				}
				case '\'': {
					char* strprescan = scanptr;

					scanptr++;

					while (*scanptr != '\'') {scanptr++;} // scan to end of token
					//
					result.tokens[result.tokencount] = (token) {TOKEN_STRING, malloc(scanptr - strprescan + 1)};
					strncpy(result.tokens[result.tokencount].value, strprescan, scanptr - strprescan + 1);

					result.tokencount++;
					break;
				}
				case '+':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_ADD, "+"};
					break;
				case '-':
					if (*(scanptr + 1) == '>') {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_RARROW, "->"};
						scanptr++;
					} else {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_SUB, "-"};
					}
					break;
				case '*':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_MUL, "*"};
					break;
				case '/':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_DIV, "/"};
					break;
				case '%':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_MOD, "*"};
					break;
				case '?':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_QUESTIONMARK, "*"};
					break;
				case '(':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_OPEN_PAREN, "("};
					break;
				case ')':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_CLOSE_PAREN, ")"};
					break;
				case '{':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_CLOSE_BRACE, "{"};
					break;
				case '}':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_CLOSE_BRACE, "}"};
					break;
				case ';':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_SEMICOLON, ";"};
					break;
				case ':':
					if (*(scanptr + 1) == ':') {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_DOUBLE_COLON, "::"};
						scanptr++;
					} else {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_COLON, ":"};
					}
					break;
				case '=':
					if (*(scanptr + 1) == '=') {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_EQ, "=="};
						scanptr++;
					} else {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_SET, "="};
					}
					break;
				case '!':
					if (*(scanptr + 1) == '=') {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_NOTEQ, "=="};
						scanptr++;
					} else {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_NOT, "!"};
					}
					break;
				case '<':
					if (*(scanptr + 1) == '=') {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_LE, "<="};
						scanptr++;
					} else if(*(scanptr + 1) == '-') {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_LARROW, "<-"};
						scanptr++;
					} else {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_LT, "<"};
					}
					break;
				case '>':
					if (*(scanptr + 1) == '=') {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_GE, ">="};
						scanptr++;
					} else {
						result.tokens[(result.tokencount)++] = (token) {TOKEN_GT, "<"};
					}
					break;
				case '\0':
					result.tokens[(result.tokencount)++] = (token) {TOKEN_EOF, ""};
					break;
			}
		}

		if ((result.tokencount % 3) == 0) {
			result.tokens = realloc(result.tokens, (result.tokencount + 6) * sizeof(token));
		}
		
		if (*scanptr == '\0') {
			break;
		}
		scanptr++;
	}
	return result;
}