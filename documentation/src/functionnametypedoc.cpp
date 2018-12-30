#include "tmyclass.h"
#include <fstream>
extern ofstream logg;
extern string knownTypesDoc(const string &type);
extern TMyClass* getKnownClass(const string &name);
extern void minimizeString(string &s);
#include "string_utilities.h"
functionNameTypeDoc::functionNameTypeDoc(string nameIn,string typeIn,int typeIdIn,string specifierIn,string docIn,string typeDocIn):nameTypeDoc(nameIn,typeIn,typeIdIn,specifierIn,docIn,typeDocIn)
{
    limiter="";
}
functionNameTypeDoc::functionNameTypeDoc(const functionNameTypeDoc &n):nameTypeDoc(n.Name(),n.Type(),n.TypeId(),n.Specifier(),n.Doc(),n.TypeDoc())
{
    limiter=n.Limiter();
    clearDescription();
    for(int i=0;i<n.DescriptionLines();i++)
	addDescriptionLine(n.Description(i));
    for(int i=0;i<n.nParameters();i++)parameter.push_back(n.getParameter(i));
}
functionNameTypeDoc::functionNameTypeDoc(const string& s)
{
    limiter="";
  parse(s);
}
functionNameTypeDoc::functionNameTypeDoc(){}
functionNameTypeDoc::~functionNameTypeDoc(){}
functionNameTypeDoc &functionNameTypeDoc::operator=(const functionNameTypeDoc & fn)
{
    setName(fn.Name());
    setType(fn.Type());
    setTypeId(fn.TypeId());
    setDoc(fn.Doc());
    setTypeDoc(fn.TypeDoc());
    setSpecifier(fn.Specifier());
    clearDescription();
    for(int i=0;i<fn.DescriptionLines();i++)addDescriptionLine(fn.Description(i));
    limiter=fn.Limiter();
    clearParameter();
    for(int i=0;i<fn.nParameters();i++)addParameter(fn.getParameter(i));
    return *this;
}
bool functionNameTypeDoc::operator==(const functionNameTypeDoc & fn)
{
    if(fn.Name()!=Name())return false;
    if(fn.Type()!=Type())return false;
    if(fn.Specifier()!=Specifier())return false;
    if(fn.Limiter()!=limiter)return false;
    if(nParameters()!=fn.nParameters())return false;
    for(int i=0;i<nParameters();i++)
	if(parameter[i].Type()!=fn.getParameter(i).Type()||
	   parameter[i].Specifier()!=fn.getParameter(i).Specifier())
	    return false;
    return true;
}
string functionNameTypeDoc::Limiter()const
{
  return limiter;
}
void functionNameTypeDoc::setLimiter(const string &s)
{
  limiter=s;
}
nameTypeDoc& functionNameTypeDoc::Parameter(int i)
{
  if(i<0||i>=(int)parameter.size())return parameter[0];
  return parameter[i];
}
nameTypeDoc functionNameTypeDoc::getParameter(int i)const
{
  if(i<0||i>=(int)parameter.size())return nameTypeDoc();
  return parameter[i];
}
int functionNameTypeDoc::nParameters()const
{
  return parameter.size();
}
void functionNameTypeDoc::clearParameter()
{
  while(!parameter.empty())parameter.pop_back();
}
void functionNameTypeDoc::addParameter(const nameTypeDoc & input)
{
  parameter.push_back(input);
}
void functionNameTypeDoc::addParameter(string nameIn,string typeIn,int typeIdIn,string specifierIn,string docIn,string typeDocIn)
{
  parameter.push_back(nameTypeDoc(nameIn,typeIn,typeIdIn,specifierIn,docIn,typeDocIn));
}
string functionNameTypeDoc::toString(int ptt)const
{
  string ret=Type();
  if(Type()!=Name())
    ret+=" "+Name()+"(";
  else
    ret+="(";
  if(Specifier()!="")
    ret=Specifier()+" "+ret;
  ret+=ParameterList(ptt)+")"+limiter;
  return ret;
}
string functionNameTypeDoc::ParameterList(int ptt)const
{
  string ret="";
  for(int i=0;i<(int)parameter.size()-1;i++)
      ret+=parameter[i].toString(ptt)+", ";
  if(parameter.size()>0)
    ret+=parameter[parameter.size()-1].toString(ptt);
  return ret;
}
string functionNameTypeDoc::htmlHREF()const
{
  string ret="";
  string t=Type();
  t=remove(t,"*");
  t=remove(t,"&");
  t=remove(t,"[");
  t=remove(t,"]");
  if(Type()==Name())
  {
      ret+=string("<A HREF=\"")+Doc()+"\">"+Name()+"</A>(";
      if(Specifier()!="")ret=Specifier()+" "+ret;
  }
  else
  {
    string td=TypeDoc();
    if(td=="")
      td=knownTypesDoc(t);
    if(td=="")
      {
	TMyClass* tmp=getKnownClass(t);
	if(tmp!=NULL)
	  td=tmp->Doc();
      }
    t=Type();
    t=replace(t,"<","&lt;");
    t=replace(t,">","&gt;");
    if(td!="")
      ret+=string("<A HREF=\"")+td+"\">"+t+"</A> <A HREF=\""+Doc()+"\">"+Name()+"</A>(";
    else 
      ret+=string("<A HREF=\"")+Doc()+"\"><vtype>"+t+"</vtype> "+Name()+"</A>(";
    if(Specifier()!="")ret=Specifier()+" "+ret;
  }
  for(int i=0;i<((int)parameter.size())-1;i++)
      ret+=parameter[i].htmlHREF()+", ";
  if(parameter.size()>0)
      ret+=parameter[parameter.size()-1].htmlHREF()+")";
  else
      ret+=")";
  ret=string("<LI>")+ret;
  return ret+" "+limiter;
}
vector<string> functionNameTypeDoc::htmlNAME(const string &classname)const
{
  vector<string> ret;
  string line,d=Doc();
  d=remove(d,"#");
  string t=Type();
  t=remove(t,"*");
  t=remove(t,"&");
  t=remove(t,"[");
  t=remove(t,"]");
  if(Type()==Name())
  {
    if(classname!="")
      line+=string("<A NAME=\"")+d+"\">"+classname+"::"+Name()+"</A>(";
    else
      line+=string("<A NAME=\"")+d+"\">"+Name()+"</A>(";
  }
  else
  {
    string td=TypeDoc();
    if(td=="")
      td=knownTypesDoc(t);
    if(td=="")
      {
	TMyClass* tmp=getKnownClass(t);
	if(tmp!=NULL)
	  td=tmp->Doc();
      }
    t=Type();
    t=replace(t,"<","&lt;");
    t=replace(t,">","&gt;");
    if(td!=""){
      if(classname!="")
	line+=string("<A HREF=\"")+td+"\">"+t+"</A> <A NAME=\""+d+"\">"+classname+"::"+Name()+"</A>(";
      else
	line+=string("<A HREF=\"")+td+"\">"+t+"</A> <A NAME=\""+d+"\">"+Name()+"</A>(";
    }
    else {
      if(classname!="")
	line+=string("<A NAME=\"")+d+"\"><vtype>"+t+"</vtype> "+classname+"::"+Name()+"</A>(";
      else
	line+=string("<A NAME=\"")+d+"\"><vtype>"+t+"</vtype> "+Name()+"</A>(";
    }
  }
  for(int i=0;i<((int)parameter.size())-1;i++)
    line+=parameter[i].htmlHREF()+", ";
  if(parameter.size()>0)
      line+=parameter[parameter.size()-1].htmlHREF();
  line+=") "+limiter;
  ret.push_back(line);
  ret.push_back("<BR/><deff>");
  for(int i=0;i<DescriptionLines();i++)ret.push_back(Description(i));
  ret.push_back("</deff>");
  return ret;
}
ostream &operator << (ostream & o, const functionNameTypeDoc & t)
{
    if(&o==&cout)
    {
	o<<"Name:"<<t.Name().data()<<endl;
	o<<"Type:"<<t.TypeId()<<" "<<t.Type().data()<<endl;
	o<<"Specifier:"<<t.Specifier().data()<<endl;
	o<<"Documented at:"<<smallerPath(t.Doc()).data()<<endl;
	o<<"Type Documented at:"<<smallerPath(t.TypeDoc()).data()<<endl;
	o<<"Limiter:"<<t.Limiter().data()<<endl;
	for(int i=0;i<t.nParameters();i++)
	    o<<"P"<<t.getParameter(i);
	for(int i=0;i<t.DescriptionLines();i++)
	    o<<"D"<<t.Description(i).data()<<endl;
	o<<"="<<endl;
    }
    else
    {
      //new version
      o<<t.Name().data()<<"\n"<<t.TypeId()<<" "<<t.Type().data()<<endl;
      if(t.Specifier()!="")o<<"S"<<t.Specifier().data()<<endl;
      if(t.Doc()!="")o<<"O"<<smallerPath(t.Doc()).data()<<endl;
      if(t.TypeDoc()!="")o<<"T"<<smallerPath(t.TypeDoc()).data()<<endl;
      if(t.Limiter()!="")o<<"L"<<t.Limiter().data()<<endl;
      for(int i=0;i<t.nParameters();i++)
	o<<"P"<<t.getParameter(i);
      for(int i=0;i<t.DescriptionLines();i++) {
        o<<"D"<<t.Description(i).data()<<endl;}
	o<<"="<<endl;
      //old version
	/*	o<<t.Name().data()<<endl;
	o<<t.TypeId()<<" "<<t.Type().data()<<endl;
	o<<t.Specifier().data()<<endl;
	o<<t.Doc().data()<<endl;
	o<<t.TypeDoc().data()<<endl;
	o<<t.Limiter().data()<<endl;
	for(int i=0;i<t.nParameters();i++)
	    o<<"P"<<t.getParameter(i);
	for(int i=0;i<t.DescriptionLines();i++)
	    o<<"D"<<t.Description(i).data()<<endl;
	    o<<"="<<endl;*/
    }
  return o;
}
istream &operator >> (istream & o, functionNameTypeDoc & t)
{
  char li[1000],c;
  int n;
  o.getline(li,1000);
  t.setName(li);
  o>>n;
  o.get(c);
  o.getline(li,1000);
  t.setTypeId(n);
  t.setType(li);
  t.setSpecifier("");
  t.setDoc("");
  t.setTypeDoc("");
  t.setLimiter("");
      //old version
#ifdef OLDINPUT
  o.getline(li,1000);
  t.setSpecifier(li);
  o.getline(li,1000);
  t.setDoc(li);
  o.getline(li,1000);
  t.setTypeDoc(li);
  o.getline(li,1000);
  t.setLimiter(li);
#endif
  o.get(c);
  t.clearDescription();
  while(c=='D'||c=='S'||c=='O'||c=='T'||c=='L'||c=='P')
    {
      if(c=='D')
	{
	  o.getline(li,1000);
	  t.addDescriptionLine(li);
	}
      else if(c=='P')
	{
	  nameTypeDoc tmp;
	  o>>tmp;
	  t.addParameter(tmp);
	}
      else if(c=='S')
	{
	  o.getline(li,1000);
	  t.setSpecifier(li);
	}
      else if(c=='O')
	{
	  o.getline(li,1000);
	  t.setDoc(expandPath(li));
	}
      else if(c=='T')
	{
	  o.getline(li,1000);
	  t.setTypeDoc(expandPath(li));
	}
      else if(c=='L')
	{
	  o.getline(li,1000);
	  t.setLimiter(li);
	}
      o.get(c);
    }
  o.getline(li,1000);
  return o;
}

