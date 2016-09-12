#include "Scanner.h"
class Compiler
{
public:
	Compiler(std::string&);
	~Compiler();
	void Start_compile();
private:
	Scanner* s;
};

