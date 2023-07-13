#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// I token della grammatica sono:
//	• una parola chiave: SET, PRINT, INPUT, IF, WHILE, ADD, SUB, MUL, DIV, GT, LT, EQ, AND, OR, NOT, TRUE, FALSE;
//	• una parentesi aperta o chiusa;
//	• un numero (definito con le regole number);
//	• un variabile (definita con le regole variable id);

// Un token è solitamente una coppia (ID, parola), dove:
// - ID è un identificativo univoco attribuito al Token
// - parola è la stringa corrispondente al Token

struct Token {
	// Definisco gli identificativi dei Token
	// constexpr -> costanti note al momento della compilazione
	// static -> costanti "a livello di classe"

	// parole chiave
	static constexpr int SET = 0;
	static constexpr int PRINT = 1;
	static constexpr int INPUT = 2;
	static constexpr int IF = 3;
	static constexpr int WHILE = 4;
	static constexpr int ADD = 5;
	static constexpr int SUB = 6;
	static constexpr int MUL = 7;
	static constexpr int DIV = 8;
	static constexpr int GT = 9;
	static constexpr int LT = 10;
	static constexpr int EQ = 11;
	static constexpr int AND = 12;
	static constexpr int OR = 13;
	static constexpr int NOT = 14;
	static constexpr int TRUE = 15;
	static constexpr int FALSE = 16;
	//	parentesi
	static constexpr int LP = 17;
	static constexpr int RP = 18;
	//	numero e variabile
	static constexpr int NUM = 19;
	static constexpr int VAR = 20;
	

	static constexpr const char* id2word[]{
		"SET", "PRINT", "INPUT", "IF", "WHILE", 
		"ADD", "SUB", "MUL", "DIV", "GT", "LT", "EQ", 
		"AND", "OR", "NOT", "TRUE", "FALSE", 
		"(", ")", "NUM", "VAR"
	};

	Token(int t, const char* w) : tag{ t }, word{ w } { }

	Token(int t, std::string w) : tag{ t }, word{ w } { }

	// La coppia (ID, parola) che costituisce il Token
	int tag;
	std::string word;
};

std::ostream& operator<<(std::ostream& os, const Token& t);

#endif