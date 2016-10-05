#include "Token.h"
#include <initializer_list>
#include <vector>
class NonTerminal
{
public:
	NonTerminal(const std::initializer_list<Token::Kind>& il);
	NonTerminal(const std::initializer_list<Token::Kind>& il, std::initializer_list<NonTerminal>);
	NonTerminal(const std::initializer_list<NonTerminal> il);
	~NonTerminal();
	std::vector<Token::Kind> firstset;
};
