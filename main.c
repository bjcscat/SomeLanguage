#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>

#include "lexer.h"

int main(int argc, char **argv) {

  if (argc<2) {
    puts("no input");
    return 1;
  }

  FILE* infile = fopen(argv[1],"rw");

  if (!infile) {
    puts("Missing file.");
    return 1;
  }

  struct stat fistat;
  stat("main.pi", &fistat);

  char* inputcode = malloc(fistat.st_size);
  char* inputorigin = inputcode;
  char buf = fgetc(infile);

  while (buf != EOF) {
    *(inputcode++) = buf;
    buf = fgetc(infile);
  }

	inputcode = inputorigin;
	
	lex_result lexer_result = lexer_lex(inputcode);

	for (int i = 0;i < lexer_result.tokencount; i++) {
		printf("Token ID: %i Value: %s\n",lexer_result.tokens[i].type, lexer_result.tokens[i].value);
	}

}