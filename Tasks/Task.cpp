//////////////////////////////////////////////////////////////////
// Task.cpp - Task class                                        //
//                                                              //
// Application: Project 3 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Source:      Jim Fawcett, CST 4-187, 443-3948                //
//              jfawcett@twcny.rr.com                           //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <queue>
#include <string>
#include "../ThreadPool/ThreadPool.h"
#include "Task.h"

//Static threadpool member
ThreadPool Task::pool;

//Enqueue work item to work queue
void Task::enqTask(WorkItem* task)
{
  pool.doWork(task);
}

//Start child thread that dequeus work items
void Task::start()
{
  pool.start();
}

//Wait for child thread to terminate
void Task::wait()
{
  pool.wait();
}

//Post nullptr to work queue to signal end of processing
void Task::finish()
{
  pool.finish();
}

//--------------------------------------Test stub--------------------------------------------
#ifdef TEST_TASK
int main()
{
  std::cout << "\n  main thread id = " << std::this_thread::get_id();
  Task task;
  task.start();

  // define 1st work item
  WorkItem winner;
  WorkItem wi1 = [&]() {
    std::cout << "\n  w1 working on thread " << std::this_thread::get_id();
  };

  task.enqTask(&wi1);

  // define 2nd work item
  WorkItem wi2 = [&]()
  {
    std::cout << "\n  w2 working on thread " << std::this_thread::get_id();
    size_t sum = 0;
    for (size_t i = 0; i < 100; ++i)
      sum += i;
    task.finish();
  };

  task.enqTask(&wi2);
  task.wait();
  std::cout << "\n\n";
}
#endif