#include "Scanner.h"

Scanner::Scanner(std::string& file_name_param, bool write_preprocessed_file)
{
	file_name = file_name_param;
	File_Linked* temp = new File_Linked;
	temp->the_file.open(file_name, std::ios::in);
	temp->file_name = file_name;
	file = temp;
	if (write_preprocessed_file)
		preprocessed_file.open(file_name.substr(0,file_name.length() - 1) + "i",std::ios::out);
}

Scanner::~Scanner()
{
	file->the_file.close();
	preprocessed_file.close();
}

void Scanner::get_first_non_whitespace_character() {
	while (last_character != -1 && isWhiteSpace(last_character = get_char_no_add())) {
		line += last_character;
	}
}

Token* Scanner::Get_Next_Token()
{
	Token* result;
	//Adds to line if its a one character token from last time
	if (current_lexeme.length() == 0 && last_character != '\0')
		line += last_character;
	else //else reset
		current_lexeme = "";

	if (ifs != nullptr && !ifs->is_it_true)
		if ((result = skipping_lines_until_endif(ifs->macro)) != nullptr)
			return result;

	//Ignoring whitespace and adding them to line
	get_first_non_whitespace_character();

	//End of File
	if (last_character == -1) {
		if (preprocessed_file.is_open())
			preprocessed_file << line;

		while (ifs != nullptr) {
			If_Tree_PreProcessor* temp = ifs;
			ifs = ifs->next;
			return new Token(Token::ERROR1,file->file_name + ":" + std::to_string(temp->line_number) + ": error unterminated #" + temp->macro);
		}

		file->the_file.close();
		if (file->next == nullptr)
			return new Token(Token::E0F, "-1");

		line = "";
		file = file->next;
		last_character = '\0';

		return Get_Next_Token();
	}

	first_char = true;

	switch (last_character) {
	case '#': //PreProccessor Macro Commands
		result = process_macro_command();
		if (result != nullptr)
			return result;
		return Get_Next_Token();
	case '(':
		return new Token(Token::OPEN_PAREN, "(");
	case ')':
		return new Token(Token::CLOSE_PAREN, ")");
	case '{':
		return new Token(Token::OPEN_CURLY, "{");
	case '}':
		return new Token(Token::CLOSE_CURLY, "}");
	case ';':
		return new Token(Token::SEMICOLON, ";");
	case ':':
		return new Token(Token::COLON,":");
	case '[':
		return new Token(Token::LEFT_BRACKET, "[");
	case ']':
		return new Token(Token::RIGHT_BRACKET, "]");
	case '=':
		get_next_character();
		switch (last_character) {
		case '=':
			consume_character();
			return new Token(Token::EQUAL, "==");
		default:
			return new Token(Token::ASSIGN, "=");
		};
	case '-':
		get_next_character();
		switch (last_character) {
		case '-':
			consume_character();
			return new Token(Token::DECREMENT, "--");
		case '=':
			consume_character();
			return new Token(Token::SUBASSIGN,"-=");
		case '>':
			consume_character();
			return new Token(Token::STRUCT_ELEMENT, "->");
		default:
			return new Token(Token::SUBTRACT, "-");
		};
	case '+':
		get_next_character();
		switch (last_character) {
		case '+':
			consume_character();
			return new Token(Token::INCREMENT, "++");
		case '=':
			consume_character();
			return new Token(Token::ADDASSIGN, "+=");
		default:
			return new Token(Token::ADD, "+");
		};
	case '*':
		get_next_character();
		switch (last_character) {
		case '=':
			consume_character();
			return new Token(Token::MULASSIGN, "*=");
		default:
			return new Token(Token::MULTIPLY, "*");
		};
	case '/':
		get_next_character();
		switch (last_character) {
		case '=':
			consume_character();
			return new Token(Token::DIVASSIGN, "/=");

		case '/': //Line Comment
			while ((last_character = get_char_no_add()) != '\n' && last_character != -1);
			line[line.length() - 1] = ' ';
			if (last_character != -1)
				line += last_character;
			new_line();
			return Get_Next_Token();

		case '*': //Block Comment
			line[line.length() - 1] = ' ';
			ignoreBlockComment();
			if (last_character == -1)
				return new Token(Token::ERROR1, file->file_name + ":" + std::to_string(get_line_number()) + ": error: unterminated comment");
			return Get_Next_Token();
		default:
			return new Token(Token::DIVIDE, "/");
		};
	case '\\':
		if (file->the_file.get() == '\n')
			last_character == file->the_file.get();
		else
			return new Token(Token::ERROR1,"Found \\, expect a new line afterwords");
		return Get_Next_Token();
	case '%':
		get_next_character();
		switch (last_character) {
		case '=':
			consume_character();
			return new Token(Token::MODASSIGN, "%=");
		default:
			return new Token(Token::MODULUS, "%");
		};
	case '!':
		get_next_character();
		switch (last_character) {
		case '=':
			consume_character();
			return new Token(Token::NOT_EQUAL, "!=");
		default:
			return new Token(Token::LOGICAL_NOT, "!");
		};
	case '<':
		get_next_character();
		switch (last_character) {
		case '=':
			consume_character();
			return new Token(Token::LESS_THAN_EQUAL, "<=");
		case '<':
			consume_character();
			switch (last_character) {
			case '=':
				consume_character();
				return new Token(Token::BINARY_LEFT_SHIFT_ASSIGN, "<<=");
			};
			return new Token(Token::BINARY_LEFT_SHIFT, "<<");
		default:
			return new Token(Token::LESS_THAN, "<");
		};
	case '>':
		get_next_character();
		switch (last_character) {
		case '=':
			consume_character();
			return new Token(Token::GREATER_THAN_EQUAL, ">=");
		case '>':
			get_next_character();
			switch (last_character) {
			case '=':
				consume_character();
				return new Token(Token::BINARY_RIGHT_SHIFT_ASSIGN, ">>=");
			};
			return new Token(Token::BINARY_RIGHT_SHIFT, ">>");
		default:
			return new Token(Token::GREATER_THAN, ">");
		};
	case '&':
		get_next_character();
		switch (last_character) {
		case '&':
			consume_character();
			return new Token(Token::LOGICAL_AND, "&&");
		case '=':
			consume_character();
			return new Token(Token::BINARY_AND_ASSIGN, "&=");
		default:
			return new Token(Token::BINARY_AND,"&");
		};
	case '|':
		get_next_character();
		switch (last_character) {
		case '|':
			consume_character();
			return new Token(Token::LOGICAL_OR, "||");
		case '=':
			consume_character();
			return new Token(Token::BINARY_OR_ASSIGN, "|=");
		default:
			return new Token(Token::BINARY_OR,"|");
		};
	case '^':
		get_next_character();
		switch (last_character) {
		case '=':
			consume_character();
			return new Token(Token::BINARY_XOR_ASSIGN, "^=");
		default:
			return new Token(Token::BINARY_XOR, "^");
		};
	case '~':
		return new Token(Token::BINARY_ONES_COMPLEMENT, "~");
	case ',':
		return new Token(Token::COMMA, ",");
	case '.':
		return new Token(Token::PERIOD, ".");
	case 'a':
		return possible_keyword("uto", Token::AUTO);
	case 'b':
		return possible_keyword("reak", Token::BREAK);
	case 'c':
		get_next_character();
		switch(last_character) {
		case 'a':
			return possible_keyword("se",Token::CASE);
		case 'h':
			return possible_keyword("ar",Token::CHAR);
		case 'o':
			get_next_character();
			if (last_character == 'n') {
				get_next_character();
				switch(last_character) {
				case 's':
					return possible_keyword("t", Token::CONST);
				case 't':
					return possible_keyword("inue",Token::CONTINUE);
				};
			}
		};
	case 'd':
		get_next_character();
		switch(last_character) {
		case 'e':
			return possible_keyword("fault", Token::DEFAULT);
		case 'o':
			consume_character();
			get_next_character2();
			if (last_character == 'u') {
				current_lexeme = "do";
				return possible_keyword("ble", Token::DOUBLE);
			}
			file->the_file.putback(last_character);
			last_character = 'o';
			return check_keyword_identifier(Token::DO);

		};
	case 'e':
		get_next_character();
		switch(last_character) {
		case 'l':
			return possible_keyword("se",Token::ELSE);
		case 'n':
			return possible_keyword("um",Token::ENUM);
		case 'x':
			return possible_keyword("tern",Token::EXTERN);
		};
	case 'f':
		get_next_character();
		switch(last_character) {
		case 'l':
			return possible_keyword("oat",Token::FLOAT);
		case 'o':
			return possible_keyword("r", Token::FOR);
		};
	case 'g':
		return possible_keyword("oto", Token::GOTO);
	case 'l':
		return possible_keyword("ong", Token::LONG);
	case 'i':
		get_next_character();
		switch (last_character) {
		case 'f':
			return check_keyword_identifier(Token::IF);
		case 'n':
			return possible_keyword("t",Token::INT);
		};
	case 'r':
		get_next_character();
		if (last_character == 'e') {
			get_next_character();
			switch(last_character) {
			case 't':
				return possible_keyword("urn",Token::RETURN);
			case 'g':
				return possible_keyword("ister",Token::REGISTER);
			};
		}
	case 's':
		get_next_character();
		switch(last_character) {
		case 'h':
			return possible_keyword("ort", Token::SHORT);
		case 'i':
			get_next_character();
			switch(last_character) {
			case 'g':
				return possible_keyword("ned", Token::SIGNED);
			case 'z':
				return possible_keyword("eof", Token::SIZEOF);
			};
		case 't':
			get_next_character();
			switch(last_character) {
			case 'a':
				return possible_keyword("tic", Token::STATIC);
			case 'r':
				return possible_keyword("uct", Token::STRUCT);
			};
		case 'w':
			return possible_keyword("itch",Token::SWITCH);
		}
	case 't':
		return possible_keyword("ypedef", Token::TYPEDEF);
	case 'u':
		get_next_character();
		if (last_character == 'n') {
			get_next_character();
			switch(last_character) {
			case 'i':
				return possible_keyword("on", Token::UNION);
			case 's':
				return possible_keyword("igned", Token::UNSIGNED);
			};
		}
	case 'v':
		get_next_character();
		if (last_character == 'o') {
			get_next_character();
			switch (last_character) {
			case 'i':
				return possible_keyword("d", Token::VOID);
			case 'l':
				return possible_keyword("atile", Token::VOLATILE);
			};
		}
	case 'w':
		return possible_keyword("hile", Token::WHILE);
	case '\'':
		get_next_character();
		current_lexeme = "";
		if (last_character == '\\') {
			get_next_character();
		} else if (last_character == '\'') {
			return new Token(Token::ERROR1, "Empty Character Constant");
		}
		get_next_character();
		if (last_character != '\'')
			return new Token(Token::ERROR1, "Expects ending single quote mark");
		line += last_character;
		consume_character();
		return new Token(Token::CHAR_CONSTANT, current_lexeme);
	case '"':
		get_next_character();
		current_lexeme = "";
		while (last_character != '\"') {
			get_next_character();
		}
		line += last_character;
		consume_character();
		return new Token(Token::C_STRING, current_lexeme);
	default:
		result = find_identifier();
		if (result != nullptr)
			return result;
		else {
			result = find_integer();
			if (result != nullptr)
				return result;
			return new Token(Token::ERROR1, file->file_name + "Unrecognized Token: '" + last_character + '\'');
		}
	};
}

