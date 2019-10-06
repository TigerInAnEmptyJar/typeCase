#include "readFromTade.h"
#include "logger.h"
#include <fstream>
#include <iostream>
//#include <qprocess.h>
#include "beamtimeparameter.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
char li[200];
extern bool existing(string filename);
class asciiReader
{
private:
  istream& input;

public:
  asciiReader(istream& in);
  int getInt(int v = -1);
  string getLine();
  bool eof() const;
  bool good() const;
  void clear();
};
typedef struct
{
  string feederName;
  string inputFile;
  string fifoFile;
} feederstruct_t;
void setFee(feederstruct_t& a, feederstruct_t* f)
{
  a.feederName = f->feederName;
  a.inputFile = f->inputFile;
  a.fifoFile = f->fifoFile;
};

void* feederFuction(void* params)
{
  //  cout<<"feederFunction"<<endl;
  feederstruct_t parameter;
  setFee(parameter, (feederstruct_t*)params);
  //  int ret;
  system((parameter.feederName + " " + parameter.inputFile + " " + parameter.fifoFile).data());
  return NULL;
}
asciiReader::asciiReader(istream& in) : input(in) {}
int asciiReader::getInt(int v)
{
  if (input.eof())
    return v;
  if (!input.good())
    return v;
  // cout<<input.good()<<" "<<input.eof()<<" "<<input.rdstate()<<endl;
  int n;
  input >> n;
  if (!input.good())
    return v;
  return n;
}
string asciiReader::getLine()
{
  string ret = "";
  if (input.eof())
    return ret;
  char c;
  input.get(c);
  while (c != '\n' && input.good()) {
    ret += c;
    input.get(c);
  }
  return ret;
}
bool asciiReader::eof() const { return input.eof(); }
bool asciiReader::good() const { return input.good(); }
void asciiReader::clear()
{
  if (input.good() || input.eof())
    return;
  char* li = new char[10];
  input.getline(li, 10);
  delete[] li;
}

AReadFromTade::AReadFromTade(/*ifstream **inputIn,*/ TRawHit*** hitIn, int& eventNumberIn,
                             int& triggerIn, int** numHits, int maxdet, int maxhit, int& readID,
                             void* input_mutexIn, bool& validInputIn)
    : AAlgorithm("read from tade"), EventNumber(eventNumberIn), trigger(triggerIn), maxHit(maxhit),
      maxDet(maxdet), whichID(readID), validInput(validInputIn), filetype(0)
{
  feeder = new pthread_t(); // QProcess(this);
  //   pthread_attr_init((pthread_t*)feeder);
  input_mutex = input_mutexIn;
  input = NULL; // inputIn;
  numberOfHits = numHits;
  hit = hitIn;
  init = false;
  cleanupInput = false;
}

AReadFromTade::~AReadFromTade()
{
  if (myInput && input != NULL) {
    input->close();
    delete input;
  }
  pthread_cancel(*((pthread_t*)feeder));
  delete (pthread_t*)feeder; //((QProcess*)feeder);
                             //  int ret;
  if (filetype > 0) {
    cout << "call cleaner" << endl;
    system((string("rm -f ") + scriptFile).data());
  }
  cout << "call feeder" << endl;
  if (cleanupInput) {
    cout << "call cleaner input" << endl;
    system((string("rm -f ") + fifoFile).data());
  }
  cout << "last event was " << nextEventNumber << endl;
}

