#ifndef THREADPOOL_H
#define THREADPOOL_H
//////////////////////////////////////////////////////////////////
// ThreadPool.h - ThreadPool class                              //
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
* This package provides facilities to enqueue work items and
* execute them asynchronously in a threadpool. Each work item
* will be a callable object that is supplied by application code
* and is processed sequentially on a single child thread.
*
* Required Files:
* ---------------
*   - ThreadPool.h, ThreadPool.cpp
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
#include <thread>
#include <functional>
#include "Cpp11-BlockingQueue.h"

using WorkItem = std::function<void()>;

// ThreadPool class
class ThreadPool
{
public:
  ThreadPool();
  void start();
  void doWork(WorkItem* pWi);
  void wait();
  void finish();
  ~ThreadPool();
private:
  int nThreads = 3;
  std::vector <std::thread*> pThreads;
  std::thread* _pThread;
  BlockingQueue<WorkItem*> _workItemQueue;
};

#endif