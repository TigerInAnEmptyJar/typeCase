w=new QAReadReactionFromRootWidget(inStack);
w->setID(46);
inStack->addWidget(w,"Read Reaction from Root");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

w=new QAReadFromTadeWidget(inStack);
w->setID(9);
inStack->addWidget(w,"Read from Tade");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

w=new QAReadFromSimpleTreeWidget(inStack);
w->setID(40);
inStack->addWidget(w,"Read from simple tree");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

w=new QAGenerateEventsWidget(inStack);
w->setID(39);
inStack->addWidget(w,"Generate Events");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

w=new QAGenerateEventsWithDecayWidget(inStack);
w->setID(48);
inStack->addWidget(w,"Generate Events with decay");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

w=new QAMultipleTreeInputWidget(inStack);
w->setID(55);
inStack->addWidget(w,"Read Tracks from multiple trees");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

w=new QAHitTreeInputWidget(inStack);
w->setID(60);
inStack->addWidget(w,"Read Hits from Tree");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));

w=new QATrackTreeInputWidget(inStack);
w->setID(62);
inStack->addWidget(w,"Read Tracks from Tree");
connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));
