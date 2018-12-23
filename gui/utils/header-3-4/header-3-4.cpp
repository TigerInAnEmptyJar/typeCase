#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "string_utilities.h"
using namespace std;
extern bool existing(string fil);
int main(int argc, char** argv)
{
  vector<string> filenames;
  string outDir="";
  bool do3=false,do4=false;
  for(int i=1;i<argc;i++)
    {
      string s=argv[i];
      if(s.substr(0,3)=="-o=")
	{
	  outDir=s.substr(3,s.length()-3);
	}
      else if(s=="--3")
	do3=true;
      else if(s=="--4")
	do4=true;
      else if(existing(s)&&s.substr(s.length()-2,2)==".h")
	filenames.push_back(s);
    }
  if(filenames.size()==0)
    {
      cout<<"no input files found!"<<endl;
      return 1;
    }
  for(unsigned int i=0;i<filenames.size();i++)
    {
      cout<<"converting file \""<<filenames[i].data()<<"\""<<endl;
      vector<string> code3;
      vector<string> code4;
      string s,praepc;
      char c;
      ifstream input;
      input.open(filenames[i].data());
      int mode=0;
      int sub=0;
      while(!input.eof())
	{
	  s="";input.get(c);
	  while(c!='\n'){s=s+c;input.get(c);}
	  if(s[0]=='#')
	    {
	      praepc=s.substr(0,s.find(" "));
	      if(praepc=="#ifndef"||praepc=="#ifdef")
		{
		  if(mode!=0)sub++;
		}
	      else if(praepc=="#else"&&sub==0)
		{
		  if(mode==3)mode=4;else mode=3;
		  continue;
		}
	      else if(praepc=="#endif")
		{
		  if(sub==0&&mode!=0)
		    {
		      mode=0;
		      continue;
		    }
		  else if(sub>0)sub--;
		}
	      else if(praepc=="#if")
		{
		  if(hasA(s,"#if QT_VERSION > 0x040000"))
		    {
		      mode=4;
		      sub=0;
		      continue;
		    }
		  if(hasA(s,"#if QT_VERSION < 0x040000"))
		    {
		      mode=3;
		      sub=0;
		      continue;
		    }
		  if(mode!=0)sub++;
		}
	    }
	  if(mode==3||mode==0)
	    code3.push_back(s);
	  if(mode==4||mode==0)
	    code4.push_back(s);
	}
      input.close();
      string tmp=filenames[i];
      if(outDir!="")
	{
	  tmp=filenames[i];
	  if(tmp.find("/")<tmp.npos)
	    tmp=tmp.substr(tmp.rfind("/")+1,tmp.length()-tmp.rfind("/")-1);
	  tmp=outDir+"/"+tmp;
	  tmp=tmp.substr(0,tmp.length()-2);
	  //	  cout<<"write to:\n\t\""<<tmp.data()<<"3.h\"\n\t\""<<tmp.data()<<"4.h\""<<endl;
	}
      if(do3)
	{
	  ofstream output3;
	  output3.open((tmp+"3.h").data());
	  for(unsigned int j=0;j<code3.size();j++)output3<<code3[j].data()<<endl;
	  output3.close();
	}
      if(do4)
	{
	  ofstream output4;
	  output4.open((tmp+"4.h").data());
	  for(unsigned int j=0;j<code4.size();j++)output4<<code4[j].data()<<endl;
	  output4.close();
	}
    }
}
