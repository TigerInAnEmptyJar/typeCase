/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
extern string reactionRecognitionFile;
#include "utilities.h"
void ReactionRecognitionWidget::init()
{
    selectedR=-1;
    ReadFromFile(reactionRecognitionFile.data());
    algoDisp=new AlgorithmDisplay(this);
    algoDisp->setReadOnly(false);
}
void ReactionRecognitionWidget::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void ReactionRecognitionWidget::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void ReactionRecognitionWidget::resize(int w, int h)
{
  QWidget::resize(w,h);
    int minH=210;
    int minW=350;
    int addh=h-minH-5;
    int addw=w-minW-5;
    if(addh<0)addh=0;
    if(addw<0)addw=0;
    listView1->setGeometry(10,10,addw/2+150,160+addh);
    
    textLabel2->setGeometry(addw/2+170,10,73,20);
    algoDisp->setGeometry(165+addw/2,35,addw/2+170,h-35);
    algoDisp->resize(addw/2+170,h-35);
    pushButton1->setGeometry(10,170+addh,addw/2+150,20);
}
void ReactionRecognitionWidget::OnHelpAsked()
{
  //    emit helpAsked(helpPath.path()+"/reactionRecognition.html");
    emit helpRequested(string(getenv("KTOFPACK"))+"/help/reactionRecognition.html");
}
void ReactionRecognitionWidget::OnHelpAsked(QString filename)
{
  //    emit helpAsked(filename);
    emit helpRequested(QS2s(filename));
}
void ReactionRecognitionWidget::destroy()
{
    delete algoDisp;
    while(!reacts.empty())
    {
	reacts.pop_back();
    }
}
void ReactionRecognitionWidget::OnReactionsShow()
{
    listView1->clear();
    for(unsigned int i=0;i<reacts.size();i++)
    {
#if QT_VERSION < 0x040000
	new QListViewItem(listView1,reacts[i].getName(),QString::number(reacts[i].getID()));
#else
	new QTreeWidgetItem(listView1,QStringList()<<reacts[i].getName().data()<<QString::number(reacts[i].getID()));
#endif
    }
    selectedR=-1;
}
#if QT_VERSION < 0x040000
void ReactionRecognitionWidget::OnReactionSelect(QListViewItem* it)
#else
void ReactionRecognitionWidget::OnReactionSelect(QTreeWidgetItem* it)
#endif
{
    if(it==NULL)return;
    int num=getReactionPosByName(it->text(0));
    if(num<0)return;
    selectedR=num;
    algoDisp->setAlgorithm(&reacts[num]);
}
#if QT_VERSION < 0x040000
#else
#endif
#if QT_VERSION < 0x040000
void ReactionRecognitionWidget::OnReactionRightClick(QListViewItem *it, const QPoint &p)
{
    QPopupMenu men(this);
    men.insertItem("use",this,SLOT(useReaction(int)),0,0);
    men.insertItem("don't use",this,SLOT(useReaction(int)),1,1);
    if(it==NULL)
    {
    }
    else 
    {
	int num=getReactionPosByName(it->text(0));
	if(num>=0)
	{
	    selectedR=num;
	    men.setItemChecked(0,reacts[num].IsUsed());
	    men.setItemChecked(1,!reacts[num].IsUsed());
	}
    }
    men.insertItem("new Reaction",this,SLOT(newReaction()));
    men.insertItem("install Reaction",this,SLOT(includeReaction()));
    men.insertItem("Help",this,SLOT(OnHelpAsked()));
    men.exec(p);
}
#else
void ReactionRecognitionWidget::OnReactionRightClick(const QPoint &p)
{
  QTreeWidgetItem *it=listView1->itemAt(p);
  int num=-1;
  if(it==NULL){}
  else 
    {
      num=getReactionPosByName(it->text(0));
    }
  QMenu men(this);
  QAction *ac=men.addAction("use",this,SLOT(useReaction(QAction*)));
  if(num>=0){ac->setCheckable(true);ac->setChecked(reacts[num].IsUsed());}
  ac=men.addAction("don\'t use",this,SLOT(useReaction(QAction*)));
  if(num>=0){ac->setCheckable(true);ac->setChecked(!reacts[num].IsUsed());}
  men.addAction("new Reaction",this,SLOT(newReaction()));
  men.addAction("install Reaction",this,SLOT(includeReaction()));
  men.addAction("Help",this,SLOT(OnHelpAsked()));
  men.exec(listView1->viewport()->mapToGlobal(p));
}
#endif
void ReactionRecognitionWidget::newReaction()
{
    QMessageBox::information(this, "new Reaction","You selected new Reaction from menu.\nThis is not available yet.",QMessageBox::Ok);
    //connect(fenst,SIGNAL(helpAsked(QString)),this,SLOT(OnHelpAsked(QString)));
    //disconnect(fenst);
}
void ReactionRecognitionWidget::includeReaction()
{
//    vector<algorithm_parameter> tmp;
//    algorithm_parameter *ap=new algorithm_parameter();
//    for(unsigned int i=0;i<aparams.size();i++) tmp.push_back(*aparams[i]);
//    AlgorithmInclude fenst(this);
//    connect(&fenst,SIGNAL(helpAsked(QString)),this,SLOT(OnHelpAsked(QString)));
//    fenst.setPointers(ap,&tmp);
//    fenst.exec();
//    if(fenst.result()==QWizard::Accepted)
//    {
//	if(fenst.compileAnew()&&fenst.compilePtt()>=0)
//	{
//	    QString message=QString("New Algorithm ")+ap->getName()+" has been installed; code for algorithm-call has been included.\n";
//	    if((fenst.compilePtt()&15)>0)
//		message+="To make algorithm available for analysis\nplease recompile ";
//	    if((fenst.compilePtt()&7)>0)
//		message+="algorithm- ";
//	    if((fenst.compilePtt()&15)>=9)
//		message+="and analyser- ";
//	    if((fenst.compilePtt()&15)>0)
//		message+="package and relink typeCase.\nIn case of doubt contact your system administrator.";
//	    QMessageBox::information(this,"install Algorithm",message,QMessageBox::Ok);
//	}	
//	else
//	    QMessageBox::information(this, "install Algorithm",QString("New Algorithm ")+ap->getName()+" has been installed and is available for use.",QMessageBox::Ok);
//
//    }
//    else
//    {
//	delete ap;
//    }
//    disconnect(&fenst);
}
#if QT_VERSION > 0x040000
void ReactionRecognitionWidget::useReaction(QAction* ac)
{
  if(ac==NULL)return;
  if(ac->text().contains("don\'t"))useReaction(1);
  else useReaction(0);
}
#endif
void ReactionRecognitionWidget::useReaction(int id)
{
    if(selectedR==-1)return;
    if(id==0)
    {
	if(reacts[selectedR].IsUsed()==true)return;
	reacts[selectedR].setUse(true);
	emit reactionSelected(&reacts[selectedR],true);
    }
    else if(id==1)
    {
	if(reacts[selectedR].IsUsed()==false)return;
	reacts[selectedR].setUse(false);
	emit reactionSelected(&reacts[selectedR],false);
	emit reactionUnselected(&reacts[selectedR]);
    }
}
void ReactionRecognitionWidget::OnReactionSelected(algorithm_parameter *ap)
{
    ap->setUse(true);
}
void ReactionRecognitionWidget::OnReactionUnselected(algorithm_parameter *ap)
{
    ap->setUse(false);
}
int ReactionRecognitionWidget::getReactionPosByName(QString an)
{
  int num=-1;
  for(unsigned int i=0;i<reacts.size();i++)
    if(an==reacts[i].getName().data())
      num=i;
  return num;
}
void ReactionRecognitionWidget::ReadFromFile(QString fn)
{
    if(!QFile::exists(fn))return;
    while(!reacts.empty())
    {
	reacts.pop_back();
    }
    ifstream input;
    input.open(QS2s(fn).data());
    input>>RecognitionAlgorithm;
    while(RecognitionAlgorithm.getNumberOfParam<algorithm_parameter>()>0)RecognitionAlgorithm.popParam<algorithm_parameter>();
    int num;
    input>>num;
    for(int i=0;i<num;i++)
    {
	algorithm_parameter ap;
	input>>ap;
	reacts.push_back(ap);
	if(reacts.back().IsUsed())
	{
	    RecognitionAlgorithm.addParam<algorithm_parameter>(single_parameter<algorithm_parameter>(reacts.back().getName(),reacts.back()));
	    emit reactionSelected(&reacts.back(),true);
	}
    }
    input.close();
//    ParameterRead::getAlgorithm(fn,aparams,false,0);
    selectedR=-1;
    OnReactionsShow();
}
void ReactionRecognitionWidget::WriteToFile(QString fn)
{
    ofstream output;
    output.open(QS2s(fn).data());
    output<<RecognitionAlgorithm;
    output<<reacts.size()<<" ";
    for(unsigned int i=0;i<reacts.size();i++)
	output<<reacts[i];
    output.close();
}
void ReactionRecognitionWidget::OnInsertClick()
{
       while(RecognitionAlgorithm.getNumberOfParam<algorithm_parameter>()>0)RecognitionAlgorithm.popParam<algorithm_parameter>();
       string option="__";
       for(unsigned int i=0;i<reacts.size();i++)
       {
	   if(reacts[i].IsUsed())
	   {
	       RecognitionAlgorithm.addParam<algorithm_parameter>(single_parameter<algorithm_parameter>(reacts[i].getName(),reacts[i]));
	       option=option+reacts[i].getName()+"__";
	   }
       }
       RecognitionAlgorithm.changeParam<string>("Names",option);
       algorithm_parameter *tmp=new algorithm_parameter(RecognitionAlgorithm);
       emit insertClick(tmp,false);
}
algorithm_parameter *ReactionRecognitionWidget::getAlgorithm()
{
    algorithm_parameter *tmp=new algorithm_parameter(RecognitionAlgorithm);
    return tmp;
}
