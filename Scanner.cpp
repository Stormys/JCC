#include "Scanner.h"

Scanner::Scanner(std::string file_name_param)
{
	file_name = file_name_param;
	file.open(file_name, std::ios::in);
}

Scanner::~Scanner()
{
	file.close();
}

Token* Scanner::Get_Next_Token()
{
	current_lexeme = "";
	Token* result;

	while (last_character != -1 && isWhiteSpace(last_character = file.get()))
		new_line();

	if (last_character == -1)
		return new Token(Token::E0F,"-1");

	switch (last_character) {
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
			file.putback(last_character);
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
			file.putback(last_character);
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
			file.putback(last_character);
			return new Token(Token::ADD, "+");
		};
	case '*':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::MULASSIGN, "*=");
		default:
			file.putback(last_character);
			return new Token(Token::MULTIPLY, "*");
		};
	case '/':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::DIVASSIGN, "/=");
		default:
			file.putback(last_character);
			return new Token(Token::DIVIDE, "/");
		};
	case '%':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::MODASSIGN, "%=");
		default:
			file.putback(last_character);
			return new Token(Token::MODULUS, "%");
		};
	case '!':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::NOT_EQUAL, "!=");
		default:
			file.putback(last_character);
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
			file.putback(last_character);
			return new Token(Token::BINARY_LEFT_SHIFT, "<<");
		default:
			file.putback(last_character);
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
			file.putback(last_character);
			return new Token(Token::BINARY_RIGHT_SHIFT, ">>");
		default:
			file.putback(last_character);
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
			file.putback(last_character);
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
			file.putback(last_character);
			return new Token(Token::BINARY_OR,"|");
		};
	case '^':
		get_next_character();
		switch (last_character) {
		case '=':
			return new Token(Token::BINARY_XOR_ASSIGN, "^=");
		default:
			file.putback(last_character);
			return new Token(Token::BINARY_XOR, "^");
		};
	case '~':
		return new Token(Token::BINARY_ONES_COMPLEMENT, "~");
	case ',':
		return new Token(Token::COMMA, ",");
	case '.':
		return new Token(Token::PERIOD, ".");
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
		get_next_character();
		switch (last_character) {
		case 'e':
			get_next_character();
			switch (last_character) {
			case 't':
				get_next_character();
				switch (last_character) {
				case 'u':
					get_next_character();
					switch (last_character) {
					case 'r':
						get_next_character();
						switch (last_character) {
						case 'n':
							return check_keyword_identifier(Token::RETURN);
						};
					};
				};
			};
		};
	default:
		result = find_identifier();
		if (result != nullptr)
			return result;
		else {
			result = find_integer();
			if (result != nullptr)
				return result;
			return new Token(Token::ERROR, "Unrecognized Token: " + std::to_string(last_character));
		}
	};
}

Token* Scanner::find_identifier() {
	if (current_lexeme.size() != 0 || isAlpha(last_character)) {
		while (isAlpha(last_character) || isNumeric(last_character)) {
			get_next_character();
		}
		file.putback(last_character);
		return new Token(Token::IDENTIFIER, current_lexeme);
	}
	return nullptr;
}

Token* Scanner::find_integer() {
	if (isNumeric(last_character)) {
		while (isNumeric(last_character)) {
			get_next_character();
		}
		file.putback(last_character);
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
		file.putback(last_character);
		return new Token(Token::IDENTIFIER, current_lexeme);
	}
	file.putback(last_character);
	return new Token(key, current_lexeme);
}

void Scanner::get_next_character() {
	current_lexeme += last_character;
	last_character = file.get();
}

bool Scanner::isWhiteSpace(char& c) {
	return c == ' ' || c == '\n' || c == '\t';
}

bool Scanner::isAlpha(char& c) {
	return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '_';
}

bool Scanner::isNumeric(char& c) {
	return c >= '0' && c <= '9';
}

void Scanner::new_line() {
	if (last_character == '\n')
			line_number++;
}

int Scanner::get_line_number() {
	return line_number;
}