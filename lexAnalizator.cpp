#include "lexAnalizator.h"

Lexer::Lexer(string text) {
    string alphabe = "(){};,.0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-=*/%";
    const int MAX_STATES = 21;
    const int ALPHABET_SIZE = 76;
    vector<vector<vector<int>>> transitFunction(MAX_STATES, vector<vector<int>>(ALPHABET_SIZE));
    vector<char> alphabet(ALPHABET_SIZE);
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        alphabet[i] = alphabe[i];
    }
    // Заполнение таблицы переходов
    for (int i = 0; i < MAX_STATES; ++i) {
        for (int j = 0; j < ALPHABET_SIZE; ++j) {
            transitFunction[i][j] = vector<int>();
        }
    }
    //Для начального состояния
    for (int i = 0; i < 6; i++) {
        transitFunction[0][i].push_back(1); //start(0) -> separators(1)
    }
    for (int i = 6; i < 7; i++) {
        transitFunction[0][i].push_back(-1); //start(0) -> dot(4)
    }
    for (int i = 7; i < 17; i++) {
        transitFunction[0][i].push_back(2); //start(0) -> int(2)
    }
    for (int i = 17; i < alphabet.size() - 7; i++) {
        transitFunction[0][i].push_back(3); //start(0) -> id(3)
    }
    for (int i = alphabet.size() - 6; i < alphabet.size(); i++) {
        transitFunction[0][i].push_back(20); //start(0) -> operators(20)
    }

    //Для состояния 1 - separators
    for (int i = 0; i < 6; i++) {
        transitFunction[1][i].push_back(-1); //separators(1) -> separators(1)
    }
    for (int i = 6; i < 7; i++) {
        transitFunction[1][i].push_back(-1); //separators(1) -> dot(4)
    }
    for (int i = 7; i < 17; i++) {
        transitFunction[1][i].push_back(-1); //separators(1) -> int(2)
    }
    for (int i = 17; i < alphabet.size() - 7; i++) {
        transitFunction[1][i].push_back(-1); //separators(1) -> id(3)
    }

    //Для состояния 20 - separators
    for (int i = 0; i < 6; i++) {
        transitFunction[20][i].push_back(-1); //separators(1) -> separators(1)
    }
    for (int i = 6; i < 7; i++) {
        transitFunction[20][i].push_back(-1); //separators(1) -> dot(4)
    }
    for (int i = 7; i < 17; i++) {
        transitFunction[20][i].push_back(-1); //separators(1) -> int(2)
    }
    for (int i = 17; i < alphabet.size() - 7; i++) {
        transitFunction[20][i].push_back(-1); //separators(1) -> id(3)
    }

    //Для состояния 2 - int
    for (int i = 0; i < 6; i++) {
        transitFunction[2][i].push_back(-1); //int(2) -> separators(1)
    }
    for (int i = 6; i < 7; i++) {
        transitFunction[2][i].push_back(4); //int(2) -> dot(4)
    }
    for (int i = 7; i < 17; i++) {
        transitFunction[2][i].push_back(2); //int(2) -> int(2)
    }
    for (int i = 17; i < alphabet.size() - 7; i++) {
        transitFunction[2][i].push_back(-1); //int(2) -> id(3)
    }

    //Для состояния 3 - id
    for (int i = 0; i < 6; i++) {
        transitFunction[3][i].push_back(-1); //id(3) -> separators(1)
    }
    for (int i = 6; i < 7; i++) {
        transitFunction[3][i].push_back(-1); //id(3) -> dot(4)
    }
    for (int i = 7; i < 17; i++) {
        transitFunction[3][i].push_back(-1); //id(3) -> int(2)
    }
    for (int i = 17; i < alphabet.size() - 7; i++) {
        transitFunction[3][i].push_back(3); //id(3) -> id(3)
    }
    for (int i = alphabet.size() - 6; i < alphabet.size(); i++) {
        transitFunction[3][i].push_back(-1); //start(0) -> operators(20)
    }

    //Для состояния 4 - dot
    for (int i = 0; i < 6; i++) {
        transitFunction[4][i].push_back(-1); //dot(4) -> separators(1)
    }
    for (int i = 6; i < 7; i++) {
        transitFunction[4][i].push_back(-1); //dot(4) -> dot(4)
    }
    for (int i = 7; i < 17; i++) {
        transitFunction[4][i].push_back(16); //dot(4) -> double(16)
    }
    for (int i = 17; i < alphabet.size() - 7; i++) {
        transitFunction[4][i].push_back(-1); //dot(4) -> id(3)
    }

    //Для состояния 16 - double
    for (int i = 0; i < 6; i++) {
        transitFunction[16][i].push_back(-1); //double(16) -> separators(1)
    }
    for (int i = 6; i < 7; i++) {
        transitFunction[16][i].push_back(-1); //double(16) -> dot(4)
    }
    for (int i = 7; i < 17; i++) {
        transitFunction[16][i].push_back(16); //double(16) -> dot(4)
    }
    for (int i = 17; i < alphabet.size() - 7; i++) {
        transitFunction[16][i].push_back(-1); //double(16) -> id(3)
    }

    transitFunction[0][34].push_back(5); //  r
    transitFunction[5][21].push_back(6); //  e
    transitFunction[6][36].push_back(7); //  t
    transitFunction[7][37].push_back(8); //  u
    transitFunction[8][34].push_back(9); //  r
    transitFunction[9][30].push_back(17); //  n

    transitFunction[0][25].push_back(10); //  i
    transitFunction[10][30].push_back(11); //  n
    transitFunction[11][36].push_back(18); //  t

    transitFunction[0][22].push_back(12); //  f
    transitFunction[12][28].push_back(13); //  l
    transitFunction[13][31].push_back(14); //  o
    transitFunction[14][17].push_back(15); //  a
    transitFunction[15][36].push_back(19); //  t

    for (int i = 0; i < MAX_STATES; ++i) {
        for (int j = 0; j < ALPHABET_SIZE; ++j) {
            if (transitFunction[i][j].size() == 0) {
                transitFunction[i][j].push_back(-1);
            }
        }
    }
    vector<int> finalStates;
    finalStates.push_back(1);//separators - separator
    finalStates.push_back(2);//int - const_int
    finalStates.push_back(3);//id - id
    finalStates.push_back(17);//return - keyword
    finalStates.push_back(18);//int - keyword
    finalStates.push_back(19);//float - keyword
    finalStates.push_back(16);//double - const_double
    finalStates.push_back(20);//operator - operator
    nfa nfaInstance(21, alphabet, finalStates, transitFunction);
    df = nfaInstance.createDFA();

    vector<string> Types = {
        "Separator",
        "ID",
        "Keyword",
        "Const_int",
        "Const_double",
        "Operator"
    };

    this->position = 0;
    this->text = text;
    this->type = Types;
}

int Lexer::getPos() {
    return this->position;
}

string Lexer::get_next_lexem(string text, int pos) {
    string s;
    int i = pos;
    while (text[i] != ' ' && i < text.size()) {
        s += text[i];
        i += 1;
    }

    this->position = i + 1;
    if (this->df.isAccept(s)) {
        return s;
    }
    else {
        s = '!' + s;
        return s;
    }
    return s;
}

string Lexer::lexem_Type(string lexem) {
    if (lexem[0] != '!') {
        int k = this->df.isAcceptState(lexem);

        if (k == 1) {
            return type[0]; // separator
        }
        if (k == 2) {
            return type[3]; // const_int
        }
        if (k == 16) {
            return type[4]; // const_double
        }
        if (k == 17 || k == 18 || k == 19) {
            return type[2]; // keyword (int, float, return)
        }
        if (k == 20) {
            return type[5]; // operator
        }

        return type[1]; // ID
    }
    else {
        return "!";
    }
}