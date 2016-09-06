#include "Compiler.h"
#include <iostream>

Compiler::Compiler(std::string& name)
{
//	p = new Parser();
	s = new Scanner(name);
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
	delete s;
}

int main(int argc, char* argv[]) {
	std::string s(argv[1],std::strlen(argv[1]));
	try {
		Compiler* c = new Compiler(s);
		(*c).Start_compile();
	} catch (std::ios_base::failure e) {
		std::cout << e.what() << std::endl;
	}
}
