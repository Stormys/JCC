#pragma once
//#include "Parser.h"
#include "Scanner.h"
class Compiler
{
public:
	Compiler();
	~Compiler();
	void Start_compile();
private:
	//Parser* p;
	Scanner* s;
};

