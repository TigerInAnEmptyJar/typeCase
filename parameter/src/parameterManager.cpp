#include "parameterManager.h"
//#include <qfile.h>
#include <fstream>
#include <typeinfo>
#include "versions0.h"
#include "versions1.h"
#include "versions2.h"
#include "versions3.h"
#include "versions4.h"
#include "versions5.h"
#include "versions6.h"
extern bool existing(string filename);
// bool existing(const string &filename)
// {
//   return QFile::exists(filename.data());
// }
bool check_version_type(const char *filename, int &version, int &type);
bool parameterManager::readAlgorithmParameter_ascii(string filename, vector<algorithm_parameter> &ret, int version)
{
  if(!existing(filename))return false;
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=0)return false;
    }
  else if(!(version==versionNumbers[0][0]))return false;
  if(version==-1||version==versionNumbers[0][1])
    {
      while(!ret.empty())ret.pop_back();
      ifstream input1;
      int num;
      input1.open(filename.data());
      char li[1000];
      input1.getline(li,1000);
      string line(li);
      if(!((line.find("version")<line.npos
            ||line.find("Version")<line.npos)
           &&line.find("parameter")<line.npos
           &&line.find("algorithm")<line.npos))return false;
      input1>>num;
      for(int j=0;j<num;j++)
	{
	  algorithm_parameter r;
	  input1>>r;
	  ret.push_back(r);
	}
      input1.close();
      return true;
    }
  else if(version==versionNumbers[0][0])
    {
      while(!ret.empty())
	ret.pop_back();
      ifstream input;
      input.open(filename.data());
      int num;
      input>>num;
      for(int i=0;i<num;i++)
	{
	  algorithm_parameter ap;
	  read_algorithm_parameter_0(input, ap);
	  ret.push_back(ap);
	}
      input.close();
      return true;
    }
  return false;
}
bool parameterManager::readUsedAlgorithmParameter_ascii(string filename, vector<algorithm_parameter> &ret, int version)
{
  if(!existing(filename))return false;
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=0)return false;
    }
  else if(!(version==versionNumbers[0][0]||version==-1))return false;
  if(version==-1||version==versionNumbers[0][1])
    {
      while(!ret.empty())ret.pop_back();
      ifstream input1;
      int num;
      input1.open(filename.data());
      char li[1000];
      input1.getline(li,1000);
      string line(li);
      if(!((line.find("version")<line.npos||line.find("Version")<line.npos)&&line.find("parameter")<line.npos&&line.find("algorithm")<line.npos))return false;
      input1>>num;
      for(int j=0;j<num;j++)
	{
	  algorithm_parameter r;
	  input1>>r;
	  if(r.IsUsed())
	    ret.push_back(r);
	}
      input1.close();
      return true;
    }
  else if(version==versionNumbers[0][0])
    {
      while(!ret.empty())
	ret.pop_back();
      ifstream input;
      input.open(filename.data());
      int num;
      input>>num;
      for(int i=0;i<num;i++)
	{
	  algorithm_parameter ap;
	  read_algorithm_parameter_0(input, ap);
	  if(ap.IsUsed())
	    ret.push_back(ap);
	}
      input.close();
      return true;
    }
  return false;
}
bool parameterManager::readAlgorithmParameter_ascii(string filename, vector<algorithm_parameter*> &ret, int version)
{
  if(!existing(filename))return false;
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=0)return false;
    }
  else if(!(version==versionNumbers[0][0]||version==-1))return false;
  if(version==-1||version==versionNumbers[0][1])
    {
      while(!ret.empty())ret.pop_back();
      ifstream input1;
      int num;
      input1.open(filename.data());
      char li[1000];
      input1.getline(li,1000);
      string line(li);
      if(!((line.find("version")<line.npos||line.find("Version")<line.npos)&&line.find("parameter")<line.npos&&line.find("algorithm")<line.npos))return false;
      input1>>num;
      for(int j=0;j<num;j++)
	{
	  algorithm_parameter* ap=new algorithm_parameter();
	  input1>>*ap;
	  ret.push_back(ap);
	}
      input1.close();
      return true;
    }
  else if(version==versionNumbers[0][0])
    {
      while(!ret.empty())
	ret.pop_back();
      ifstream input;
      input.open(filename.data());
      int num;
      input>>num;
      for(int i=0;i<num;i++)
	{
	  algorithm_parameter* ap=new algorithm_parameter();
	  read_algorithm_parameter_0(input, *ap);
	  ret.push_back(ap);
	}
      input.close();
      return true;
    }
  return false;
}
bool parameterManager::readUsedAlgorithmParameter_ascii(string filename, vector<algorithm_parameter*> &ret, int version)
{
  if(!existing(filename))return false;
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=0)return false;
    }
  else if(!(version==versionNumbers[0][0]||version==-1))return false;
  if(version==-1||version==versionNumbers[0][1])
    {
      while(!ret.empty())ret.pop_back();
      ifstream input1;
      int num;
      input1.open(filename.data());
      char li[1000];
      input1.getline(li,1000);
      string line(li);
      if(!((line.find("version")<line.npos||line.find("Version")<line.npos)&&line.find("parameter")<line.npos&&line.find("algorithm")<line.npos))return false;
      input1>>num;
      for(int j=0;j<num;j++)
	{
	  algorithm_parameter* ap=new algorithm_parameter();
	  input1>>*ap;
	  if(ap->IsUsed())
	    ret.push_back(ap);
	}
      input1.close();
      return true;
    }
  else if(version==versionNumbers[0][0])
    {
      while(!ret.empty())
	ret.pop_back();
      ifstream input;
      input.open(filename.data());
      int num;
      input>>num;
      for(int i=0;i<num;i++)
	{
	  algorithm_parameter* ap=new algorithm_parameter();
	  read_algorithm_parameter_0(input, *ap);
	  if(ap->IsUsed())ret.push_back(ap);
	  else delete ap;
	}
      input.close();
      return true;
    }
  return false;
}
bool parameterManager::writeAlgorithmParameter_ascii(string filename, const vector<algorithm_parameter> &ret, int version)
{
  if(version==-1||version==versionNumbers[0][1])
    {
      ofstream output;
      output.open(filename.data());
      output<<"########## algorithm parameter ####### file version :1 #######"<<endl;
      output<<ret.size()<<" ";
      for(unsigned int i=0;i<ret.size();i++)
	output<<ret[i]<<endl;
      output.close();
      return true;
    }
  else if(version==versionNumbers[0][0])
    {
      ofstream output;
      output.open(filename.data());
      output<<ret.size()<<" ";
      for(unsigned int i=0;i<ret.size();i++)
	write_algorithm_parameter_0(output,ret[i]);
      output.close();
      return true;
    }
  return false;
}
bool parameterManager::writeAlgorithmParameter_ascii(string filename, const vector<algorithm_parameter*> &ret, int version)
{
  if(version==-1||version==versionNumbers[0][1])
    {
      ofstream output;
      output.open(filename.data());
      output<<"########## algorithm parameter ####### file version :1 #######"<<endl;
      output<<ret.size()<<" ";
      for(unsigned int i=0;i<ret.size();i++)
	output<<*ret[i]<<endl;
      output.close();
      return true;
    }
  else if(version==versionNumbers[0][0])
    {
      ofstream output;
      output.open(filename.data());
      output<<ret.size()<<" ";
      for(unsigned int i=0;i<ret.size();i++)
	write_algorithm_parameter_0(output,*ret[i]);
      output.close();
      return true;
    }
  return false;
}

