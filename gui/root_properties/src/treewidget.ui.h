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
void TreeWidget::init()
{
    currentCut=NULL;
    mathexpressions.push_back("sin");
    mathexpressions.push_back("cos");
    mathexpressions.push_back("tan");
    mathexpressions.push_back("exp");
    mathexpressions.push_back("log");
    mathexpressions.push_back("abs");
    mathexpressions.push_back("pow");
    mathexpressions.push_back("M_PI");
    mathexpressions.push_back("TMath::Pi");
    ownexpressions.push_back("M_Miss");
    ownexpressions.push_back("M_Inv");
    ownexpressions.push_back("angle");
    ownexpressions.push_back("CM");
    particleManips.push_back("mass");
    particleManips.push_back("momentum");
    particleManips.push_back("energy");
    particleManips.push_back("pt");
    particleManips.push_back("pl");
    particleManips.push_back("vt");
    particleManips.push_back("vl");
    binning=100;
    binning2D=40;
    binning3D=40;
    if(!Eparticles::IsInit())Eparticles::init();
    initParticle1.setPM(vector3D(0,0,0),Eparticles::getMassG(14));
    initParticle2.setPM(vector3D(0,0,0),Eparticles::getMassG(14));
}
void TreeWidget::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void TreeWidget::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void TreeWidget::resize(int w, int h)
{
    textLabel6->setGeometry(10,h-60,w-20,23);
    lineEdit1->setGeometry(10,h-35,w-20,23);
    listView1->resize(((w-30)/2),(h-155));
    textLabel7->setGeometry(((w-30)/2)+20,65,((w-30)/2),23);
    listView2->setGeometry(((w-30)/2)+20,90,((w-30)/2),h-155);
    textLabel1_2->setGeometry(((w-30)/2)+20,10,74,23);
    lineEdit2->setGeometry(((w-30)/2)+20,35,((w-30)/2),23);
}
void TreeWidget::setTree(TTree *treeIn)
{
    textLabel2->setText(treeIn->GetName());
    textLabel5->setText(QString::number(treeIn->GetEntries()));
    tree=treeIn;
    listView1->clear();
    QString branchtype;
    QListViewItem *it=0, *ac=0;
    QString s1,s2;
    for(int i=0;i<tree->GetListOfBranches()->GetEntries();i++)
    {
	s1="";
	s2="";
	if(((TBranch*)tree->GetListOfBranches()->At(i))->FindLeaf("theta")!=NULL&&
	   ((TBranch*)tree->GetListOfBranches()->At(i))->FindLeaf("ID")!=NULL)
	    branchtype="track";
	else if(((TBranch*)tree->GetListOfBranches()->At(i))->FindLeaf("det")!=NULL&&
		((TBranch*)tree->GetListOfBranches()->At(i))->FindLeaf("el")!=NULL&&
		((TBranch*)tree->GetListOfBranches()->At(i))->FindLeaf("qdcR")!=NULL)
	    branchtype="hit";
	else if(((TBranch*)tree->GetListOfBranches()->At(i))->GetListOfLeaves()->GetEntries()<=1)
	{
	    s1=QString::number(tree->GetMinimum(tree->GetListOfBranches()->At(i)->GetName()));
	    s2=QString::number(tree->GetMaximum(tree->GetListOfBranches()->At(i)->GetName()));
	    branchtype="leaf";
	}
	else
	    branchtype="unknown";
	it=new QListViewItem(listView1,tree->GetListOfBranches()->At(i)->GetName(),branchtype,s1,s2);
	if(branchtype=="track")
	{
	    Double_t val1,val2;
	    TBranch *br=((TBranch*)tree->GetListOfBranches()->At(i));
	    s1=QString(br->GetName());
	    val1=tree->GetMinimum((s1+"/theta").ascii());
	    val2=tree->GetMaximum((s1+"/theta").ascii());
	    new QListViewItem(it,"theta","double",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/phi").ascii());
	    val2=tree->GetMaximum((s1+"/phi").ascii());
	    new QListViewItem(it,"phi","double",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/beta").ascii());
	    val2=tree->GetMaximum((s1+"/beta").ascii());
	    new QListViewItem(it,"beta","double",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/momentum").ascii());
	    val2=tree->GetMaximum((s1+"/momentum").ascii());
	    new QListViewItem(it,"momentum","double",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/nEl").ascii());
	    val2=tree->GetMaximum((s1+"/nEl").ascii());
	    new QListViewItem(it,"nEl","int",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/ID").ascii());
	    val2=tree->GetMaximum((s1+"/ID").ascii());
	    new QListViewItem(it,"ID","int",QString::number(val1),QString::number(val2));
	    ac=new QListViewItem(it,"ss","int[]","","");
	    for(int j=0;j<17;j++)
	    {
		val1=tree->GetMinimum((s1+"/ss["+QString::number(j)+"]").ascii());
		val2=tree->GetMaximum((s1+"/ss["+QString::number(j)+"]").ascii());
		new QListViewItem(ac,QString("ss[")+QString::number(j)+"]",QString::number(val1),QString::number(val2));
	    }
	    ac=new QListViewItem(it,"tdc","double[]","","");
	    for(int j=0;j<17;j++)
	    {
		val1=tree->GetMinimum((s1+"/tdc["+QString::number(j)+"]").ascii());
		val2=tree->GetMaximum((s1+"/tdc["+QString::number(j)+"]").ascii());
		new QListViewItem(ac,QString("tdc[")+QString::number(j)+"]",QString::number(val1),QString::number(val2));
	    }
	    ac=new QListViewItem(it,"qdc","double[]","","");
	    for(int j=0;j<17;j++)
	    {
		val1=tree->GetMinimum((s1+"/qdc["+QString::number(j)+"]").ascii());
		val2=tree->GetMaximum((s1+"/qdc["+QString::number(j)+"]").ascii());
		new QListViewItem(ac,QString("qdc[")+QString::number(j)+"]",QString::number(val1),QString::number(val2));
	    }
	}
	else if(branchtype=="hit")
	{
	    Double_t val1,val2;
	    TBranch *br=((TBranch*)tree->GetListOfBranches()->At(i));
	    s1=QString(br->GetName());
	    val1=tree->GetMinimum((s1+"/det").ascii());
	    val2=tree->GetMaximum((s1+"/det").ascii());
	    new QListViewItem(it,"det","int",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/el").ascii());
	    val2=tree->GetMaximum((s1+"/el").ascii());
	    new QListViewItem(it,"el","int",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/qdcR").ascii());
	    val2=tree->GetMaximum((s1+"/qdcR").ascii());
	    new QListViewItem(it,"qdcR","int",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/tdcR").ascii());
	    val2=tree->GetMaximum((s1+"/tdcR").ascii());
	    new QListViewItem(it,"tdcR","int",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/qdcC").ascii());
	    val2=tree->GetMaximum((s1+"/qdcC").ascii());
	    new QListViewItem(it,"qdcC","double",QString::number(val1),QString::number(val2));
	    val1=tree->GetMinimum((s1+"/tdcC").ascii());
	    val2=tree->GetMaximum((s1+"/tdcC").ascii());
	    new QListViewItem(it,"tdcC","double",QString::number(val1),QString::number(val2));
	}
	else if(branchtype=="unknown")
	{
	    Double_t val1,val2;
	    TBranch *br=((TBranch*)tree->GetListOfBranches()->At(i));
	    TLeaf* lv;
	    s1=QString(br->GetName());
	    for(int j=0;j<((TBranch*)tree->GetListOfBranches()->At(i))->GetListOfLeaves()->GetEntries();j++)
	    {
		lv=((TLeaf*)((TBranch*)tree->GetListOfBranches()->At(i))->GetListOfLeaves()->At(i));
		val1=tree->GetMinimum((s1+"/"+lv->GetName()).ascii());
		val2=tree->GetMaximum((s1+"/"+lv->GetName()).ascii());
		new QListViewItem(it,((TBranch*)tree->GetListOfBranches()->At(i))->GetListOfLeaves()->At(j)->GetName(),QString::number(val1),QString::number(val2));
	    }
	}
    }
}
void TreeWidget::OnDrawEnter()
{
    int nEntries=0;
    QString s=lineEdit1->text();
    s=s.replace("#","");
    if(regularExpression(s))
	nEntries=tree->Draw(s.ascii(),getApplyCut(), lineEdit2->text().ascii());
    else
	complicatedDraw(lineEdit1->text());
    textLabel6->setText(QString("Draw: (")+QString::number(nEntries)+")");
    emit drawingDone();
    drawHistory.push_back(lineEdit1->text());
    drawHistoryPos=-1;
}
void TreeWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_F2||e->key()==Qt::Key_F10||e->key()==Qt::Key_Up||e->key()==Qt::Key_Down)
    {
	if(e->key()==Qt::Key_F2||(drawHistoryPos==-1&&(e->key()==Qt::Key_Up||e->key()==Qt::Key_Down)))
	{
	    drawHistoryPos=drawHistory.size()-1;
	    if(drawHistoryPos>=0)lineEdit1->setText(drawHistory[drawHistoryPos]);
	}
	else if(e->key()==Qt::Key_Up)
	{
	    if(drawHistoryPos==0)
	    {
		lineEdit1->clear();
	    }
	    else 
	    {	
		drawHistoryPos--;
		lineEdit1->setText(drawHistory[drawHistoryPos]);
	    }
	}
	else if(e->key()==Qt::Key_Down)
	{
	    if(drawHistoryPos<(int)drawHistory.size()-1)
		drawHistoryPos++;
	    lineEdit1->setText(drawHistory[drawHistoryPos]);
	}
	else if(e->key()==Qt::Key_F10)
	{
	    while(!drawHistory.empty())drawHistory.pop_back();
	    drawHistoryPos=-1;
	}
    }
    else
	QWidget::keyPressEvent(e);
}
bool TreeWidget::regularExpression(QString exp)
{
    vector<QString>expressions;
    exp.replace(")"," ");
    exp.replace("("," ");
    exp.replace("+"," ");
    exp.replace("-"," ");
    exp.replace("*"," ");
    exp.replace("/"," ");
    exp.replace(","," ");
    exp.replace(":"," ");
    for(unsigned int i=0;i<mathexpressions.size();i++)
	exp.replace(mathexpressions[i]+"("," (");
    while(exp[0]==' ')exp=exp.right(exp.length()-1);
    while(exp.length()>0)
    {
	expressions.push_back(exp.left(exp.find(" ")));
	exp=exp.right(exp.length()-expressions.back().length());
	if(QString::number(expressions.back().toDouble())==expressions.back())
	    expressions.pop_back();
	while(exp[0]==' ')exp=exp.right(exp.length()-1);
    }
    vector<QString> subExpr;
    QString tmp;
    for(unsigned int i=0;i<expressions.size();i++)
    {
	while(!subExpr.empty())subExpr.pop_back();
	if(expressions[i].contains("."))
	{
	    tmp=expressions[i];
	    while(tmp.contains("."))
	    {
		subExpr.push_back(tmp.left(tmp.find(".")));
		tmp=tmp.right(tmp.length()-tmp.find(".")-1);
	    }
	    subExpr.push_back(tmp);
	}
	else
	    subExpr.push_back(expressions[i]);
	TBranch *br=tree->GetBranch(subExpr[0]);
	if(br==NULL)return false;
	for(unsigned int j=1;j<subExpr.size();j++)
	{
	    if(j==subExpr.size()-1)
	    {
		if(br->FindLeaf(subExpr[j])==NULL)
		    return false;
	    }
	    else
		br=br->FindBranch(subExpr[j]);
	    if(br==NULL)return false;
	}
    }
    return true;
}
void TreeWidget::complicatedDraw(QString expression)
{
    vector<TLeaf*> leaves;
    vector<TBranch*> branches;
    vector<int> branchtype;
    QString exp=expression;
    vector<QString>expressions;
    exp.replace(")"," ");
    exp.replace("("," ");
    exp.replace("+"," ");
    exp.replace("-"," ");
    exp.replace("*"," ");
    exp.replace("/"," ");
    exp.replace(","," ");
    exp.replace(":"," ");
    for(unsigned int i=0;i<mathexpressions.size();i++)
	exp.replace(mathexpressions[i]+"("," (");
    for(unsigned int i=0;i<ownexpressions.size();i++)
	exp.replace(ownexpressions[i]+"("," (");
    while(exp[0]==' ')exp=exp.right(exp.length()-1);
    cout<<expression<<endl<<exp<<endl<<endl;
    while(exp.length()>0)
    {
	expressions.push_back(exp.left(exp.find(" ")));
	exp=exp.right(exp.length()-expressions.back().length());
	if(QString::number(expressions.back().toDouble())==expressions.back())
	    expressions.pop_back();
	bool found=false;
	for(unsigned int i=0;i<expressions.size()-1;i++)
	    if(expressions[i]==expressions.back())
		found=true;
	if(found)expressions.pop_back();
	while(exp[0]==' ')exp=exp.right(exp.length()-1);
    }
    for(unsigned int i=0;i<expressions.size();i++)cout<<expressions[i]<<endl;
    cout<<endl;
    vector<QString> subExpr;
    QString tmp;
    for(unsigned int i=0;i<expressions.size();i++)
    {
	while(!subExpr.empty())subExpr.pop_back();
	if(expressions[i].contains("."))
	{
	    tmp=expressions[i];
	    while(tmp.contains("."))
	    {
		subExpr.push_back(tmp.left(tmp.find(".")));
		tmp=tmp.right(tmp.length()-tmp.find(".")-1);
	    }
	    subExpr.push_back(tmp);
	}
	else
	    subExpr.push_back(expressions[i]);
	for(unsigned int j=0;j<subExpr.size();j++)cout<<subExpr[j].ascii()<<endl;
	cout<<endl;
	TBranch *br=tree->GetBranch(subExpr[0].ascii());
	if(br==NULL)
	{
	    cout<<"Bad expression in TTree::Draw()"<<endl;
	    return;
	}
	if(subExpr.size()==1&&tree->GetLeaf(subExpr[0].ascii())!=NULL)
	{
	    leaves.push_back(tree->GetLeaf(subExpr[0].ascii()));
	    branches.push_back(NULL);
	    branchtype.push_back(-1);
	}
	else
	{
	    branches.push_back(tree->GetBranch(subExpr[0].ascii()));
	    branchtype.push_back(getBranchType(tree->GetBranch(subExpr[0].ascii())));
	    leaves.push_back(NULL);
	}
	for(unsigned int j=1;j<subExpr.size();j++)
	{
	    cout<<"Branch:"<<br->GetName()<<endl;
	    if(j==subExpr.size()-1)
	    {
		if(br->FindLeaf(subExpr[j].ascii())==NULL)
		{
		    cout<<"leaf not found - looking for particle manips"<<endl;
		    bool found=false;
		    for(unsigned int k=0;k<particleManips.size();k++)
			if(subExpr[j]==particleManips[k])
			    found=true;
		    if(found)
		    {
			branches.push_back(tree->GetBranch(subExpr[0].ascii()));
			branchtype.push_back(getBranchType(tree->GetBranch(subExpr[0].ascii())));
			leaves.push_back(NULL);
			cout<<"found expression: "<<subExpr[j]<<" in particleManips"<<endl;
		    }
		    else
		    {
			cout<<"Bad expression in TTree::Draw()"<<endl;
			return;
		    }
		}
		else
		{
		    branches.push_back(NULL);
		    branchtype.push_back(-1);
		    leaves.push_back(br->FindLeaf(subExpr[j].ascii()));
		}
	    }
	    else
		br=br->FindBranch(subExpr[j].ascii());
	    if(br==NULL)
	    {
		cout<<"Bad expression in TTree::Draw()"<<endl;
		return;
	    }
	}
    }
    exp=expression;
    cout<<exp<<endl;
    for(unsigned int i=0;i<expressions.size();i++)
	exp.replace(expressions[i],QString("[")+QString::number(i)+"]");
    cout<<exp<<endl;
    int dim=exp.contains(":")+1;
    if(dim>3)
    {
	cout<<"Too many dimensions to display. No more than 3D-Histograms possible!"<<endl;
	return;
    }
    cout<<"define tree"<<endl;
    TF1 **f=new TF1*[dim];
    TTree tmptree("tmptree","tmptree");
    tmptree.SetDirectory(0);
    Float_t x,y,z;
    cout<<"define functions and branches"<<endl;
    switch(dim)
    {
    case 1:
	f[0]=new TF1("fx",exp.ascii());
	tmptree.Branch("x",&x,"x/F");
	break;
    case 2:
	f[0]=new TF1("fx",exp.left(exp.find(":")).ascii());
	f[1]=new TF1("fy",exp.right(exp.length()-exp.find(":")-1).ascii());
	tmptree.Branch("x",&x,"x/F");
	tmptree.Branch("y",&y,"y/F");
	break;
    case 3:
	f[0]=new TF1("fx",exp.left(exp.find(":")).ascii());
	f[1]=new TF1("fy",exp.mid(exp.find(":")+1, exp.length()-exp.findRev(":")-1-exp.find(":")-1).ascii());
	f[2]=new TF1("fz",exp.right(exp.length()-exp.findRev(":")-1).ascii());
	tmptree.Branch("x",&x,"x/F");
	tmptree.Branch("y",&y,"y/F");
	tmptree.Branch("z",&z,"z/F");
	break;
    }
    vector3D v;
    momentum4D m;
    float t,p,b,pv;
    int id;
    if(!Eparticles::IsInit())Eparticles::init();
    cout<<"fill tree"<<endl;
    QString s;    
    for(int i=0;i<tree->GetEntries();i++)
    {
	for(unsigned int j=0;j<leaves.size();j++)
	{
	    if(leaves[j]!=NULL)
	    {
		leaves[j]->GetBranch()->GetEntry(i);
		for(int k=0;k<dim;k++)
		    f[k]->SetParameter(j,leaves[j]->GetValue());
	    }
	    else
	    {
		branches[j]->GetEntry(i);
		if(branchtype[j]==3)
		{
		    t=branches[j]->FindLeaf("theta")->GetValue();
		    p=branches[j]->FindLeaf("phi")->GetValue();
		    b=branches[j]->FindLeaf("beta")->GetValue();
		    id=(int)branches[j]->FindLeaf("ID")->GetValue();
		    v.setValues(sin(t)*cos(p),sin(t)*sin(p),cos(t));
		    m.setVM(v*b,Eparticles::getMassG(id));
		    s=expressions[j].right(expressions[j].length()-expressions[j].findRev(".")-1);
		    if(s=="pt"||s=="pl"||s=="vl"||s=="vt")
		    {
			pv=0;
			if(s=="pt")
			    pv=sin(t)*branches[j]->FindLeaf("momentum")->GetValue();
			else if(s=="pl")
			    pv=cos(t)*branches[j]->FindLeaf("momentum")->GetValue();
			else if(s=="vt")
			    pv=sin(t)*b;
			else if(s=="vl")
			    pv=cos(t)*b;
			for(int k=0;k<dim;k++)
			    f[k]->SetParameter(j,pv);
		    }
		    else
		    {
			for(int k=0;k<dim;k++)
			    f[k]->SetParameter(j,getParticleValue(m,s));
		    }	
		}
	    }
	}
	switch(dim)
	{
	case 3: z=f[2]->Eval(0);
	case 2: y=f[1]->Eval(0);
	case 1: x=f[0]->Eval(0);
	}
	tmptree.Fill();
    }
    cout<<"done"<<endl<<"define histo"<<endl;
    TH1* fillhisto;
    fillhisto=NULL;
    float minx=0,miny=0,minz=0,maxx=0,maxy=0,maxz=0;
    switch(dim)
    {
    case 3:
	minz=tmptree.GetMinimum("z");
	maxz=tmptree.GetMaximum("z");
	minz=minz*(minz<0?1.1: 0.9);
	maxz=maxz*(maxz>0?1.1: 0.9);
    case 2:
	miny=tmptree.GetMinimum("y");
	maxy=tmptree.GetMaximum("y");
	miny=miny*(miny<0?1.1: 0.9);
	maxy=maxy*(maxy>0?1.1: 0.9);
    case 1:
	minx=tmptree.GetMinimum("x");
	maxx=tmptree.GetMaximum("x");
	minx=minx*(minx<0?1.1: 0.9);
	maxx=maxx*(maxx>0?1.1: 0.9);
    }
    cout<<"draw tree to histo"<<endl;
    int nEntries=0;
    tree->AddFriend(&tmptree,"tmptree");
    switch(dim)
    {
    case 1:
	fillhisto=new TH1F(expression.ascii(),expression.ascii(),binning,minx,maxx);
	nEntries=tree->Draw((QString("tmptree.x>>")+expression).ascii(),getApplyCut(),lineEdit2->text().ascii());
	break;
    case 2:
	fillhisto=new TH2F(expression.ascii(),expression.ascii(),binning2D,minx,maxx,binning2D,miny,maxy);
	nEntries=tree->Draw((QString("tmptree.y:tmptree.x>>")+expression).ascii(),getApplyCut(),lineEdit2->text().ascii());
	break;
    case 3:
	fillhisto=new TH3F(expression.ascii(),expression.ascii(),binning3D,minx,maxx,binning3D,miny,maxy,binning3D,minz,maxz);
	nEntries=tree->Draw((QString("tmptree.z:tmptree.y:tmptree.x>>")+expression).ascii(),getApplyCut(),lineEdit2->text().ascii());
	break;
    }
    tree->RemoveFriend(&tmptree);
    cout<<"done"<<endl;
    textLabel6->setText(QString("Draw: (")+QString::number(nEntries)+")");
    if(fillhisto!=NULL)
	emit treeDrawn(fillhisto);
}
int TreeWidget::getBranchType(TBranch *br)
{
    if(br->FindLeaf("theta")!=NULL&&
       br->FindLeaf("phi")!=NULL&&
       br->FindLeaf("beta")!=NULL&&
       br->FindLeaf("ID")!=NULL)
	return 3;
    else if(br->FindLeaf("det")!=NULL&&
	    br->FindLeaf("el")!=NULL&&
	    br->FindLeaf("qdcR")!=NULL)
	return 1;
    return -1;	
}
float TreeWidget::getParticleValue(const momentum4D &m, const QString &s)
{
    if(s=="mass")
	return m.Mass();
    else if(s=="momentum")
	return m.Momentum().R();
    else if(s=="energy")
	return m.W();
    else if(s=="pt")
	return (m.Momentum()-vector3D(0,0,1)*(m.Momentum()*vector3D(0,0,1))).R();
    else if(s=="pl")
	return (m.Momentum()*vector3D(0,0,1));
    return -1;
}
void TreeWidget::OnBranchDoubleClick(QListViewItem *it)
{
    if(it==NULL)return;
    if(it->firstChild()!=NULL)return;
    int nEntries=0;
    QString expression=it->text(0);
    while(it->parent()!=NULL)
    {
	expression=it->parent()->text(0)+"."+expression;
	it=it->parent();
    }
    nEntries=tree->Draw(expression.ascii(),getApplyCut(),lineEdit2->text().ascii());
    textLabel6->setText(QString("Draw: (")+QString::number(nEntries)+")");
    emit drawingDone();
    drawHistory.push_back(expression);
}
void TreeWidget::OnContextMenuCutsRequest(QListViewItem *it, const QPoint &p)
{
    currentCut=it;
    int nCuts=0;
    QListViewItem *it2=listView2->firstChild();
    while(it2!=NULL)
    {
	if(listView2->isSelected(it2))nCuts++;
	it2=it2->nextSibling();
    }
    QPopupMenu pm(this);
    pm.insertItem("new 1D Cut",this,SLOT(define1DCut()));
    pm.insertItem("new 2D Cut",this,SLOT(define2DCut()));
    if(nCuts>1)
    {
	pm.insertItem("Combine Cuts with OR",this,SLOT(cutOR()));
	pm.insertItem("Combine Cuts with AND",this,SLOT(cutAND()));
    }
    if(it!=NULL)
    {
	pm.insertItem("Invert Cut",this,SLOT(cutNot()));
	pm.insertItem("change Name",this,SLOT(changeCutName()));
	pm.insertItem("change Definition",this,SLOT(changeCutDefinition()));
	pm.insertItem("remove Cut",this,SLOT(removeCut()));
    }
    pm.insertItem("load cuts from file", this,SLOT(loadCuts()));
    pm.insertItem("write cuts to file",this,SLOT(writeCuts()));
    pm.exec(p);
}
void TreeWidget::define1DCut()
{
  QString qs=QStringInputDialog::getLine(this,"new Cut: specify name","");
  if(qs=="")return;
  QString cutName=fenst.line();
  int num=-1;
  for(unsigned int i=0;i<cuts1D.size();i++)
    if(cutName==cuts1D[i].GetName())
      num=i;
  fenst.setCaption("Name already used, give new one:");
  while(num>=0)
    {
      fenst.exec();
      if(fenst.result()!=QDialog::Accepted)return;
      cutName=fenst.line();
      num=-1;
      for(unsigned int i=0;i<cuts1D.size();i++)
	if(cutName==cuts1D[i].GetName())
	  num=i;
    }
  fenst.setCaption("new Cut: specify cut");
  fenst.exec();
  if(fenst.result()!=QDialog::Accepted)return;
  QString cutDef=fenst.line();
  TCut newCut(cutName.ascii(),cutName.ascii());
  newCut=cutDef.ascii();
  cuts1D.push_back(newCut);
  new QListViewItem(listView2,cutName,cutDef);
}
void TreeWidget::define2DCut()
{
}
void TreeWidget::removeCut()
{
    QListViewItem *it=currentCut;
    if(it==NULL)return;
    int num=-1;
    for(unsigned int i=0;i<cuts1D.size();i++)
	if(it->text(0)==cuts1D[i].GetName())
	    num=i;
    if(num<0)
    {
	for(unsigned int i=0;i<cuts2D.size();i++)
	    if(it->text(0)==cuts2D[i].GetName())
		num=i;
	if(num<0)return;
	removeFromVector<TCutG>(cuts2D,cuts2D[num]);
    }
    else
    {
	removeFromVector<TCut>(cuts1D,cuts1D[num]);
    }
    listView2->takeItem(it);
    delete it;
    currentCut=NULL;
}
void TreeWidget::changeCutName()
{
    TObject *obj=getCut(currentCut);
    if(obj==NULL)return;
    QStringInputDialog fenst(this);
    fenst.setCaption("change Cut: specify name");
    fenst.lineEdit->setText(obj->GetName());
    fenst.exec();
    if(fenst.result()!=QDialog::Accepted)return;
    QString cutName=fenst.line();
    ((TNamed*)obj)->SetName(cutName.ascii());
    currentCut->setText(0,cutName);
}
void TreeWidget::changeCutDefinition()
{
    TObject *obj=getCut(currentCut);
    if(obj==NULL)return;
    if(obj->IsA()==TCut::Class())
    {
	QStringInputDialog fenst(this);
	fenst.setCaption("change Cut: specify definition");
	fenst.lineEdit->setText(obj->GetTitle());
	fenst.exec();
	if(fenst.result()!=QDialog::Accepted)return;
	QString cutName=fenst.line();
	(*((TCut*)obj))=cutName.ascii();
	currentCut->setText(1,cutName);
    }
}
void TreeWidget::cutOR()
{
    QString cutName=getNewName("newCut");
    QString definition;
    TCut ret(cutName.ascii());
    ret="";
    int nCuts=0;
    QListViewItem *it=listView2->firstChild();
    while(it!=NULL)
    {
	if(listView2->isSelected(it))
	{
	    definition+=QString("||(")+it->text(0)+")";
	    nCuts++;
	}
	it=it->nextSibling();
    }
    definition=definition.right(definition.length()-2);
    if(nCuts==0)
	return;
    it=listView2->firstChild();
    while(it!=NULL)
    {	
	if(listView2->isSelected(it))
	{
	    TObject *addCut=getCut(it);
	    if(addCut==NULL)continue;
	    if(addCut->IsA()==TCut::Class())
		ret=ret||(*((TCut*)addCut));
	    else
		ret=ret||addCut->GetName();//(*((TCutG*)addCut));
	}
	it=it->nextSibling();
    }
    new QListViewItem(listView2,cutName,definition);
    ret.SetName(cutName);
    cuts1D.push_back(ret);
}
void TreeWidget::cutAND()
{
    QString cutName=getNewName("newCut");
    QString definition;
    TCut ret(cutName.ascii());
    ret="";
    int nCuts=0;
    QListViewItem *it=listView2->firstChild();
    while(it!=NULL)
    {
	if(listView2->isSelected(it))
	{
	    definition+=QString("&&(")+it->text(0)+")";
	    nCuts++;
	}
	it=it->nextSibling();
    }
    definition=definition.right(definition.length()-2);
    if(nCuts==0)
	return;
    it=listView2->firstChild();
    while(it!=NULL)
    {	
	if(listView2->isSelected(it))
	{
	    TObject *addCut=getCut(it);
	    if(addCut==NULL)continue;
	    if(addCut->IsA()==TCut::Class())
		ret=ret&&(*((TCut*)addCut));
	    else
		ret=ret&&addCut->GetName();//(*((TCutG*)addCut));
	}
	it=it->nextSibling();
    }
    new QListViewItem(listView2,cutName,definition);
    ret.SetName(cutName);
    cuts1D.push_back(ret);
}
void TreeWidget::cutNot()
{
    TObject *obj=getCut(currentCut);
    if(obj==NULL)return;
    QString cutName=getNewName(QString("not_")+obj->GetName()+"_");
    if(obj->IsA()==TCut::Class())
    {
	TCut tmp;
	tmp=!(*(TCut*)obj);
	new QListViewItem(listView2,cutName,QString("!(")+obj->GetTitle()+")");
	tmp.SetName(cutName.ascii());
	cuts1D.push_back(tmp);
    }
    else if(obj->IsA()==TCutG::Class())
    {
//	TCutG tmp;
//	tmp.SetName(cutName.ascii());
//	tmp=!(*(TCutG*)obj);
//	new QListViewItem(listView2,cutName,QString("!(")+obj->GetTitle()+")");
//	cuts2D.push_back(tmp);
   }
}
void TreeWidget::loadCuts()
{
    QString s=QFileDialog::getOpenFileName(typeCaseHome.path(), "root files (*.root)", this, 0, "select file with cuts");
    if(!QFile::exists(s))return;
    TFile f(s.ascii(),"READ");
    vector<TObject*> returns;
    vector<TObject*> templates;
    TCut tmp1;
    TCutG tmp2;
    templates.push_back(&tmp1);
    templates.push_back(&tmp2);
    getObjectsFromRootFile(&f,templates,returns);
    for(unsigned int i=0;i<returns.size();i++)
    {
	if(returns[i]->IsA()==TCut::Class())
	{
	    TCut tmp;
	    tmp=*((TCut*)returns[i]);
	    tmp.SetName(returns[i]->GetName());
	    if(cutNameExists(returns[i]->GetName()))
		tmp.SetName(getNewName(returns[i]->GetName()).ascii());
	    cuts1D.push_back(tmp);
	    new QListViewItem(listView2,tmp.GetName(),tmp.GetTitle());
	}
	else
	{
	    TCutG tmp=*(TCutG*)returns[i];
	    if(cutNameExists(returns[i]->GetName()))
		tmp.SetName(getNewName(returns[i]->GetName()).ascii());
	    cuts2D.push_back(tmp);
	    new QListViewItem(listView2,tmp.GetName(),tmp.GetTitle());
	}
    }
    f.Close();
}
void TreeWidget::writeCuts()
{
    QString s=QFileDialog::getSaveFileName(typeCaseHome.path(), "root files (*.root)", this, 0, "Select file name for writing cuts");
    if(s=="")return;
    TFile f(s.ascii(),"RECREATE");
    f.cd();
    for(unsigned int i=0;i<cuts1D.size();i++)
	cuts1D[i].Write();
    for(unsigned int i=0;i<cuts2D.size();i++)
	cuts2D[i].Write();
    f.Close();
}
bool TreeWidget::cutNameExists(QString testName)
{
    bool found=false;
    QListViewItem *it=listView2->firstChild();
    while(it!=NULL)
    {
	if(it->text(0)==testName)
	    found=true;
	it=it->nextSibling();
    }
    return found;
}
QString TreeWidget::getNewName(QString testName)
{
    QString cutName=testName;
    if(!cutNameExists(cutName))return cutName;
    int num=0;
    while(cutNameExists(cutName+QString::number(num)))
	num++;
    return cutName+QString::number(num);
}
TObject *TreeWidget::getCut(QListViewItem *it)
{
    if(it==NULL)return NULL;
    int num=-1;
    for(unsigned int i=0;i<cuts1D.size();i++)
	if(it->text(0)==cuts1D[i].GetName())
	    num=i;
    if(num<0)
    {
	for(unsigned int i=0;i<cuts2D.size();i++)
	    if(it->text(0)==cuts2D[i].GetName())
		num=i;
	if(num<0)return NULL;
	return &cuts2D[num];
    }
    return &cuts1D[num];
}
TCut TreeWidget::getApplyCut()
{
    TCut ret;
    int nCuts=0;
    QListViewItem *it=listView2->firstChild();
    while(it!=NULL)
    {
	if(listView2->isSelected(it))nCuts++;
	it=it->nextSibling();
    }
    if(nCuts==0)
    {
	ret="";
	return ret;
    }
    it=listView2->firstChild();
    while(it!=NULL)
    {	
	if(listView2->isSelected(it))
	{
	    TObject *addCut=getCut(it);
	    if(addCut==NULL)continue;
	    if(addCut->IsA()==TCut::Class())
		ret=ret&&(*((TCut*)addCut));
	    else
		ret=ret&&addCut->GetName();//(*((TCutG*)addCut));
	}
	it=it->nextSibling();
    }
    return ret;	
}
void TreeWidget::setInitParticles()
{
    ParticleMomentumDefinition fenst(this);
    fenst.setParticles(initParticle1, initParticle2);
    fenst.exec();
    if(fenst.result()==QDialog::Accepted)
    {
	initParticle1=fenst.getParticle(0);
	initParticle2=fenst.getParticle(1);
    }
}