functionNameTypeDoc functionNameTypeDoc::parseIt(const string & expression)
{
  return functionNameTypeDoc(expression);
}
void functionNameTypeDoc::parse(const string &s1)
{
  logg<<s1.data()<<endl;
  string s=s1;
  clearParameter();
  string inlines;
  if(hasA(s,"//"))
    {
      string comment=s.substr(s.find("//"),s.length()-s.find("//"));
      addDescriptionLine(comment);
      s=s.substr(0,s.find("//"));
    }
  string left,right,inBrackets;
  if(hasA(s,"operator"))
    {
      int opPos=s.find("operator");
      while(isSpace(s[opPos+8]))s=s.substr(0,opPos+8)+s.substr(opPos+9,s.length()-opPos-9);
    }
  if((contains(s,"(")==2)&&(contains(s,")")==2)&&(hasA(s,"operator")))
    {
      right=s.substr(s.rfind(")")+1,s.length()-s.rfind(")")-1);
      s=s.substr(0,s.rfind(")"));
      left=s.substr(0,s.find("(")+1);
      s=s.substr(s.find("(")+1,s.length()-s.find("(")-1);
      left=left+s.substr(0,s.find("("));
      inBrackets=s.substr(s.find("(")+1,s.length()-s.find("(")-1);
    }
  else
    {
      inBrackets=s.substr(s.find("(")+1,s.length()-s.find("(")-1);
      inBrackets=inBrackets.substr(0,inBrackets.rfind(")"));
      left=s.substr(0,s.find("("));
      right=s.substr(s.rfind(")")+1,s.length()-s.rfind(")")-1);
    }
  if(hasA(right,"{"))
    {
      inlines=right.substr(right.find("{")+1,right.rfind("}")-right.find("{")-1);
      right=remove(right,inlines.data());
      right=remove(right,"{}");
    }
  minimizeString(left);
  minimizeString(right);
  minimizeString(inBrackets);
  right=remove(right," ");
  if(right==";"||right==" "||right=="")limiter="";
  else limiter=remove(right,";");
  if(inlines!="")
    {
      limiter+=" inline";
      addDescriptionLine(string("{")+inlines+"}");
    }
  if(hasA(left," "))// type" "functionName 
    {
      nameTypeDoc tmp(left);
      setName(tmp.Name());
      setType(tmp.Type());
      setTypeId(tmp.TypeId());
      setTypeDoc(tmp.TypeDoc());
      setSpecifier(tmp.Specifier());
    }
  else //constructor or destructor
    {
      setName(left);
      setType(left);
      setTypeId(nameTypeDoc::typeIDS(left));
      setTypeDoc("");
      setSpecifier("");
    }
  if(inBrackets.length()==0)return;
  vector<string> parts;
  string word;
  int brck=0,i=0;
  word="";
  while(i<(int)inBrackets.length())
    {
      if(inBrackets[i]=='('||inBrackets[i]==')'||inBrackets[i]==',')
	{
	  if(inBrackets[i]=='(')brck++;
	  if(inBrackets[i]==')')brck--;
	  if(inBrackets[i]==','&&brck==0)
	    {
	      parts.push_back(word);
	      word="";
	    }
	  else
	    {
	      word+=inBrackets[i];
	    }
	}
      else 
	word+=inBrackets[i];
      i++;
    }
  i++;
  parts.push_back(word);
  vector<string> tps;
  vector<int>ntps;
  int gtp;
  for(unsigned int j=0;j<parts.size();j++)
    {
      nameTypeDoc tmp1(parts[j]);
      if(tmp1.Name()!="")
	{
	  if(tmp1.Type()=="")
	    {
	      string s=lower(tmp1.Name());
	      s=remove(s,"*");
	      s=remove(s,"&");
	      s=remove(s,"[");
	      s=remove(s,"]");
	      gtp=-1;
	      for(unsigned int i=0;i<tps.size();i++)
		if(tps[i]==s)gtp=i;
	      if(gtp==-1){gtp=tps.size();tps.push_back(s);ntps.push_back(-1);}
	      ntps[gtp]++;
	      tmp1.setType(tmp1.Name());
	      tmp1.setName(string("tmp")+s+string_number(ntps[gtp]));
	    }
	  else if(tmp1.Type()=="const")
	    {
	      string s=lower(tmp1.Name());
	      s=remove(s,"*");
	      s=remove(s,"&");
	      s=remove(s,"[");
	      s=remove(s,"]");
	      gtp=-1;
	      for(unsigned int i=0;i<tps.size();i++)
		if(tps[i]==s)gtp=i;
	      if(gtp==-1){gtp=tps.size();tps.push_back(s);ntps.push_back(-1);}
	      tmp1.setSpecifier(tmp1.Type());
	      tmp1.setType(tmp1.Name());
	      tmp1.setName(string("tmp")+s+string_number(ntps[gtp]));
	    }
	  parameter.push_back(tmp1);
	}
    }
}