bool parameterManager::readDetectorParameter_ascii(string filename, vector<detector_parameter> &ret, int version)
{
  reaction_parameter colision;
  return parameterManager::readDetectorParameter_ascii(filename, ret, colision, version);
}

bool parameterManager::readDetectorParameter_ascii(string filename, vector<detector_parameter> &ret, reaction_parameter &col, int version)
{
  if(!existing(filename)){return false;}
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=5)return false;
    }
  else if(!(version==versionNumbers[5][0]||version==-1||version==versionNumbers[5][1]||version==versionNumbers[5][2]))return false;
  if(version==-1||version==versionNumbers[5][2])
    {
      ifstream inputS;
      inputS.open(filename.data());
      int numberOfDetectors;
      reaction_parameter colision;
      char li[300];
      inputS.getline(li,300);
      if(string(li).find("detector parameter")>=string(li).npos)
	{
	  inputS.close();
	  return false;
	}
      inputS >> col;
      inputS >> numberOfDetectors;
      while(!ret.empty())ret.pop_back();
      for(int i=0;i<numberOfDetectors;i++)
	{
	  detector_parameter d;
	  inputS >> d;
	  ret.push_back(d);
	}
      inputS.close();
      return true;
    }
  else if(version==versionNumbers[5][1])
    {
      ifstream inputS;
      inputS.open(filename.data());
      int numberOfDetectors;
      reaction_parameter colision;
      char li[300];
      inputS.getline(li,300);
      if(string(li).find("detector parameter")>=string(li).npos)
	{
	  inputS.close();
	  return false;
	}
      inputS >> col;
      inputS >> numberOfDetectors;
      while(!ret.empty())ret.pop_back();
      for(int i=0;i<numberOfDetectors;i++)
	{
	  detector_parameter d;
	  read_detector_parameter_1(inputS,d);
	  ret.push_back(d);
	}
      inputS.close();
      return true;
    }
  else if(version==versionNumbers[5][0])
    {
      ifstream inputS;
      inputS.open(filename.data());
      int numberOfDetectors;
      inputS >> col;
      inputS >> numberOfDetectors;
      while(!ret.empty())ret.pop_back();
      for(int i=0;i<numberOfDetectors;i++)
	{
	  detector_parameter d;
	  read_detector_parameter_0(inputS,d);
	  ret.push_back(d);
	}
      inputS.close();
      return true;
    }
  return false;
}

