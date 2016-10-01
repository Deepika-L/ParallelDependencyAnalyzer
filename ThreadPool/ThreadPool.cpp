//////////////////////////////////////////////////////////////////
// ThreadPool.cpp - ThreadPool class                            //
//                                                              //
// Application: Project 3 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Source:      Jim Fawcett, CST 4-187, 443-3948                //
//              jfawcett@twcny.rr.com                           //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <conio.h>
#include "ThreadPool.h"

//Create threadpool
ThreadPool::ThreadPool()
{}

//Wait for child thread to terminate
void ThreadPool::wait()
{
  for (auto pthread : pThreads) {
    pthread->join();
  }
}

//Post nullptr to work queue to signal end of processing
void ThreadPool::finish()
{
  for (int i = 0; i < nThreads; i++)
  {
    doWork(nullptr);
  }
}

//Enqueue work item to work queue
void ThreadPool::doWork(WorkItem* pWi)
{
  WorkItem *nWi = new WorkItem;
  nWi = pWi;
  _workItemQueue.enQ(nWi);
}

//Start child thread that dequeus work items
void ThreadPool::start()
{
  int c = 0;
  while (c < nThreads) {
    WorkItem threadProc =
      [&]() {
      while (true)
      {
        WorkItem* pWi = _workItemQueue.deQ();
        if (pWi == nullptr)
        {
          std::cout << "\n  shutting down work item processing on thread " << std::this_thread::get_id();
          return;
        }
        (*pWi)();
      }
    };
    _pThread = new std::thread(threadProc);
    pThreads.push_back(_pThread);
    c++;
  }
}

//Clean up heap
ThreadPool::~ThreadPool()
{
  for (auto pthread : pThreads) {
    delete pthread;
  }
}

//--------------------------------------Test stub--------------------------------------------
#ifdef TEST_THREADPOOL
int main()
{
  std::cout << "\n  main thread id = " << std::this_thread::get_id();

  ThreadPool pool;
  pool.start();

  // define 1st work item
  WorkItem winner;
  WorkItem wi1 = [&]() {
    std::cout << "\n  w1 working on thread " << std::this_thread::get_id();
  };

  pool.doWork(&wi1);

  // define 2nd work item
  WorkItem wi2 = [&]()
  {
    std::cout << "\n  w2 working on thread " << std::this_thread::get_id();
    size_t sum = 0;
    for (size_t i = 0; i < 100; ++i)
      sum += i;
    pool.finish();
  };

  pool.doWork(&wi2);

  pool.wait();
  std::cout << "\n\n";
}
#endif