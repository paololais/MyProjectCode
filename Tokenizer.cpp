#include <string>
#include <sstream>

#include "Tokenizer.h"
#include "Exceptions.h"

void Tokenizer::tokenizeInputFile(std::ifstream &inputFile, std::vector<Token> &inputTokens)
{
	// Leggo il file carattere per carattere
	char ch;
	ch = inputFile.get();

	// variabile per parole chiave
	std::string keyword;

	while (!inputFile.eof())
	{
		if (std::isspace(ch))
		{
			// Salto lo "spazio bianco"
			ch = inputFile.get();
			continue;
		} else if (std::isalpha(ch)) {
			// variabile o keyword
			keyword.clear(); // Svuota la stringa keyword
			do {
				keyword.push_back(ch);
				ch = inputFile.get();
			} while (std::isalpha(ch));
			//"SET", "PRINT", "INPUT", "IF", "WHILE",
			//"ADD", "SUB", "MUL", "DIV", "GT", "LT", "EQ",
			//"AND", "OR", "NOT", "TRUE", "FALSE",
			//"NUM", "VAR"
			if (keyword == "SET")
			{
				inputTokens.push_back(Token{Token::SET, Token::id2word[Token::SET]});
			}
			else if (keyword == "PRINT")
			{
				inputTokens.push_back(Token{Token::PRINT, Token::id2word[Token::PRINT]});
			}
			else if (keyword == "INPUT")
			{
				inputTokens.push_back(Token{Token::INPUT, Token::id2word[Token::INPUT]});
			}
			else if (keyword == "IF")
			{
				inputTokens.push_back(Token{Token::IF, Token::id2word[Token::IF]});
			}
			else if (keyword == "WHILE")
			{
				inputTokens.push_back(Token{Token::WHILE, Token::id2word[Token::WHILE]});
			}
			else if (keyword == "ADD")
			{
				inputTokens.push_back(Token{Token::ADD, Token::id2word[Token::ADD]});
			}
			else if (keyword == "SUB")
			{
				inputTokens.push_back(Token{Token::SUB, Token::id2word[Token::SUB]});
			}
			else if (keyword == "MUL")
			{
				inputTokens.push_back(Token{Token::MUL, Token::id2word[Token::MUL]});
			}
			else if (keyword == "DIV")
			{
				inputTokens.push_back(Token{Token::DIV, Token::id2word[Token::DIV]});
			}
			else if (keyword == "GT")
			{
				inputTokens.push_back(Token{Token::GT, Token::id2word[Token::GT]});
			}
			else if (keyword == "LT")
			{
				inputTokens.push_back(Token{Token::LT, Token::id2word[Token::LT]});
			}
			else if (keyword == "EQ")
			{
				inputTokens.push_back(Token{Token::EQ, Token::id2word[Token::EQ]});
			}
			else if (keyword == "AND")
			{
				inputTokens.push_back(Token{Token::AND, Token::id2word[Token::AND]});
			}
			else if (keyword == "OR")
			{
				inputTokens.push_back(Token{Token::OR, Token::id2word[Token::OR]});
			}
			else if (keyword == "NOT")
			{
				inputTokens.push_back(Token{Token::NOT, Token::id2word[Token::NOT]});
			}
			else if (keyword == "TRUE")
			{
				inputTokens.push_back(Token{Token::TRUE, Token::id2word[Token::TRUE]});
			}
			else if (keyword == "FALSE")
			{
				inputTokens.push_back(Token{Token::FALSE, Token::id2word[Token::FALSE]});
			} 
			else if (keyword == "BLOCK")
			{
				inputTokens.push_back(Token{Token::BLOCK, Token::id2word[Token::BLOCK]});
			} 
			else
			{
				inputTokens.push_back(Token{Token::VAR, keyword});
			}	
		}
		else if (ch == '(') {
			inputTokens.push_back(Token{Token::LP, Token::id2word[Token::LP]});
			ch = inputFile.get();
		} else if (ch == ')') {
			inputTokens.push_back(Token{Token::RP, Token::id2word[Token::RP]});
			ch = inputFile.get();
		} else if (ch == '-' || std::isdigit(ch)) {
		// numero intero, può essere preceduto da segno "-"
			std::stringstream tmp{};
			tmp << ch;
			do {
				ch = inputFile.get();
				if (std::isdigit(ch))
					tmp << ch;
			} while (std::isdigit(ch));
			inputTokens.push_back(Token{Token::NUM, tmp.str()});
		} else {
			// Ci sono dei simboli non riconosciuti dal tokenizzatore nel file
			// Utilizzo il meccanismo delle eccezioni per segnalare l'errore lessicale
			std::stringstream tmp{};
			tmp << "Errore lessicale sul simbolo: " << ch;
			throw LexicalError(tmp.str());
		}
	}
}