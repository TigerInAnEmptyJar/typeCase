w=new QAGenerateOutputWidget(outStack);
w->setID(34);
outStack->addWidget(w,"Write to Simple Tree");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

w=new QAWriteHistosToRootFileWidget(outStack);
w->setID(25);
outStack->addWidget(w,"Write Histograms to ROOT-file");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

w=new QAHitTreeOutputWidget(outStack);
w->setID(61);
outStack->addWidget(w,"Write Hits to hit-tree-ROOT-file");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

//w=new QAWriteHistosToRootFileWidget(outStack);
//w->setID(63);
//outStack->addWidget(w,"Write Tracks to track-tree-ROOT-file");
//connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));
