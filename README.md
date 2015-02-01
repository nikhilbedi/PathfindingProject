# PathfindingProject
Project 1 for Artificial Intelligence (CS360 -- Spring 2015)

• You can compile your code with
g++ problem.cpp proj1.cpp -o proj1
For Windows, change proj1 to proj1.exe.
• If there is no compilation error, you can run your program with
./proj1 layout name question number
For Windows, change ./proj1 to proj1.exe.
• If your algorithm is correct, it will solve question question number on layout
layout name. The input layout file is layouts/layout name.lay. The resulting
path is saved in layout name.out. A record file layout name stats.out
is also generated.



Example
We provide a simple breadth-first search example for Question 1.
1. Compile the original code with
g++ problem.cpp proj1.cpp -o proj1
5http://www.mingw.org/
6http://www.cygwin.com/2. Run breadth-first search on the layout bigMaze with
./proj1 bigMaze 0
It will expand 600+ states and generate the path file bigMaze.out and the
statistics file bigMaze stats.out. Check that these files indeed exist.
3. Visualize the path and expanded states with
python pacman.py -l bigMaze
