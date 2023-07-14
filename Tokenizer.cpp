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
		//"SET", "PRINT", "INPUT", "IF", "WHILE", 
		//"ADD", "SUB", "MUL", "DIV", "GT", "LT", "EQ", 
		//"AND", "OR", "NOT", "TRUE", "FALSE", 
		//"(", ")", "NUM", "VAR"
		if (ch == 'SET'){
			inputTokens.push_back(Token{ Token::SET, Token::id2word[Token::SET] });
		} else if (ch =='PRINT') {
			inputTokens.push_back(Token{ Token::PRINT, Token::id2word[Token::PRINT] });
		}  else if (ch =='INPUT') {
			inputTokens.push_back(Token{ Token::INPUT, Token::id2word[Token::INPUT] });
		} else if (ch == 'IF') {
			inputTokens.push_back(Token{ Token::IF, Token::id2word[Token::IF] });
		} else if (ch == 'WHILE') {
			inputTokens.push_back(Token{ Token::WHILE, Token::id2word[Token::WHILE] });
		} else if (ch == 'ADD') {
			inputTokens.push_back(Token{ Token::ADD, Token::id2word[Token::ADD] });
		} else if (ch == 'SUB') {
			inputTokens.push_back(Token{ Token::SUB, Token::id2word[Token::SUB] });
		} else if (ch == 'MUL') {
			inputTokens.push_back(Token{ Token::MUL, Token::id2word[Token::MUL] });
		} else if (ch == 'DIV') {
			inputTokens.push_back(Token{ Token::DIV, Token::id2word[Token::DIV] });
		} else if (ch == 'GT') {
			inputTokens.push_back(Token{ Token::GT, Token::id2word[Token::GT] });
		} else if (ch == 'LT') {
			inputTokens.push_back(Token{ Token::LT, Token::id2word[Token::LT] });
		} else if (ch == 'EQ') {
			inputTokens.push_back(Token{ Token::EQ, Token::id2word[Token::EQ] });
		} else if (ch == 'AND') {
			inputTokens.push_back(Token{ Token::AND, Token::id2word[Token::AND] });
		} else if (ch == 'OR') {
			inputTokens.push_back(Token{ Token::OR, Token::id2word[Token::OR] });
		} else if (ch == 'NOT') {
			inputTokens.push_back(Token{ Token::NOT, Token::id2word[Token::NOT] });
		} else if (ch == 'TRUE') {
			inputTokens.push_back(Token{ Token::TRUE, Token::id2word[Token::TRUE] });
		} else if (ch == 'FALSE') {
			inputTokens.push_back(Token{ Token::FALSE, Token::id2word[Token::FALSE] });
		} else if (ch == '(') {
			inputTokens.push_back(Token{ Token::LP, Token::id2word[Token::LP] });
		} else if (ch == ')') {
			inputTokens.push_back(Token{ Token::RP, Token::id2word[Token::RP] });
		} 
		// numero intero, può essere preceduto da segno "-"
		else if (ch == '-' || std::isdigit(ch)) {
			std::stringstream tmp{};
			tmp << ch;
			do {
				ch = inputFile.get();
				if (std::isdigit(ch)) tmp << ch;
			} while (std::isdigit(ch));
			inputTokens.push_back(Token{ Token::NUM, tmp.str() });
			continue;
		} 
		// variabile
		else if (std::isalpha(ch))
		{
			std::stringstream tmp{};
			tmp << ch;
			do {
				ch = inputFile.get();
				if (std::isalpha(ch)) tmp << ch;
			} while (std::isalpha(ch));
			inputTokens.push_back(Token{ Token::VAR, tmp.str() });
			continue;
		}
		else {
			// Ci sono dei simboli non riconosciuti dal tokenizzatore nel file
			// Utilizzo il meccanismo delle eccezioni per segnalare l'errore lessicale
			std::stringstream tmp{};
			tmp << "Errore lessicale sul simbolo: " << ch;
			throw LexicalError(tmp.str());
		}
		ch = inputFile.get();
	}
}