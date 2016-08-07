#pragma once
#include "Token.h"
#include <fstream>
#include <unordered_map>
#include <iostream>

class Scanner
{
public:
	Scanner(std::string& file_name_param, bool);
	~Scanner();
	Token* Get_Next_Token();
	int get_line_number();
private:
	struct File_Linked {
		std::ifstream the_file;
		std::string file_name;
		File_Linked* next = nullptr;
	};

	static bool isWhiteSpace(char& c);
	static bool isAlpha(char& c);
	static bool isNumeric(char& c);

	void ignoreWhiteSpaceAndComments(); //Ignores tabs and spaces and comments but not new line

	void ignoreBlockComment();

	void get_next_character();
	void consume_character();

	void get_first_non_whitespace_character();

	char get_char_no_add();

	void new_line();
	Token* find_identifier();
	Token* find_integer();
	Token* check_keyword_identifier(Token::Kind);
	Token* possible_keyword(std::string,Token::Kind);

	Token* process_macro_command();
	File_Linked * find_file_in_standard_lib(std::string);
	std::unordered_map<std::string, std::string> Defined_Macros = {};
	Token* is_defined_macro(std::string);

	bool expanding_macro = false;
	std::string macro_to_be_expanded;
	int macro_location;

	std::string file_name;
	File_Linked* file;
	std::ofstream preprocessed_file;
	char last_character = '\0';
	std::string current_lexeme = "";
	std::string line;
	bool first_char = false;
	int line_number = 1;
};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
