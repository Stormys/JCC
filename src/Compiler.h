#include <string.h>
#include "Scanner.h"
class Compiler
{
public:
	Compiler(std::string&);
	~Compiler();

	void Scanner_compile();
	void Parser_compile();
private:
	Scanner* s;
};
