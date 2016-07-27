#include<string>
#pragma once
class Token
{
public:
	static enum Kind { SUBASSIGN, ADDASSIGN, MULASSIGN, DIVASSIGN, MODASSIGN, LOGICAL_NOT, NOT_EQUAL, LESS_THAN, LESS_THAN_EQUAL, GREATER_THAN, GREATER_THAN_EQUAL,
		BINARY_LEFT_SHIFT, BINARY_RIGHT_SHIFT, BINARY_LEFT_SHIFT_ASSIGN, BINARY_RIGHT_SHIFT_ASSIGN,
		LOGICAL_AND, BINARY_AND, BINARY_AND_ASSIGN, LOGICAL_OR, BINARY_OR, BINARY_OR_ASSIGN,
		BINARY_XOR, BINARY_ONES_COMPLEMENT, BINARY_XOR_ASSIGN,
		COMMA, PERIOD, LEFT_BRACKET, RIGHT_BRACKET, STRUCT_ELEMENT,
		EQUAL,
		AUTO,
		DOUBLE,
		STRUCT,
		INT,
		IF,
		VOID,
		OPEN_PAREN,
		CLOSE_PAREN,
		OPEN_CURLY,
		CLOSE_CURLY,
		SEMICOLON,
		RETURN,
		ADD,
		SUBTRACT,
		DIVIDE,
		MULTIPLY,
		ASSIGN,
		MODULUS,			
		INCREMENT,
		DECREMENT,
		IDENTIFIER,
		INTEGER,
		ERROR,
		E0F
	} Type;
	static std::string Kind_TEXT[];
	Token(Kind temp, std::string lexeme);
	~Token();

	std::string get_lexeme();
	Kind get_kind();

private:
	std::string lexeme;
	Kind kind;
};