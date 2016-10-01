#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
//////////////////////////////////////////////////////////////////
// TypeAnalysis.h - TypeAnalysis package                        //
//                                                              //
// Application: Project 3 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Source:      Jim Fawcett, CST 4-187, 443-3948                //
//              jfawcett@twcny.rr.com                           //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package finds all the types defined in a C++ file. It does
* this by building rules to detect type definitions and capture
* their fully qualified names and files where they are defined. It
* buils a TypeTable with this information.
*
* Required Files:
* ---------------
*   - TypeAnalysis.h ,TypeAnalysis.cpp,
*     Parser.h, Parser.cpp, SemiExpression.h, SemiExpression.cpp,
*     tokenizer.h, tokenizer.cpp,
*     ActionsAndRules.h, ActionsAndRules.cpp,
*     ConfigureParser.h, ConfigureParser.cpp,
*     ASTree.h ASTree.cpp,
*     ASTNode.h, ASTNode.cpp
*
* Build Process:
* --------------
*  devenv ParallelDependencyAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
*  Ver 1 : 1 Apr 16
*
*/
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>


//Class for a single record of TypeTable
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

  //Add namespace where type identifier found
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


//Type Table class
template<typename TableRecord>
class TypeTable
{
public:
  using iterator = typename std::unordered_map<std::string, TableRecord>::iterator;

  //Add record to type table
  void addRecord(TableRecord& record)
  {
    std::unordered_map<std::string, TypeTableRecord>::iterator duplicate = _records.find(record.name());
    if (duplicate == _records.end())
    {
      std::pair<std::string, TableRecord> nRecord(record.name(), record);
      _records.insert(nRecord);
    }
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

  //Append a partial typetable
  void append(TypeTable<TypeTableRecord> t) {
    std::unordered_map<std::string, TableRecord> nRecords = t.records();
    for (auto record : nRecords) {
      std::unordered_map<std::string, TypeTableRecord>::iterator duplicate = _records.find(record.first);
      if (duplicate == _records.end())
      {
        _records.insert(record);
      }
    }
  }

  //Show record
  void showRecord(TableRecord& record)
  {
    std::cout << "\n  ";
    std::cout << std::setw(60) << record.fileName();
    std::cout << std::setw(20) << record.name();
    std::cout << std::setw(20) << record.type();
    for (auto ns : record)
      std::cout << std::setw(20) << ns;
  }

  //Show record header
  void showRecordHeader()
  {
    std::cout << std::left << "\n  ";
    std::cout << std::setw(60) << "Filename";
    std::cout << std::setw(20) << "Name";
    std::cout << std::setw(20) << "Type";
    std::cout << std::setw(20) << "Namespaces" << "\n";
    std::cout << " " << std::setw(60) << std::string(60, '-');
    std::cout << std::setw(20) << std::string(20, '-');
    std::cout << std::setw(20) << std::string(20, '-');
    std::cout << std::setw(20) << std::string(20, '-');
  }

  //Show type table
  void showTypeTable(TypeTable<TableRecord>& table)
  {
    for (std::pair<std::string, TableRecord> record : table.records())
      showRecord(record.second);
  }

  std::unordered_map<std::string, TableRecord>& records() {
    return _records;
  }

  //Find record in typetable
  int find(std::string s) {
    try {
      TableRecord t = _records.at(s);
      return 1;
    }
    catch (std::exception) {
      return 0;
    }
  }

private:
  std::unordered_map<std::string, TableRecord> _records;
};


//Type Analysis class
template<typename TableRecord>
class TypeAnalysis {
  TypeTable<TableRecord> _table;
public:
  TypeAnalysis() {}

  //Construct partial type table
  void buildTable(std::string fileSpec) {
    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    Repository* currRepo = configure.Repo();
    TypeTable<TypeTableRecord>& ttable = currRepo->typetable();
    configure.Attach(fileSpec);
    try
    {
      while (pParser->next())
        pParser->parse();
      //std::cout << "\n";
      _table = ttable;

    }
    catch (std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
      return;
    }
  }

  //Get type table
  TypeTable<TypeTableRecord>& getTable() {
    return _table;
  }
};

#endif