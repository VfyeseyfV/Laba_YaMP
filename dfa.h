#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
class dfa
{
private:
	
	int countStates; // количество состояний автомата
	std::vector<char> alphabet; // входной алфавит
	std::vector<int> finalStates; // множество допускающих состояний
	std::vector<std::vector<int>> transitFunction; // функция переходов 
	// transitFunction[i][j] = k означает, что из i-го состояния по j-му символу входного алфавита автомат переходит в состояние k
	int getState(int state, char c);
	
	std::string getlexem(int state, char c);
	std::set<int> inverse(int state, int k);
public:
	dfa();
	dfa(int countStates, std::vector<char> alphabet, std::vector<int> finalStates, std::vector<std::vector<int>> transitFunction);
	dfa minimize();
	std::vector<std::vector<int>> quotientSet();
	bool isAccept(std::string s);
	int isAcceptState(std::string s);
};
