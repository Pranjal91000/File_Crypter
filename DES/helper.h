#include <string>
#include<iostream>
#include<iomanip>
using namespace std;

string shift_bit(string s, int n)
	{
		string k = "";

		for (int i = n; i < s.size(); i++)
			k += s[i];

		for (int i = 0; i < n; i++)
			k += s[i];

		return k;
	}


int pow(int x, int y) {
    if (y == 0) return 1; 
    if (y == 1) return x; 
    
    int ans = x;
    for (int i = 2; i <= y; i++) {
        ans *= x;
    }
    return ans;
}

//Conversion to helper functions

string Bin_To_Hex(string s)
{
	string hex = "";
	for (int i = 0; i < s.size(); i += 4)
	{
		string k = "";
		for (int j = i; j < i + 4; j++)
			k += s[j];
		if (k == "0000")
			hex += '0';
		else if (k == "0001")
			hex += '1';
		else if (k == "0010")
			hex += '2';
		else if (k == "0011")
			hex += '3';
		else if (k == "0100")
			hex += '4';
		else if (k == "0101")
			hex += '5';
		else if (k == "0110")
			hex += '6';
		else if (k == "0111")
			hex += '7';
		else if (k == "1000")
			hex += '8';
		else if (k == "1001")
			hex += '9';
		else if (k == "1010")
			hex += 'A';
		else if (k == "1011")
			hex += 'B';
		else if (k == "1100")
			hex += 'C';
		else if (k == "1101")
			hex += 'D';
		else if (k == "1110")
			hex += 'E';
		else if (k == "1111")
			hex += 'F';
	}
	return hex;
}

string Hex_To_Bin(const string& hex) {
    string binary = "";
    for (char hexChar : hex) {
        int val = (hexChar >= '0' && hexChar <= '9') ? hexChar - '0' : (hexChar - 'A' + 10);
        for (int i = 3; i >= 0; --i) {
            binary += (val >> i) & 1 ? '1' : '0';
        }
    }
    return binary;
}


string Dec_To_Bin(int n)
{
	string bin = "";
	while (n > 0)
	{
		bin = (char)(n % 2 + '0') + bin;
		n /= 2;
	}
	while (bin.size() < 4)
		bin = '0' + bin;
	return bin;
}


// Utility helper functions
string stringToHex(const string& input) 
{
    stringstream ss;
    ss << hex << setfill('0');
    for (unsigned char c : input) {
        ss << setw(2) << static_cast<int>(c);
    }
    return ss.str();
}

// Pad the input to ensure it's a multiple of 8 bytes (16 hex characters)
string padInput(const string& input) {
    size_t remainder = input.size() % 64;
    if (remainder != 0) {
        size_t padLength = 64 - remainder;
        return input + string(padLength, '0');  // Pad with 0s (or another padding scheme)
    }
    return input;
}


bool validateHexInput(const string& input) 
{
    for (char c : input) 
    {
        if (!((c <= 'f' && c >= 'a') ||
              (c <= 'F' && c >= 'A') ||
              (c >= '0' && c <= '9')))
        {
            return false;
        }
    }
    return true;
}


string binToHex(string s)
{
	string hex = "";
	for (int i = 0; i < s.size(); i += 4)
	{
		string k = "";
		for (int j = i; j < i + 4; j++)
			k += s[j];
		if (k == "0000")
			hex += '0';
		else if (k == "0001")
			hex += '1';
		else if (k == "0010")
			hex += '2';
		else if (k == "0011")
			hex += '3';
		else if (k == "0100")
			hex += '4';
		else if (k == "0101")
			hex += '5';
		else if (k == "0110")
			hex += '6';
		else if (k == "0111")
			hex += '7';
		else if (k == "1000")
			hex += '8';
		else if (k == "1001")
			hex += '9';
		else if (k == "1010")
			hex += 'A';
		else if (k == "1011")
			hex += 'B';
		else if (k == "1100")
			hex += 'C';
		else if (k == "1101")
			hex += 'D';
		else if (k == "1110")
			hex += 'E';
		else if (k == "1111")
			hex += 'F';
	}
	return hex;
}

string hexToBin(string s)
{
	string bin = "";
	for (int i = 0; i < s.size(); i++)
	{
		switch (s[i])
		{
		case '0': bin += "0000"; break;
		case '1': bin += "0001"; break;
		case '2': bin += "0010"; break;
		case '3': bin += "0011"; break;
		case '4': bin += "0100"; break;
		case '5': bin += "0101"; break;
		case '6': bin += "0110"; break;
		case '7': bin += "0111"; break;
		case '8': bin += "1000"; break;
		case '9': bin += "1001"; break;
		case 'A':
		case 'a': bin += "1010"; break;
		case 'B':
		case 'b': bin += "1011"; break;
		case 'C':
		case 'c': bin += "1100"; break;
		case 'D':
		case 'd': bin += "1101"; break;
		case 'E':
		case 'e': bin += "1110"; break;
		case 'F':
		case 'f': bin += "1111"; break;

		}
	}
	return bin;
}

string decToBin(int n)
{
	string bin = "";
	while (n > 0)
	{
		bin = (char)(n % 2 + '0') + bin;
		n /= 2;
	}
	while (bin.size() < 4)
		bin = '0' + bin;
	return bin;
}