bool parameterManager::writeDetectorParameter_ascii(string filename, const vector<detector_parameter> &ret, const reaction_parameter &col, int version)
{
  if(version==-1||version==versionNumbers[5][2])
    {
      ofstream out;
      out.open(filename.data());
      out<<"########## detector parameter ####### file version :"<<versionNumbers[5][2]<<" #######"<<endl;
      out << col<<" ";
      out << ret.size()<<endl;
      for(unsigned int i=0;i<ret.size();i++)
	out<<ret[i];
      out.close();
      return true;
    }
  else if(version==versionNumbers[5][1])
    {
      ofstream out;
      out.open(filename.data());
      out<<"########## detector parameter ####### file version :"<<versionNumbers[5][1]<<" #######"<<endl;
      out << col<<" ";
      out << ret.size()<<endl;
      for(unsigned int i=0;i<ret.size();i++)
	write_detector_parameter_1(out,ret[i]);
      out.close();
      return true;
    }
  else if(version==versionNumbers[5][0])
    {
      ofstream out;
      out.open(filename.data());
      out << col;
      out << ret.size()<<endl;
      for(unsigned int i=0;i<ret.size();i++)
	write_detector_parameter_0(out,ret[i]);
      out.close();
      return true;
    }
  return false;
}

bool parameterManager::readMaterialParameter_ascii(string filename, vector<material_parameter> &ret, int version)
{
  if(!existing(filename))return false;
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=6)return false;
    }
  else if(!(version==versionNumbers[6][0]||version==-1))return false;
  if(version==-1||version==versionNumbers[6][0])
    {
      ifstream input;
      input.open(filename.data());
      int numberOfMaterials;
      input >> numberOfMaterials;
      for(int i=0;i<numberOfMaterials;i++)
	{
	  material_parameter m;
	  input >> m;
	  ret.push_back(m);
	}
      input.close();
      return true;
    }
  return false;
}

bool parameterManager::writeMaterialParameter_ascii(string filename, const vector<material_parameter> &ret, int version)
{
  if(version==-1||version==versionNumbers[6][0])
    {
      ofstream output;
      output.open(filename.data());
      output<<ret.size()<<" ";
      for(unsigned int i=0;i<ret.size();i++)
	output<<ret[i];
      output.close();
      return true;
    }
  return false;
}

bool parameterManager::readSetup_ascii(string filename, vector<detector_parameter> &ret, vector<material_parameter> &mret, reaction_parameter &col, int version, string filename1)
{
  if(!existing(filename)){return false;}
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=5)return false;
    }
  else if(!(version==versionNumbers[5][0]||version==-1))return false;
  if(existing(filename1))
    {
      if(check_version_type(filename.data(),v,t))
	{
	  if(t!=6)return false;
	}
      else if(!(version==versionNumbers[6][0]||version==-1))return false;
    }
  if(version==-1||version==versionNumbers[5][1]||version==versionNumbers[5][0])
    {
      bool r1=readDetectorParameter_ascii(filename, ret, col, version);
      bool r2=readMaterialParameter_ascii(filename1, mret, version);
      for(unsigned int i=0;i<ret.size();i++)
	if(ret[i].getMaterial()<(int)mret.size()&&ret[i].getMaterial()>=0)
	  ret[i].setMaterial(&mret[ret[i].getMaterial()]);
      return (r1&&r2);
    }
  return false;
}

bool parameterManager::writeSetup_ascii(string filename, string filename1, const vector<detector_parameter> &ret, const vector<material_parameter> &mret, const reaction_parameter &col, int version)
{
  if(version==-1||version==versionNumbers[5][0])
    {
      return (writeDetectorParameter_ascii(filename, ret, col, version)&&
	      writeMaterialParameter_ascii(filename1, mret, version));
    }
  return false;
}

