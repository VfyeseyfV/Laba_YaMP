#include <iostream>
#include <fstream>
#include "nfa.h"
#include "dfa.h"
#include <vector>
#include <string>
#include <algorithm>
#include "HashTab.cpp"
#include "SyntAnalizator.h"


using namespace std;
//asd 213 432.32 ; int intt in2t int return float reretuturnnn 23. . 244 as sad 244.2 returns


int main() {
    std::ifstream in("C:/Users/Win10gpt/source/repos/╦рср_▀ья_1/╦рср_▀ья_1/input.txt");
    std::ofstream out("C:/Users/Win10gpt/source/repos/╦рср_▀ья_1/╦рср_▀ья_1/output.txt");
    Hash_tab tab(10000);
    vector <string> Musor;
    string text, str;
    while (in >> str) {
        text += str + ' ';
    }
    //Lexer lx(text);
    int k;
    //tab.read_all_Tokens(text, Musor);
    
    string input = "int main ( ) { int d ; int a , b ; a = ( ( 3.234 ) - 3 * 4 ) ; return a ; } ";
    Parser parser(text);
    try {
        parser.parse();
        std::ofstream output("output.txt");
        parser.printTree(output, parser.getRoot());
        output.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    //tab.print(Musor);
    
    return 0;
}