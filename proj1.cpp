#include <queue>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include "problem.h"

using namespace std;
/// you can add whatever helper functions/variables you need here.

string computeDirection(pair<int,int> start, pair<int,int> goal)
{
	int x = goal.first - start.first; // if 1, East. -1, West
	if(x > 0)
		return "East";
	else(x < 0)
		return "West";

	int y = goal.second - start.second; // if 1, South. -1, North
	if(y > 0)
		return "South";
	else(y < 0)
		return "North";

	return "";
}

int computeManhattanDistance(pair<int,int> start, pair<int,int> goal)
{
	int xDist = start.first - goal.first;
	int yDist = start.second - goal.second;
	if(xDist < 0)	xDist *= -1;
	if(yDist < 0)	yDist *= -1;

	return xDist + yDist;
}

pair<int,int>& obtainSmallestCostNode(map<pair<int, int>, int > &gValues, set< pair<int, int> >& s, pair<int, int> &goal)
{
	// if(s.empty())
	// 	return 0;
	int min = 100000;
	pair<int, int> n;
	set< pair<int, int> >::iterator it = s.begin();
	for(; it != s.end(); it++)
	{
		pair<int, int> node = *it;
		map< pair<int,int>, int >::iterator nodeIterator = gValues.find(node);
		int g = nodeIterator->second;
		int f = computeManhattanDistance(nodeIterator->first, goal) + g;
		if(f < min)
		{
			n = node;
			min = f;
		}
	}
	return n;
}

vector<string> questionOne(Problem &problem)
{
	vector<string> path;
	set< pair<int,int> > closedSet;
	set< pair<int,int> > openSet;
	map< pair<int,int>, pair<int,int> > parents; 
	map< pair<int,int>, int> gValues; 
	pair<int,int> current = problem.getStartState();
	pair<int,int> start = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();

	// If there is no goal, return empty
	if (goals.size() <= 0)
		return vector<string>();

	// Add the first node to the closed set
	// The closed set contains nodes that have already been evaluated
	pair<int,int> goal = goals[0];
	closedSet.insert(current);
	gValues.insert(make_pair(current, 0));
	do 
	{
		//cout << "Current State: " << current->location.first << " " << current->location.second << endl;
		// For each node adjacent to current, evaluate it
		vector<pair<int, int> > neighbors = problem.getSuccessors(current);
		for(int i=0; i<neighbors.size(); ++i)
		{
			pair<int,int> n = neighbors[i];
			// If this node is in the closedSet, simply continue to the next neighbor
			if(closedSet.find(n) != closedSet.end())
			{
				continue;
			}

			// If this node is in the openSet, check if we should reassign the parent
			set< pair<int,int> >::iterator it = openSet.find(n);
			if (it != openSet.end())	// check if this searches by value, not reference (otherwise, modify comparison operator)
			{
				// Obtain current g value
				map< pair<int,int>, int >::iterator currentNode = gValues.find(current);
				int current_g = currentNode->second;
				int new_g = current_g + 1;

				// Obtain n's g value (if the n is in the openSet, it is strictly assumed it has been assigned a g-value)
				map< pair<int,int>, int >::iterator nNode = gValues.find(n);
				int old_g = nNode->second;

				// if current is a better parent, replace
				if(new_g < old_g)	
				{
					nNode->second = new_g;
					//gValues.insert(make_pair(n, current_g + 1));
				}
			}
			// else, compute heuristic and add to open set
			else 
			{
				parents.insert(make_pair(n, current)); 	// n's parent is the current node
				//n->h = computeManhattanDistance(n, goal);
				// Obtain current G value, and use that to compute n's g value
				map< pair<int,int>, int >::iterator currentNode = gValues.find(current);
				int current_g = currentNode->second;
				gValues.insert(make_pair(n, current_g + 1));
				openSet.insert(n);
			}
		}

		// if the open set is empty, we are done
		if(openSet.empty())
			break;

		// Grab the node with smallest f value off of openset, and move current to the closed set
		pair <int, int> pastState = current;
		current = obtainSmallestCostNode(gValues, openSet, goal);
		closedSet.insert(current);
		openSet.erase(current);
		path.push_back(computeDirection(pastState, current));
	}
	while(current.first != goal.first || current.second != goal.second);	// TODO verify you are performing a deep comparison (x1=x2, y1=y2)

	// Reverse path
	vector<string> reversePath;
	for(int i = 0; i < path.size(); i++)
		reversePath[i] = path[path.size()-i-1];
	return reversePath;
}

