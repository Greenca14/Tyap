/*S→I=E; Операторы присваивания переменной с именем I значения выражения E. Переменная с именем I определяется, если она не была определена ранее

E→E+T|E-T|T Выражения с традиционными арифметическими операциями +, –,*, / и круглыми скобками
T→T*M|T/M|M
M→(E)|I|C

I→AK|A Идентификаторы
K→AK|DK|A|D

C→DC|D Константы

A→а|b|c|d|e|f|g|h|i|j|k|l|m| Буквы
n|o|p|q|r|s|t|u|v|w|x|y|z|_

D→0|1|2|3|4|5|6|7 Восьмеричные цифры*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

enum class TypeErrors {
    SYNTAX_ERROR,
    UNDEF_ID,
    ID_MISS,
    MISSING_SYMBOL,
};

struct Var {
    std::string IdName;
    long value;
    Var(const std::string& src, long value) : IdName(src), value(value) {}
};

std::ifstream in;

class Analyzer {
private:
    std::vector<Var> varlist;
    char c = EOF;
    int last = -1;

public:
    void GetSymbol();
    long GetVarValue(const std::string&);
    long& GetVarAddr(const std::string&);

    void MethodS();
    long MethodE();
    long MethodT();
    long MethodM();
    long MethodI();
    std::string MethodIs();
    long MethodC();
    void Print();
    void PrintAll();
    void Run();
};

void PrintError(TypeErrors typeError, const std::string& param = "") {
    switch (typeError) {
    case TypeErrors::SYNTAX_ERROR:
        std::cout << "Error: Syntax error.\n";
        break;
    case TypeErrors::ID_MISS:
        std::cout << "Error: Invalid character in identifier: " << param << "\n";
        break;
    case TypeErrors::UNDEF_ID:
        std::cout << "Error: Undefined identifier: " << param << "\n";
        break;
    case TypeErrors::MISSING_SYMBOL:
        std::cout << "Error: Missing " << param << ".\n";
        break;
    }
    in.close();
    exit(1);
}

int main() {
    in.open("in.txt", std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error! Can't open file.\n";
        return 2;
    }

    Analyzer analyzer;
    analyzer.Run();
    in.close();
    return 0;
}

void Analyzer::GetSymbol() {
    c = in.get();
}

long Analyzer::GetVarValue(const std::string& str) {
    for (const auto& var : varlist) {
        if (var.IdName == str)
            return var.value;
    }
    PrintError(TypeErrors::UNDEF_ID, str);
    return 0;
}

long& Analyzer::GetVarAddr(const std::string& name) {
    for (auto& var : varlist) {
        if (var.IdName == name)
            return var.value;
    }
    varlist.emplace_back(name, 0);
    last = varlist.size() - 1;
    return varlist[last].value;
}

void Analyzer::MethodS() {
    std::string varName = MethodIs();

    long& varAddress = GetVarAddr(varName);

    if (c != '=') PrintError(TypeErrors::MISSING_SYMBOL, "'='");
    GetSymbol(); // Пропуск '='
    varAddress = MethodE();

    if (c != ';') PrintError(TypeErrors::MISSING_SYMBOL, "';'");
    GetSymbol(); // Пропуск ';'
}

long Analyzer::MethodE() {
    long x = MethodT();
    while (c == '+' || c == '-') {
        char op = c;
        GetSymbol();
        long y = MethodT();
        if (op == '+') x += y;
        else x -= y;
    }
    return x;
}

long Analyzer::MethodT() {
    long x = MethodM();
    while (c == '*' || c == '/') {
        char op = c;
        GetSymbol();
        long y = MethodM();
        if (op == '*') x *= y;
        else x /= y;
    }
    return x;
}

long Analyzer::MethodM() {
    if (c == '(') {
        GetSymbol(); // Пропуск '('
        long result = MethodE();
        if (c != ')') PrintError(TypeErrors::MISSING_SYMBOL, "')'");
        GetSymbol(); // Пропуск ')'
        return result;
    }
    else if (std::isdigit(c)) {
        return MethodC();
    }
    else if (std::isalpha(c) || c == '_') {
        return MethodI();
    }
    PrintError(TypeErrors::SYNTAX_ERROR);
    return 0;
}

long Analyzer::MethodI() {
    std::string id;
    while (std::isalpha(c) || c == '_') {
        id += c;
        GetSymbol();
    }
    while (std::isdigit(c)) {
        if (c >= '0' && c <= '7') {
            id += c;
            GetSymbol();

        }
        else {
            PrintError(TypeErrors::SYNTAX_ERROR);
        }
    }
    return GetVarValue(id);
}

std::string Analyzer::MethodIs() {
    std::string id;
    while (std::isalpha(c) || c == '_') {
        id += c;
        GetSymbol();
    }
    while (std::isdigit(c)) {
        if (c >= '0' && c <= '7') {
            id += c;
            GetSymbol();

        }
        else {
            PrintError(TypeErrors::SYNTAX_ERROR);
        }
    }
    return id;
}

long Analyzer::MethodC() {
    long value = 0;

    while (std::isdigit(c)) { 
        if (c < '0' || c > '7') {
            PrintError(TypeErrors::SYNTAX_ERROR, std::string(1, c));
            return 0;
        }

        value = value * 8 + (c - '0'); // Конвертируем символ в число
        GetSymbol();
    }

    return value;
}

void Analyzer::Print() {
    if (last < 0) {
        std::cout << "No variables defined yet.\n";
    }
    else {
        std::cout << varlist[last].IdName << " = " << varlist[last].value << std::endl;
    }
}

void Analyzer::PrintAll() {
    if (varlist.empty()) {
        std::cout << "No variables defined yet.\n";
    }
    else {
        for (const auto& var : varlist) {
            std::cout << var.IdName << " = " << var.value << std::endl;
        }
    }
}

void Analyzer::Run() {
    int operatorCount = 0;
    GetSymbol();
    while (c != EOF) {
        while (std::isspace(c)) GetSymbol(); // Пропускаем пробелы

        std::cout << "Operator: " << ++operatorCount << std::endl;
        MethodS();
        Print();
    }
    std::cout << "Result (" << varlist.size() << " variables defined in " << operatorCount << " operators)\n";
    PrintAll();
}