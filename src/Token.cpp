#include "Token.h"

std::string Token::Kind_TEXT[] = { "SUBASSIGN","ADDASSIGN", "MULASSIGN", "DIVASSIGN", "MODASSIGN", "LOGICAL_NOT", "NOT_EQUAL",
"LESS_THAN", "LESS_THAN_EQUAL", "GREATER_THAN", "GREATER_THAN_EQUAL", "BINARY_LEFT_SHIFT", "BINARY_RIGHT_SHIFT", "BINARY_LEFT_SHIFT_ASSIGN", "BINARY_RIGHT_SHIFT_ASSIGN",
"LOGICAL_AND", "BINARY_AND", "BINARY_AND_ASSIGN", "LOGICAL_OR", "BINARY_OR", "BINARY_OR_ASSIGN", "BINARY_XOR", "BINARY_ONES_COMPLEMENT", "BINARY_XOR_ASSIGN",
"COMMA", "PERIOD", "LEFT_BRACKET", "RIGHT_BRACKET" , "STRUCT_ELEMENT",
"EQUAL","AUTO", "BREAK", "GOTO","LONG", "WHILE","TYPEDEF",
"DOUBLE",
"STRUCT",
"INT",
"IF",
"VOID",
"OPEN_PAREN",
"CLOSE_PAREN",
"OPEN_CURLY",
"CLOSE_CURLY",
"SEMICOLON",
"RETURN",
"ADD",
"SUBTRACT",
"DIVIDE",
"MULTIPLY",
"ASSIGN",
"MODULUS",
"INCREMENT",
"DECREMENT",
"IDENTIFIER",
"INTEGER",
"ERROR",
"E0F" };

Token::Token(Kind temp, const std::string& input)
{
	lexeme = input;
	kind = temp;
}

Token::~Token() //everything in it is static memory.
{
}

const std::string& Token::get_lexeme() {
	return lexeme;
}

Token::Kind Token::get_kind() {
	return kind;
}