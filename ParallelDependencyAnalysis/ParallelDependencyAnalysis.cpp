/////////////////////////////////////////////////////////////////////////////
// ParallelDependencyAnalysis.cpp - ParallelDependencyAnalysis package     //
//                                                                         //
// Application: Project 3 for CIS 687, Spring 2016                         //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015            //
// Author:      Deepika Lakshmanan                                         //
//              dlakshma@syr.edu                                           //
/////////////////////////////////////////////////////////////////////////////

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
#include "../ThreadPool/Cpp11-BlockingQueue.h"
#include "../FileMgr/IFileMgr.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/FileSystem.h"
#include "../Tasks/Task.h"
#include "ParallelDependencyAnalysis.h"

using namespace FileManager;

using WorkItem = std::function<void()>;

#ifdef TEST_PARALLELDEPENDENCYANALYSIS
//--------------------------------------Test stub----------------------------------------
int main()
{
  Task task;
  ParallelDependencyAnalysis pda(task);
  std::string path = FileSystem::Path::getFullFileSpec("../Test");
  IFileMgr* pFmgr = FileMgrFactory::create(path);
  WorkItem ParseFile;
  WorkItem MergeT;
  WorkItem DepAnal;
  task.start();
  pFmgr->regForFiles(&pda);
  pFmgr->regForDirs(&pda);
  pFmgr->regForDone(&pda);
  pFmgr->addPattern("*.cpp");
  pFmgr->addPattern("*.h");

  WorkItem findFiles = [&]() {
    std::stringstream msg;
    msg << "\n  Finding files working on thread " << std::this_thread::get_id() << "\n";
    std::cout << msg.str();
    msg.str("");
    pFmgr->search();
    msg << "\n  Done looking for files on thread " << std::this_thread::get_id() << "\n";
    std::cout << msg.str();
    msg.str("");
  };
  task.enqTask(&findFiles);

  task.wait();
  std::cout << "\n\n";
 // getchar();
}
#endif