#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <bitset>
#include <time.h>

using namespace std;

string ptBlock1;
string ptBlock2;
string ptBlock3;
string ptBlock4;

const string KEY1 = "1010";
const string KEY2 = "0010";
const string KEY3 = "0011";
const string KEY4 = "1010";


string doXor(const string a, const string b) {
	string val = "";
	for (int i = 0; i < a.length(); i++) {
		if (a.at(i) == b.at(i)) {
			val.append("0");
		}
		else {
			val.append("1");
		}
	}
	return val;
}
int convertBinaryStringToInt(const string in) {
	if (in == "00") {
		return 0;
	}
	else if (in == "01") {
		return 1;
	}
	else if (in == "10") {
		return 2;
	}
	else if (in == "11") {
		return 3;
	}
	else {
		cerr << "invalid binary conversion" << endl;
		return -1;
	}
}
int getSub(const string in, int box) {
	int r = convertBinaryStringToInt(in.substr(2,4));
	int c = convertBinaryStringToInt(in.substr(0,2));

	int S1[4][4] = {
		{ 15, 10, 2, 5 },
		{ 8, 4, 11, 6 },
		{ 1, 0, 14, 7 },
		{ 9, 3, 12, 13 }
	};
	int S2[4][4] = {
		{ 4, 0, 15, 10 },
		{ 8, 9, 7, 13 },
		{ 5, 1, 6, 11 },
		{ 2, 3, 14, 12 }
	};

	int out;
	if (box == 1) {
		out = S1[r][c];
	}
	else if (box == 2) {
		out = S2[r][c];
	}
	else {
		cerr << "Error: bad S-Box selection" << endl;
	}
	return out;
}
void setBlocks(const string in) {
	ptBlock1 = "";
	ptBlock2 = "";
	ptBlock3 = "";
	ptBlock4 = "";
	for (int i = 0; i < 16; i++) {
		if (i < 4) {
			string toAdd(1, in[i]);
			ptBlock1.append(1, in[i]);
		}
		else if (i < 8) {
			string toAdd(1, in[i]);
			ptBlock2.append(1, in[i]);
		}
		else if (i < 12) {
			string toAdd(1, in[i]);
			ptBlock3.append(1, in[i]);
		}
		else { //if i < 16
			string toAdd(1, in[i]);
			ptBlock4.append(1, in[i]);
		}
	}
}
/*
//old encryption algorithm
string encrypt(const string in) {
	string copy = in;
	setBlocks(copy);
	int cipherBlock1 = getSub(doXor(ptBlock2, KEY1), 1);
	int cipherBlock2 = getSub(doXor(ptBlock4, KEY3), 2);
	int cipherBlock3 = getSub(doXor(ptBlock1, KEY2), 1);
	int cipherBlock4 = getSub(doXor(ptBlock3, KEY4), 2);

	string cipherText;
	cipherText.append(bitset<4>(cipherBlock1).to_string());
	cipherText.append(bitset<4>(cipherBlock2).to_string());
	cipherText.append(bitset<4>(cipherBlock3).to_string());
	cipherText.append(bitset<4>(cipherBlock4).to_string());

	return cipherText;
}
*/
string intToBinary(const int in) {
	switch (in) {
		case 0:
			return "0000";
		case 1:
			return "0001";
		case 2:
			return "0010";
		case 3:
			return "0011";
		case 4:
			return "0100";
		case 5:
			return "0101";
		case 6:
			return "0110";
		case 7:
			return "0111";
		case 8:
			return "1000";
		case 9:
			return "1001";
		case 10:
			return "1010";
		case 11:
			return "1011";
		case 12:
			return "1100";
		case 13:
			return "1101";
		case 14:
			return "1110";
		case 15:
			return "1111";
		default:
			cerr << "bad intToBinary operation" << endl;
			return "";
	}
}
string createRandomString() {
	string randString = "";

	srand(time(NULL));
	for (int i = 0; i < 16; i++) {
		if ((rand() % 2 + 1) == 0) { //generate rand number between 1 and 2 inclusive
			randString.append("0");
		}
		else {
			randString.append("1");
		}
	}
	return randString;
}
//improved encryption algorithm
string encrypt(const string in) {
	string copy = in;
	string rand = createRandomString();
	string XORwithRand = doXor(copy, rand);

	setBlocks(XORwithRand);
	int cipherBlock1 = getSub(doXor(ptBlock2, KEY1), 1);
	int cipherBlock2 = getSub(doXor(ptBlock4, KEY3), 2);
	int cipherBlock3 = getSub(doXor(ptBlock1, KEY2), 1);
	int cipherBlock4 = getSub(doXor(ptBlock3, KEY4), 2);

	int lvl2CipherBlock1 = getSub(doXor(intToBinary(cipherBlock3), KEY4), 1);
	int lvl2CipherBlock2 = getSub(doXor(intToBinary(cipherBlock2), KEY1), 2);
	int lvl2CipherBlock3 = getSub(doXor(intToBinary(cipherBlock4), KEY3), 1);
	int lvl2CipherBlock4 = getSub(doXor(intToBinary(cipherBlock1), KEY2), 2);

	string cipherText;
	cipherText.append(bitset<4>(cipherBlock1).to_string());
	cipherText.append(bitset<4>(cipherBlock2).to_string());
	cipherText.append(bitset<4>(cipherBlock3).to_string());
	cipherText.append(bitset<4>(cipherBlock4).to_string());

	return cipherText;
}