bool parameterManager::readShapeParameter_ascii(string filename, vector<shape_parameter> &ret, int version)
{
  if(!existing(filename))return false;
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=1)return false;
    }
  else if(!(version==versionNumbers[1][0]||version==-1))return false;
  if(version==versionNumbers[1][0])
    {
      int num;
      ifstream input;
      input.open(filename.data());
      input>>num;
      while(!ret.empty())ret.pop_back();
      for(int i=0;i<num;i++)
	{
	  shape_parameter sh;
	  read_shape_parameter_0(input,sh);
	  ret.push_back(sh);
	}
      input.close();
      return true;
    }
  else if(version==-1||version==versionNumbers[1][1])
    {
      ifstream inputS;
      inputS.open(filename.data());
      int numberOfShapes;
      reaction_parameter colision;
      char li[300];
      inputS.getline(li,300);
      if(string(li).find("shape parameter")>=string(li).npos)
	{
	  inputS.close();
	  return false;
	}
      inputS >> numberOfShapes;
      while(!ret.empty())ret.pop_back();
      for(int i=0;i<numberOfShapes;i++)
	{
	  shape_parameter d;
	  inputS >> d;
	  ret.push_back(d);
	}
      inputS.close();
      return true;
    }
  return false;
}

bool parameterManager::writeShapeParameter_ascii(string filename, const vector<shape_parameter> &ret, int version)
{
  if(version==versionNumbers[1][0])
    {
      ofstream out;
      out.open(filename.data());
      out << ret.size()<<endl;
      for(unsigned int i=0;i<ret.size();i++)
	write_shape_parameter_0(out,ret[i]);
      out.close();
      return true;
    }
  else if(version==-1||version==versionNumbers[1][1])
    {
      ofstream out;
      out.open(filename.data());
      out<<"########## shape parameter ####### file version :1 #######"<<endl;
      out << ret.size()<<" "<<endl;
      for(unsigned int i=0;i<ret.size();i++)
	out<<ret[i];
      out.close();
      return true;
    }
  return false;
}

bool parameterManager::readPaintParameter_ascii(string filename, vector<vector<paint_parameter> > &ret, int version)
{
  if(!existing(filename))return false;
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=4)return false;
    }
  else if(!(version==versionNumbers[4][0]||version==-1))return false;
  if(version==-1||version==versionNumbers[4][0])
    {
      ifstream input;
      input.open(filename.data());
      char c;
      int nParameter, nTypes;
      input.get(c);while(c!='\n'&&!input.eof())input.get(c);
      input>>nTypes;
      for(int i=0;i<nTypes;i++)
	{
	  input>>nParameter;
	  vector<paint_parameter> tmp;
	  for(int j=0;j<nParameter;j++)
	    {
	      paint_parameter pm;
	      input>>pm;
	      tmp.push_back(pm);
	    }
	  ret.push_back(tmp);
	}
      input.close();
    }
  return true;
}

bool parameterManager::writePaintParameter_ascii(string filename, const vector<vector<paint_parameter> > &ret, int version)
{
  if(version==-1||version==versionNumbers[4][0])
    {
      ofstream output;
      output.open(filename.data());
      output<<"########## paint parameter ####### file version :"<<versionNumbers[4][0]<<" #######"<<endl;
      output<<ret.size()<<" ";
      for(unsigned int i=0;i<ret.size();i++)
	{
	  output<<ret[i].size()<<" ";
	  for(unsigned int j=0;j<ret[i].size();j++)
	    output<<ret[i][j]<<endl;
	}
      output.close();
    }
  return true;
}

bool parameterManager::readBeamTimeParameter_ascii(string filename, vector<beamTime_parameter> &ret, int version)
{
  if(!existing(filename))return false;
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=2)return false;
    }
  else if(!(version==versionNumbers[2][0]||version==-1||version==versionNumbers[2][1]||version==versionNumbers[2][2]))return false;
  if(version==-1||version==versionNumbers[2][0]||version==versionNumbers[2][1]||version==versionNumbers[2][2])
    {
      while(!ret.empty())ret.pop_back();
      ifstream input;
      input.open(filename.data());
      int numberOfBeamTimes;
      input >> numberOfBeamTimes;	
      for(int i=0;i<numberOfBeamTimes;i++)
	{
	  beamTime_parameter d;
	  input >> d;
	  ret.push_back(d);
	}
      input.close();
      return true;
    }
  return false;
}

