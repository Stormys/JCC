#include <string>
#include <vector>
#include <algorithm>
class SymbolTable {
public:
  SymbolTable();
  SymbolTable(SymbolTable*);
  ~SymbolTable();

  void insert(std::string&);
  std::string& lookup(std::string&);
  std::string table_results();
  int get_depth();
  SymbolTable* get_parent();
private:
  std::vector<std::string>* table;
  SymbolTable* parent = nullptr;
  int depth;
};
