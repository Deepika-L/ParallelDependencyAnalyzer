/////////////////////////////////////////////////////////////////////
// FileMgr.cpp - find files matching specified patterns            //
//               on a specified path                               //
// ver 2.0                                                         //
// Jim Fawcett, MidtermCodeSupplement, Spring 2016                 //
/////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include <iostream>

#ifdef TEST_FILEMGR

using namespace FileManager;

struct FileHandler : IFileEventHandler
{
  void execute(const std::string& fileSpec)
  {
    //return fileSpec;
    //std::cout << "\n  --   " << fileSpec;
  }
};

struct DirHandler : IDirEventHandler
{
  void execute(const std::string& dirSpec)
  {
    //return dirSpec;
    //std::cout << "\n  ++ " << dirSpec;
  }
};

struct PathHandler : IFileEventHandler, IDirEventHandler
{
  std::string dirPath;
  std::string filePath;
  void IDirEventHandler::execute(const std::string& dirSpec)
  {
    //return dirSpec;
    dirPath = dirSpec;
    //std::cout << "\n  ++ " << dirSpec;
  }
  void IFileEventHandler::execute(const std::string& fileSpec)
  {
    //return dirSpec;
    filePath = dirPath + "\\" + fileSpec;
    std::cout << "\n  ++ " << filePath;
  }
};
int main()
{
  std::cout << "\n  Testing FileMgr";
  std::cout << "\n =================";

  std::string path = FileSystem::Path::getFullFileSpec("../Test");
  IFileMgr* pFmgr = FileMgrFactory::create(path);

 // FileHandler fh;
 // DirHandler dh;
  PathHandler ph;
  
  //pFmgr->regForFiles(&fh);
  //pFmgr->regForDirs(&dh);

  pFmgr->regForFiles(&ph);
  pFmgr->regForDirs(&ph);

  //pFmgr->addPattern("*.h");
  pFmgr->addPattern("*.cpp");
  //pFmgr->addPattern("*.log");

  pFmgr->search();
//  std::cout<<dh.execute();
 // fh.execute;
  std::cout << "\n\n";
  getchar();
  return 0;
}
#endif