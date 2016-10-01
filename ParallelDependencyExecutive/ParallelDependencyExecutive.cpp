/////////////////////////////////////////////////////////////////////////////
// ParallelDependencyExecutive.cpp - ParallelDependencyExecutive package   //
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
#include "ParallelDependencyExecutive.h"
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
#include "../ParallelDependencyAnalysis/ParallelDependencyAnalysis.h"

using namespace FileManager;

using WorkItem = std::function<void()>;

#ifdef TEST_PARALLELDEPENDENCYEXECUTIVE

int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cout << "\n  please enter root path and filename pattern to process on command line\n\n";
    return 1;
  }
  std::string dir_path = argv[1];

  Task task;
  ParallelDependencyAnalysis pda(task);
  std::string path = FileSystem::Path::getFullFileSpec(dir_path);
  IFileMgr* pFmgr = FileMgrFactory::create(path);
  for (int i = 2; i < argc; ++i) {
    pFmgr->addPattern(argv[i]);
  }
  WorkItem ParseFile;
  WorkItem MergeT;
  WorkItem DepAnal;
  task.start();
  pFmgr->regForFiles(&pda);
  pFmgr->regForDirs(&pda);
  pFmgr->regForDone(&pda);

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
}
#endif