bool parameterManager::readRunParameter_ascii(string filename, vector<run_parameter> &ret, int version)
{
  if(!existing(filename))return false;
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=3)return false;
    }
  else if(!(version==versionNumbers[3][0]||version==-1||version==versionNumbers[3][1]))
    return false;
  if(version==versionNumbers[3][0])
    {
      //   cout<<3<<endl;
      while(!ret.empty())ret.pop_back();
      ifstream input1;
      int numRuns;
      input1.open(filename.data());
      input1>>numRuns;
      for(int j=0;j<numRuns;j++)
	{
	  run_parameter r;
	  read_run_parameter_0(input1,r);
	  //	  r.setParentNumber(i);
	  //	  r.setParent(&bret[i]);
	  ret.push_back(r);
	}
      input1.close();
      return true;
    }
  else if(version==versionNumbers[3][1])
    {
      //   cout<<3<<endl;
      while(!ret.empty())ret.pop_back();
      ifstream input1;
      int numRuns;
      input1.open(filename.data());
      input1>>numRuns;
      for(int j=0;j<numRuns;j++)
	{
	  run_parameter r;
	  read_run_parameter_1(input1,r);
	  //	  r.setParentNumber(i);
	  //	  r.setParent(&bret[i]);
	  ret.push_back(r);
	}
      input1.close();
      return true;
    }
  else if(version==-1||version==versionNumbers[3][2])
    {
      while(!ret.empty())ret.pop_back();
      ifstream input1;
      int numRuns;
      input1.open(filename.data());
      char li[1000];
      input1.getline(li,1000);
      string line(li);
      if(!((line.find("version")<line.npos
            ||line.find("Version")<line.npos)
           &&line.find("parameter")<line.npos
           &&line.find("run")<line.npos))return false;
      input1>>numRuns;
      for(int j=0;j<numRuns;j++)
	{
	  run_parameter r;
	  input1>>r;
	  //	  r.setParentNumber(i);
	  //	  r.setParent(&bret[i]);
	  ret.push_back(r);
	}
      input1.close();
      return true;
    }
  return false;
}

bool parameterManager::addRunParameter_ascii(string filename, vector<run_parameter> &ret, int version)
{
  int v,t;
  if(check_version_type(filename.data(),v,t))
    {
      if(t!=3)return false;
    }
  else if(!(version==versionNumbers[3][0]||version==-1||version==versionNumbers[3][1]))return false;
  if(version==versionNumbers[3][0])
    {
      ifstream input1;
      int numRuns;
      input1.open(filename.data());
      input1>>numRuns;
      for(int j=0;j<numRuns;j++)
	{
	  run_parameter r;
	  read_run_parameter_0(input1,r);
	  //	  r.setParentNumber(i);
	  //	  r.setParent(&bret[i]);
	  ret.push_back(r);
	}
      input1.close();
      return true;
    }
  else if(version==versionNumbers[3][1])
    {
      ifstream input1;
      int numRuns;
      input1.open(filename.data());
      input1>>numRuns;
      for(int j=0;j<numRuns;j++)
	{
	  run_parameter r;
	  read_run_parameter_1(input1,r);
	  //	  r.setParentNumber(i);
	  //	  r.setParent(&bret[i]);
	  ret.push_back(r);
	}
      input1.close();
      return true;
    }
  else if(version==-1||version==versionNumbers[3][2])
    {
      ifstream input1;
      int numRuns;
      input1.open(filename.data());
      char li[1000];
      input1.getline(li,1000);
      string line(li);
      if(!((line.find("version")<line.npos||line.find("Version")<line.npos)&&line.find("parameter")<line.npos&&line.find("run")<line.npos))return false;
      input1>>numRuns;
      for(int j=0;j<numRuns;j++)
	{
	  run_parameter r;
	  input1>>r;
	  //	  r.setParentNumber(i);
	  //	  r.setParent(&bret[i]);
	  ret.push_back(r);
	}
      input1.close();
      return true;
    }
  return false;
}

bool parameterManager::readDataBasis_ascii(string filename, vector<beamTime_parameter> &bret, vector<run_parameter> &rret, int version)
{
  if(!existing(filename))return false;
  if(version==-1||version==versionNumbers[2][0]||version==versionNumbers[2][1]||version==versionNumbers[2][2])
    {
      bool bea=readBeamTimeParameter_ascii(filename, bret, version);
      if(!bea)return false;
      int st=0;
      while(!rret.empty())rret.pop_back();
      for(unsigned int i=0;i<bret.size();i++)
	{
	  if(!existing(bret[i].getFileName()))
	    cout<<bret[i].getFileName().data()<<" not found!"<<endl;
	  bea=(bea&&addRunParameter_ascii(bret[i].getFileName(), rret, version));
	  for(unsigned int j=st;j<rret.size();j++)
	    {
		rret[j].setParentNumber(i);
		rret[j].setParent(&bret[i]);
	    }
	  st=rret.size();
	}
      return bea;
    }
  return false;
}

bool parameterManager::readDataBasis_ascii(string filename, vector<beamTime_parameter> &bret, vector<vector<run_parameter> > &rret, int version)
{
  if(!existing(filename))return false;
  if(version==-1||version==versionNumbers[2][0]||version==versionNumbers[2][1]||version==versionNumbers[2][2])
    {
      bool bea=readBeamTimeParameter_ascii(filename, bret, version);
      if(!bea)return false;
      for(unsigned int i=0;i<bret.size();i++)
	{
	  vector<run_parameter> tmp;
	  bea=(bea&&readRunParameter_ascii(bret[i].getFileName(), tmp, version));
	  rret.push_back(tmp);
	  for(unsigned int j=0;j<tmp.size();j++)
	    {
		rret[i][j].setParentNumber(i);
		rret[i][j].setParent(&bret[i]);
	    }
	}
      return bea;
    }
  return false;
}

