#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

typedef enum States { Normal, Slash, Comment, Star, SlashSlash, Odinar, Stroka, SlashOdinar, SlashStroka,
				Block, Nol, Shesnad, Vosmir, Desatich, Unsigned, UnLong, UnLongLong, Long, LongLong };

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
					outFile.put('\'');
				}
				else if (c == '\"') {
					State = Stroka;
					outFile.put('\"');
				}
				else if (c == '_' || (c >= '65' && c<= '122')) {
					State = Block;
				}
				else if (c == '0') {
					State = Nol;
					outFile.put(c);
				}
				else if (c <= '1' && c >= '9') {
					State = Desatich;
					outFile.put(c);
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
				else {
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
				if (c == '\r') {
					outFile.put('\r');
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
					outFile.put('\'');
				}
				else if (c == '\\') {
					State = SlashOdinar;
					outFile.put('\\');
				}
				else {
					outFile.put(c);
				}
				break;
			case Stroka:
				if (c == '\"') {
					State = Normal;
					outFile.put('\"');
				}
				else if (c == '\\') {
					State = SlashStroka;
					outFile.put('\\');
				}
				else {
					outFile.put(c);
				}
				break;
			case SlashOdinar:
				outFile.put(c);
				State = Odinar;
				break;
			case SlashStroka:
				outFile.put(c);
				State = Stroka;
				break;
			case Block:
				if (c == '_' || (c >= '65' && c <= '122')) {
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
				if (c == ' ') {
					State = Normal;
					outFile << c << "\tint";
				}
				else if (c == 'x') {
					State = Shesnad;
					outFile << c;
				}
				else if (c >= '0' && c <= '7') {
					State = Vosmir;
					outFile << c;
				}
				else if (c == '_' || (c >= '65' && c <= '122')) {
					State = Block;
					outFile << c << "\tError";
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
				break;
			case Shesnad:
				if ((c >= '0' && c <= '9') || (c >= '65' && c <= '70')) {
					State = Shesnad;
					outFile << c;
				}
				else if (c >= '65' && c <= '122') {
					State = Normal;
					outFile << c << "\t Shestnadcatirichnaia";
				}
				else if (c == '_' || (c >= '65' && c <= '122')) {
					State = Block;
					outFile << c << "\tError";
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
				break;
			case Vosmir:
				if (c >= '0' && c <= '7') {
					State = Vosmir;
					outFile << c;
				}
				else if (c >= '65' && c <= '122') {
					State = Normal;
					outFile << c << "\t Vosmirichnaia";
				}
				else if (c == '_' || (c >= '65' && c <= '122') || c == '8' || c == '9') {
					State = Block;
					outFile << c << "\tError";
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
				break;
			case Desatich:
				if (c >= '0' && c <= '9') {
					State = Desatich;
					outFile << c;
				}
				else if (c == '_' || (c >= '65' && c <= '122')) {
					State = Block;
					outFile << c << "\tError";
				}
				else if (c >= '65' && c <= '122') {
					State = Normal;
					outFile << c << "\t Desiatichnaia";
				}
				else if (c == '85' || c == '117') {
					State = Unsigned;
					outFile << c;
				}
				else if (c == '76' || c == '108') {
					State = Long;
					outFile << c;
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
				break;
			case Unsigned:
				if (c == ' ') {
					State = Normal;
					outFile << c << "\tUnsigned";
				}
				else if (c == '76' || c == '108') {
					State = UnLong;
					outFile << c;
				}
				else if (c == '_' || (c >= '65' && c < '76') || (c > '76' && c <= '90') || (c >= '97' && c < '108') || (c > '108' && c <= '122')) {
					State = Block;
					outFile << c << "\tError";
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
				break;
			case UnLong:
				if (c == ' ') {
					State = Normal;
					outFile << c << "\tUnsigned Long";
				}
				else if (c == '76' || c == '108') {
					State = UnLongLong;
					outFile << c;
				}
				else if (c == '_' || (c >= '65' && c < '76') || (c > '76' && c <= '90') || (c >= '97' && c < '108') || (c > '108' && c <= '122')) {
					State = Block;
					outFile << c << "\tError";
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
				break;
			case UnLongLong:
				if (c == ' ') {
					State = Normal;
					outFile << c << "\tUnsigned Long Long";
				}
				else if (c != ' ') {
					State = Block;
					outFile << c << "\tError";
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
				break;
			case Long:
				if (c == ' ') {
					State = Normal;
					outFile << c << "\tLong";
				}
				else if (c == '76' || c == '108') {
					State = LongLong;
					outFile << c;
				}
				else if (c == '_' || (c >= '65' && c < '76') || (c > '76' && c <= '90') || (c >= '97' && c < '108') || (c > '108' && c <= '122')) {
					State = Block;
					outFile << c << "\tError";
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
				break;
			case LongLong:
				if (c == ' ') {
					State = Normal;
					outFile << c << "\tLong Long";
				}
				else if (c != ' ') {
					State = Block;
					outFile << c << "\tError";
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
				break;
			}
		}
	}

	inFile.close();
	outFile.close();
	return 0;
}