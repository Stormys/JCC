#include "Token.h"
#include <unordered_set>
class NonTerminal
{
public:
	NonTerminal(std::unordered_set<Token::Kind>& temp);
	~NonTerminal();
	std::unordered_set<Token::Kind> firstset;
};


