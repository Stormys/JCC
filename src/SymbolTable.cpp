#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
  table = new std::vector<std::string>();
  depth = 0;
}

SymbolTable::SymbolTable(SymbolTable* parent)
{
  table = new std::vector<std::string>();
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

void SymbolTable::insert(std::string& value)
{
  table->push_back(value);
}

std::string& SymbolTable::lookup(std::string& value)
{
  auto result = std::find(table->begin(),table->end(),value);
  if (result != table->end())
    return value;
  if (parent != nullptr)
    return parent->lookup(value);
  throw;
}

std::string SymbolTable::table_results()
{
  std::string result = "";
  if (parent != nullptr) {
    result += parent->table_results();
  }
  for (auto i = table->begin(); i != table->end(); i++) {
    for (int i = 0; i < depth; i++)
      result += " ";
    result += *i + '\n';
  }
  return result;
}

int SymbolTable::get_depth() {
  return depth;
}

SymbolTable* SymbolTable::get_parent() {
  return parent;
}
