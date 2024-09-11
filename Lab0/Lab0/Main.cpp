#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

int main() {
	setlocale(LC_ALL, "russian");
	//fstream fs;  
	//fs.open("in.c");

	//string str, str1, res;
	//bool comm = false;

	//while (!getline(fs, str).fail()) {
	//	//cout << str << endl;
	//	str1 += str;
	//}
	//fs.close();
	//cout << str1 << endl << endl;


	/*
	for (size_t i = 0; i < str1.length(); ++i) {
		if (str1[i] == '/' && str1[i + 1] == '*' && !comm) {
			comm = true;
			i++;
		}
		else if (str1[i] == '*' && str1[i + 1] == '/' && comm) {
			comm = false;
			i++;
		}
		else if (!comm) {
			res += str1[i];
		}
	}

	cout << res << endl;*/

	ifstream inFile("in.c");
	ofstream outFile("out.c");

	string str, res;
	bool comm = false;

	if (!inFile.is_open() || !outFile.is_open()) {
		std::cerr << "Ошибка открытия файлов." << std::endl;
		return 1;
	}

	while (getline(inFile, str)) {
		for (size_t i = 0; i < str.length(); ++i) {
			if (str[i] == '/' && str[i + 1] == '*' && !comm) {
				comm = true;
				i++;
			}
			else if (str[i] == '*' && str[i + 1] == '/' && comm) {
				comm = false;
				i++;
			}
			else if (!comm) {
				res += str[i];	
			}	
		}
		outFile << res << endl;
		res = ' ';
	}

	cout << "Многострочные комментарии успешно удалены!" << endl;

	inFile.close();
	outFile.close();

	return 0;
}