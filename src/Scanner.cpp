#include "Scanner.h"

Scanner::Scanner(std::string file_name_param, bool write_preprocessed_file)
{
	file_name = file_name_param;
	File_Linked* temp = new File_Linked;
	temp->the_file.open(file_name, std::ios::in);
	file = temp;
	if (write_preprocessed_file)
		preprocessed_file.open(file_name.substr(0,file_name.length() - 1) + "i");
}

Scanner::~Scanner()
{
	file->the_file.close();
	preprocessed_file.close();
}

Token* Scanner::Get_Next_Token()
{
	//Adds to line if its a one character token from last time
	if (current_lexeme.length() == 0 && last_character != '\0')
		line += last_character;
	else //else reset lexeme to empty
		current_lexeme = "";

	Token* result;

	//Ignoring whitespace and adding them to line
	while (last_character != -1 && isWhiteSpace(last_character = file->the_file.get())) {
		line += last_character;
		new_line();
	}
	if (last_character == -1) {
		if (preprocessed_file.is_open())
			preprocessed_file << line;
		line = "";
		if (file->next == nullptr)
			return new Token(Token::E0F, "-1");
		file = file->next;
		last_character = '\0';
		return Get_Next_Token();
	}

	switch (last_character) {
	case '#': //Preproccessor Macro Commands
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
	case '[':
		return new Token(Token::LEFT_BRACKET, "[");
	case ']':
		return new Token(Token::RIGHT_BRACKET, "]");
	case '=':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::EQUAL, "==");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::ASSIGN, "=");
		};
	case '-':
		get_next_character();
		switch (last_character) {
		case '-':
			return new Token(Token::DECREMENT, "--");
		case '=':
			return new Token(Token::SUBASSIGN,"-=");
		case '>':
			return new Token(Token::STRUCT_ELEMENT, "->");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::SUBTRACT, "-");
		};
	case '+':
		get_next_character();
		switch (last_character) {
		case '+':
			return new Token(Token::INCREMENT, "++");
		case '=':
			return new Token(Token::ADDASSIGN, "+=");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::ADD, "+");
		};
	case '*':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::MULASSIGN, "*=");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::MULTIPLY, "*");
		};
	case '/':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::DIVASSIGN, "/=");
		case '/': //Line Comment
			while ((last_character = file->the_file.get()) != '\n' && last_character != -1);
			line[line.length() - 1] = ' ';
			if (last_character != -1)
				line += last_character;
			new_line();
			return Get_Next_Token();
		case '*': //Block Comment
			line[line.length() - 1] = ' ';
			while ((last_character = file->the_file.get()) != -1) {
				if (last_character == '*') {
					if ((last_character = file->the_file.get()) == '/')
						return Get_Next_Token();
					else if (last_character == -1)
						return new Token(Token::ERROR1, "End of file found inside comment");
				}
			}
			return new Token(Token::ERROR1, "End of file found inside comment");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::DIVIDE, "/");
		};
	case '%':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::MODASSIGN, "%=");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::MODULUS, "%");
		};
	case '!':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::NOT_EQUAL, "!=");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::LOGICAL_NOT, "!");
		};
	case '<':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::LESS_THAN_EQUAL, "<=");
		case '<':
			get_next_character();
			switch (last_character) {
			case '=':
				return new Token(Token::BINARY_LEFT_SHIFT_ASSIGN, "<<=");
			};
			file->the_file.putback(last_character);
			return new Token(Token::BINARY_LEFT_SHIFT, "<<");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::LESS_THAN, "<");
		};
	case '>':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::GREATER_THAN_EQUAL, ">=");
		case '>':
			get_next_character();
			switch (last_character) {
			case '=':
				return new Token(Token::BINARY_RIGHT_SHIFT_ASSIGN, ">>=");
			};
			file->the_file.putback(last_character);
			return new Token(Token::BINARY_RIGHT_SHIFT, ">>");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::GREATER_THAN, ">");
		};
	case '&':
		get_next_character();
		switch (last_character) {
		case '&':
			return new Token(Token::LOGICAL_AND, "&&");
		case '=':
			return new Token(Token::BINARY_AND_ASSIGN, "&=");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::BINARY_AND,"&");
		};
	case '|':
		get_next_character();
		switch (last_character) {
		case '|':
			return new Token(Token::LOGICAL_OR, "||");
		case '=':
			return new Token(Token::BINARY_OR_ASSIGN, "|=");
		default:
			file->the_file.putback(last_character);
			return new Token(Token::BINARY_OR,"|");
		};
	case '^':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::BINARY_XOR_ASSIGN, "^=");
		default:
			file->the_file.putback(last_character);
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
			get_next_character();
			switch (last_character) {
			case 't':
				return check_keyword_identifier(Token::INT);
			};
		};
	case 'r':
		return possible_keyword("eturn",Token::RETURN);
	case 't':
		return possible_keyword("ypedef", Token::TYPEDEF);
	case 'w':
		return possible_keyword("hile", Token::WHILE);
	default:
		result = find_identifier();
		if (result != nullptr)
			return result;
		else {
			result = find_integer();
			if (result != nullptr)
				return result;
			return new Token(Token::ERROR1, "Unrecognized Token: " + std::to_string(last_character));
		}
	};
}

