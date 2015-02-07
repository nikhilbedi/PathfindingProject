#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "problem.h"

using namespace std;

Problem::Problem(vector<string> b)
{
	_stateCount = 0;
	_board = b;
	_height = _board.size();
	if (_height <= 0)
		throw;
	_width = _board[0].length();
	_goals.clear();
	_queries.clear();
	_counts.clear();
	for (int i=0; i<_height; ++i)
	{
		if (_board[i].length() != _width)
		{
			_height = i;
			break;
		}
		for(int j=0; j<_width; ++j)
		{
			if (_board[i][j] == 'P')
				_startState = make_pair(i,j);
			else if (_board[i][j] == '.')
				_goals.push_back(make_pair(i,j));
		}
	}
}

pair<int, int> Problem::getStartState()
{
	return _startState;	
}

vector<pair<int, int> > Problem::getGoals()
{
	return _goals;	
}

vector<pair<int, int> > Problem::getSuccessors(pair<int, int> state)
{
	++_stateCount;
	if (_counts.count(state) == 0)
	{
		_counts[state] = 1;
		_queries.push_back(state);
	} else 
		++_counts[state];
	vector<pair<int, int> > ret;
	ret.clear();
	if (state.first>0 && _board[state.first-1][state.second]!='%')
		ret.push_back(make_pair(state.first-1, state.second));
	if (state.first<_height-1 && _board[state.first+1][state.second]!='%')
		ret.push_back(make_pair(state.first+1, state.second));
	if (state.second>0 && _board[state.first][state.second-1]!='%')
		ret.push_back(make_pair(state.first, state.second-1));
	if (state.second<_width-1 && _board[state.first][state.second+1]!='%')
		ret.push_back(make_pair(state.first, state.second+1));
	return ret;
}

int Problem::getExpansionCounts()
{
	return _stateCount;	
}

void Problem::dumpQueries(string taskName)
{
	
	//(i, j) => (j, h-1-i)
	try{
		ofstream fout(taskName.c_str());
		fout << _stateCount << endl;
		for (vector<pair<int, int> >::iterator iter = _queries.begin(); iter != _queries.end(); ++iter)
		{
			fout << iter->second << " " << _height - 1 - iter->first << endl;
		}
		fout.close();
	} catch (...) {}
}
