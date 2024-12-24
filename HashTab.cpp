#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "lexAnalizator.h"
#include <algorithm>

using namespace std;

struct Token {
    string type;
    string lexeme;
    Token() {
    };
    Token(string l, string t) : lexeme(l), type(t) {};
    string get_lexem() {
        return lexeme;
    }
    string get_type() {
        return type;
    }
};

class Hash_tab {
public:
    Token* table;
    int size;

    Hash_tab() :size(0) {}
    Hash_tab(int size) : size(size) {
        table = new Token[size];
    }

    ~Hash_tab() {
        delete[] table;
    }

    int hash(Token tk) {
        int res = 0;
        for (int i = 0; i < tk.get_lexem().size(); ++i) {
            res += tk.get_lexem()[i];
        }
        for (int i = 0; i < tk.get_type().size(); ++i) {
            res += tk.get_type()[i];
        }
        while (table[res].get_lexem() != "") {
            if (table[res].get_lexem() == tk.get_lexem()) {
                return res % size;
            }
            ++res;
            res %= size;
        }
        return res % size;
    }

    int find(const string& lexeme) {
        for (int i = 0; i < size; ++i) {
            if (table[i].get_lexem() == lexeme) {
                return 1; // Лексема найдена
            }
        }
        return -1; // Лексема не найдена
    }

    void read_all_Tokens(string text, vector <string>& Musor) {
        Lexer lx(text);

        int k;
        while (lx.getPos() < text.size()) {
            k = lx.getPos();

            if (lx.get_next_lexem(text, lx.getPos())[0] == '!') {
                lx.get_next_lexem(text, k).erase(0, 1);
                Musor.push_back(lx.get_next_lexem(text, k));
            }
            else {
                Token tk(lx.get_next_lexem(text, k), lx.lexem_Type(lx.get_next_lexem(text, k)));

                insert(tk);
            }
        }
    }


    void insert(Token token) {
        int index = hash(token);

        // Проверяем, занят ли индекс
        while (!table[index].get_lexem().empty() && table[index].get_lexem() != token.get_lexem()) {
            index = (index + 1) % size; // Линейное пробирование
        }

        // Вставляем токен в найденный индекс
        table[index] = token;
    }

    int get_size() {
        return size;
    }
    string getLexemeType(string lexeme) {
        for (int i = 0; i < this->get_size(); ++i) {
            if (this->table[i].get_lexem() == lexeme) {
                return this->table[i].get_type(); // Возвращаем тип лексемы
            }
        }
        return "Lexeme not found"; // Если лексема не найдена
    }



    void print(vector<string> Musor) {
        std::ofstream out("C:/Users/Win10gpt/source/repos/Лаба_Ямп_1/Лаба_Ямп_1/output.txt");
        //cout << "          12131212312312      " << size<<endl;
        for (int i = 0; i < size; ++i) {
            if (!table[i].get_lexem().empty()) {
                cout << "Type: " << table[i].get_type();
                for (int j = table[i].get_type().size(); j < 25; ++j) {
                    cout << ' ';
                }
                cout << "Lexeme: " << table[i].get_lexem();
                for (int j = table[i].get_lexem().size(); j < 25; ++j) {
                    cout << ' ';
                }
                cout << "Index: " << i << endl;
            }
        }
        cout << endl;
        cout << "Musor: " << endl;
        for (int i = 0; i < Musor.size(); ++i) {
            cout << Musor[i].erase(0, 1) << "   ";
        }
    }
};