Token* Scanner::process_macro_command() {

	std::string macro_command, follow_up;
	Token* result;
	ignoreWhiteSpaceAndComments();

	if ((last_character = file->the_file.peek()) == '\n')
		return nullptr;

	first_char = false;
	if (current_lexeme.size() != 0 || isAlpha(last_character)) {
		while (isAlpha(last_character) || isNumeric(last_character)) {
			get_next_character();
		}
	}
	macro_command = current_lexeme;
	current_lexeme = "";

	if (macro_command == "include") {

		//Ignore whitespace between macro command
		ignoreWhiteSpaceAndComments();
		last_character = file->the_file.get();

		//Check for delimiter
		if (last_character != '"' && last_character != '<')
			return new Token(Token::ERROR1, file->file_name + ":" + std::to_string(get_line_number()) + ": error: #include expects \"FILENAME\" or <FILENAME>");

		char ending_delim = (last_character == '"' ? '"' : '>' );

		while ((last_character = file->the_file.get()) != ending_delim && last_character != '\n' && last_character != -1)
			follow_up += last_character;

		if (last_character != ending_delim)
			return new Token(Token::ERROR1, file->file_name + ":" + std::to_string(get_line_number()) + ": error: missing " + ending_delim + " terminating character");

		File_Linked* temp;
		result = extra_tokens_macro_command(macro_command);

		if (ending_delim == '>') {
			temp = find_file_in_standard_lib(follow_up);
			if (temp == nullptr)
				return new Token(Token::ERROR1,file->file_name + ":" + std::to_string(get_line_number()) + ": error: " + follow_up + ": No such file or directory" );
			file = temp;

		} else {
			temp = new File_Linked;
			temp->the_file.open(follow_up);
			if (!temp->the_file.is_open())
				return new Token(Token::ERROR1,file->file_name + ":" + std::to_string(get_line_number()) + ": error: " + follow_up + ": No such file or directory" );
			temp->next = file;	
			file = temp;
		}
		return result;

	} else if (macro_command == "define") {
		if ((result = parse_for_macro_identifier("define")) != nullptr)
			return result;

		ignoreWhiteSpaceAndComments();
		while ((last_character = file->the_file.get()) != '\n' && last_character != -1)
			if (!possibleCommentLetsIgnoreIt())
				follow_up += last_character;

		file->the_file.unget();

		if (Defined_Macros.find(current_lexeme) != Defined_Macros.end()) {
			Defined_Macros[current_lexeme] = follow_up;
			return new Token(Token::WARNING, file->file_name + ":" + std::to_string(get_line_number()) + ": warning: \"" + current_lexeme + "\" redefined" );
		}
		Defined_Macros.insert({current_lexeme,follow_up});
		return nullptr;

	} else if (macro_command == "undef") {
		if ((result = parse_for_macro_identifier("undef")) != nullptr)
			return result;
		Defined_Macros.erase(current_lexeme);

	} else if (macro_command == "ifdef") {
		if ((result = parse_for_macro_identifier("ifdef")) != nullptr)
			return result;

		If_Tree_PreProcessor* temp = new If_Tree_PreProcessor;
		temp->macro = "ifdef";
		temp->next = ifs;
		temp->line_number = get_line_number();
		temp->is_it_true = Defined_Macros.find(current_lexeme) != Defined_Macros.end();
		ifs = temp;

		return extra_tokens_macro_command(macro_command);

	} else if (macro_command == "ifndef") {
		if ((result = parse_for_macro_identifier("ifndef")) != nullptr)
			return result;

		If_Tree_PreProcessor* temp = new If_Tree_PreProcessor;
		temp->macro = "ifndef";
		temp->line_number = get_line_number();
		temp->next = ifs;
		temp->is_it_true = Defined_Macros.find(current_lexeme) == Defined_Macros.end();
		ifs = temp;

		return extra_tokens_macro_command(macro_command);

	} else if (macro_command == "endif") {
		if (ifs == nullptr)
			return new Token(Token::ERROR1,file->file_name + ":" + std::to_string(get_line_number()) + ": error #endif without #if");
		ifs = ifs -> next;
		return extra_tokens_macro_command(macro_command);

	} else if (macro_command == "if") {

	} else if (macro_command == "else") {

	} else if (macro_command == "error") {
		ignoreWhiteSpaceAndComments();
		while ((last_character = file->the_file.get()) != '\n' && last_character != -1) {
			if (isWhiteSpace(last_character))
				ignoreWhiteSpaceAndComments();
			follow_up += last_character;
		}
		return new Token(Token::ERROR1,file->file_name + ":" + std::to_string(get_line_number()) + ": #error " + follow_up);
	} else if (macro_command == "pragma") {
	}

	return nullptr;
}

