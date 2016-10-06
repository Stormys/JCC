#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
  table = new std::vector<std::string>(2);
  depth = 0;
}

SymbolTable::SymbolTable(SymbolTable* parent)
{
  table = new std::vector<std::string>(2);
  this->parent = parent;
  depth = parent->get_depth() + 1;
}

SymbolTable::~SymbolTable()
{
  while (parent != nullptr) {
    SymbolTable* temp = parent;
    delete parent;
    parent = temp;
  }
  delete table;
}

int SymbolTable::get_depth() {
  return depth;
}
