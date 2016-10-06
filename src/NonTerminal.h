#include "Token.h"
#include <initializer_list>
#include <vector>
#include <algorithm>
class NonTerminal
{
public:
	NonTerminal(const std::initializer_list<Token::Kind>& il);
	NonTerminal(const std::initializer_list<Token::Kind>& il, const std::initializer_list<NonTerminal>&);
	NonTerminal(const std::initializer_list<NonTerminal>& il);
	~NonTerminal();

	void add_other_nonterminal(const std::initializer_list<NonTerminal>& il);
	bool find(Token::Kind) const;

	std::vector<Token::Kind> firstset;
};
