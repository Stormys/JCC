#pragma once
#include "Token.h"
#include <fstream>
#ifdef _WIN32
#include <string.h>
#include <windows.h>
#endif
class Scanner
{
public:
	Scanner(std::string file_name_param, bool);
	~Scanner();
	Token* Get_Next_Token();
	int get_line_number();
private:
	struct File_Linked {
		std::ifstream the_file;
		File_Linked* next = nullptr;
	};

	static bool isWhiteSpace(char c);
	static bool isAlpha(char& c);
	static bool isNumeric(char& c);

	void get_next_character();
	void new_line();
	Token* find_identifier();
	Token* find_integer();
	Token* check_keyword_identifier(Token::Kind);

	Token* process_macro_command();
	File_Linked* find_file_in_standard_lib(std::string);

	std::string file_name;
	File_Linked* file;
	std::ofstream preprocessed_file;
	char last_character = '\0';
	std::string current_lexeme = "";
	std::string line;
	int line_number = 1;
};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        