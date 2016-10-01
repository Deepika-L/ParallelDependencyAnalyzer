//////////////////////////////////////////////////////////////////
// TypeAnalysis.cpp - TypeAnalysis package                      //
//                                                              //
// Application: Project 3 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Source:      Jim Fawcett, CST 4-187, 443-3948                //
//              jfawcett@twcny.rr.com                           //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <io.h>
#include <fstream>
#include <unordered_set>
#include "TypeAnalysis.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../SemiExp/itokcollection.h"
#include "../ASTree/ASTNode.h"
#include "../ASTree/ASTree.h"
#include "../Utilities/Utilities.h"

//--------------------------------------Test stub--------------------------------------------
#ifdef TEST_TYPEANALYSIS
using namespace Scanner;
using Token = std::string;
int main()
{
  TypeAnalysis<TypeTableRecord> ta;
  std::string fileSpec = "../Test/Tokenizer.cpp";
  ta.buildTable(fileSpec);
  TypeTable<TypeTableRecord> table = ta.getTable();
  TypeTable<TypeTableRecord>* nTable = new TypeTable<TypeTableRecord>;

  fileSpec = "../Test/Tokenizer.h";
  ta.buildTable(fileSpec);
  TypeTable<TypeTableRecord> table2 = ta.getTable();

  nTable->append(table);
  nTable->append(table2);
  nTable->showRecordHeader();
  nTable->showTypeTable(*nTable);
  std::cout << "\n\n";
}
#endif