bool parameterManager::writeBeamTimeParameter_ascii(string filename, const vector<beamTime_parameter> &ret, int version)
{
  if(version==-1||version==versionNumbers[2][0]||version==versionNumbers[2][1]||version==versionNumbers[2][2])
    {
      ofstream input;
      input.open(filename.data());
      input <<ret.size()<<" ";
      for(unsigned int i=0;i<ret.size();i++)
	{
	  input << ret[i];
	}
      input.close();
      return true;
    }
  return false;
}

bool parameterManager::writeRunParameter_ascii(string filename, const vector<run_parameter> &ret, int beamTime, int version)
{
  if(version==versionNumbers[3][0])
    {
      ofstream output;
      int nums=0;
      for(unsigned int i=0;i<ret.size();i++)
	if(ret[i].getParentNumber()==beamTime)
	  nums++;
      output.open(filename.data());
      output<<nums<<" ";
      for(int j=0;j<(int)ret.size();j++)
	if(ret[j].getParentNumber()==beamTime)
	  write_run_parameter_0(output,ret[j]);
      output.close();
      return true;
    }
  else if(version==versionNumbers[3][1])
    {
      ofstream output;
      int nums=0;
      for(unsigned int i=0;i<ret.size();i++)
	if(ret[i].getParentNumber()==beamTime)
	  nums++;
      output.open(filename.data());
      output<<nums<<" ";
      for(int j=0;j<(int)ret.size();j++)
	if(ret[j].getParentNumber()==beamTime)
	  write_run_parameter_1(output,ret[j]);
      output.close();
      return true;
    }
  else if(version==-1||version==versionNumbers[3][2])
    {
      ofstream output;
      int nums=0;
      for(unsigned int i=0;i<ret.size();i++)
	if(ret[i].getParentNumber()==beamTime)
	  nums++;
      output.open(filename.data());
      output<<"########## run parameter ####### file version :1 #######"<<endl;
      output<<nums<<" ";
      for(int j=0;j<(int)ret.size();j++)
	if(ret[j].getParentNumber()==beamTime)
	  output<<ret[j];
      output.close();
      return true;
    }
  return false;
}

bool parameterManager::writeRunParameter_ascii(string filename, const vector<run_parameter> &ret, beamTime_parameter *beamTime, int version)
{
  if(version==versionNumbers[3][0])
    {
      ofstream output;
      int nums=0;
      for(unsigned int i=0;i<ret.size();i++)
	if(ret[i].getParent()==beamTime)
	  nums++;
      output.open(filename.data());
      output<<nums<<" ";
      for(int j=0;j<(int)ret.size();j++)
	if(ret[j].getParent()==beamTime)
	  write_run_parameter_0(output,ret[j]);
      output.close();
      return true;
    }
  else if(version==versionNumbers[3][1])
    {
      ofstream output;
      int nums=0;
      for(unsigned int i=0;i<ret.size();i++)
	if(ret[i].getParent()==beamTime)
	  nums++;
      output.open(filename.data());
      output<<nums<<" ";
      for(int j=0;j<(int)ret.size();j++)
	if(ret[j].getParent()==beamTime)
	  write_run_parameter_1(output,ret[j]);
      output.close();
      return true;
    }
  if(version==-1||version==versionNumbers[3][2])
    {
      ofstream output;
      int nums=0;
      for(unsigned int i=0;i<ret.size();i++)
	if(ret[i].getParent()==beamTime)
	  nums++;
      output.open(filename.data());
      output<<"########## run parameter ####### file version :1 #######"<<endl;
      output<<nums<<" ";
      for(int j=0;j<(int)ret.size();j++)
	if(ret[j].getParent()==beamTime)
	  output<<ret[j];
      output.close();
      return true;
    }
  return false;
}

bool parameterManager::writeDataBasis_ascii(string filename, const vector<beamTime_parameter> &bret, const vector<run_parameter> &rret, int version)
{
  if(version==-1||version==versionNumbers[2][0]||version==versionNumbers[2][1]||version==versionNumbers[2][2])
    {
      bool bea=writeBeamTimeParameter_ascii(filename, bret, version);
      if(!bea)return false;
      for(unsigned int i=0;i<bret.size();i++)
	bea=(bea&&writeRunParameter_ascii(bret[i].getFileName(), rret, (beamTime_parameter*)(&(bret[i])), version));
      return bea;
    }
  return false;
}

