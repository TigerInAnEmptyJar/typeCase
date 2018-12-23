#include <qapplication.h>
#include "typecasemw.h"
#include<qdir.h>
#include<qfileinfo.h>
QDir typeCaseBase;
QDir typeCaseHome;
QDir ktofDir;
QDir calibrationDir;
QDir geoDir;
QDir dataDir;
QDir helpPath;

int main( int argc, char ** argv )
{
    typeCaseHome.setPath(QString(getenv("HOME"))+"/.typeCase");
    string progname(argv[0]);
    if(progname.find("/")<progname.npos)
    {
	typeCaseBase=QDir(progname.substr(0,progname.rfind("/")).data());
    }
    else
    {
	string pathvar(getenv("PATH")),pathpart;
	while(pathvar.length()>1)
	{
	    if(pathvar.find(":")<pathvar.npos)
		pathpart=pathvar.substr(0,pathvar.find(":"));
	    else
		pathpart=pathvar;
	    pathvar=pathvar.substr(pathvar.find(":")+1,pathvar.length()-pathvar.find(":")-1);
	    pathpart=remove(pathpart,":");
	    QFileInfo tmp((pathpart+"/"+progname).data());
	    if(tmp.exists()&&tmp.isExecutable())
	    {
		typeCaseBase=QDir(pathpart.data());
		pathvar="";
	    }
	}
	if(typeCaseBase.path()==".")
	    typeCaseBase=QDir::current();
	typeCaseBase=QDir((string(getenv("KTOFPACK"))+"/gui/typeCase/").data());
    }
    helpPath=typeCaseBase.path()+"/helps";
    ktofDir=QDir(QString(getenv("KTOFPACK")));
    geoDir=ktofDir;//QDir(QString(getenv("GEOPACK")));
    dataDir=QDir(QString((string(getenv("KTOFPACK"))+"/data/algorithms").data()));
    QApplication a( argc, argv );
    typeCaseMW w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
