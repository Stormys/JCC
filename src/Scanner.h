#pragma once

#include "Token.h"
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>

class Scanner
{
public:
	Scanner(std::string& file_name_param);
	~Scanner();
	Token* Get_Next_Token();
	int get_line_number();
private:
	struct If_Tree_PreProcessor {
		std::string macro;
		If_Tree_PreProcessor* next;
		int line_number;
		bool is_it_true;
		bool else_case = false;
		bool ran_else = false;
	};

	struct File_Linked {
		std::ifstream the_file;
		std::string file_name;
		int line_number = 1;
		File_Linked* next = nullptr;
		If_Tree_PreProcessor* ifs = nullptr;
	};

	struct Macro_Info {
		std::string expanded_form;
		std::vector<std::string> params;
		bool function_macro = false;

		Macro_Info(){}
		Macro_Info(std::string hi):expanded_form(hi){}
	};

	struct Expanded_Macro {
		std::string macro_to_be_expanded;
		int macro_location;
		std::vector<std::string> param_names;
		std::vector<std::string> param_values;
		Expanded_Macro* next;
	};

	static bool isWhiteSpace(char& c);
	static bool isAlpha(char& c);
	static bool isNumeric(char& c);

	void ignoreWhiteSpaceAndComments(); //Ignores tabs and spaces and comments but not new line

	void ignoreBlockComment(); // Ignore Block comments

	bool possibleCommentLetsIgnoreIt();

	void get_next_character(); //Adds character to buffer if necessary
	void get_next_character2(); //Actually gets character
	void consume_character(); //Consumes character

	void get_first_non_whitespace_character();

	char get_char_no_add();

	void new_line();
	Token* find_identifier();
	Token* find_integer();
	Token* check_keyword_identifier(Token::Kind);
	Token* possible_keyword(std::string,Token::Kind);
	Token* process_macro_command(bool);
	Token* extra_tokens_macro_command(const std::string&,int);

	File_Linked * find_file_in_standard_lib(std::string);
	std::unordered_map<std::string, Macro_Info*> Defined_Macros{ {"__GNUC__",new Macro_Info("4")}, {"__GNUC_MINOR__",new Macro_Info("2")}, {"__STDC_VERSION__", new Macro_Info("199901")}, \
		{"__x86_64__",new Macro_Info("1")}};
	Token* is_defined_macro(std::string);
	Token* parse_for_macro_identifier(std::string,int);
	Token* skipping_lines_until_endif(const std::string&);
	std::string obtain_follow_up_defined_macro();
	void find_identifier_str();
	void backslash_ignore();
	int process_if_statement();

	Expanded_Macro* macro = nullptr;

	std::string file_name;
	File_Linked* file;
	char last_character = '\0';
	std::string current_lexeme = "";
	bool first_char = false;
	bool first_char_in_line = true;
	std::string local_path;
	bool check_for_luck = true;

	std::vector<Token::Kind> preprocessor_number_op{Token::LOGICAL_NOT,Token::ADD,Token::SUBTRACT,Token::BINARY_ONES_COMPLEMENT};

	std::vector<Token::Kind> preprocessor_if_op{Token::ADD, Token::SUBTRACT, Token::DIVIDE, Token::MULTIPLY, Token::BINARY_LEFT_SHIFT, Token::BINARY_RIGHT_SHIFT, \
	Token::LESS_THAN, Token::LESS_THAN_EQUAL, Token::GREATER_THAN, Token::GREATER_THAN_EQUAL, Token::EQUAL, Token::NOT_EQUAL, Token::BINARY_AND, Token::BINARY_OR, Token::BINARY_XOR, \
	Token::LOGICAL_AND, Token::LOGICAL_OR, Token::MODULUS};

	std::vector<Token::Kind> preprocessor_identifier{Token::IDENTIFIER,Token::INT};
};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