bool parameterManager::writeDataBasis_ascii(string filename, const vector<beamTime_parameter> &bret, const vector<vector<run_parameter> > &rret, int version)
{
  if(version==-1||version==versionNumbers[2][0]||version==versionNumbers[2][1]||version==versionNumbers[2][2])
    {
      bool bea=writeBeamTimeParameter_ascii(filename, bret, version);
      if(!bea)return false;
      for(unsigned int i=0;i<bret.size();i++)
	{
	  if(rret.size()<=i)
	    {
	      bea=false;
	      continue;
	    }
	  bea=(bea&&writeRunParameter_ascii(bret[i].getFileName(), rret[i], (beamTime_parameter*)(&(bret[i])), version));
	}
      return bea;
    }
  return false;
}
template<class X> int parameterManager::numberOfVersions()
{
  if(!isInit)init();
  if(typeid(X)==typeid(algorithm_parameter))
    return nVersions[0];
  if(typeid(X)==typeid(shape_parameter))
    return nVersions[1];
  if(typeid(X)==typeid(beamTime_parameter))
    return nVersions[2];
  if(typeid(X)==typeid(run_parameter))
    return nVersions[3];
  if(typeid(X)==typeid(paint_parameter))
    return nVersions[4];
  if(typeid(X)==typeid(detector_parameter))
    return nVersions[5];
  if(typeid(X)==typeid(material_parameter))
    return nVersions[6];
  return 0;
}
template<class X> int parameterManager::version(int num)
{
  if(!isInit)init();
  int max=numberOfVersions<X>();
  if(num>=max)return -1;
  if(typeid(X)==typeid(algorithm_parameter))
    return versionNumbers[0][num];
  if(typeid(X)==typeid(shape_parameter))
    return versionNumbers[1][num];
  if(typeid(X)==typeid(beamTime_parameter))
    return versionNumbers[2][num];
  if(typeid(X)==typeid(run_parameter))
    return versionNumbers[3][num];
  if(typeid(X)==typeid(paint_parameter))
    return versionNumbers[4][num];
  if(typeid(X)==typeid(detector_parameter))
    return versionNumbers[5][num];
  if(typeid(X)==typeid(material_parameter))
    return versionNumbers[6][num];
  return -1;
}
//template<class X> QDate parameterManager::versionDate(int num)
template<class X> tm parameterManager::versionDate(int num)
{
  if(!isInit)init();
  int max=numberOfVersions<X>();
  //  if(num>=max)return QDate();
  tm dummy;
  if(num>=max)return dummy;
  if(typeid(X)==typeid(algorithm_parameter))
    return versionDates[0][num];
  if(typeid(X)==typeid(shape_parameter))
    return versionDates[1][num];
  if(typeid(X)==typeid(beamTime_parameter))
    return versionDates[2][num];
  if(typeid(X)==typeid(run_parameter))
    return versionDates[3][num];
  if(typeid(X)==typeid(paint_parameter))
    return versionDates[4][num];
  if(typeid(X)==typeid(detector_parameter))
    return versionDates[5][num];
  if(typeid(X)==typeid(material_parameter))
    return versionDates[6][num];
  return dummy;
}
bool parameterManager::isInit=false;
int *parameterManager::nVersions=NULL;
int **parameterManager::versionNumbers=NULL;
//QDate **parameterManager::versionDates=NULL;
tm **parameterManager::versionDates=NULL;
bool parameterManager::IsInit()
{
  return isInit;
}
tm makeDate(int year,int month,int day,int hour,int minute,int second)
{
  tm ret;
  ret.tm_sec=second;
  ret.tm_min=minute;
  ret.tm_hour=hour;
  ret.tm_mday=day;
  ret.tm_mon=month;
  ret.tm_year=year-1900;
  return ret;
}
void parameterManager::init()
{
  if(isInit)return;
  isInit=true;
  versionNumbers=new int *[7];
  //  versionDates=new QDate*[7];
  versionDates=new tm*[7];
  nVersions=new int[7];
  nVersions[0]=2;//algorithm_parameter
  nVersions[1]=2;//shape_parameter
  nVersions[2]=3;//beamTime_parameter
  nVersions[3]=3;//run_parameter
  nVersions[4]=1;//paint_parameter
  nVersions[5]=3;//detector_parameter
  nVersions[6]=1;//material_parameter
  for(int i=0;i<7;i++)
    {
      versionNumbers[i]=new int[nVersions[i]];
      //      versionDates[i]=new QDate[nVersions[i]];
      versionDates[i]=new tm[nVersions[i]];
    }
  //algorithm_parameter
  versionNumbers[0][0]=0;
  versionNumbers[0][1]=1;
//   versionDates[0][0]=QDate(2005,7,1);
//   versionDates[0][1]=QDate(2007,2,6);
  versionDates[0][0]=makeDate(2005,7,1,0,0,0);
  versionDates[0][1]=makeDate(2007,2,6,0,0,0);
  //shape_parameter
  versionNumbers[1][0]=0;
  versionNumbers[1][1]=1;
//   versionDates[1][0]=QDate(2005,7,1);
//   versionDates[1][1]=QDate(2009,11,30);
  versionDates[1][0]=makeDate(2005,7,1,0,0,0);
  versionDates[1][1]=makeDate(2009,11,30,0,0,0);
  //beamTime_parameter
  versionNumbers[2][0]=0;
  versionNumbers[2][1]=1;
  versionNumbers[2][2]=2;
//   versionDates[2][0]=QDate(2005,7,1);
//   versionDates[2][1]=QDate(2007,1,24);
//   versionDates[2][2]=QDate(2009,3,2);
  versionDates[2][0]=makeDate(2005,7,1,0,0,0);
  versionDates[2][1]=makeDate(2007,1,24,0,0,0);
  versionDates[2][2]=makeDate(2009,3,2,0,0,0);
  //run_parameter
  versionNumbers[3][0]=0;
  versionNumbers[3][1]=1;
  versionNumbers[3][2]=2;
//   versionDates[3][0]=QDate(2005,7,1);
//   versionDates[3][1]=QDate(2007,1,24);
//   versionDates[3][2]=QDate(2009,3,2);
  versionDates[3][0]=makeDate(2005,7,1,0,0,0);
  versionDates[3][1]=makeDate(2007,1,24,0,0,0);
  versionDates[3][2]=makeDate(2009,3,2,0,0,0);
  //paint_parameter
  versionNumbers[4][0]=0;
//   versionDates[4][0]=QDate(2005,7,1);
  versionDates[4][0]=makeDate(2005,7,1,0,0,0);
  //detector_parameter
  versionNumbers[5][0]=0;
  versionNumbers[5][1]=1;
  versionNumbers[5][2]=2;
//   versionDates[5][0]=QDate(2005,7,1);
//   versionDates[5][1]=QDate(2008,10,21);
//   versionDates[5][2]=QDate(2009,11,30);
  versionDates[5][0]=makeDate(2005,7,1,0,0,0);
  versionDates[5][1]=makeDate(2008,10,21,0,0,0);
  versionDates[5][2]=makeDate(2009,11,30,0,0,0);
  //material_parameter
  versionNumbers[6][0]=-1;
//   versionDates[6][0]=QDate(2005,7,1);
  versionDates[6][0]=makeDate(2005,7,1,0,0,0);
}


