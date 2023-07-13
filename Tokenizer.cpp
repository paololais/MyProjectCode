#include <string>
#include <sstream>

#include "Tokenizer.h"
#include "Exceptions.h"

void Tokenizer::tokenizeInputFile(std::ifstream& inputFile,
	std::vector<Token>& inputTokens) {

	// Leggo il file carattere per carattere
	char ch;
	ch = inputFile.get();
	
	while (!inputFile.eof()) {
		if (std::isspace(ch)) {
			// Salto lo "spazio bianco"
			ch = inputFile.get();
			continue;
		}
		if (ch == '(') {
			inputTokens.push_back(Token{ Token::LP, Token::id2word[Token::LP] });
		} else if (ch == ')') {
			inputTokens.push_back(Token{ Token::RP, Token::id2word[Token::RP] });
		} else if (ch == '+') {
			inputTokens.push_back(Token{ Token::ADD, Token::id2word[Token::ADD] });
		} else if (ch == '-') {
			inputTokens.push_back(Token{ Token::SUB, Token::id2word[Token::SUB] });
		} else if (ch == '*') {
			inputTokens.push_back(Token{ Token::MUL, Token::id2word[Token::MUL] });
		} else if (ch == '/') {
			inputTokens.push_back(Token{ Token::DIV, Token::id2word[Token::DIV] });
		} else if (std::isdigit(ch)) {
			// Costante intera
			std::stringstream tmp{};
			tmp << ch;
			do {
				ch = inputFile.get();
				if (std::isdigit(ch)) tmp << ch;
			} while (std::isdigit(ch));
			inputTokens.push_back(Token{ Token::NUM, tmp.str() });
			continue;
		} else {
			// Ci sono dei simboli non riconosciuti dal tokenizzatore nel file
			// Utilizzo il meccanismo delle eccezioni per segnalare l'errore lessicale
			std::stringstream tmp{};
			tmp << "Errore lessicale sul simbolo: " << ch;
			throw LexicalError(tmp.str());
		}
		ch = inputFile.get();
	}
}