#ifndef TOKENS_HPP
#define TOKENS_HPP

enum TokenType {
		// Palavras reservadas da linguagem
		TOKEN_PROGRAM = 256,
		TOKEN_VAR,
		TOKEN_INTEGER,
		TOKEN_BOOLEAN,
		TOKEN_PROCEDURE,
		TOKEN_FUNCTION,
		TOKEN_BEGIN,
		TOKEN_END,
		TOKEN_IF,
		TOKEN_THEN,
		TOKEN_ELSE,
		TOKEN_READ,
		TOKEN_FALSE,
		TOKEN_TRUE,
		TOKEN_WHILE,
		TOKEN_DO,
		TOKEN_WRITE,
		TOKEN_OR,
		TOKEN_NOT,
		TOKEN_DIV,

		// Identificador e Número
		TOKEN_ID,
		TOKEN_NUM,

		// Símbolos especiais
		TOKEN_ASSIGN,		// :=
		TOKEN_NEQ,			// <>
		TOKEN_LT,           // <
		TOKEN_LTE,          // <=
		TOKEN_GT,          	// >
		TOKEN_GTE,			// >=
};

#endif
