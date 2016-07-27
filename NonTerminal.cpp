#include "NonTerminal.h"

NonTerminal::NonTerminal(std::unordered_set<Token::Kind>& temp)
{
	firstset = temp;
}


NonTerminal::~NonTerminal()
{
}

