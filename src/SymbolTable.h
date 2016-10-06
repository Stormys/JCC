#include <string>
#include <vector>
class SymbolTable {
public:
  SymbolTable();
  SymbolTable(SymbolTable*);
  ~SymbolTable();

  int get_depth();
private:
  std::vector<std::string>* table;
  SymbolTable* parent = nullptr;
  int depth;
};
