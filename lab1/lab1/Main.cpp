#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

typedef enum States { Normal, Slash, Comment, Star };

int main(int argc, char **argv) {
	ifstream inFile("in.c");
	ofstream outFile("out.c");

	if (!inFile.is_open() || !outFile.is_open()) {
		cout << "Ошибка открытия файла." << endl;
		return 1;
		inFile.close();
		outFile.close();
	}
	else{
	States State = Normal;
	char c;

	while (c = inFile.get() && !EOF) { //начало автомата


	}
	inFile.close();
	outFile.close();
	}
	return 0;
}