Token* Scanner::extra_tokens_macro_command(const std::string& macro_command) {
	ignoreWhiteSpaceAndComments();
	if (file->the_file.peek() != '\n') {
		while ((last_character = file->the_file.get()) != '\n' && last_character != -1);
		file->the_file.unget();
		return new Token(Token::WARNING,file->file_name + ":" + std::to_string(get_line_number()) + ": warning: extra tokens at end of #" + macro_command + " directive (ignoring these tokens)");
	}
	return nullptr;
}

Token* Scanner::skipping_lines_until_endif(const std::string& macro) {
	Token* result;
	while ((last_character = file->the_file.get()) != -1) {
		if (last_character == '\n') {
			ignoreWhiteSpaceAndComments();
			if ((last_character = file->the_file.get()) == '#') {
				if ((result = parse_for_macro_identifier("endif")) == nullptr) {
					if (current_lexeme == "endif"){
						ifs = ifs -> next;
						current_lexeme = "";
						if (ifs == nullptr)
							return nullptr;
					} else if (current_lexeme == "ifdef") {
						If_Tree_PreProcessor* temp = new If_Tree_PreProcessor;
						temp->macro = current_lexeme;
						temp->next = ifs;
						temp->line_number = get_line_number();
						temp->is_it_true = false;
						ifs = temp;
					}
					current_lexeme = "";
					continue;
				}
				return result;
			} else if (last_character == '\n')
				file->the_file.putback(last_character);
		}
	}
	return new Token(Token::ERROR1, file->file_name + ":" + std::to_string(ifs->line_number) + ": error: unterminated #" + macro);
}


