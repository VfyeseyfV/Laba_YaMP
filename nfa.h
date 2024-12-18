#pragma once
#include "dfa.h"
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <algorithm>
class nfa
{
private:
	int countStates; // количество состояний автомата
	std::vector<char> alphabet; // входной алфавит
	std::vector<int> finalStates; // множество допускающих состояний
	std::vector<std::vector<std::vector<int>>> transitFunction; // функция переходов
	// transitFunction[i][j] = q означает, что из i-го состояния по j-му символу входного алфавита автомат переходит в множество состояний, который лежат в векторе q
	std::set<int> getStates(std::vector<int> states, char c);
public:
	nfa(int countStates, std::vector<char> alphabet, std::vector<int> finalStates, std::vector<std::vector<std::vector<int>>> transitFunction);
	bool isAccept(std::string s);
	dfa createDFA();
};
