#include "string_utilities.h"
#include <cstdio>
#include <sys/stat.h>
unsigned int MAXSTRINGLENGTH = 20;
unsigned int MAXDIGITS = 4;
char getDigit(int n);
string string_number(int number, int base)
{
  if (number == 0)
    return string("0");
  char st[100];
  sprintf(st, "%d", number);
  string ret = st;
  remove(ret, ' ');
  return ret;
  //   int n;
  //   string sign=(number>0?"":"-");
  //   if(number<0)number=-number;
  //   while(number>0)
  //     {
  //       n=number%base;
  //       number=number/base;
  //       ret=getDigit(n)+ret;
  //     }
  //   return sign+ret;
}
string string_number(long int number, int base)
{
  if (number == 0)
    return string("0");
  char st[100];
  sprintf(st, "%d", (int)number);
  string ret = st;
  remove(ret, ' ');
  return ret;
  //   string ret="";
  //   int n;
  //   string sign=(number>0?"":"-");
  //   if(number<0)number=-number;
  //   while(number>0)
  //     {
  //       n=number%base;
  //       number=number/base;
  //       ret=getDigit(n)+ret;
  //     }
  //   return sign+ret;
}
string string_number(unsigned int number, int base)
{
  if (number == 0)
    return string("0");
  char st[100];
  sprintf(st, "%d", number);
  string ret = st;
  remove(ret, ' ');
  return ret;
  //   string ret="";
  //   int n;
  //   while(number>0)
  //     {
  //       n=number%base;
  //       number=number/base;
  //       ret=getDigit(n)+ret;
  //     }
  //   return ret;
}
#include <iostream>
string string_number(float number, int base)
{
  if (number == 0)
    return string("0");
  char st[100];
  sprintf(st, "%g", number);
  string ret = st;
  remove(ret, ' ');
  return ret;
  //   string ret="",digits="";
  //   int n;
  //   string sign=(number>0?"":"-");
  //   if(number<0)number=-number;
  //   int before=(int)number;
  //   float after=number-before;
  //   if(before==0)ret="0";
  //   while(before>0)
  //     {
  //       n=before%base;
  //       before=before/base;
  //       ret=getDigit(n)+ret;
  //     }
  //   if(after>0)
  //     ret=ret+'.';
  //   after=after*base;
  //   while(after>0&&digits.length()<MAXDIGITS)
  //     {
  //       n=((int)after)%base;
  //       after=after*base;
  //       digits=digits+getDigit(n);
  //     }
  //   return sign+ret+digits;
}
string string_number(double number, int base)
{
  if (number == 0)
    return string("0");
  char st[100];
  sprintf(st, "%g", number);
  string ret = st;
  remove(ret, ' ');
  return ret;
  //   string ret="",digits="";
  //   int n;
  //   string sign=(number>0?"":"-");
  //   if(number<0)number=-number;
  //   int before=(int)number;
  //   float after=number-before;
  //   if(before==0)ret="0";
  //   while(before>0)
  //     {
  //       n=before%base;
  //       before=before/base;
  //       ret=getDigit(n)+ret;
  //     }
  //   if(after>0)
  //     ret=ret+'.';
  //   after=after*base;
  //   while(after>0&&digits.length()<MAXDIGITS)
  //     {
  //       n=((int)after)%base;
  //       after=after*base;
  //       digits=digits+getDigit(n);
  //     }
  //   return sign+ret+digits;
}
string string_number(long double number, int base)
{
  if (number == 0)
    return string("0");
  char st[100];
  sprintf(st, "%g", (double)number);
  string ret = st;
  remove(ret, ' ');
  return ret;
  //   string ret="",digits="";
  //   int n;
  //   string sign=(number>0?"":"-");
  //   if(number<0)number=-number;
  //   int before=(int)number;
  //   float after=number-before;
  //   if(before==0)ret="0";
  //   while(before>0)
  //     {
  //       n=before%base;
  //       before=before/base;
  //       ret=getDigit(n)+ret;
  //     }
  //   if(after>0)
  //     ret=ret+'.';
  //   after=after*base;
  //   while(after>0&&digits.length()<MAXDIGITS)
  //     {
  //       n=((int)after)%base;
  //       after=after*base;
  //       digits=digits+getDigit(n);
  //     }
  //   return sign+ret+digits;
}
string string_number(double number, char f, int prec)
{
  if (number == 0)
    return string("0");
  char st[100];
  sprintf(st, "%g", number);
  string ret = st;
  remove(ret, ' ');
  return ret;
  //   string ret="",digits="";
  //   int n;
  //   string sign=(number>0?"":"-");
  //   if(number<0)number=-number;
  //   int before=(int)number;
  //   if(f=='e')
  //     before=(int)number;
  //   float after=number-before;
  //   if(before==0)ret="0";
  //   while(before>0)
  //     {
  //       n=before%10;
  //       before=before/10;
  //       ret=getDigit(n)+ret;
  //     }
  //   if(after>0)
  //     ret=ret+'.';
  //   after=after*10;
  //   while(after>0&&ret.length()<MAXDIGITS&&(int)ret.length()<prec)
  //     {
  //       n=((int)after)%10;
  //       after=after*10;
  //       digits=digits+getDigit(n);
  //     }
  //   return sign+ret+digits;
}
char getDigit(int n)
{
  switch (n) {
  case 0:
    return '0';
  case 1:
    return '1';
  case 2:
    return '2';
  case 3:
    return '3';
  case 4:
    return '4';
  case 5:
    return '5';
  case 6:
    return '6';
  case 7:
    return '7';
  case 8:
    return '8';
  case 9:
    return '9';
  case 10:
    return 'A';
  case 11:
    return 'B';
  case 12:
    return 'C';
  case 13:
    return 'D';
  case 14:
    return 'E';
  case 15:
    return 'F';
  case 16:
    return 'G';
  case 17:
    return 'H';
  case 18:
    return 'I';
  case 19:
    return 'J';
  case 20:
    return 'K';
  }
  return '0';
}
#include <stdlib.h>
int toInt(string s) { return atoi(s.data()); }
float toFloat(string s) { return atof(s.data()); }
double toDouble(string s) { return strtod(s.data(), NULL); }
string remove(string str, const char* s)
{
  string ret = str;
  string tmp(s);
  int l = tmp.length();
  while (ret.find(s) < ret.npos) {
    ret.replace(ret.find(s), l, "");
  }
  return ret;
}
string remove(string str, char c)
{
  string ret = "";
  for (unsigned int i = 0; i < str.length(); i++)
    if (str[i] != c)
      ret = ret + str[i];
  return ret;
}
int contains(string str, char c)
{
  int ret = 0;
  for (unsigned int i = 0; i < str.length(); i++)
    if (str[i] == c)
      ret++;
  return ret;
}
int contains(string str, const char* s)
{
  int ret = 0;
  string tmp = str;
  string tmp1(s);
  int l = tmp1.length();
  while (tmp.find(s) < tmp.npos) {
    ret++;
    tmp.replace(tmp.find(s), l, "");
  }
  return ret;
}
bool hasA(string str, const char* s) { return (str.find(s) < str.npos); }
bool hasA(string str, const string& s) { return (str.find(s) < str.npos); }
bool hasA(string str, const char& s) { return (str.find(s) < str.npos); }
bool isSpace(char c) { return (c == ' ' || c == '\t'); }
bool isNumber(char c)
{
  if (c == '0')
    return true;
  if (c == '1')
    return true;
  if (c == '2')
    return true;
  if (c == '3')
    return true;
  if (c == '4')
    return true;
  if (c == '5')
    return true;
  if (c == '6')
    return true;
  if (c == '7')
    return true;
  if (c == '8')
    return true;
  if (c == '9')
    return true;
  return false;
}

