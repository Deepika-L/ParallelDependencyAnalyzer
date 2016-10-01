#ifndef DEPENDENCYANALYSIS_H
#define DEPENDENCYANALYSIS_H
//////////////////////////////////////////////////////////////////
// DependencyAnalysis.h - DependencyAnalysis package            //
//                                                              //
// Application: Project 3 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Author:      Deepika Lakshmanan                              // 
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package finds all other files from the file collection on
* which they depend. This package  intentionally does not record
* dependencies of a file on files outside the file set,
* e.g., language and platform libraries.
*
* Required Files:
* ---------------
*   - DependencyAnalysis.h, DependencyAnalysis.cpp
*     TypeAnalysis.h ,TypeAnalysis.cpp,
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
#include <fstream>
#include <unordered_map>
#include "../ThreadPool/Cpp11-BlockingQueue.h"
#include "../Tokenizer/Tokenizer.h"
#include "../TypeAnalysis/TypeAnalysis.h"

using namespace Scanner;

//Data structure to hold dependency information of a file
class DependencyGraphRecord
{
public:

  std::string& name() { return _fname; }
  std::vector<std::string>& dependencies() { return _fdependencies; }

  //Add dependency when found
  void addDependency(std::string dep)
  {
    if (find(_fdependencies.begin(), _fdependencies.end(), dep) == _fdependencies.end())
      _fdependencies.push_back(dep);
  }

  size_t numberOfDependencies() { return _fdependencies.size(); }
  std::vector<std::string>::iterator begin() { return _fdependencies.begin(); }
  std::vector<std::string>::iterator end() { return _fdependencies.end(); }

private:
  std::string _fname;
  std::vector<std::string> _fdependencies;
};


//Dependency analysis class
class DependencyAnalysis {
private:
  BlockingQueue<DependencyGraphRecord> dependencies;

public:
  DependencyAnalysis() {}

  //Conduct dependency analysis on file
  void doAnalysis(std::string fileSpec, TypeTable<TypeTableRecord> *MTable) {
    DependencyGraphRecord nFile;
    std::stringstream msg;
    msg << "\n  Dependency Analysis for " << fileSpec << " working on thread " << std::this_thread::get_id() << "\n";
    std::cout << msg.str();
    msg.str("");
    try {
      std::ifstream in(fileSpec);
      if (!in.good()) {
        std::cout << "\n  can't open " << fileSpec << "\n\n";
        return;
      }

      nFile.name() = fileSpec;
      std::string newSpecialChars = "., :, +, +=, \n { }";
      Toker toker;
      toker.returnComments();
      toker.setSpecialTokens(newSpecialChars);
      in.clear();
      in.seekg(std::ios::beg);
      toker.attach(&in);

      do {
        std::string tok = toker.getTok();
        if (MTable->find(tok)) {
          std::unordered_map<std::string, TypeTableRecord>::iterator dependency = MTable->records().find(tok);
          if (dependency == MTable->records().end())
          {
          }
          else
          {
            nFile.addDependency(dependency->second.fileName());
          }
        }
      } while (in.good());
    }
    catch (std::exception& ex)
    {
      std::cout << "\n  " << ex.what();
    }
    dependencies.enQ(nFile);
  }

  //Display dependencies
  void display() {
    DependencyGraphRecord record;
    std::stringstream msg;
    for (size_t i = 0; i < dependencies.size(); i++) {
      record = dependencies.deQ();
      msg << "\n\n " << record.name() << " depends on :\n  ";
      if (record.dependencies().size()==1) {
        msg << " File does not depend on any other files\n ";
      }
      for (auto d : record.dependencies()) {
        if (d != record.name())
          msg << d << "\n  ";
      }
      std::cout << msg.str();
      msg.str("");
    }
  
  }
};
#endif
