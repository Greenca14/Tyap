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
#include <unordered_map>


using namespace std;

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

struct Triad {
    string op;
    string op1;
    string op2;
};

std::ifstream in;
std::ofstream out;

class Analyzer {
private:
    std::vector<Var> varlist;
    char c = EOF;
    int last = -1;


    unordered_map<string, int> idTriads;
    vector<Triad> triads;
    int triadCounter = 1; // счетчик
    bool definingVar = false; // флаг 

public:
    void GetSymbol();
    long GetVarValue(const std::string&);

    void MethodS();
    long MethodE();
    long MethodT();
    long MethodM();
    long MethodI();
    long MethodC();
    void Run();
    int addTriad(const string&, const string&, const string&);
    void writeTriads(ofstream&);
    void optimizeTriads();
    void writeOptimizedTriads(ofstream&);
    void applyConstantReplacement(size_t, vector<bool>&);
    void evaluateConstantOperations(size_t, vector<bool>&);
    void applyVariableReplacement(size_t, vector<bool>&);
};

void PrintError(TypeErrors typeError, const std::string& param = "") {
    switch (typeError) {
    case TypeErrors::SYNTAX_ERROR:
        std::cout << "Error: Syntax error." << param << "\n";
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
    in.open("in.c", std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error! Can't open file.\n";
        return 2;
    }

    out.open("out.c", std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error! Can't open file.\n";
        return 2;
    }

    Analyzer analyzer;
    analyzer.Run();
    in.close();
    out.close();
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

void Analyzer::MethodS() {
    definingVar = true;
    int varTriadIndex = MethodI();
    definingVar = false;

    if (c == '=') {
        GetSymbol(); // Пропуск '='
        int exprTriadIndex = MethodE();

        addTriad("=", "^" + to_string(varTriadIndex), "^" + to_string(exprTriadIndex));

        if (c != ';') PrintError(TypeErrors::MISSING_SYMBOL, "';'");
        GetSymbol(); // Пропуск ';'
    }
    else {
        PrintError(TypeErrors::MISSING_SYMBOL, "'='");
    }
}

long Analyzer::MethodE() {
    int leftIdTriad = MethodT();

    while (c == '+' || c == '-') {
        char op = c;
        GetSymbol();
        int rightIdTriad = MethodT();
        leftIdTriad = addTriad(string(1, op), "^" + to_string(leftIdTriad), "^" + to_string(rightIdTriad));
    }
    return leftIdTriad;
}

long Analyzer::MethodT() {
    int leftIdTriad = MethodM();
    while (c == '*' || c == '/') {
        char op = c;
        GetSymbol();
        int rightIdTriad = MethodM();
        leftIdTriad = addTriad(string(1, op), "^" + to_string(leftIdTriad), "^" + to_string(rightIdTriad));
    }
    return leftIdTriad;
}

long Analyzer::MethodM() {
    if (c == '(') {
        GetSymbol(); // Пропуск '('
        int triadIndex = MethodE();
        if (c != ')') PrintError(TypeErrors::MISSING_SYMBOL, "')'");
        GetSymbol(); // Пропуск ')'
        return triadIndex;
    }
    else if (std::isdigit(c)) {
        return MethodC();
    }
    else if (std::isalpha(c) || c == '_') {
        return MethodI();
    }
    else {
        PrintError(TypeErrors::SYNTAX_ERROR, std::string(1, c));
    }
    return 0;
}

long Analyzer::MethodI() {
    std::string id;
    if (isalpha(c) || (c >= '0' && c <= '7') || c == '_') {

        while (std::isalpha(c) || c == '_') { //methodK
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

        if (definingVar) {
            // определение переменной:
            int triadIndex = addTriad("V", id, "Ø");
            idTriads[id] = triadIndex;
            return triadIndex;
        }
        else {
            // использовавшаяся переменной:
            if (idTriads.find(id) == idTriads.end()) {
                PrintError(TypeErrors::SYNTAX_ERROR);
            }
            int triadIndex = addTriad("V", id, "Ø");
            idTriads[id] = triadIndex;
            return triadIndex;
        }
    }
    else {
        PrintError(TypeErrors::SYNTAX_ERROR);
    }

    return GetVarValue(id);
}

long Analyzer::MethodC() {
    long value = 0;

    while (std::isdigit(c)) {
        if (c < '0' || c > '7') {
            PrintError(TypeErrors::SYNTAX_ERROR, std::string(1, c));
            return 0;
        }

        value = value * 8 + (c - '0'); // Конвертация 8сс в 10сс
        GetSymbol();
    }

    return addTriad("C", to_string(value), "Ø");
}

int Analyzer::addTriad(const string& op, const string& op1, const string& op2) {
    triads.push_back({ op, op1, op2 });
    return triadCounter++;
}

void Analyzer::writeTriads(ofstream& output) {
    output << "Triads:" << endl;
    for (size_t i = 0; i < triads.size(); ++i) {
        output << i + 1 << ": " << triads[i].op << "(" << triads[i].op1 << ", " << triads[i].op2 << ")" << endl;
    }
}

void Analyzer::optimizeTriads() {
    vector<bool> markedForRemoval(triads.size(), false);

    for (size_t i = 0; i < triads.size(); ++i) {
        if (!markedForRemoval[i]) {
            // Проверяем условия последовательно
            applyConstantReplacement(i, markedForRemoval);//1
            evaluateConstantOperations(i, markedForRemoval);//3
            applyVariableReplacement(i, markedForRemoval);//4
        }
    }

    // Удаляем помеченные тройники
    for (size_t i = 0; i < triads.size(); ++i) {
        if (markedForRemoval[i]) {
            triads[i].op = "R";
        }
    }
}

void Analyzer::applyConstantReplacement(size_t i, vector<bool>& markedForRemoval) {
    const Triad& triad = triads[i];
    if (triad.op != "=" && triad.op2[0] == '^') {
        size_t index = 0;
        size_t leftIndex = 0;
        for (size_t j = 1; j < triad.op2.size(); ++j) {
            leftIndex = index * 10 + (triad.op2[j] - '0'); // конвертация вручную
        }
        index--;
        if (triads[index].op == "C") {
            triads[i].op2 = triads[index].op1;
            markedForRemoval[index] = true;
        }
    }
}

void Analyzer::evaluateConstantOperations(size_t i, vector<bool>& markedForRemoval) {
    const Triad& triad = triads[i];
    if (triad.op != "=" && triad.op1[0] == '^' && triad.op2[0] != '^') {
        size_t index = 0;
        size_t leftIndex = 0;
        for (size_t j = 1; j < triad.op2.size(); ++j) {
            leftIndex = index * 10 + (triad.op2[j] - '0'); // конвертация вручную
        }
        index--;
        if (triads[leftIndex].op == "C" && isdigit(triad.op2[0])) {
            int leftValue = stoi(triads[leftIndex].op1);
            int rightValue = stoi(triad.op2);
            int result = 0;

            if (triad.op == "+") result = leftValue + rightValue;
            else if (triad.op == "-") result = leftValue - rightValue;
            else if (triad.op == "*") result = leftValue * rightValue;
            else if (triad.op == "/") result = leftValue / rightValue;

            triads[i] = { "C", to_string(result), "Ø" };
            markedForRemoval[leftIndex] = true;
        }
    }
}

void Analyzer::applyVariableReplacement(size_t i, vector<bool>& markedForRemoval) {
    const Triad& triad = triads[i];
    if (triad.op == "=" && triad.op1[0] == '^') {
        size_t index = 0;
        size_t leftIndex = 0;
        for (size_t j = 1; j < triad.op2.size(); ++j) {
            leftIndex = index * 10 + (triad.op2[j] - '0'); // конвертация вручную
        }
        index--;
        if (triads[leftIndex].op == "V") {
            triads[i].op1 = triads[leftIndex].op1;
            markedForRemoval[leftIndex] = true;
        }
    }
}

void Analyzer::writeOptimizedTriads(ofstream& output) {
    output << "Optimized Triads:" << endl;
    for (size_t i = 0; i < triads.size(); ++i) {
        if (triads[i].op != "R") {
            output << i + 1 << ": " << triads[i].op << "(" << triads[i].op1 << ", " << triads[i].op2 << ")" << endl;
        }
    }
}

void Analyzer::Run() {
    int operatorCount = 0;
    GetSymbol();
    while (c != EOF) {
        while (std::isspace(c)) GetSymbol();
        MethodS();

    }
    writeTriads(out);

    optimizeTriads();
    writeOptimizedTriads(out);
    std::cout << "Parceng Compilated!\n";
}