bool isNumber(string str)
{
  if (!isNumber(str[0]) && str[0] != '-')
    return false;
  for (unsigned int i = 1; i < str.length(); i++)
    if (!isNumber(str[i]))
      return false;
  return true;
}
bool isDigit(string str)
{
  if (contains(str, '.') > 1)
    return false;
  if (!isNumber(str[0]) && str[0] != '-' && str[0] != '.')
    return false;
  for (unsigned int i = 1; i < str.length(); i++)
    if (!isNumber(str[i]) && str[i] != '.')
      return false;
  return true;
}
string lower(string str)
{
  string ret = "";
  char c;
  for (unsigned int i = 0; i < str.length(); i++) {
    c = str[i];
    switch (str[i]) {
    case 'A':
      c = 'a';
      break;
    case 'B':
      c = 'b';
      break;
    case 'C':
      c = 'c';
      break;
    case 'D':
      c = 'd';
      break;
    case 'E':
      c = 'e';
      break;
    case 'F':
      c = 'f';
      break;
    case 'G':
      c = 'g';
      break;
    case 'H':
      c = 'h';
      break;
    case 'I':
      c = 'i';
      break;
    case 'J':
      c = 'j';
      break;
    case 'K':
      c = 'k';
      break;
    case 'L':
      c = 'l';
      break;
    case 'M':
      c = 'm';
      break;
    case 'N':
      c = 'n';
      break;
    case 'O':
      c = 'o';
      break;
    case 'P':
      c = 'p';
      break;
    case 'Q':
      c = 'q';
      break;
    case 'R':
      c = 'r';
      break;
    case 'S':
      c = 's';
      break;
    case 'T':
      c = 't';
      break;
    case 'U':
      c = 'u';
      break;
    case 'V':
      c = 'v';
      break;
    case 'W':
      c = 'w';
      break;
    case 'X':
      c = 'x';
      break;
    case 'Y':
      c = 'y';
      break;
    case 'Z':
      c = 'z';
      break;
    }
    ret = ret + c;
  }
  return ret;
}
string upper(string str)
{
  string ret = "";
  char c;
  for (unsigned int i = 0; i < str.length(); i++) {
    c = str[i];
    switch (str[i]) {
    case 'a':
      c = 'A';
      break;
    case 'b':
      c = 'B';
      break;
    case 'c':
      c = 'C';
      break;
    case 'd':
      c = 'D';
      break;
    case 'e':
      c = 'E';
      break;
    case 'f':
      c = 'F';
      break;
    case 'g':
      c = 'G';
      break;
    case 'h':
      c = 'H';
      break;
    case 'i':
      c = 'I';
      break;
    case 'j':
      c = 'J';
      break;
    case 'k':
      c = 'K';
      break;
    case 'l':
      c = 'L';
      break;
    case 'm':
      c = 'M';
      break;
    case 'n':
      c = 'N';
      break;
    case 'o':
      c = 'O';
      break;
    case 'p':
      c = 'P';
      break;
    case 'q':
      c = 'Q';
      break;
    case 'r':
      c = 'R';
      break;
    case 's':
      c = 'S';
      break;
    case 't':
      c = 'T';
      break;
    case 'u':
      c = 'U';
      break;
    case 'v':
      c = 'V';
      break;
    case 'w':
      c = 'W';
      break;
    case 'x':
      c = 'X';
      break;
    case 'y':
      c = 'Y';
      break;
    case 'z':
      c = 'Z';
      break;
    }
    ret = ret + c;
  }
  return ret;
}
string replace(string str, const char* s, const char* with)
{
  if (!hasA(str, s))
    return str;
  string ret;
  string s1 = str;
  string s2 = s;
  while (hasA(s1, s)) {
    if (s1.find(s) > 0)
      ret += s1.substr(0, s1.find(s2)) + with;
    else
      ret += with;
    s1 = s1.substr(s1.find(s2) + s2.length(), s1.length() - s1.find(s2) - s2.length());
  }
  return ret + s1;
}
string replace(string str, const string& s, const string& with)
{
  if (!hasA(str, s))
    return str;
  string ret = "";
  string s1 = str;
  while (hasA(s1, s)) {
    if (s1.find(s) > 0)
      ret += s1.substr(0, s1.find(s)) + with;
    else
      ret += with;
    s1 = s1.substr(s1.find(s) + s.length(), s1.length() - s1.find(s) - s.length());
  }
  return ret + s1;
}
bool existing(string strFilename)
{
  struct stat stFileInfo;
  bool blnReturn;
  int intStat;
  // Attempt to get the file attributes
  intStat = stat(strFilename.c_str(), &stFileInfo);
  if (intStat == 0) {
    // We were able to get the file attributes
    // so the file obviously exists.
    blnReturn = true;
  } else {
    // We were not able to get the file attributes.
    // This may mean that we don't have permission to
    // access the folder which contains this file. If you
    // need to do that level of checking, lookup the
    // return values of stat which will give you
    // more details on why stat failed.
    blnReturn = false;
  }
  return (blnReturn);
}
