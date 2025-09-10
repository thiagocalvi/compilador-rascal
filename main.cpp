#include <iostream>
#include <string.h>
#include <stdio.h>

#include "scanner.hpp"
#include "tokens.hpp"

FILE *yyin;

extern int line_num;
extern int col_num;

int main(int argc, char **argv) {
		if (argc > 1) {
				yyin = fopen(argv[1], "r");
				if (!yyin) {
						std::cerr << "Erro: não foi possível abrir o arquivo " << argv[1] << "\n";
						return 1;
				}
		}

		yyFlexLexer scanner;
		int token;

		while (token = scanner.yylex()) {
				int token_col = col_num - strlen(scanner.YYText());

				std::cout << "Token: " << token;
				std::cout << "\t[Linha: " << line_num << ", Coluna: " << token_col << "\n]"; 
		}

		if (yyin != stdin) {
				fclose(yyin);
		}

		return 0;
}
