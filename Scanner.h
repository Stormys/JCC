#pragma once
#include "Token.h"
#include <fstream>

class Scanner
{
public:
	Scanner(std::string file_name_param);
	~Scanner();
	Token* Get_Next_Token();
	int get_line_number();
private:
	static bool isWhiteSpace(char& c);
	static bool isAlpha(char& c);
	static bool isNumeric(char& c);
	void get_next_character();
	void new_line();
	Token* find_identifier();
	Token* find_integer();
	Token* check_keyword_identifier(Token::Kind);

	std::string file_name;
	std::ifstream file;
	char last_character;
	std::string current_lexeme = "";
	int line_number = 1;
};