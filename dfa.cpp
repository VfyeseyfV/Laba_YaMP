#include "dfa.h"
#include <string>
#include <iostream>
dfa::dfa() : countStates(0), alphabet(), finalStates(), transitFunction() {}
dfa::dfa(int countStates, std::vector<char> alphabet, std::vector<int> finalStates, std::vector<std::vector<int>> transitFunction)
{
	this->countStates = countStates;
	this->alphabet = alphabet;
	this->finalStates = finalStates;
	this->transitFunction.resize(countStates);
	for (int i = 0; i < countStates; i++)
		this->transitFunction[i] = transitFunction[i];
}


// Находит состояние автомата по текущему состоянию state и входному символу c
int dfa::getState(int state, char c)
{
	int i = 0;
	while (alphabet[i] != c)
	{
		i++;
		if (i == alphabet.size())
			throw std::exception("Wrong symbol");
	}
	return transitFunction[state][i];
}


// Проверяет, допускается ли строка s автоматом
bool dfa::isAccept(std::string s)
{
	int state = 0;
	for (char c : s)
	{
		state = getState(state, c);
		if (state == -1)
			return false;
	}
	if (std::find(finalStates.begin(), finalStates.end(), state) != finalStates.end())
		return true;
	return false;
}

int dfa::isAcceptState(std::string s){
	int state = 0;
	for (char c : s)
	{
		state = getState(state, c);
		if (state == -1)
			return state;
	}
	if (std::find(finalStates.begin(), finalStates.end(), state) != finalStates.end())
		return state;
	return state;
}


// Находит все состояния, из которых автомат переходит по k-му алфавитному символу в состояние state
std::set<int> dfa::inverse(int state, int k)
{
	std::set<int> result;
	for (int i = 0; i < countStates; i++)
		if (transitFunction[i][k] == state)
			result.insert(i);
	return result;
}


// Строит множество классов эквивалентности на множестве состояний (нужно для построения минимального автомата)
std::vector<std::vector<int>> dfa::quotientSet()
{
	std::vector<std::vector<int>> table(countStates, std::vector<int>(countStates, 0));
	std::queue<std::pair<int, int>> q;
	for (int i = 0; i < countStates; i++)
	{
		if (std::find(finalStates.begin(), finalStates.end(), i) == finalStates.end())
			for (int x : finalStates)
			{
				table[i][x] = 1;
				table[x][i] = 1;
				q.push(std::pair<int, int>(i, x));
			}
	}
	while (!q.empty())
	{
		std::pair<int, int> p = q.front();
		q.pop();
		int x = p.first;
		int y = p.second;
		for (int i = 0; i < alphabet.size(); i++)
		{
			auto s1 = inverse(x, i);
			auto s2 = inverse(y, i);
			if (s1.size() != 0 && s2.size() != 0)
			{
				for (int j : s1)
					for (int k : s2)
						if (table[j][k] != 1)
						{
							table[j][k] = 1;
							table[k][j] = 1;
							q.push(std::pair<int, int>(j, k));
						}
			}
		}
	}
	std::vector<int> mark(countStates, 0);
	std::vector<std::vector<int>> result;
	std::vector<int> t;
	for (int i = 0; i < countStates; i++)
	{
		if (mark[i] == 0)
		{
			mark[i] = 1;
			t.push_back(i);
			for (int j = i + 1; j < countStates; j++)
				if (table[i][j] == 0)
				{
					mark[j] = 1;
					t.push_back(j);
				}
			result.push_back(t);
			t.clear();
		}

	}
	return result;
}


// Строит эквивалентный ДКА с минимальным числом состояний
dfa dfa::minimize()
{
	auto q = quotientSet();
	std::vector<int> minFinalStates;
	int minCountStates = q.size();
	std::vector<std::vector<int>> minTransitFunction(minCountStates, std::vector<int>(alphabet.size(), -1));
	for (int i = 0; i < q.size(); i++)
		for (int j : q[i])
		{
			if (std::find(finalStates.begin(), finalStates.end(), j) != finalStates.end())
			{
				minFinalStates.push_back(i);
				break;
			}
		}
	for (int i = 0; i < minCountStates; i++)
	{
		for (int j = 0; j < alphabet.size(); j++)
		{
			int k = transitFunction[q[i][0]][j];
			for (int l = 0; l < minCountStates; l++)
				if (std::find(q[l].begin(), q[l].end(), k) != q[l].end())
				{
					minTransitFunction[i][j] = l;
					break;
				}
		}
	}
	auto minDfa = dfa(minCountStates, alphabet, minFinalStates, minTransitFunction);
	return minDfa;
}

