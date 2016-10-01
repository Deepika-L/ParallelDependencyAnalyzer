#include <string>
#include <vector>
class TypeTableRecord
{
public:
  using Namespace = std::string;
  using Name = std::string;
  using Type = std::string;
  using FileName = std::string;
  using iterator = std::vector<Namespace>::iterator;

  Name& name() { return _name; }
  Type& type() { return _type; }
  FileName& fileName() { return _whereDefined; }
  void addNameSpace(const Namespace& ns)
  {
    if (find(_namespaces.begin(), _namespaces.end(), ns) == _namespaces.end())
      _namespaces.push_back(ns);
  }
  size_t numberOfNameSpaces() { return _namespaces.size(); }
  iterator begin() { return _namespaces.begin(); }
  iterator end() { return _namespaces.end(); }
  Namespace operator[](size_t n) const
  {
    if (n < 0 || n >= _namespaces.size())
      throw(std::exception("index out of range"));
    return _namespaces[n];
  }
private:
  Name _name;
  Type _type;
  FileName _whereDefined;
  std::vector<Namespace> _namespaces;
};

/////////////////////////////////////////////////////////////////////
// beginning of solution
//   - needs a few words about the record structure
//   - With a Record class that holds required information the
//     Table only needs to add records, store them, and provide access.

template<typename TableRecord>
class TypeTable
{
public:
  using iterator = typename std::vector<TableRecord>::iterator;

  void addRecord(const TableRecord& record)
  {
    _records.push_back(record);
  }
  TableRecord& operator[](size_t n)
  {
    if (n < 0 || n >= _records.size())
      throw(std::exception("index out of range"));
    return _records[n];
  }
  TableRecord operator[](size_t n) const
  {
    if (n < 0 || n >= _records.size())
      throw(std::exception("index out of range"));
    return _records[n];
  }
  iterator begin() { return _records.begin(); }
  iterator end() { return _records.end(); }
private:
  std::vector<TableRecord> _records;
};

// end of solution