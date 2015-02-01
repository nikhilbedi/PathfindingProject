#ifndef _PROBLEM_H_
#define _PROBLEM_H_
#include <vector>
#include <string>
#include <set>

using namespace std;

class Problem {
	vector<string> _board;
	vector<pair<int, int> > _goals;
	pair<int, int> _startState;
	int _stateCount, _height, _width;
	vector<pair<int, int> > _queries;
	map<pair<int, int>, int> _counts;
	
	public:
	Problem(vector<string> b);
	pair<int, int> getStartState();
	vector<pair<int, int> > getGoals();
	vector<pair<int, int> > getSuccessors(pair<int, int> state);
	int getExpansionCounts();
	void dumpQueries(string taskName);
};
#endif
