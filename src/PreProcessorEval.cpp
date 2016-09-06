//GLOBAL string to eval
long expression_8();

std::string our_string;
int placeholder = 0;

char peek() {
	return our_string[placeholder];
}

char get() {
	return our_string[placeholder++];
}

long expression4() {
	long result = get() - '0';
	while (peek() >= '0' && peek() <= '9')
		result = 10*result +get() - '0';
	return result;
}

long expression3() {
	if (peek() >= '0' && peek() <= '9')
		return expression4();
	else if (peek() == '(') {
		get();
		long result = expression_8();
		get();
		return result;
	} else if (peek() == '-')
	{
		get();
		return -expression3();
	}
	return 0;
}

long expression2() {
	long result = expression3();
	while (peek() == '!' || peek() == '~')
		if (get() == '!') 
			result = !expression3();
		else
			result = ~expression3();
	return result;
}


long expression1() {
	long result = expression2();
	while (peek() == '*' || peek() == '/')
		if (get() == '*')
			result *= expression2(); 	
		else
			result /= expression2();
	return result;
}

long expression0() {
	long result = expression1();
	while (peek() == '+' || peek() == '-')
		if (get() == '+')
			result += expression1();
		else
			result -= expression1();
	return result;
}

long expression_1() {
	long result = expression0();
	while (peek() == '<' || peek() == '>') {
		if (peek() == '<' && our_string[placeholder + 1] == '<') {
			get();
			get();
			result = result << expression0();
		} else if (peek() == '>' && our_string[placeholder +1] == '>') {
			get();
			get();
			result = result >> expression0();
		} else
			return result;
	}
	return result;
}

long expression_2() {
	long result = expression_1();
	while (peek() == '<' || peek() == '>') {
		if (get() == '<') { 
			if (peek() == '=') {
				get();
				result = result <= expression_1();
			} else
				result = result < expression_1();
		} else if (get() == '>') {
			if (peek() == '=') {
				get();
				result = result >= expression_1();
			} else
				result = result > expression_1();
		}
	}
	return result;
}

long expression_4() {
	long result = expression_2();
	while (peek() == '&') {
		if (our_string[placeholder + 1] != '&') {
			get();
			result = result & expression_2();
		} else
			return result;
	}
	return result;
}


long expression_5() {
	long result = expression_4();
	while (peek() == '^')
		if (get() == '^')
			result = result ^ expression_4();
	return result;
}


long expression_6() {
	long result = expression_2();
	while (peek() == '|') {
		if (our_string[placeholder + 1] != '|') {
			get();
			result = result | expression_2();
		} else
			return result;
	}
	return result;
}

long expression_7() {
	long result = expression_6();
	while(peek() == '&') {
		if (our_string[placeholder +1] == '&') {
			get();
			get();
			long temp = expression_6();
			result = result && temp;
		} else
			result = expression_6();
	}
	return result;
}

long expression_8() {
	long result = expression_7();
	while (peek() == '|') {
		if (our_string[placeholder + 1] == '|') {
			get();
			get();
			long temp = expression_7();
			result = result || temp;
		} else
			result = expression_7();
	}
	return result;
}

bool run_eval(std::string eval_temp) {
	our_string = eval_temp;
	placeholder = 0;
	long result = expression_8();
	return result;
}
