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
		int line_number = 1;
		File_Linked* next = nullptr;
	};

	struct Expanded_Macro {
		std::string macro_to_be_expanded;
		int macro_location;
		Expanded_Macro* next;
	};

	struct If_Tree_PreProcessor {
		std::string macro;
		If_Tree_PreProcessor* next;
		int line_number;
		bool is_it_true;
	};

	static bool isWhiteSpace(char& c);
	static bool isAlpha(char& c);
	static bool isNumeric(char& c);

	void ignoreWhiteSpaceAndComments(); //Ignores tabs and spaces and comments but not new line

	void ignoreBlockComment();

	bool possibleCommentLetsIgnoreIt();

	void get_next_character();
	void get_next_character2();
	void consume_character();

	void get_first_non_whitespace_character();

	char get_char_no_add();

	void new_line();
	Token* find_identifier();
	Token* find_integer();
	Token* check_keyword_identifier(Token::Kind);
	Token* possible_keyword(std::string,Token::Kind);
	Token* process_macro_command();
	Token* extra_tokens_macro_command(const std::string&);

	File_Linked * find_file_in_standard_lib(std::string);
	std::unordered_map<std::string, std::string> Defined_Macros = {};
	Token* is_defined_macro(std::string);
	Token* parse_for_macro_identifier(std::string);
	Token* skipping_lines_until_endif(const std::string&);

	Expanded_Macro* macro = nullptr;
	If_Tree_PreProcessor* ifs = nullptr;

	std::string file_name;
	File_Linked* file;
	std::ofstream preprocessed_file;
	char last_character = '\0';
	std::string current_lexeme = "";
	std::string line;
	bool first_char = false;
};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
