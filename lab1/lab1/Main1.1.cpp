#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

typedef enum States { Normal, Slash, Comment, Star, SlashSlash };

int main(int argc, char** argv) {
	ifstream inFile("in.c");
	ofstream outFile("out.c");

	if (!inFile.is_open() || !outFile.is_open()) {
		cout << "Error opening file." << endl;
		return 1;
		inFile.close();
		outFile.close();
	}
	else {
		States State = Normal;
		char c;

		while ((c = inFile.get()) != EOF) { 
			switch (State)
			{
			case Normal:
				if (c == '/') {
					State = Slash;
				}
				else {
					outFile.put(c);
				}
				break;
			case Slash:
				if (c == '*') {
					State = Comment;
				}
				else if (c == '/') {
					State = SlashSlash;
				}
				else{
					State = Normal;
					outFile.put('/');
					outFile.put(c);
				}
				break;
			case SlashSlash:
				if (c == '\n') {
					outFile.put('\n');
					State = Normal;
				}
				break;
			case Comment:
				if (c == '*') {
					State = Star;
				}
				break;
			case Star:
				if (c == '/') {
					State = Normal;
				}
				else if (c == '*') {
				}
				else {
					State = Comment;
				}
				break;
			}
		}
	}

	inFile.close();
	outFile.close();
	return 0;
}