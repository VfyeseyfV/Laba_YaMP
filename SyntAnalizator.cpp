#include "SyntAnalizator.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "lexAnalizator.h"
#include <algorithm>

using namespace std;

Parser::Parser(const std::string& input) : input(input), position(0), line(1), column(0), root(nullptr), lexer(input), tab(1000) {}
bool firstErrorPrinted = false;
void Parser::get_next_lexem() {
    if (position >= input.size()) {
        lexeme = "#"; // Конец ввода
        return;
    }

    lexeme = lexer.get_next_lexem(input, position);
    column += 1;
    position = lexer.getPos();

    if (lexeme == "!+") {
        lexeme = "+";
    }
    else if (lexeme[0] == '!') {
        error("Invalid lexeme: " + lexeme.substr(1));
    }
}

TreeNode* Parser::getRoot() const {
    return root;
}

void Parser::printError(string message){
    if (!firstErrorPrinted) {
        firstErrorPrinted = true; // Устанавливаем флаг, что первая ошибка была выведена
        cout << message << endl;
    }

}

void Parser::parse() {
    get_next_lexem();
    root = Function();
    if (lexeme != "#") error("Unexpected input at the end");
}

TreeNode* Parser::Function() {
    TreeNode* node = new TreeNode("Function");

    node->children.push_back(Begin());
    node->children.push_back(Descriptions());
    node->children.push_back(Operators());
    node->children.push_back(End());
    if (node->children[0]->Type != node->children[3]->Type) {
        printError("Uncorrect returned value");
    }
    //vector<string> v;
    //tab.print(v);

    node->Tr = node->children[0]->Tr + "\n" + " " + node->children[1]->Tr + node->children[2]->Tr + node->children[3]->Tr;
    cout << node->Tr;
    return node;
}

TreeNode* Parser::Begin() {
    TreeNode* node = new TreeNode("Begin");
    node->children.push_back(Type());
    node->Type = node->children[0]->Tr;
    node->children.push_back(FunctionName());
    if (lexeme != "(") error("Expected '('");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    if (lexeme != ")") error("Expected ')'");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    if (lexeme != "{") error("Expected '{'");
    node->children.push_back(new TreeNode(lexeme));

    node->Tr = node->children[0]->Tr + " " + node->children[1]->Tr + " 2 FDECL";

    get_next_lexem();
    return node;
}

TreeNode* Parser::End() {
    TreeNode* node = new TreeNode("End");
    if (lexeme != "return") error("Expected 'return'");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    node->children.push_back(Id());

    node->Type = tab.getLexemeType(node->children[1]->Tr);
    if (lexeme != ";") error("Expected ';'");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    if (lexeme != "}") error("Expected '}'");
    node->children.push_back(new TreeNode(lexeme));
    if (node->children[1]->Tr.find(".") != string::npos) {
            error("Expected 'id_name'");
    }
    if (tab.find(node->children[1]->Tr) == -1) {
        printError("Non Indefinitly peremennaya v returne: " + node->children[0]->Tr );
    }
    node->Tr = node->children[1]->Tr + " return";

    get_next_lexem();
    return node;
}

TreeNode* Parser::FunctionName() {
    if (lexer.lexem_Type(lexeme) == "ID") {
        TreeNode* node = new TreeNode("FunctionName");
        node->Tr = lexeme;
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        return node;
    }
    else {
        error("Expected Function 'id_name'");
        return nullptr;
    }
}

TreeNode* Parser::Descriptions() {
    TreeNode* node = new TreeNode("Descriptions");
    node->children.push_back(Descr());
    node->children.push_back(DescriptionsPrime());

    node->Tr = node->children[0]->Tr + " " + node->children[1]->Tr;

    return node;
}

TreeNode* Parser::DescriptionsPrime() {
    if (lexeme == "int" || lexeme == "float") {
        TreeNode* node = new TreeNode("Descriptions'");
        node->children.push_back(Descriptions());
        node->Tr = node->children[0]->Tr;
        return node;
    }
    else {
        return new TreeNode("Descriptions'");
    }
}

int counter = 0;
TreeNode* Parser::Descr() {
    counter = 0;
    TreeNode* node = new TreeNode("Descr");
    node->children.push_back(Type());
    node->Type = node->children[0]->Tr;
    node->children.push_back(VarList(node->Type));
    if (lexeme != ";") error("Expected ';'");
    node->children.push_back(new TreeNode(lexeme));

    node->Tr = node->children[0]->Tr + " " + node->children[1]->Tr + " " + std::to_string(node->children[1]->count + 1) + " DECL" + "\n";

    get_next_lexem();
    return node;
}

