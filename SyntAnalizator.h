#pragma once

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "lexAnalizator.h"
#include "HashTab.cpp"

struct TreeNode {
    std::string rule;
    std::vector<TreeNode*> children;
    std::string Type;
    int count = 0;
    std::string Tr;
    
    TreeNode(const std::string& r) : rule(r) {}
    ~TreeNode() {
        for (auto child : children) delete child;
    }
};

class Parser {
public:
    Parser(const std::string& input);
    void parse();
    Hash_tab tab;
    void printTree(std::ostream& os, TreeNode* node, int indent = 0) const;
    TreeNode* getRoot() const; // ƒобавл€ем метод дл€ получени€ корневого узла
    void printError(string message);

private:
    std::string input;
    size_t position = 0;
    std::string lexeme;
    size_t line;
    size_t column;
    TreeNode* root;
    Lexer lexer;
    string determineRightSideType(const std::vector<std::string>& lexemes);
    
    void get_next_lexem();
    TreeNode* Function();
    TreeNode* Begin(); 
    TreeNode* End();
    TreeNode* FunctionName();
    TreeNode* Descriptions();
    TreeNode* DescriptionsPrime();
    TreeNode* Operators();
    TreeNode* OperatorsPrime();
    TreeNode* Descr();
    TreeNode* VarList(string s);
    TreeNode* VarListPrime(string s);
    TreeNode* Type();
    TreeNode* Op();
    TreeNode* SimpleExpr();
    TreeNode* Expr();
    TreeNode* ExprPrime();
    TreeNode* Term();
    TreeNode* TermPrime();
    TreeNode* Id();
    TreeNode* Const();

    void error(const std::string& message);
};