Token* Scanner::process_macro_command() {
	std::string macro_command, follow_up;
	file->the_file >> macro_command;
	if (macro_command == "include") {
		last_character = file->the_file.get();
		while ((last_character == ' ' || last_character == '\t'))
			last_character = file->the_file.get();
		if (last_character == '\n')
			return new Token(Token::ERROR1, "PreProccessor Command: Expects a file name.");
		if (last_character != 34 && last_character != '<')
			return new Token(Token::ERROR1, "PreProccessor Command: Unrecongized symbol");
		file->the_file >> follow_up;
		if (follow_up[follow_up.length() - 1] != last_character && follow_up[follow_up.length() - 1] != '>')
			return new Token(Token::ERROR1, "Preproccessor Command: Unrecongized symbol!");
		follow_up = follow_up.substr(0, follow_up.length() - 1);
		if (last_character == '<') {
			file = find_file_in_standard_lib(follow_up);
		}	else {
			File_Linked* temp = new File_Linked;
			temp->the_file.open(follow_up);
			temp->next = file;	
			file = temp;
		}
	}
	last_character = '\0';
	return nullptr;
}

Scanner::File_Linked* Scanner::find_file_in_standard_lib(std::string lib_name) {
	//Windows Case 10
	std::string path = "/Program Files (x86)/Windows Kits/10/Include/10.0.10150.0/ucrt/" + lib_name;
	WIN32_FIND_DATA FindFileData;
	HANDLE fHandle = FindFirstFile(path.c_str(), &FindFileData);

	File_Linked* temp = new File_Linked;
	temp->the_file.open(path);
	temp->next = file;
	return temp;

	// No linux/mac Case yet
}

Token* Scanner::find_identifier() {
	if (current_lexeme.size() != 0 || isAlpha(last_character)) {
		while (isAlpha(last_character) || isNumeric(last_character)) {
			get_next_character();
		}
		file->the_file.putback(last_character);
		return new Token(Token::IDENTIFIER, current_lexeme);
	}
	return nullptr;
}

Token* Scanner::find_integer() {
	if (isNumeric(last_character)) {
		while (isNumeric(last_character)) {
			get_next_character();
		}
		file->the_file.putback(last_character);
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
		file->the_file.putback(last_character);
		return new Token(Token::IDENTIFIER, current_lexeme);
	}
	file->the_file.putback(last_character);
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
	last_character = file->the_file.get();
}

bool Scanner::isWhiteSpace(char c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

bool Scanner::isAlpha(char& c) {
	return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '_';
}

bool Scanner::isNumeric(char& c) {
	return c >= '0' && c <= '9';
}

void Scanner::new_line() {
	if (last_character == '\n') {
		line_number++;
		if (preprocessed_file.is_open())
			preprocessed_file << line;
		line = "";
	}
}

int Scanner::get_line_number() {
	return line_number;
}