#include <string.h>
#include "Scanner.h"
#include "Parser.h"
class Compiler
{
public:
	Compiler();
	~Compiler();

	void Scanner_compile(std::string&);
	void Parser_compile(std::string&);
private:
};
