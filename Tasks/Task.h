#ifndef TASK_H
#define TASK_H
//////////////////////////////////////////////////////////////////
// Task.h - Task class                                          //
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
* This package provides a wrapper around the ThreadPool package and
* has a static ThreadPool instance for the duration of the application.
* The package acts as an interface between the application code and the 
* Threadpool.
*
* Required Files:
* ---------------
*   - ThreadPool.h, ThreadPool.cpp, Task.h, Task.cpp
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
#include "../ThreadPool/ThreadPool.h"

using WorkItem = std::function<void()>;

class Task {
  static ThreadPool pool;
public:
 void enqTask(WorkItem* task);
 void start();
 void wait();
 void finish();
};

#endif