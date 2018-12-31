#ifndef STRINGUTILITIES
#define STRINGUTILITIES
#include <string>
using namespace std;
string string_number(int number, int base = 10);
string string_number(long int number, int base = 10);
string string_number(unsigned int number, int base = 10);
string string_number(float number, int base = 10);
string string_number(double number, int base = 10);
string string_number(long double number, int base = 10);
string string_number(double n, char f, int prec);
int toInt(string s);
float toFloat(string s);
double toDouble(string s);
string remove(string str, const char* s);
string remove(string srt, char c);
int contains(string str, char c);
int contains(string str, const char* s);
bool hasA(string str, const string& s);
bool hasA(string str, const char* s);
bool hasA(string str, const char& s);
bool isSpace(char c);
bool isNumber(char c);
bool isNumber(string str);
bool isDigit(string str);
string lower(string str);
string upper(string str);
string replace(string str, const char* s, const char* with);
string replace(string str, const string& s, const string& with);

#endif