vector<string> questionTwo(Problem &problem)
{
	/// write your own code here
	return vector<string>();
}


vector<string> questionThree(Problem &problem)
{
	/// write your own code here
	return vector<string>();
}

vector<string> questionFour(Problem &problem)
{
	/// write your own code here
	return vector<string>();	
}

//int getF(map<pair<int, int>, pair<int,int> > &parents, pair<int, int> goal)



/// Do not change codes below
vector<string> getPath(map<pair<int, int>, pair<int,int> > &parent, pair<int, int> goal)
{
	vector<string> path;
	pair<int, int> state = goal;
	int dx, dy;
	while (parent[state] != state)
	{
		dx = state.first - parent[state].first;
		dy = state.second - parent[state].second;
		if (dx>0)
			path.push_back("South");
		else if (dx<0)
			path.push_back("North");
		else if (dy>0)
			path.push_back("East");
		else
			path.push_back("West");
		state = parent[state];
	}
	reverse(path.begin(), path.end());
	return path;
}

vector<string> questionZero(Problem &problem)
{
	// A simple BFS to find the path from the start to the first goal.
	queue<pair<int, int> > q;
	map<pair<int, int>, pair<int, int> > parent;
	pair<int, int> start = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();
	if (goals.size() <= 0)
		return vector<string>();
	pair<int, int> goal = goals[0];
	q.push(start);
	parent[start] = start;
	while (!q.empty())
	{
		pair<int, int> thisState=q.front();
		q.pop();
		vector<pair<int, int> > successors = problem.getSuccessors(thisState);
		for(int i=0; i<successors.size(); ++i)
		{
			if (parent.count(successors[i]) == 0)
			{
				q.push(successors[i]);
				parent[successors[i]] = thisState;	
			}
		}
		if (parent.count(goal) != 0)
		{
			return getPath(parent, goal);
		}
	}
	return vector<string>();
}

void error()
{
	cout <<"run \'proj1 layout_name question_number\'" <<endl;
}

int main(int argc, char**argv)
{
	if (argc<3)	
	{
		error();
		return 0;
	}
	vector<string> _board;
	_board.clear();
	string namePattern = argv[1];
	string inputName; 
#ifdef _WIN32
	inputName = "layouts\\" + namePattern + ".lay";
#else
	inputName = "layouts/" + namePattern + ".lay";
#endif
	string outputName = namePattern + ".out";
	string queryName = namePattern + "_stats.out";
	try {
		ifstream fin;
		fin.open(inputName.c_str());
		while (!fin.eof())
		{
			string str;
			getline(fin, str);
			if (!fin.eof())
				_board.push_back(str);
		}
		fin.close();
	} catch (...) {
		cout << "Error while loading the layout file..." << endl;
		return 1;
	}
	Problem problem(_board);
	vector<string> _path;
	switch (argv[2][0]){
		case '1': _path = questionOne(problem); break;
		case '2': _path = questionTwo(problem); break;
		case '3': _path = questionThree(problem); break;
		case '4': _path = questionFour(problem); break;
		default: _path = questionZero(problem);
	}
	try {
		ofstream fout;
		fout.open(outputName.c_str());
		for(int i=0; i<_path.size(); ++i)
		{
			fout << _path[i] << endl;
		}
		fout.close();
	} catch (...){
		cout << "Error while saving the results..." << endl;
		return 1;
	}
	cout << "Number of expanded states: " << problem.getExpansionCounts() << endl;
	cout << "Results are saved in " << outputName << endl; 
	problem.dumpQueries(queryName);
	return 0;
}
