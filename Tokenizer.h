#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <fstream>

#include "Token.h"

class Tokenizer {
	// Costruttore, distruttore, ecc. vanno bene i default

public: 
	// Il file viene preso per riferimento perché la lettura ne modifica lo stato!
	std::vector<Token> operator()(std::ifstream& inputFile) {
		std::vector<Token> inputTokens;
		tokenizeInputFile(inputFile, inputTokens);
		return inputTokens;
	}

private:
	// Il metodo privato prende sia il file, sia il vettore di Token per riferimento
	// non costante: il file viene letto, il vettore di Token viene riempito
	void tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens);

};

#endif
