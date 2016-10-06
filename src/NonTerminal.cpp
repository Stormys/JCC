#include "NonTerminal.h"

NonTerminal::NonTerminal(const std::initializer_list<Token::Kind>& il) : firstset(il)
{
}

NonTerminal::NonTerminal(const std::initializer_list<Token::Kind>& il, const std::initializer_list<NonTerminal>& il2) : firstset(il)
{
	add_other_nonterminal(il2);
}

NonTerminal::NonTerminal(const std::initializer_list<NonTerminal>& il)
{
	firstset = {};
	add_other_nonterminal(il);
}

void NonTerminal::add_other_nonterminal(const std::initializer_list<NonTerminal>& il)
{
	for (auto i = il.begin(); i != il.end(); i++) {
		firstset.insert(firstset.begin(),i->firstset.begin(),i->firstset.end());
	}
}

bool NonTerminal::find(Token::Kind needle) const
{
	return std::find(firstset.begin(),firstset.end(),needle) != firstset.end();
}

NonTerminal::~NonTerminal()
{
}
