#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "dfa.h" 
#include "nfa.h"

using namespace std;

class Lexer {
private:
    string text;
    dfa df;
    int position;
    vector <string> type;
public:
    Lexer(string text);
    int getPos();
    string get_next_lexem(string text, int pos);
    string lexem_Type(string lexem);
};