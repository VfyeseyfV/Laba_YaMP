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
        lexeme = "";
        type = "";
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

public:
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
        Token temp_token(lexeme, ""); // —оздаем временный токен дл€ поиска
        int index = hash(temp_token); // ¬ычисл€ем индекс с помощью хеш-функции

        while (!table[index].get_lexem().empty()) {
            if (table[index].get_lexem() == lexeme) {
                return index;
            }
            index += 1;
            index %= size; // ѕереходим к следующему индексу
        }

        return -1;
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
         table[index] = token;
        
    }

    int get_size() {
        return size;
    }

    void print(vector<string> Musor) {
        std::ofstream out("C:/Users/Win10gpt/source/repos/Ћаба_ямп_1/Ћаба_ямп_1/output.txt");

        for (int i = 0; i < size; ++i) {
            if (!table[i].get_lexem().empty()) {
                out << "Type: " << table[i].get_type();
                for (int j = table[i].get_type().size(); j < 25; ++j) {
                    out << ' ';
                }
                out << "Lexeme: " << table[i].get_lexem();
                for (int j = table[i].get_lexem().size(); j < 25; ++j) {
                    out << ' ';
                }
                out<< "Index: " << i << endl;
            }
        }
        out << endl;
        out << "Musor: " << endl;
        for (int i = 0; i < Musor.size(); ++i) {
            out << Musor[i].erase(0, 1) << "   ";
        }
    }
};

