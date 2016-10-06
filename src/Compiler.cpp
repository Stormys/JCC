#include "Compiler.h"
#include <iostream>

Compiler::Compiler()
{
}

Compiler::~Compiler()
{
}


// Runs Preprocessor/Scanner(Lexical Analysis)/Parser
void Compiler::Parser_compile(std::string& name) {
	Parser *p = new Parser(name);
	(*p).Program();
}

//Only runs Preprocessor/Scanner(Lexical Analysis)
void Compiler::Scanner_compile(std::string& name) {
	Scanner* s = new Scanner(name);
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
	Compiler* c;
	try {
		if (argc >= 2) {
			if (!strcmp(argv[1], "-s")) {
				std::string s(argv[2],strlen(argv[2]));
				c = new Compiler();
				(*c).Scanner_compile(s);
			}  else {
				std::string s(argv[1],strlen(argv[1]));
				c = new Compiler();
				(*c).Parser_compile(s);
			}
		}
	} catch (std::ios_base::failure e) {
		std::cout << e.what() << std::endl;
	}
}
