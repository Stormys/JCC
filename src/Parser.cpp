#include "Parser.h"

NonTerminal DECLARATION ({ Token::INT });
NonTerminal STATEMENT_BLOCK ({ Token::RETURN}, {DECLARATION});
NonTerminal TYPES ({ Token::INTEGER });
NonTerminal VARIABLE_ASSIGNMENT ({ Token::ASSIGN });
NonTerminal FUNCTION ({Token::OPEN_PAREN});


Parser::Parser()
{
	std::string t = "test1.c";
	s = new Scanner(t);
	currentToken = (*s).Get_Next_Token();
}

Parser::~Parser()
{
}

bool Parser::expect(const NonTerminal& temp) {
	if (accept(temp))
		return true;
	std::cout << "Line " << (*s).get_line_number() << ": Unexpect Symbol: " << (*currentToken).get_lexeme() << std::endl;
	return false;
}

bool Parser::expect(Token::Kind temp) {
	if (accept(temp))
		return true;
	std::cout << "Line " << (*s).get_line_number() << ": PARSING ERROR: Found " << Token::Kind_TEXT[(*currentToken).get_kind()] << " when expected: " << Token::Kind_TEXT[temp] << std::endl;
	return false;
}

bool Parser::accept(Token::Kind temp) {
	if (have(temp)) {
		currentToken = (*s).Get_Next_Token();
		return true;
	}
	return false;
}

bool Parser::accept(const NonTerminal& temp) {
	if (have(temp)) {
		currentToken = (*s).Get_Next_Token();
		return true;
	}
	return false;
}

bool Parser::have(Token::Kind temp) {
	return (*currentToken).get_kind() == temp;
}

bool Parser::have(const NonTerminal& temp) {
	return temp.find((*currentToken).get_kind());
}

void Parser::function(bool definition) {
	expect(Token::OPEN_PAREN);
	expect(Token::CLOSE_PAREN);
	if (definition && have(Token::OPEN_CURLY)) {
		expect(Token::OPEN_CURLY);
		statement_block();
		expect(Token::CLOSE_CURLY);
	}
	else {
		expect(Token::SEMICOLON);
	}
}

void Parser::declaration(bool definition) { //Variable and Function Dec/Def
	accept(DECLARATION);
	expect(Token::IDENTIFIER);
	if (have(FUNCTION)) {
		function(definition);
	}
	 if (have(VARIABLE_ASSIGNMENT)) { //Variable definition
		expect(Token::ASSIGN);
		expect(TYPES);
		expect(Token::SEMICOLON);
	}
	else { //just a declaration
		expect(Token::SEMICOLON);
	}
}

void Parser::statement_block() { //Function Block
	while (have(STATEMENT_BLOCK)) {
		if (have(Token::RETURN)) {
			return_statement();
		}
		else if (have(DECLARATION)) {
			declaration(false);
		}
	}
}

void Parser::return_statement() {
	expect(Token::RETURN);
	accept(TYPES);
	expect(Token::SEMICOLON);
}

void Parser::statement() { //Statements(outside of functions)
	while (have(Token::INT)) {
		if (have(DECLARATION)) {
			declaration(true);
		}
	}
}

void Parser::Program() {
	statement();
	expect(Token::E0F);
}