Token* Scanner::parse_for_macro_identifier(std::string macro) {
	ignoreWhiteSpaceAndComments();
	last_character = file->the_file.peek();
	if (last_character == '\n')
		return new Token(Token::ERROR1, file->file_name + ":" + std::to_string(get_line_number()) + ": error: no macro name given in #" + macro + " directive");
	if (!isAlpha(last_character))
		return new Token(Token::ERROR1, file->file_name + ":" + std::to_string(get_line_number()) + ": error: macros names must be identifiers");

	first_char = false;
	if (current_lexeme.size() != 0 || isAlpha(last_character)) {
		while (isAlpha(last_character) || isNumeric(last_character)) {
			get_next_character();
		}
	}
	return nullptr;
}

Token* Scanner::is_defined_macro(std::string text) {
	std::unordered_map<std::string,std::string>::const_iterator is_it_a_macro = Defined_Macros.find(current_lexeme);
	if (is_it_a_macro == Defined_Macros.end())
		return new Token(Token::IDENTIFIER, current_lexeme);
	Expanded_Macro* temp = new Expanded_Macro;
	temp->macro_to_be_expanded = is_it_a_macro->second;
	temp -> macro_location = 0;
	temp -> next = macro;
	macro = temp;
	return Get_Next_Token();
}

