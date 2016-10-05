#include "Compiler.h"
#include <iostream>

Compiler::Compiler(std::string& name)
{
	s = new Scanner(name);
}

Compiler::~Compiler()
{
	delete s;
}


// Runs Preprocessor/Scanner(Lexical Analysis)/Parser
void Compiler::Parser_compile() {

}

//Only runs Preprocessor/Scanner(Lexical Analysis)
void Compiler::Scanner_compile() {
	Token* currentToken = (*s).Get_Next_Token();
	while ((*currentToken).get_kind() != Token::E0F) {
		std::cout << Token::Kind_TEXT[(*currentToken).get_kind()] <<  " - " <<  (*currentToken).get_lexeme() << std::endl;
		if ((*currentToken).get_kind() == Token::ERROR1)
			break;
		delete currentToken;
		currentToken = (*s).Get_Next_Token();
	}
	delete currentToken;
}

int main(int argc, char* argv[]) {
	std::string s(argv[1],strlen(argv[1]));
	try {
		Compiler* c = new Compiler(s);
		(*c).Scanner_compile();
		delete c;
	} catch (std::ios_base::failure e) {
		std::cout << e.what() << std::endl;
	}
}
