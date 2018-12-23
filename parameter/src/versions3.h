 
void write_run_parameter_0(ostream& o,const run_parameter &d)
{
    o<<d.getType()<<" "<<d.getParentNumber()<<" "<<d.getRunNumber()<<" ";
    if(d.hasOwnSetup())
	o<<1<<" ";
    else
	o<<0<<" ";
    if(d.hasOwnCalibration())
	o<<1;
    else
	o<<0;
    o<<d.getName().data()<<endl;
    vector<string> dis=d.getDescription();
    o<<dis.size();
    for(unsigned int i=0;i<dis.size();i++)
	o<<dis[i].data()<<endl;
    o<<" "<<d.getNumberOfFiles()<<" ";
    for(int i=0;i<d.getNumberOfFiles();i++)
      o<<d.getFileType(i)<<" "<<d.getFile(i).data()<<endl;
    if(d.hasOwnSetup())
	o<<d.getSetupFile().data()<<endl;
    if(d.hasOwnCalibration())
    {
      o<<d.getNumberOfCalibrationFiles()<<" ";
      for(int i=0;i<d.getNumberOfCalibrationFiles();i++)
	{
	  o<<d.getCalibrationFile(i).data()<<endl;
	}
    }
}
void read_run_parameter_0(istream& i, run_parameter &d)
{
    int zahl;
    char li[500];
    char c;
    i>>zahl;
    d.setType(zahl);
    i>>zahl;
    d.setParentNumber(zahl);
    i>>zahl;
    d.setRunNumber(zahl);
    i>>zahl;
    d.setOwnSetup(zahl==1);
    i>>zahl;
    d.setOwnCalibration(zahl==1);
    i.getline(li,250);
    d.setName(string(li));
    i>>zahl;
    vector<string> dis;
    for(int I=0;I<zahl;I++)
    {
	i.getline(li,250);
	dis.push_back(string(li));
    }
    d.setDescription(dis);
    i>>zahl;
    int zahl2;
    for(int I=0;I<zahl;I++)
      {
	i>>zahl2;
	i.get(c);
	i.getline(li,250);
	d.addFile(string(li),zahl2);
      }
    if(d.hasOwnSetup())
    {
	i.getline(li,250);
	d.setSetupFile(string(li));
    }
    if (d.hasOwnCalibration())
    {
      i>>zahl;
      i.get(c);
      for(int I=0;I<zahl;I++)
	{
	  i.getline(li,250);
	  d.addCalibrationFile(string(li));
	}
    }
}
void write_run_parameter_1(ostream& o,const run_parameter &d)
{
    o<<d.getType()<<" "<<d.getParentNumber()<<" "<<d.getRunNumber()<<" ";
    if(d.hasOwnSetup())
	o<<1<<" ";
    else
	o<<0<<" ";
    if(d.hasOwnCalibration())
	o<<1;
    else
	o<<0;
    o<<d.getName().data()<<endl;
    vector<string> dis=d.getDescription();
    o<<dis.size();
    for(unsigned int i=0;i<dis.size();i++)
	o<<dis[i].data()<<endl;
    o<<" "<<d.getNumberOfFiles()<<" ";
    for(int i=0;i<d.getNumberOfFiles();i++)
      o<<d.getFileType(i)<<" "<<d.getFileEvents(i)<<" "<<d.getFile(i).data()<<endl;
    //    o<<d.getFileName().data()<<endl;
    if(d.hasOwnSetup())
	o<<d.getSetupFile().data()<<endl;
    if(d.hasOwnCalibration())
    {
      o<<d.getNumberOfCalibrationFiles()<<" ";
      for(int i=0;i<d.getNumberOfCalibrationFiles();i++)
	{
	  o<<d.getCalibrationFile(i).data()<<endl;
	}
    }
}
void read_run_parameter_1(istream& i, run_parameter &d)
{
    int zahl;
    char li[500];
    char c;
    i>>zahl;
    d.setType(zahl);
    i>>zahl;
    d.setParentNumber(zahl);
    i>>zahl;
    d.setRunNumber(zahl);
    i>>zahl;
    d.setOwnSetup(zahl==1);
    i>>zahl;
    d.setOwnCalibration(zahl==1);
    i.getline(li,250);
    d.setName(string(li));
    i>>zahl;
    vector<string> dis;
    for(int I=0;I<zahl;I++)
    {
	i.getline(li,250);
	dis.push_back(string(li));
    }
    d.setDescription(dis);
    //insert 
    i>>zahl;
    int zahl2;
    long zahl3;
    for(int I=0;I<zahl;I++)
      {
	i>>zahl2>>zahl3;
	i.get(c);
	i.getline(li,250);
	d.addFile(string(li),zahl2,zahl3);
      }
    if(d.hasOwnSetup())
    {
	i.getline(li,250);
	d.setSetupFile(string(li));
    }
    if (d.hasOwnCalibration())
    {
      //insert
      i>>zahl;
      i.get(c);
      for(int I=0;I<zahl;I++)
	{
	  i.getline(li,250);
	  d.addCalibrationFile(string(li));
	}
    }
}