Scanner::File_Linked* Scanner::find_file_in_standard_lib(std::string lib_name) {
	//Currently only looks in /usr/include/ (Maybe more on this later)
	File_Linked* temp = new File_Linked;
	temp->file_name = lib_name;
	temp->the_file.open("/usr/include/" + lib_name);
	temp->next = file;
	if (temp->the_file.is_open())
		return temp;
	return nullptr;
}

Token* Scanner::find_identifier() {
	if (current_lexeme.size() != 0 || isAlpha(last_character)) {
		while (isAlpha(last_character) || isNumeric(last_character)) {
			get_next_character();
		}
		return is_defined_macro(current_lexeme);
	}
	return nullptr;
}

Token* Scanner::find_integer() {
	if (isNumeric(last_character)) {
		while (isNumeric(last_character) || last_character == 'E' || last_character == 'e') {
			get_next_character();
		}
		return new Token(Token::INTEGER, current_lexeme);
	}
	return nullptr;
}

Token* Scanner::check_keyword_identifier(Token::Kind key) {
	get_next_character();
	if (isAlpha(last_character) || isNumeric(last_character)) {
		while (isAlpha(last_character) || isNumeric(last_character)) {
			get_next_character();
		}
		return is_defined_macro(current_lexeme);
	}
	return new Token(key, current_lexeme);
}

