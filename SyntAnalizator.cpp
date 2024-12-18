#include "SyntAnalizator.h"

Parser::Parser(const std::string& input) : input(input), position(0), line(1), column(0), root(nullptr), lexer(input) {}

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

    node->Tr = node->children[0]->Tr + "\n" + node->children[1]->Tr + "\n" + node->children[2]->Tr + "\n" + node->children[3]->Tr;
    cout << node->Tr;
    return node;
}

TreeNode* Parser::Begin() {
    TreeNode* node = new TreeNode("Begin");
    node->children.push_back(Type());
    node->children.push_back(FunctionName());
    if (lexeme != "(") error("Expected '('");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    if (lexeme != ")") error("Expected ')'");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    if (lexeme != "{") error("Expected '{'");
    node->children.push_back(new TreeNode(lexeme));

    node->Tr = node->children[0]->Tr + " " + node->children[1]->Tr;

    get_next_lexem();
    return node;
}

TreeNode* Parser::End() {
    TreeNode* node = new TreeNode("End");
    if (lexeme != "return") error("Expected 'return'");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    node->children.push_back(Id());
    if (lexeme != ";") error("Expected ';'");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    if (lexeme != "}") error("Expected '}'");
    node->children.push_back(new TreeNode(lexeme));

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

TreeNode* Parser::Descr() {
    TreeNode* node = new TreeNode("Descr");
    node->children.push_back(Type());
    node->children.push_back(VarList());
    if (lexeme != ";") error("Expected ';'");
    node->children.push_back(new TreeNode(lexeme));

    node->Tr = node->children[0]->Tr + " " + node->children[1]->Tr + " " + std::to_string(node->children[1]->count + 1) + " DECL";

    get_next_lexem();
    return node;
}

TreeNode* Parser::VarList() {
    TreeNode* node = new TreeNode("VarList");
    node->children.push_back(Id());
    node->children.push_back(VarListPrime());
    node->Tr = node->children[0]->Tr + node->children[1]->Tr;
    node->count = node->children[1]->count + 1; // Считаем текущую переменную
    return node;
}

TreeNode* Parser::VarListPrime() {
    if (lexeme == ",") {
        TreeNode* node = new TreeNode("VarList'");
        node->children.push_back(new TreeNode(lexeme));

        get_next_lexem();
        node->children.push_back(VarList());
        node->count += node->children[1]->count; // Считаем текущую переменную
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

TreeNode* Parser::Op() {
    TreeNode* node = new TreeNode("Op");
    node->children.push_back(Id());
    if (lexeme != "=") error("Expected '='");
    node->children.push_back(new TreeNode(lexeme));
    get_next_lexem();
    node->children.push_back(Expr());
    if (lexeme != ";") error("Expected ';'");
    node->children.push_back(new TreeNode(lexeme));

    node->Tr = node->children[0]->Tr + " " + node->children[2]->Tr + " =";

    get_next_lexem();
    return node;
}

TreeNode* Parser::Expr() {
    TreeNode* node = new TreeNode("Expr");
    node->children.push_back(Term());
    node->children.push_back(ExprPrime());
    node->Tr = node->children[0]->Tr + " " + node->children[1]->Tr;
    return node;
}

TreeNode* Parser::ExprPrime() {
    if (lexeme == "+" || lexeme == "-") {
        TreeNode* node = new TreeNode("Expr'");
        string s = lexeme;
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        node->children.push_back(Expr());
        node->Tr = node->children[1]->Tr + s;
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
        node->Tr = node->children[1]->Tr + s;
        return node;
    }
    else {
        return new TreeNode("Term'");
    }
}

TreeNode* Parser::SimpleExpr() {
    if (lexer.lexem_Type(lexeme) == "ID") {
        TreeNode* node = new TreeNode("SimpleExpr");
        node->children.push_back(Id());
        node->Tr = node->children[0]->Tr;
        return node;
    }
    else if (lexer.lexem_Type(lexeme) == "Const_int" || lexer.lexem_Type(lexeme) == "Const_double") {
        TreeNode* node = new TreeNode("SimpleExpr");
        node->children.push_back(Const());
        node->Tr = node->children[0]->Tr;
        return node;
    }
    else if (lexeme == "(") {
        TreeNode* node = new TreeNode("SimpleExpr");
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        node->children.push_back(Expr());
        if (lexeme != ")") error("Expected ')'");
        node->children.push_back(new TreeNode(lexeme));
        get_next_lexem();
        node->Tr = node->children[1]->Tr;
        return node;
    }
    else {
        error("Expected 'id_name', 'int_num', 'float_num', or '('");
        return nullptr;
    }
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