/* Part 1
int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Error: No plaintext argument" << endl;
		return(-1);
	}
	string plainText = argv[1];
	setBlocks(plainText);

	string cipherText = encrypt();

	cout << cipherText << endl;

	return 0;
}
*/
string flipBit(const string in, int idx) {
	string out = in;
	if (in.at(idx) == '0') {
		out.replace(idx, 1, "1");
	}
	else if (in.at(idx) == '1') {
		out.replace(idx, 1, "0");
	}
	else { //error, string input is neither 0 or 1
		return 0; 
	}
	return out;
}
int compareBits(const string in1, const string in2) {
	int diffCount = 0;
	for (int i = 0; i < 16; i++) {
		if (in1.at(i) != in2.at(i)) {
			diffCount++;
		}
	}
	return diffCount;
}
double calcAvalanche(const string in) {
	const string originalCipherText = encrypt(in);
	//cout << "og cipher text " << originalCipherText << endl;
	int totalDiffBits = 0;

	for (int i = 0; i < 16; i++) {
		string flippedString = flipBit(in, i);
		//cout << "flipped string " << flippedString << endl;
		string alteredCipherText = encrypt(flippedString);
		//cout << "altcipher text " << alteredCipherText << endl;
		totalDiffBits += compareBits(originalCipherText, alteredCipherText);
	}
	double avalanchePercent = totalDiffBits / 256.0 * 100;
	//cout << "total altered bits over 16 bit flips: " << totalDiffBits << endl;
	return avalanchePercent;
}
int main(int argc, char* argv[]) {
	string pt1 = "1001010001100110";
	string pt2 = "0010100111000010";
	string pt3 = "0101110011100010";
	string pt4 = "1001110000100111";
	string pt5 = "1011110111011111";
	string pt6 = "0001110100010011";
	string pt7 = "1110000111000011";
	string pt8 = "0011111000000010";
	string pt9 = "0101001111011011";
	string pt10 = "1100001001110100";	double ava1 = calcAvalanche(pt1);	double ava2 = calcAvalanche(pt2);	double ava3 = calcAvalanche(pt3);	double ava4 = calcAvalanche(pt4);	double ava5 = calcAvalanche(pt5);	double ava6 = calcAvalanche(pt6);	double ava7 = calcAvalanche(pt7);	double ava8 = calcAvalanche(pt8);	double ava9 = calcAvalanche(pt9);	double ava10 = calcAvalanche(pt10);	cout << "avalanche on plaintext 1: " << ava1 << endl;
	cout << "avalanche on plaintext 2: " << ava2 << endl;
	cout << "avalanche on plaintext 3: " << ava3 << endl;
	cout << "avalanche on plaintext 4: " << ava4 << endl;
	cout << "avalanche on plaintext 5: " << ava5 << endl;
	cout << "avalanche on plaintext 6: " << ava6 << endl;
	cout << "avalanche on plaintext 7: " << ava7 << endl;
	cout << "avalanche on plaintext 8: " << ava8 << endl;
	cout << "avalanche on plaintext 9: " << ava9 << endl;
	cout << "avalanche on plaintext 10: " << ava10 << endl;
	
	double avgAva = (ava1 + ava2 + ava3 + ava4 + ava5 +
				ava6 + ava7 + ava8 + ava9 + ava10) / 10;
	cout << endl << "average avalanche: " << avgAva << endl;
}


