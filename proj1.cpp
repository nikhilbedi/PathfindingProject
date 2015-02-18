#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include "problem.h"

using namespace std;

// Name: Nikhil Bedi
// SID: 5960795379

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

/// you can add whatever helper functions/variables you need here.

int computeManhattanDistance(pair<int,int> start, pair<int,int> goal)
{
	int xDist = start.first - goal.first;
	int yDist = start.second - goal.second;
	if(xDist < 0)	xDist *= -1;
	if(yDist < 0)	yDist *= -1;

	return xDist + yDist;
}

int computeEuclideanDistance(pair<int,int> start, pair<int,int> goal)
{
	// Optimizations
		// We don't compute the square root because it is expensive and unnecessary
	int xDist = start.first - goal.first;
	int yDist = start.second - goal.second;
	xDist *= xDist;
	yDist *= yDist;
	return xDist + yDist;
}

pair<int,int>& obtainSmallestCostNode(map<pair<int, int>, int > &gValues, set< pair<int, int> >& s, pair<int, int> &goal, float weight)
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
		float h = (float) computeManhattanDistance(nodeIterator->first, goal) * weight;
		float f = h + (float)g;
		if(f < min)
		{
			n = node;
			min = f;
		}
	}
	return n;
}

vector<string> aStar(pair<int,int>& current, pair<int,int>& goal, Problem& problem)
{
	vector<string> path;
	set< pair<int,int> > closedSet;
	set< pair<int,int> > openSet;
	map< pair<int,int>, pair<int,int> > parents; 
	map< pair<int,int>, int> gValues; 

	// Add the first node to the closed set
	// The closed set contains nodes that have already been evaluated
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
		current = obtainSmallestCostNode(gValues, openSet, goal, 1);
		closedSet.insert(current);
		openSet.erase(current);
	}
	while(current.first != goal.first || current.second != goal.second);

	// Remove first element, because for some reason it's garbage :P
	path = getPath(parents, goal);
	path.erase(path.begin());
	return path;
}

vector<string> aStarTricky(pair<int,int>& current, pair<int,int>& goal, Problem& problem, vector< pair<int,int> >& goals, int weight)
{
	vector<string> path;
	set< pair<int,int> > closedSet;
	set< pair<int,int> > openSet;
	map< pair<int,int>, pair<int,int> > parents; 
	map< pair<int,int>, int> gValues; 

	// Add the first node to the closed set
	// The closed set contains nodes that have already been evaluated
	closedSet.insert(current);
	gValues.insert(make_pair(current, 0));
	do 
	{
		// For each node adjacent to current, evaluate it
		vector<pair<int, int> > neighbors = problem.getSuccessors(current);

		// A Star tricky addition. For each neighbor, check if it is a goal that is NOT the goal we're looking for
		for(int i =0; i < neighbors.size(); i++)
		{
			pair<int,int> node = neighbors[i];
			if (node.first != goal.first || node.second != goal.second)
			{
				for(int j = 0; j < goals.size(); j++)
				{
					if(goals[j].first == node.first && goals[j].second == node.second)
					{
						// Reassign goal?
						goal = goals[j];
						goals.erase(goals.begin() + j);
						break;
					}
				}
			}
		}

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
		current = obtainSmallestCostNode(gValues, openSet, goal, weight);
		closedSet.insert(current);
		openSet.erase(current);
	}
	while(current.first != goal.first || current.second != goal.second);

	// Remove first element, because for some reason it's garbage :P
	path = getPath(parents, goal);
	path.erase(path.begin());
	return path;
}

vector<string> questionOne(Problem &problem)
{
	pair<int,int> current = problem.getStartState();
	pair<int,int> start = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();

	// If there is no goal, return empty
	if (goals.size() <= 0)
		return vector<string>();

	pair<int,int> goal = goals[0];

	return aStar(current, goal, problem);
}

vector<string> questionTwo(Problem &problem)
{
	vector<string> mergedPath;

	pair<int,int> current = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();

	// If there is no goal, return empty
	if (goals.size() <= 0)
		return vector<string>();

	
	// While there is still a goal
	//int i = 0;
	while(!goals.empty())
	{
		// Assign goal to be closest Manhattan distance from the current state
		pair<int,int> goal;
		int min = 1000000;
		int indexToRemove = 0;
		for(int i = 0; i < goals.size(); i++)
		{
			int temp = computeManhattanDistance(current, goals[i]);
			if(temp < min)
			{
				min = temp;
				goal = goals[i];
				indexToRemove = i;
			}
		}

		// remove the chosen goal from the goals list
		goals.erase(goals.begin() + indexToRemove);

		// Obtain path and add to mergedPath
		vector<string> path = aStar(current, goal, problem);
		mergedPath.insert(mergedPath.end(), path.begin(), path.end());
	}

	return mergedPath;
}