Token* Scanner::possible_keyword(std::string keyword,Token::Kind key) {
	for (char c : keyword) {
		get_next_character();
		if (last_character != c)
			return find_identifier();
	}
	return check_keyword_identifier(key);
}

void Scanner::get_next_character() {
	current_lexeme += last_character;
	line += last_character;

	if (first_char)
		first_char = false;
	else
		consume_character();
	get_next_character2();
}

void Scanner::get_next_character2() {
	if (macro != nullptr && macro->macro_location != macro->macro_to_be_expanded.length()) {
		last_character = macro->macro_to_be_expanded[macro->macro_location];
	} else if (macro != nullptr) {
		macro = macro->next;
		get_next_character2();
	} else{
		last_character = file->the_file.peek();
	}
}

void Scanner::consume_character() {
	if (macro == nullptr)
		file->the_file.get();
	else
		++macro->macro_location;
}

char Scanner::get_char_no_add() {
	if (macro != nullptr && macro->macro_location != macro->macro_to_be_expanded.length()) {
		return macro->macro_to_be_expanded[macro->macro_location++];
	} else if (macro != nullptr) {
		macro = macro->next;
		get_char_no_add();
	} else {
		last_character = file->the_file.get();
		if (last_character == '\n')
			new_line();
		return last_character;
	}
}

bool Scanner::isWhiteSpace(char& c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

bool Scanner::isAlpha(char& c) {
	return (c >= 'A' && c <= 'Z') ||(c >= 'a' && c <= 'z') || c == '_';
}

bool Scanner::isNumeric(char& c) {
	return c >= '0' && c <= '9';
}

void Scanner::new_line() {
	file->line_number++;
	if (preprocessed_file.is_open())
		preprocessed_file << line;
	line = "";
}

void Scanner::ignoreWhiteSpaceAndComments() {
	char c;
	while ((c = file->the_file.peek()) == ' ' || c == '\t' || c == '/') {
		if (c == '/') {
			last_character = file->the_file.get();
			if (file->the_file.peek() == '*')
				ignoreBlockComment();
			else {
				file->the_file.putback(last_character);
				return;
			}
		} else
		last_character = file->the_file.get();
	}
}

bool Scanner::possibleCommentLetsIgnoreIt() {
	if (last_character == '/') {
		if (file->the_file.peek() == '/') {
			while((last_character = file->the_file.get()) != '\n' && last_character != -1);
			file->the_file.unget();
			return true;
		} else if (file->the_file.peek() == '*') {
			ignoreBlockComment();
			last_character = ' ';
		}
	}
	return false;
}

void Scanner::ignoreBlockComment() {
	while ((last_character = get_char_no_add()) != -1) {
		if (last_character == '*') {
			if ((last_character = get_char_no_add()) == '/')
				return; //Block Comment Done
			else if (last_character == -1)
				return;
		}
	}
}

int Scanner::get_line_number() {
	return file->line_number;
}
