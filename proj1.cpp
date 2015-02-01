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

struct Node 
{
	Node* parent;
	pair<int, int> location;
	int f;
	int g;
	int h;

	Node(pair<int, int> loc)
	{
		location = loc;
		f = 0;
		g = 0;
		h = 0;
		parent = NULL;
	}

	bool operator==(Node& other)
	{
		return this->location.first == other.location.first && this->location.second == other.location.second;
	}
};

int computeManhattanDistance(Node* start, Node* goal)
{
	int xDist = start->location.first - goal->location.first;
	int yDist = start->location.second - goal->location.second;
	if(xDist < 0)	xDist *= -1;
	if(yDist < 0)	yDist *= -1;

	return xDist + yDist;
}

Node* obtainSmallestCostNode(set<Node*>& s)
{
	if(s.empty())
		return NULL;
	int min = 100000;
	Node* n = NULL;
	set<Node*>::iterator it = s.begin();
	for(; it != s.end(); it++)
	{
		if((*it)->f < min)
		{
			n = *it;
			min = n->f;
		}
	}
	return n;
}

vector<string> questionOne(Problem &problem)
{
	/// write your own code here

	set<Node*> closedSet;
	set<Node*> openSet;
	Node* current = new Node(problem.getStartState());
	Node* start = current;
	vector<pair<int, int> > goals = problem.getGoals();

	// If there is no goal, return empty
	if (goals.size() <= 0)
		return vector<string>();

	// Add the first node to the closed set
	// The closed set contains nodes that have already been evaluated
	Node* goal = new Node(goals[0]);
	closedSet.insert(current);

	do 
	{
		//cout << "Current State: " << current->location.first << " " << current->location.second << endl;
		// For each node adjacent to current, evaluate it
		vector<pair<int, int> > neighbors = problem.getSuccessors(current->location);
		for(int i=0; i<neighbors.size(); ++i)
		{
			Node* n = new Node(neighbors[i]);
			// If this node is in the closedSet, simply continue
			if(closedSet.find(n) != closedSet.end())
			{
				continue;
			}

			// If this node is in the openSet, check if we should reassign the parent
			set<Node*>::iterator it = openSet.find(n);
			if (it != openSet.end())	// check if this searches by value, not reference (otherwise, modify comparison operator)
			{
				int new_g = current->g + 1;
				if(new_g < (*it)->g)	// if current is a better parent, replace
				{
					(*it)->g = new_g;
					(*it)->f = (*it)->h + (*it)->g; // h(x) + g(x)
				}
			}
			// else, compute heuristic and add to open set
			else 
			{
				n->parent = current;
				n->h = computeManhattanDistance(n, goal);
				n->g = current->g + 1;
				n->f = n->g + n->h;
				openSet.insert(n);
			}
		}

		// if the open set is empty, we are done
		if(openSet.empty())
			break;

		// Grab the node with smallest f value off of openset, and move current to the closed set
		current = obtainSmallestCostNode(openSet);
		closedSet.insert(current);
		openSet.erase(current);
	}
	while(current->location.first != goal->location.first || current->location.second != goal->location.second);	// TODO verify you are performing a deep comparison (x1=x2, y1=y2)
	
	return vector<string>();
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
