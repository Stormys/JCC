#include "Compiler.h"
#include <iostream>

Compiler::Compiler()
{
//	p = new Parser();
	s = new Scanner("test1.c",true);
}

Compiler::~Compiler()
{

}

void Compiler::Start_compile() {
//	(*p).Program();
	Token* currentToken = (*s).Get_Next_Token();
	while ((*currentToken).get_kind() != Token::E0F) {
		std::cout << Token::Kind_TEXT[(*currentToken).get_kind()] <<  " - " <<  (*currentToken).get_lexeme() << std::endl;
		if ((*currentToken).get_kind() == Token::ERROR1)
			break;
		currentToken = (*s).Get_Next_Token();
	}
}

int main() {
	Compiler* c = new Compiler();
	std::cout << "Starting to compile" << std::endl;
	(*c).Start_compile();
}