void SetupWidget::resize( const QSize & s ){resize(s.width(),s.height());}
void SetupWidget::resizeEvent( QResizeEvent * e ){resize(e->size().width(),e->size().height());}
void SetupWidget::resize( int w, int h )
{
  QWidget::resize(w,h);
  tabWidget->resize(w-15,h-25);
}
void SetupWidget::setSetup(vector<detector_parameter> &d,vector<material_parameter> &m,vector<shape_parameter> &s,reaction_parameter &r)
{
  vector<string> tmpm;
  for(unsigned int i=0;i<m.size();i++)tmpm.push_back(m[i].getName());
  detwidget->setDetectors(d);
  detwidget->setDefinedShapes(s);
  detwidget->setMaterials(tmpm);
  shpwidget->setShapes(s);
  matwidget->setMaterials(m);
  rdisplay->setReaction(r);
  rdisplay->setMaterials(tmpm);
}
void SetupWidget::setSetup(vector<detector_parameter*> &d,vector<material_parameter*> &m,vector<shape_parameter*> &s,reaction_parameter &r)
{
  vector<shape_parameter> tmps;
  for(unsigned int i=0;i<s.size();i++)tmps.push_back(*s[i]);
  vector<string> tmpm;
  for(unsigned int i=0;i<m.size();i++)tmpm.push_back(m[i]->getName());
  detwidget->setDetectors(d);
  detwidget->setDefinedShapes(tmps);
  detwidget->setMaterials(tmpm);
  shpwidget->setShapes(s);
  matwidget->setMaterials(m);
  rdisplay->setReaction(r);
  rdisplay->setMaterials(tmpm);
}
void SetupWidget::init(){
}
void SetupWidget::destroy(){}
void SetupWidget::setShapes(vector<shape_parameter> &s){shpwidget->setShapes(s);}
void SetupWidget::setShapes(vector<shape_parameter*> &s){shpwidget->setShapes(s);}
void SetupWidget::setDetectors(vector<detector_parameter> &d){detwidget->setDetectors(d);}
void SetupWidget::setDetectors(vector<detector_parameter*> &d){detwidget->setDetectors(d);}
void SetupWidget::setMaterials(vector<material_parameter> &m){matwidget->setMaterials(m);}
void SetupWidget::setMaterials(vector<material_parameter*> &m){matwidget->setMaterials(m);}
void SetupWidget::setReaction(reaction_parameter &r){rdisplay->setReaction(r);}
void SetupWidget::setReaction(reaction_parameter* r){rdisplay->setReaction(*r);}
