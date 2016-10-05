#include "NonTerminal.h"

NonTerminal::NonTerminal(const std::initializer_list<Token::Kind>& il) : firstset(il)
{
}

NonTerminal::NonTerminal(const std::initializer_list<Token::Kind>& il, const std::initializer_list<NonTerminal> il2) : firstset(il)
{
	for (auto i = il2.begin(); i != il2.end(); i++) {
		firstset.insert(firstset.begin(),i->firstset.begin(), i->firstset.end());
	}
}

NonTerminal::NonTerminal(const std::initializer_list<NonTerminal> il)
{
	firstset = {};
	for (auto i = il.begin(); i != il.end(); i++) {
		firstset.insert(firstset.begin(),i->firstset.begin(),i->firstset.end());
	}
}

NonTerminal::~NonTerminal()
{
}