int getCostOfActualPath(pair<int,int>& start, map< pair<int, int>, map< pair<int, int>, int > > distanceMatrix)
{
	if(distanceMatrix.empty())
	{
		cout << "Distance matrix is empty!" << endl;
		return 0;
	}
	// for each item in the distance matrix for the key, start
	if(distanceMatrix.find(start) == distanceMatrix.end())
	{
		cout << "Size: " << distanceMatrix.size() << endl;
		return 0;
	}
	map< pair<int,int>, int> myMap = distanceMatrix.find(start)->second;
	map< pair<int,int>, int>::iterator it;
	int min = 100000;
	pair<int,int> newStart;
	for(it = myMap.begin(); it != myMap.end(); it++)
	{
		// determine smallest cost node (verify if visited by checking if the key exists in the distance matrix)
		if(it->second < min && (distanceMatrix.find(it->first) != distanceMatrix.end()))
		{
			// Assign smallest cost node to newStart
			newStart = it->first;
			min = it->second;
		}
	}
	// remove start from matrixd
	distanceMatrix.erase(start);
	return getCostOfActualPath(newStart, distanceMatrix) + min;
}

map< pair<int,int>, map<pair<int,int>, int> > createMatrixFromNodes(vector<pair<int,int> >& goals, Problem& problem)
{
	map< pair<int,int>, map<pair<int,int>, int> > distanceMatrix;
	// for each node x
		// for each node y, where y != x
			// determine actual distance between x and y
	for(int i = 0; i < goals.size(); i++)
	{
		pair<int,int> current = goals[i];
		map< pair<int,int>, int> distances;
		for(int j = 0; j < goals.size(); j++)
		{
			if (i != j && (distanceMatrix.find(goals[j]) == distanceMatrix.end()))
			{
				vector<string> path = aStar(current, goals[j], problem);
				distances.insert(make_pair(goals[j], path.size()));
			}
		}
		distanceMatrix.insert(make_pair(current, distances));
	}
	return distanceMatrix;
}

vector<string> questionThree(Problem &problem)
{
	vector<string> mergedPath;

	pair<int,int> current = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();

	// If there is no goal, return empty
	if (goals.size() <= 0)
		return vector<string>();

	
	// While there is still a goal
	while(!goals.empty())
	{
		// Assign goal to be closest Manhattan distance from the current state
		pair<int,int> goal;
		int min = 1000000;
		int indexToRemove = 0;
		for(int i = 0; i < goals.size(); i++)
		{
			int temp = computeManhattanDistance(current, goals[i]);
			if(temp < min)
			{
				min = temp;
				goal = goals[i];
				indexToRemove = i;
			}
		}

		// Obtain path and add to mergedPath
		pair<int,int> attemptGoal = goal;	// TODO remove
		vector<string> path = aStarTricky(current, goal, problem, goals, 1);
		mergedPath.insert(mergedPath.end(), path.begin(), path.end());

		// remove the chosen goal
		goals.erase(goals.begin() + indexToRemove);
	}

	return mergedPath;

	// Different method which actually gets thousands of expansions, but incorrect answer...
	// Create matrix of actual distances between every goal
	// determine closest of goals remaining with heuristict
		// heuristic: add g + distances 
	/*map< pair<int, int>, map< pair<int, int>, int> > distanceMatrix;
	goals.push_back(current);
	distanceMatrix = createMatrixFromNodes(goals, problem);
	cout << "REAL Size: " << distanceMatrix.size() << endl;
	goals.erase(goals.begin() + goals.size());
	// while there are still goals
	while(!goals.empty())
	{
		int min = 100000;
		pair<int, int> smallestCostNode;
		int indexToRemove = 0;
		// for each goal g remaining
		for(int i = 0; i < goals.size(); i++)
		{
			// find shortest path with g chosen first
			int temp = getCostOfActualPath(goals[i], distanceMatrix);
			if(temp < min)
			{
				min = temp;
				smallestCostNode = goals[i];
				indexToRemove = i;
			}
		}
		// Head to the shortest cost goal g
		vector<string> path = aStar(current, smallestCostNode, problem);
		mergedPath.insert(mergedPath.end(), path.begin(), path.end());

		// remove the chosen goal
		distanceMatrix.erase(goals[indexToRemove]);
		goals.erase(goals.begin() + indexToRemove);
	}

	return mergedPath;*/
}

vector<string> questionFour(Problem &problem)
{
	vector<string> mergedPath;

	pair<int,int> current = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();

	// If there is no goal, return empty
	if (goals.size() <= 0)
		return vector<string>();

	
	// While there is still a goal
	while(!goals.empty())
	{
		// Assign goal to be closest Manhattan distance from the current state
		pair<int,int> goal;
		int min = 1000000;
		int indexToRemove = 0;
		for(int i = 0; i < goals.size(); i++)
		{
			int temp = computeManhattanDistance(current, goals[i]);
			if(temp < min)
			{
				min = temp;
				goal = goals[i];
				indexToRemove = i;
			}
		}

		// Obtain path and add to mergedPath
		pair<int,int> attemptGoal = goal;	// TODO remove
		vector<string> path = aStarTricky(current, goal, problem, goals, 1);
		mergedPath.insert(mergedPath.end(), path.begin(), path.end());

		// remove the chosen goal
		goals.erase(goals.begin() + indexToRemove);
	}

	return mergedPath;
}



/// Do not change codes below


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
			if (str.length() > 0)
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
