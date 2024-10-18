#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

typedef enum States { Normal, Slash, Comment, Star, SlashSlash, Odinar, Stroka, SlashOdinar, SlashStroka,
				Block, Nol, Shesnad, Vosmir, Desatich, Unsigned, UnLong, UnLongLong, Long, LongLong, X_Const };

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
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else if (c == '_' || (c >= 'A' && c <= 'Z') ||(c >= 'a' && c <= 'z')) {
					State = Block;
				}
				else if (c == '0') {
					State = Nol;
					outFile.put(c);
				}
				else if (c >= '1' && c <= '9') {
					State = Desatich;
					outFile.put(c);
				}
				break;
			case Slash:
				if (c == '*') {
					State = Comment;
				}
				//else if (c >= '0' && c <= '7') {
				//	State = Vosmir;
				//}
				//else if (c >= '0' && c <= '9') {
				//	State = Desatich;
				//}
				//else if ((c >= '0' && c <= '9')||(c >= 'A' || c <= 'F')) {
				//	State = Shesnad;
				//}
				else if (c == '/') {
					State = SlashSlash;
				}
				else {
					State = Normal;
				}
				break;
			case SlashSlash:
				if (c == '\n') {
					State = Normal;
				}
				if (c == '\r') {
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
			case Odinar:
				if (c == '\'') {
					State = Normal;
				}
				else if (c == '\\') {
					State = SlashOdinar;
				}
				break;
			case Stroka:
				if (c == '\"') {
					State = Normal;
				}
				else if (c == '\\') {
					State = SlashStroka;
				}
				break;
			case SlashOdinar:
				State = Odinar;
				break;
			case SlashStroka:
				State = Stroka;
				break;
			case Block:
				if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
					State = Block;
				}
				else if (c == '/') {
					State = Slash;
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else {
					State = Normal;
				}
				break;
			case Nol:
				if (c == 'x' || c == 'X') {
					State = X_Const;
					outFile << c;
				}
				else if (c >= '0' && c <= '7') {
					State = Vosmir;
					outFile << c;
				}
				else if (c == 'u' || c == 'U') {
					State = Unsigned;
					outFile << c;
				}
				else if (c == 'l' || c == 'L') {
					State = Long;
					outFile << c;
				}
				else if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tInt\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else {
					State = Normal;
					outFile << c << "\tInt\n";
				}
				break;
			case X_Const:
				if ((c >= 'A' && c <= 'F') || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')) {
					State = Shesnad;
					outFile << c;
				}
				else if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tInt\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				break;
			case Shesnad:
				if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
					State = Shesnad;
					outFile << c;
				}
				else if (c == 'u' || c == 'U') {
					State = Unsigned;
					outFile << c;
				}
				else if (c == 'l' || c == 'L') {
					State = Long;
					outFile << c;
				}
				else if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tInt\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else {
					State = Normal;
					outFile << "\tInt\n";
				}
				break;
			case Vosmir:
				if (c >= '0' && c <= '7') {
					State = Vosmir;
					outFile << c;
				}
				else if (c == 'u' || c == 'U') {
					State = Unsigned;
					outFile << c;
				}
				else if (c == 'l' || c == 'L') {
					State = Long;
					outFile << c;
				}
				else if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '8' || c == '9') {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tInt\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else {
					State = Normal;
					outFile << "\tInt\n";
				}
				break;
			case Desatich:
				if (c >= '0' && c <= '9') {
					State = Desatich;
					outFile << c;
				}
				else if (c == 'U' || c == 'u') {
					State = Unsigned;
					outFile << c;
				}
				else if (c == 'L' || c == 'l') {
					State = Long;
					outFile << c;
				}
				else if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tInt\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else {
					State = Normal;
					outFile << "\tInt\n";
				}
				break;
			case Unsigned:
				if (c == 'L' || c == 'l') {
					State = UnLong;
					outFile << c;
				}
				else if (c == '_' || (c >= 'A' && c < 'L') || (c > 'L' && c <= 'Z') || (c >= 'a' && c < 'l') || (c > 'l' && c <= 'z')) {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tUnsigned\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else {
					State = Normal;
					outFile << "\tUnsigned\n";
				}
				break;
			case UnLong:
				if (c == 'L' || c == 'l') {
					State = UnLongLong;
					outFile << c;
				}
				else if (c == '_' || (c >= 'A' && c < 'L') || (c > 'L' && c <= 'Z') || (c >= 'a' && c < 'l') || (c > 'l' && c <= 'z')) {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tUnsigned Long\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else {
					State = Normal;
					outFile << "\tUnsigned Long\n";
				}
				break;
			case UnLongLong:
				if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tUnsigned Long Long\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else{
					State = Normal;
					outFile << "\tUnsigned Long Long\n";
				}
				break;
			case Long:
				if (c == 'L' || c == 'l') {
					State = LongLong;
					outFile << c;
				}
				else if (c == '_' || (c >= 'A' && c < 'L') || (c > 'L' && c <= 'Z') || (c >= 'a' && c < 'l') || (c > 'l' && c <= 'z')) {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tLong\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else {
					State = Normal;
					outFile << "\tLong\n";
				}
				break;
			case LongLong:
				if (c == 'u' || c == 'U') {
					State = UnLongLong;
					outFile << c;
				}
				else if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
					State = Block;
					outFile << c << "\tError\n";
				}
				else if (c == '/') {
					State = Slash;
					outFile << "\tLong Long\n";
				}
				else if (c == '\'') {
					State = Odinar;
				}
				else if (c == '\"') {
					State = Stroka;
				}
				else {
					State = Normal;
					outFile << "\tLong Long\n";
				}
				break;
			}
		}
	}

	inFile.close();
	outFile.close();
	return 0;
}
