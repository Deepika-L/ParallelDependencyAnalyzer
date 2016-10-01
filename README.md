# Parallel Dependency Analyzer

Code analysis consists of extracting lexical content from source code files, analyzing the code's syntax from its lexical content, and building an Abstract Syntax Tree (AST) that holds the results of our analysis. It is then fairly easy to build several backends that can do further analyses on the AST to construct code metrics, search for particular constructs, or some other interesting features of the code.

In this third project I have built and tested a type-based dependency analyzer that uses the analysis machinery developed in the first two projects and exercises the new thread facilities provided in the C++11 standard, as implemented in Visual Studio 2015. I have added, to the packages from Projects #1 and #2, the new packages:

﻿ThreadPool:

Provides facilities to enqueue work items and execute them asynchronously. Each work item will be a callable object2 that is supplied by application code.

﻿Tasks:

Provides a Task class that executes a specified callable object on a ThreadPool thread, using services of the ThreadPool package.

﻿TypeAnalysis:

Finds all the types defined in each of a collection of C++ source files. It does this by building rules to detect type definitions: classes, structs, enums, typedefs, and aliases, and capture their fully qualified names and files where they are defined.

﻿DependencyAnalysis:

Finds, for each file in a specified file collection, all other files from the file collection on which they depend. File A depends on file B, if and only if, it uses the name of any type or global function defined in file B. It might do that by calling a function or method of a type or by inheriting the type. Note that this intentionally does not record dependencies of a file on files outside the file set, e.g., language and platform libraries.

﻿ParallelDependencyAnalysis:

Evaluates the dependency graph for all the packages in a specified file collection. For each file the analyses run asynchronously, using the facilities of the Task class. I expect you will find it easiest to implement this in two passes over the file collection. In the first pass we find, asynchronously, all the types defined in the collection using services of the TypeAnalysis package. In the second pass we find, asynchronously, all the dependencies between files using the DependencyAnalysis package.

﻿ParallelDependencyExecutive:

Collects all of the files matching one or more patterns in a directory tree rooted at a specified path. On that collection it conducts a dependency analysis using the services of the ParallelDependencyAnalysis package. It then then displays the results by listing each of the files and there dependencies on two adjacent lines (possibly wrapped).