TreeNode* Parser::VarList(string s) {
    TreeNode* node = new TreeNode("VarList");
    string str = s;
    counter += 1;
    node->children.push_back(Id());

    if (tab.find(node->children[0]->Tr) != -1) {
        printError ("Redefinition of variable: " + node->children[0]->Tr);
    }

    Token t(node->children[0]->Tr, str);
    tab.insert(t);
    node->children.push_back(VarListPrime(str));

    node->Tr = node->children[0]->Tr + node->children[1]->Tr;
    node->count = node->children[1]->count + 1;
    return node;
}

TreeNode* Parser::VarListPrime(string s) {
    if (lexeme == ",") {
        TreeNode* node = new TreeNode("VarList'");
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        node->children.push_back(VarList(s));
        node->count += node->children[1]->count;
        node->Tr = node->children[0]->Tr + " " + node->children[1]->Tr;
        return node;
    }
    else {
        return new TreeNode("VarList'");
    }
}

TreeNode* Parser::Type() {
    if (lexeme == "int" || lexeme == "float") {
        TreeNode* node = new TreeNode("Type");
        node->Tr = lexeme;
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        return node;
    }
    else {
        error("Expected Type 'int' or 'float'");
        return nullptr;
    }
}

TreeNode* Parser::Operators() {
    TreeNode* node = new TreeNode("Operators");
    node->children.push_back(Op());
    node->children.push_back(OperatorsPrime());
    node->Tr = node->children[0]->Tr + " " + node->children[1]->Tr;
    return node;
}

TreeNode* Parser::OperatorsPrime() {
    if (lexer.lexem_Type(lexeme) == "ID") {
        TreeNode* node = new TreeNode("Operators'");
        node->children.push_back(Operators());
        node->Tr = node->children[0]->Tr;
        return node;
    }
    else {
        return new TreeNode("Operators'");
    }
}
std::vector<std::string> rightSideTypes; // Глобальный вектор для хранения лексем справа от '='


TreeNode* Parser::Op() {
    TreeNode* node = new TreeNode("Op");
    node->children.push_back(Id());

    // Проверка, что переменная объявлена
    if (tab.find(node->children[0]->Tr) == -1) {
        printError ("Variable '" + node->children[0]->Tr + "' is not declared");
    }

    // Получаем тип переменной слева от '='
    string leftType = tab.getLexemeType(node->children[0]->Tr);

    if (lexeme != "=") error("Expected '='");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    node->children.push_back(Expr());

    // Флаг для отслеживания, была ли уже выведена ошибка
    bool typeMismatchErrorPrinted = false;
    for (int i = 1; i < rightSideTypes.size(); ++i) {
        if (rightSideTypes[i] == "") {
            rightSideTypes[i] = rightSideTypes[i-1];
        };
    }


    // Проверяем, что тип выражения справа от '=' совпадает с типом переменной слева
    for (const auto& type : rightSideTypes) {
        if (leftType != type && !typeMismatchErrorPrinted) {
            printError("Types are different: left side is '" + leftType + "', but right '" + type + "'");
            typeMismatchErrorPrinted = true; // Устанавливаем флаг, чтобы больше не выводить ошибку
        }
    }

    // Очищаем вектор для следующего использования
    rightSideTypes.clear();

    if (lexeme != ";") error("Expected ';'");
    node->children.push_back(new TreeNode(lexeme));

    node->Tr = node->children[0]->Tr + " " + node->children[2]->Tr + " =" + "\n";

    get_next_lexem();
    return node;
}




TreeNode* Parser::Expr() {
    TreeNode* node = new TreeNode("Expr");
    node->children.push_back(Term());
    node->children.push_back(ExprPrime());

    // Добавляем тип первого операнда в вектор
    rightSideTypes.push_back(node->children[0]->Type);

    // Проверка типов и инициализации
    if (node->children[0]->Type != node->children[1]->Type) {
        //cout << "Type mismatch in expression";
    }

    node->Type = node->children[0]->Type; // Устанавливаем тип узла
    node->Tr = node->children[0]->Tr+" " + node->children[1]->Tr;
    return node;
}

TreeNode* Parser::ExprPrime() {
    if (lexeme == "+" || lexeme == "-") {
        TreeNode* node = new TreeNode("Expr'");
        string s = lexeme;
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        node->children.push_back(Expr());
        node->Tr = node->children[1]->Tr + " " + s;

        // Добавляем тип следующего операнда в вектор
        rightSideTypes.push_back(node->children[1]->Type);

        return node;
    }
    else {
        return new TreeNode("Expr'");
    }
}

