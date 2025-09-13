#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

#include "scanner.hpp"
#include "tokens.hpp"

extern int line_num;
extern int col_num;

int main(int argc, char **argv) {
		std::istream* input_stream;
		std::ifstream file_stream;

		if (argc > 1) {
				file_stream.open(argv[1]);

				if (!file_stream.is_open()) {
						std::cerr << "Erro: não foi possível abrir o arquivo " << argv[1] << "\n";
						return 1;
				} 

				input_stream = &file_stream;
		}

		yyFlexLexer scanner;
		
		scanner.yyrestart(input_stream);

		int token;
				
		while ((token = scanner.yylex())) {
				int token_col = col_num - strlen(scanner.YYText());
				
				std::cout << "Token: " << token;
				
				if (token == TOKEN_ID || token == TOKEN_NUM) {
						std::cout << " (" << scanner.YYText() << ") ";
				}

				std::cout << "\t[Linha: " << line_num << ", Coluna: " << token_col << "]\n"; 
		}

		if (argc > 1) {
				file_stream.close();
		}

		return 0;
}
