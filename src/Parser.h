/*
JCC 's LL(1) Top down Parser.
*/
#include "NonTerminal.h"
#include "Scanner.h"
#include "SymbolTable.h"
#include <iostream>
#pragma once
class Parser
{
public:
	Parser(std::string&);
	~Parser();
	void Program();
private:
	bool expect(const NonTerminal&);
	bool expect(Token::Kind temp);
	bool accept(Token::Kind temp);
	bool accept(const NonTerminal& temp);
	bool have(Token::Kind temp);
	bool have(const NonTerminal& temp);

	void declare_symbol(std::string&);

	void function(bool definition);
	void variable_assign();
	void variable_declaration();
	void declaration(bool);
	void statement();
	void statement_block();
	void return_statement();

	Scanner* s;
	Token* currentToken;
	SymbolTable* symboltable;
	std::string prev;
};