void AReadFromTade::process()
{
  if (input == NULL) {
    validInput = false;
    return;
  }
  //   if(whichID==9)
  {
    asciiReader reader(*input);
    if (!init) {
      string s = reader.getLine();
      while (!(s.find("vent") < s.npos || s.find("Mcarlo") < s.npos || s.find("onte") < s.npos ||
               input->eof())) {
        //  cout<<s.length()<<endl;
        nextEventNumber = reader.getInt(); // number;
        nextEventCount = reader.getInt();  // number;
        reader.getInt();
        nextTrigger = reader.getInt(); // number;
        s = reader.getLine();          // string(li);
        // cout<<s.data()<<endl;
      }
      init = true;
    }
    bool condition = true;
    int data[4] = {-1, -1, -1, -1}, n;
    TRawHit* hut;
    string s;
    int count;
    while (condition) {
      s = "";
      count = -1;
      for (int i = 0; i < maxDet; i++) {
        (*(numberOfHits[i])) = 0;
      }
      while (
          !((s.find("vent") < s.npos) || (s.find("Mcarlo") < s.npos) || s.find("onte") < s.npos)) {
        if (count >= 0) {
          if (((data[2] < maxDet) && (data[3] < 1000000) && (*numberOfHits[data[2]] < maxHit) &&
               (data[2] >= 0) && (data[3] >= 0))) {
            n = *(numberOfHits[data[2]]);
            hut = hit[data[2]][n];
            hut->setDetector(data[2]);
            hut->setElement(data[3]);
            hut->setTDC(data[0]);
            hut->setADC(data[1]);
            (*(numberOfHits[data[2]])) = (*(numberOfHits[data[2]])) + 1;
            //		      cout<<(*(numberOfHits[data[2]]))+1<<"
            //"<<*hut<<endl;
          }
        }
        count++;
        for (int i = 0; i < 4; i++) {
          //		  cout<<input->good()<<" ";
          // if(!input->good())cout<<nextEventNumber<<" "<<input->good()<<endl;
          reader.clear();
          if (!(reader.good())) // input->good()))
          {
            anaLog << "input not good " << i << endli;
            init = false;
            validInput = false;
            //		      pthread_mutex_unlock((pthread_mutex_t*)input_mutex);
            return;
          }
          data[i] = reader.getInt();
        }
        //	      cout<<nextEventNumber<<", det:"<<data[2]<<",
        // el:"<<data[3];
        s = reader.getLine(); // string(li);
        //   cout<<"\""<<s.data()<<"\""<<endl;
        if (data[2] < 0) {
          anaLog << "negative detector ID read!" << endli;
          data[2] = maxDet + 1;
        }
        if (data[3] < 0) {
          anaLog << "negative element number read!" << endli;
          data[3] = maxHit + 1;
        }
      }
      //	  cout<<count<<endl;
      if (count == nextEventCount)
        condition = false;
      else {
        condition = true;
        anaLog << "corrupt event (" << nextEventNumber << "): wrong number of hits in header ("
               << count << " vs. " << nextEventCount << "). Read new event " << data[1] << endli;
      }
      EventNumber = nextEventNumber;
      nextEventCount = data[1];
      nextEventNumber = data[0];
      trigger = nextTrigger;
      nextTrigger = data[3];
    }
    // emit eventRead(t,a,0);
    //      pthread_mutex_unlock((pthread_mutex_t*)input_mutex);
  }
  return;
}