TreeNode* Parser::Term() {
    TreeNode* node = new TreeNode("Term");
    node->children.push_back(SimpleExpr());
    node->children.push_back(TermPrime());

    // Добавляем тип первого операнда в вектор
    rightSideTypes.push_back(node->children[0]->Type);

    // Проверка типов и инициализации
    if (node->children[0]->Type != node->children[1]->Type) {
        //cout << "Type mismatch in term";
    }

    node->Type = node->children[0]->Type; // Устанавливаем тип узла
    node->Tr = node->children[0]->Tr + " " + node->children[1]->Tr;
    return node;
}

TreeNode* Parser::TermPrime() {
    if (lexeme == "*" || lexeme == "/" || lexeme == "%") {
        TreeNode* node = new TreeNode("Term'");
        string s = lexeme;
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        node->children.push_back(Term());
        node->Tr = node->children[1]->Tr + " " + s;
        if (s == "%") {
            rightSideTypes.push_back("int");
        }
        else {
            // Добавляем тип следующего операнда в вектор
            rightSideTypes.push_back(node->children[1]->Type);
        }

        return node;
    }
    else {
        return new TreeNode("Term'");
    }
}

TreeNode* Parser::SimpleExpr() {
    if (lexer.lexem_Type(lexeme) == "ID") {
        TreeNode* node = new TreeNode("SimpleExpr");
        if (lexeme.find(".") != string::npos) {
            Token t(lexeme, "float");
            tab.insert(t);
        }

        node->children.push_back(Id());

        // Проверка, что переменная объявлена
        if (tab.find(node->children[0]->Tr) == -1) {
            printError ("Variable '" + node->children[0]->Tr + "' is not declared");
        }

        // Устанавливаем тип узла
        node->Type = tab.getLexemeType(node->children[0]->Tr);
        node->Tr = node->children[0]->Tr;
        return node;
    }
    else if (lexer.lexem_Type(lexeme) == "Const_int" || lexer.lexem_Type(lexeme) == "Const_double") {
        TreeNode* node = new TreeNode("SimpleExpr");
        if (lexer.lexem_Type(lexeme) == "Const_int") {
            node->Type = "int";
        }
        else {
            node->Type = "float";
        }
        node->children.push_back(Const());

        // Устанавливаем тип узла
        node->Tr = node->children[0]->Tr;
        return node;
    }
    else if (lexeme == "(") {
        TreeNode* node = new TreeNode("SimpleExpr");
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        node->children.push_back(Expr());

        // Проверяем, что типы совпадают
        /*if (node->children[1]->Type != node->children[1]->Type) {
            error("Type mismatch in expression");
        }*/

        if (lexeme != ")") error("Expected ')'");
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        node->Tr = node->children[1]->Tr;
        rightSideTypes.push_back(node->children[1]->Type);
        return node;
    }
    else {
        error("Expected 'id_name', 'int_num', 'float_num', or '('");
        return nullptr;
    }
}

string Parser::determineRightSideType(const std::vector<std::string>& lexemes) {
    string type = "";

    for (const auto& lexeme : lexemes) {
        string currentType = tab.getLexemeType(lexeme);

        // Если тип лексемы уже определен, проверяем совместимость
        if (type != "") {
            if (type != currentType) {
                printError ("Types are different in right side: " + type + " vs " + currentType );
            }
        }
        // Иначе устанавливаем тип лексемы
        else {
            type = currentType;
        }
    }

    // Если тип не определен, выбрасываем ошибку
    if (type == "") {
        printError ("Right side has no type");
    }

    return type;
}



TreeNode* Parser::Id() {
    if (lexer.lexem_Type(lexeme) == "ID") {
        TreeNode* node = new TreeNode("Id");
        node->Tr = lexeme;
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        return node;
    }
    else {
        error("Expected 'id_name'");
        return nullptr;
    }
}

TreeNode* Parser::Const() {
    if (lexer.lexem_Type(lexeme) == "Const_int" || lexer.lexem_Type(lexeme) == "Const_double") {
        TreeNode* node = new TreeNode("Const");
        node->Tr = lexeme;
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        return node;
    }
    else {
        error("Expected 'int_num' or 'double_num'");
        return nullptr;
    }
}

void Parser::error(const std::string& message) {
    throw std::runtime_error("Number of lexeme " + std::to_string(column) + ": " + message);
}

void Parser::printTree(std::ostream& os, TreeNode* node, int indent) const {
    if (!node) return;
    for (int i = 0; i < indent; ++i) os << "     ";
    os << node->rule << std::endl;
    for (auto child : node->children) printTree(os, child, indent + 1);
}