bool check_version_type(const char *filename, int &version, int &type)
{
  if(!existing(filename))return false;
  ifstream input;
  input.open(filename);
  char li[1000];
  input.getline(li,1000);
  string line(li);
  if(!((line.find("version")<line.npos||line.find("Version")<line.npos)&&line.find("parameter")<line.npos))
      return false;
  string names[7]={"algorithm","shape","beamTime","run","paint","detector","material"};
  int t=-1;
  for(int i=0;i<7;i++)
    if(line.find(names[i])<line.npos)t=i;
  if(t<0)return false;
  type=t;
  int pos=line.find("ersion")+6;
  while(line[pos]!=':'&&pos<(int)line.length())
    pos++;
  if(pos==(int)line.length())return false;
  int p2=pos+1;
  while((line[p2]!=' ')&&p2<(int)line.length())
    p2++;
  if(p2==(int)line.length())return false;
  version=toInt(line.substr(pos+1,p2-pos-1));
  input.close();
  int f=-1;
  switch(type)
    {
    case 0:
      for(int i=0;i<parameterManager::numberOfVersions<algorithm_parameter>();i++)
	if(parameterManager::version<algorithm_parameter>(i)==version)
	  f=i;
      break;
    case 1:
      for(int i=0;i<parameterManager::numberOfVersions<shape_parameter>();i++)
	if(parameterManager::version<shape_parameter>(i)==version)
	  f=i;
      break;
    case 2:
      for(int i=0;i<parameterManager::numberOfVersions<beamTime_parameter>();i++)
	if(parameterManager::version<beamTime_parameter>(i)==version)
	  f=i;
      break;
    case 3:
      for(int i=0;i<parameterManager::numberOfVersions<run_parameter>();i++)
	if(parameterManager::version<run_parameter>(i)==version)
	  f=i;
      break;
    case 4:
      for(int i=0;i<parameterManager::numberOfVersions<paint_parameter>();i++)
	if(parameterManager::version<paint_parameter>(i)==version)
	  f=i;
      break;
    case 5:
      for(int i=0;i<parameterManager::numberOfVersions<detector_parameter>();i++)
	if(parameterManager::version<detector_parameter>(i)==version)
	  f=i;
      break;
    case 6:
      for(int i=0;i<parameterManager::numberOfVersions<material_parameter>();i++)
	if(parameterManager::version<material_parameter>(i)==version)
	  f=i;
      break;
    }
  if(f<0)return false;
  return true;
}


