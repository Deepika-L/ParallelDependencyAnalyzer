#ifndef PARALLELDEPENDENCYANALYSIS_H
#define PARALLELDEPENDENCYANALYSIS_H
/////////////////////////////////////////////////////////////////////////////
// ParallelDependencyAnalysis.h - ParallelDependencyAnalysis package       //
//                                                                         //
// Application: Project 3 for CIS 687, Spring 2016                         //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015            //
// Author:      Deepika Lakshmanan                                         //
//              dlakshma@syr.edu                                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package evaluates the dependency graph for all the packages in a specified
* file collection. For each file the analyses run asynchronously, using the
* facilities of the Task class.
*
* Required Files:
* ---------------
*   - ParallelDependencyAnalysis.h, ParallelDependencyAnalysis.cpp
*     DependencyAnalysis.h, DependencyAnalysis.cpp
*     TypeAnalysis.h ,TypeAnalysis.cpp,
*     Parser.h, Parser.cpp, SemiExpression.h, SemiExpression.cpp,
*     tokenizer.h, tokenizer.cpp,
*     ActionsAndRules.h, ActionsAndRules.cpp,
*     ConfigureParser.h, ConfigureParser.cpp,
*     ASTree.h ASTree.cpp,
*     ASTNode.h, ASTNode.cpp
*     FileMgr.h, FileSystem.h, IFileMgr.h
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
#include <queue>
#include <string>
#include <conio.h>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include "../DependencyAnalysis/DependencyAnalysis.h"
#include "../ThreadPool/Cpp11-BlockingQueue.h"
#include "../FileMgr/IFileMgr.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/FileSystem.h"
#include "../Tasks/Task.h"

using namespace FileManager;
using WorkItem = std::function<void()>;

//Class to carry out parallel dependency analysis of a collection of files
class ParallelDependencyAnalysis : public IFileEventHandler, public IDirEventHandler, public IDoneEventHandler
{
  Task* pTask;
  std::string dirPath;
  std::string filePath;
  int doneSpec;
  BlockingQueue<TypeTable<TypeTableRecord>> _partialTables;
  WorkItem ParseFile;
  WorkItem MergeT;
  WorkItem DepAnal;
  WorkItem DepDisplay;
  TypeAnalysis<TypeTableRecord> ta;
  TypeTable<TypeTableRecord> *MTable = new TypeTable<TypeTableRecord>;
  BlockingQueue<std::string> _Tfiles;
  BlockingQueue<std::string> _Dfiles;
  DependencyAnalysis da;

public:
  ParallelDependencyAnalysis(Task& t) {
    pTask = &t;
  }

  //Found directory event handler
  void IDirEventHandler::execute(const std::string& dirSpec)
  {
    dirPath = dirSpec;
  }

  //Found file event handler
  void IFileEventHandler::execute(const std::string& fileName)
  {
    std::string file = dirPath + "\\" + fileName;
    _Tfiles.enQ(file);
    _Dfiles.enQ(file);

    ParseFile = [=]()
    {
      std::string fileSpec = _Tfiles.deQ();
      std::stringstream msg;
      msg << "\n\n  Type Analysis working for " << fileSpec << " on thread " << std::this_thread::get_id() << "\n";
      std::cout << msg.str();
      msg.str("");
      ta.buildTable(fileSpec);
      TypeTable<TypeTableRecord> ttable = ta.getTable();
      _partialTables.enQ(ttable);

    };

    pTask->enqTask(&ParseFile);
  }

  //File searching done event handler
  void IDoneEventHandler::execute(const int& doneSpec)
  {
    MergeT = [=]()
    {
      std::stringstream msg;
      msg << "\n\n  Merging partial typetables working on thread " << std::this_thread::get_id() << "\n";
      std::cout << msg.str();
      msg.str("");
      TypeTable<TypeTableRecord> _Table;
      for (int i = 0; i < doneSpec; i++) {
        _Table = _partialTables.deQ();
        MTable->append(_Table);
      }

      msg << "\n\n  Finished merging partial typetables on thread " << std::this_thread::get_id() << "\n";
      std::cout << msg.str();
      msg.str("");
      endOfMerge();
    };

    pTask->enqTask(&MergeT);
  }

  //Return merged TypeTable
  TypeTable<TypeTableRecord>* getMTable() {
    return MTable;
  }

  //Get partial typetables
  BlockingQueue<TypeTable<TypeTableRecord>>* getPartials() {
    return &_partialTables;
  }

  //End of merge table event handler
  void endOfMerge() {
    MTable->showRecordHeader();
    MTable->showTypeTable(*MTable);
    std::stringstream msg;
    msg << "\n\n  Begin pass 2 to conduct dependency analysis on files...\n\n\n";
    std::cout << msg.str();
    msg.str("");

    size_t nFiles = _Dfiles.size();
    for (size_t i = 0; i < nFiles; i++) {

      DepAnal = [=]()
      {
        std::string fileSpec = _Dfiles.deQ();
        da.doAnalysis(fileSpec, MTable);
        da.display();
      };
      pTask->enqTask(&DepAnal);
    }

    pTask->finish();
  }
};
#endif