void AReadFromTade::newEvent(int evtNr, int count, int trigger)
{
  nextEventCount = count;
  nextEventNumber = evtNr;
  nextTrigger = trigger;
  //  cout<<"AReadFromTade::newEvent()"<<endl;
}
// void AReadFromTade::Streamer(TBuffer &b)
// {
// }
void AReadFromTade::newInput(const string& filename)
{
  validInput = false;
  if (filename != "") {
    validInput = true;
    if (input != NULL && myInput) {
      input->close();
      delete input;
    }
    anaLog << "open file: " << filename << " for TADE-input" << endli;
    input = new ifstream();
    input->open(filename.data());
    myInput = true;
    //    emit newRun(input);
  }
}
#include <QtCore/QDir>
#include <sys/stat.h>
#include <sys/types.h>
void AReadFromTade::newInput(run_parameter& r)
{
  anaLog << "AReadFromTade::newInput()" << endli;
  string filename = "", actualFileName = "";
  for (size_t i = 0; i < r.getNumberOfFiles(); i++)
    if (r.getFileType(i) == 0) {
      filename = r.getFile(i);
      break;
    }
  if (input != NULL && myInput) {
    input->close();
    delete input;
    input = NULL;
    if (filetype > 0) {
      cout << "call cleaner" << endl;
      system((string("rm -f ") + scriptFile).data());
    }
    cout << "call feeder" << endl;
    if (cleanupInput) {
      pthread_cancel(*((pthread_t*)feeder));
      cout << "call cleaner input" << endl;
      system((string("rm -f ") + fifoFile).data());
    }
    // #if QT_VERSION < 0x040000
    //       ((QProcess*)feeder)->clearArguments();
    // #endif
  }
  validInput = false;
  string file_name, directory_name = "", file_ext = "";
  file_name = filename;
  if (file_name.find("/") < file_name.npos) {
    directory_name = file_name.substr(0, file_name.rfind("/"));
    file_name =
        file_name.substr(file_name.rfind("/") + 1, file_name.length() - 1 - file_name.rfind("/"));
  }
  if (file_name.find(".") < file_name.npos) {
    file_ext =
        file_name.substr(file_name.find(".") + 1, file_name.length() - 1 - file_name.find("."));
    file_name = file_name.substr(0, file_name.find("."));
  }
  //  cout<<"\""<<filename.data()<<"\" \""<<file_name.data()<<"\"
  //  \""<<file_ext.data()<<"\"
  //  \""<<directory_name.data()<<"\""<<endl;
  bool needsFifo = (file_ext.find("fifo") < file_ext.npos || !existing(filename));
  bool needsdecompression = false;
  bool needsdecompression2 = false;
  string exts[8] = {"tade",    "evt",    "tade.zip", "tade.gz",
                    "evt.zip", "evt.gz", "tade.bz2", "evt.bz2"};
  int ft = -1;
  for (int i = 7; i >= 0; i--) {
    if (file_ext == exts[i] || existing(directory_name + "/" + file_name + "." + exts[i]))
      ft = i;
  }
  //   cout<<filename.data()<<" "<<file_ext.data()<<" "<<ft<<endl;;
  if (ft == -1) {
    if (file_ext == "tade" || file_ext == "evt")
      filetype = 0;
    else if (file_ext == "tade.zip" || file_ext == "tade.gz" || file_ext == "evt.zip" ||
             file_ext == "evt.gz")
      filetype = 1;
    else if (file_ext == "tade.bz2" || file_ext == "evt.bz2")
      filetype = 2;
    else if (file_ext == "fifo" || file_ext == "tade.fifo" || file_ext == "evt.fifo") {
      filetype = 8;
    }
  } else
    filetype = (ft > 1 ? 1 : 0);
  if (needsFifo || ft == 2 || ft == 3 || ft == 4 || ft == 5 || ft == 6 || ft == 7)
    needsFifo = true;
  if (ft == 2 || ft == 3 || ft == 4 || ft == 5 || ft == 6 || ft == 7)
    needsdecompression = true;
  if (ft == 6 || ft == 7)
    needsdecompression2 = true;
  validInput = (ft >= 0);
  //   cout<<needsdecompression<<" "<<needsdecompression2<<endl;
  actualFileName = filename;
  if (needsFifo) {
    cleanupInput = !existing(fifoFile);
    fifoFile = (directory_name + "/" + file_name + ".fifo");
    if (!existing(fifoFile))
      mkfifo(fifoFile.data(), S_IRUSR | S_IWUSR | S_IRGRP);
    scriptFile = string("feeder_") + string_number(getpid()) + ".sh";
    ofstream scripter;
    scripter.open(scriptFile.data());
    if (!needsdecompression)
      scripter << "cat $1 > $2" << endl;
    else if (!needsdecompression2)
      scripter << "zcat $1 > $2" << endl;
    else
      scripter << "bzcat $1 > $2" << endl;
    scripter.close();
    //      int ret;
    system((string("chmod u=rwx ") + scriptFile).data());
    feederstruct_t tmp;
    tmp.feederName = scriptFile;
    if (filetype < 8)
      tmp.inputFile = filename;
    else
      tmp.inputFile = actualFileName;
    tmp.fifoFile = fifoFile;
    pthread_create((pthread_t*)feeder, NULL, feederFuction, &tmp);
    /*
#if QT_VERSION < 0x040000
    ((QProcess*)feeder)->addArgument(scriptFile);
    if(filetype<8)
      ((QProcess*)feeder)->addArgument(filename);
    else
      ((QProcess*)feeder)->addArgument(actualFileName);
    ((QProcess*)feeder)->addArgument(fifoFile);
    ((QProcess*)feeder)->start();
#else
    if(filetype<8)
      ((QProcess*)feeder)->start((scriptFile+" "+filename+" "+fifoFile).data());
    else
      ((QProcess*)feeder)->start((scriptFile+" "+actualFileName+"
"+fifoFile).data());
#endif
    */
    actualFileName = fifoFile;
  }

  // cout<<"open file:\""<<actualFileName.data()<<"\""<<endl;
  validInput = true;
  anaLog << "open file: " << filename << " for TADE-input";
  if (needsFifo) {
    anaLog << " fifo via ";
    if (!needsdecompression)
      anaLog << "cat ";
    else if (!needsdecompression2)
      anaLog << "zcat ";
    else
      anaLog << "bzcat ";
    anaLog << "\"" << fifoFile.data() << "\"" << endli;
  }

  input = new ifstream();
  // cout<<"file open:"<<(filetype==0?filename.data():fifoFile.data())<<endl;
  input->open(actualFileName.data()); //(filetype==0?filename.data():fifoFile.data()));
  cout << filename << " opened for TADE-input";
  if (needsFifo) {
    cout << " fifo via";
    if (!needsdecompression)
      cout << " cat ";
    else if (!needsdecompression2)
      cout << " zcat ";
    else
      cout << " bzcat ";
    cout << "\"" << fifoFile.data() << "\"" << endl;
  }
  //       cout<<actualFileName.data()<<" opened for TADE-data-input \n\t";
  //       switch(filetype)
  // 	{
  // 	case 1:cout<<" via zcat \""<<fifoFile.data()<<"\"";break;
  // 	case 2:cout<<" via bzcat \""<<fifoFile.data()<<"\"";break;
  // 	case 8:cout<<" fifo via cat \""<<fifoFile.data()<<"\"";break;
  // 	case 9:cout<<" fifo via zcat \""<<fifoFile.data()<<"\"";break;
  // 	case 10:cout<<" fifo via bzcat \""<<fifoFile.data()<<"\"";break;
  // 	}
  //       cout<<endl;
  myInput = true;
  //  emit newRun(input);
}
void AReadFromTade::newInput(ifstream* in)
{
  validInput = false;
  if (in != NULL) {
    input = in;
    validInput = true;
    myInput = false;
  }
}
