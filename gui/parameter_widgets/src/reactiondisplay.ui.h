#include "utilities.h"
bool ReactionDisplay::readOnly(){return ReadOnly;}
bool ReactionDisplay::doDraw(){return DrawShape;}
void ReactionDisplay::setDraw(bool value){DrawShape=value;resize(width(),height());}
extern int getShapeHeight(const shape_parameter &sh);

void ReactionDisplay::setReaction( reaction_parameter & reactionIn )
{
  sp=&reactionIn;
  copy=reactionIn;
  copyshape=copy.getTargetShape();
  if(beamInput!=NULL)
    {
      if(ReadOnly)
	((QLabel*)beamInput)->setText(string_number(copy.getBeamMomentum(0)).data());
      else
	((QLineEdit*)beamInput)->setText(string_number(copy.getBeamMomentum(0)).data());
    }
  twoBeam->setChecked(copy.hasTwoBeams());
  if(targetInput!=NULL)
    {
      targetInput->setEnabled(copy.hasTwoBeams());
      if(ReadOnly)
	{
	  if(copy.hasTwoBeams())
	    ((QLabel*)targetInput)->setText(string_number(copy.getBeamMomentum(1)).data());
	  else
	    ((QLabel*)targetInput)->setText("0");
	}
      else
	{
	  if(copy.hasTwoBeams())
	    ((QLineEdit*)targetInput)->setText(string_number(copy.getBeamMomentum(1)).data());
	  else
	    ((QLineEdit*)targetInput)->setText("0");
	}
      targetInput->setEnabled(copy.hasTwoBeams());
    }
  if(ReadOnly)
    {
/*       if(copy.getMaterial(0)>=0&&copy.getMaterial(0)<(int)materials.size()) */
/* 	((QLabel*)matBeam)->setText(materials[copy.getMaterial(0)].data()); */
/*       if(copy.getMaterial(1)>=0&&copy.getMaterial(1)<(int)materials.size()) */
/* 	((QLabel*)matTarget)->setText(materials[copy.getMaterial(1)].data()); */
    }
  else
    {
      setComboBoxCurrentItem(((QComboBox*)matBeam),copy.getMaterial(0));
      setComboBoxCurrentItem(((QComboBox*)matTarget),copy.getMaterial(1));
    }
  shape->setShape(copyshape);
  resize(width(),height());
}
extern base_shape* getShape(const shape_parameter& param);
#include "volumeShape.h"
void ReactionDisplay::OnDraw()
{
  if(drawer!=NULL)
    {
      delete drawer;
      drawer=NULL;
    }
  drawer=getShape(copyshape);
  if(drawer==NULL)return;
  vector4D bb;
  shapeDraw->GetCanvas()->cd()->Clear();
  drawer->Draw(point3D(0,0,-100),plane3D(point3D(0,0,10),vector3D(0,0,1)),&bb);
  float dx,dy,mx,my;
  dx=bb[2]-bb[0];
  dy=bb[3]-bb[1];
  mx=bb[0]+dx/2.;
  my=bb[1]+dy/2.;
  shapeDraw->GetCanvas()->Range(mx-dx*0.6,my-dy*0.6,mx+dx*.6,my+dy*.6);
  shapeDraw->Refresh();
}
void ReactionDisplay::setReadOnly( bool value )
{
  if(value==ReadOnly)return;
  ReadOnly=value;
  shape->setReadOnly(value);
/*   if(matBeam!=NULL){myLayout->remove(matBeam);delete matBeam;} */
/*   if(matTarget!=NULL){myLayout->remove(matTarget);delete matTarget;} */
/*   if(ReadOnly) */
/*     { */
/*       matBeam=new QLabel(this); */
/*       if(copy.getMaterial(0)>=0&&copy.getMaterial(0)<(int)materials.size()) */
/* 	((QLabel*)matBeam)->setText(materials[copy.getMaterial(0)].data()); */
/*       matTarget=new QLabel(this); */
/*       if(copy.getMaterial(1)>=0&&copy.getMaterial(1)<(int)materials.size()) */
/* 	((QLabel*)matTarget)->setText(materials[copy.getMaterial(1)].data()); */
/*     } */
/*   else */
/*     { */
/*       matBeam=new QComboBox(this); */
/*       matTarget=new QComboBox(this); */
/*       ((QComboBox*)matBeam)->clear(); */
/*       ((QComboBox*)matTarget)->clear(); */
/*       for(unsigned int i=0;i<materials.size();i++) */
/* 	{ */
/* 	  add2ComboBox((QComboBox*)matBeam,materials[i].data()); */
/* 	  add2ComboBox((QComboBox*)matTarget,materials[i].data()); */
/* 	} */
/*       setComboBoxCurrentItem(((QComboBox*)matTarget),copy.getMaterial(1)); */
/*       setComboBoxCurrentItem(((QComboBox*)matBeam),copy.getMaterial(0)); */
/*       connect( matTarget,SIGNAL(highlighted(int)),this,SLOT(onPropertyChange())); */
/*       connect( matBeam,SIGNAL(highlighted(int)),this,SLOT(onPropertyChange())); */
/*     } */
/* /\*   matBeam->setGeometry(60+5,1,width()/2-(60+6),23); *\/ */
/*   matBeam->show(); */
/* /\*   matTarget->setGeometry(60+5,1,width()/2-(60+6),23); *\/ */
/*   matTarget->show(); */
/*   myLayout->addWidget(matBeam,1,0); */
/*   myLayout->addWidget(matBeam,5,0); */
  matTarget->setEnabled(!ReadOnly);
  matBeam->setEnabled(!ReadOnly);
  setComboBoxCurrentItem(((QComboBox*)matTarget),copy.getMaterial(1)); 
  setComboBoxCurrentItem(((QComboBox*)matBeam),copy.getMaterial(0)); 
  beamInput->setEnabled(!ReadOnly);
  targetInput->setEnabled(!ReadOnly&&copy.hasTwoBeams());
  twoBeam->setEnabled(!ReadOnly);
  resize(width(),height());
}
void ReactionDisplay::onPropertyChange()
{
  QObject *tmp=focusWidget();
  if(tmp==NULL)return;
  if(tmp->parent()==NULL)return;
  while(tmp->parent()!=NULL&&tmp->parent()!=this)tmp=tmp->parent();
  if(tmp->parent()==NULL)return;
  if(tmp!=NULL)
    {
      if(tmp==matBeam)
	{
 	  if(!ReadOnly) 
/* 	    { */
/* 	      if(copy.getMaterial(0)>=0&&copy.getMaterial(0)<(int)materials.size()) */
/* 		((QLabel*)matBeam)->setText(materials[copy.getMaterial(0)].data()); */
/* 	    } */
/* 	  else */
	    copy.setMaterial(0,comboBoxCurrentItem((QComboBox*)matBeam));
	}
      else if(tmp==matTarget)
	{
	  if(!ReadOnly)
/* 	    { */
/* 	      if(copy.getMaterial(1)>=0&&copy.getMaterial(1)<(int)materials.size()) */
/* 		((QLabel*)matTarget)->setText(materials[copy.getMaterial(1)].data()); */
/* 	    } */
/* 	  else */
	    copy.setMaterial(1,comboBoxCurrentItem((QComboBox*)matTarget));
	}
      else if(tmp==beamInput)
	{
	  if(ReadOnly)
	    beamInput->setText(string_number(copy.getBeamMomentum(0)).data());
	  else
	    copy.setBeamMomentum(beamInput->text().toFloat(),0);
	}
      else if(tmp==targetInput)
	{
	  if(ReadOnly)
	    targetInput->setText(string_number(copy.getBeamMomentum(1)).data());
	  else
	    copy.setBeamMomentum(targetInput->text().toFloat(),1);
	}
      else if(tmp==twoBeam)
	{
	  if(ReadOnly)
	    twoBeam->setChecked(copy.hasTwoBeams());
	  else
	    {
	      copy.setTwoBeams(twoBeam->isChecked());
	      if(targetInput!=NULL)targetInput->setEnabled(twoBeam->isChecked());
	    }
	}
    }
  if(!ReadOnly)
    {
      copy.setTargetShape(copyshape);
      (*sp)=copy;
      emit changed();
    }
}
void ReactionDisplay::onBeamNumberChange()
{
  if(ReadOnly)
    twoBeam->setChecked(copy.hasTwoBeams());
  else
    {
      copy.setTwoBeams(twoBeam->isChecked());
      if(targetInput!=NULL)targetInput->setEnabled(twoBeam->isChecked());
      copy.setTargetShape(copyshape);
      if(sp!=NULL)(*sp)=copy;
      emit changed();
    }
}
void ReactionDisplay::init()
{
  drawer=NULL;
  sp=NULL;
  shape->setShape(copyshape);
  shape->show();
}
void ReactionDisplay::destroy()
{
  if(drawer!=NULL)
    {
      delete drawer;
      drawer=NULL;
    }
}
void ReactionDisplay::setMaterials(const vector<string>& mats)
{
  while(!materials.empty())materials.pop_back();
  for(unsigned int i=0;i<mats.size();i++)
    materials.push_back(mats[i]);
/*   if(ReadOnly) */
/*     { */
/*       if(copy.getMaterial(0)>=0&&copy.getMaterial(0)<(int)materials.size()) */
/* 	((QLabel*)matBeam)->setText(materials[copy.getMaterial(0)].data()); */
/*       if(copy.getMaterial(1)>=0&&copy.getMaterial(1)<(int)materials.size()) */
/* 	((QLabel*)matTarget)->setText(materials[copy.getMaterial(1)].data()); */
/*     } */
/*   else */
/*     { */
/*       if(matBeam!=NULL) */
/* 	{ */
	  ((QComboBox*)matBeam)->clear();
	  for(unsigned int i=0;i<mats.size();i++)
	    add2ComboBox((QComboBox*)matBeam,mats[i].data());
	  setComboBoxCurrentItem(((QComboBox*)matBeam),copy.getMaterial(0));
/* 	} */
/*       if(matTarget!=NULL) */
/* 	{ */
	  ((QComboBox*)matTarget)->clear();
	  for(unsigned int i=0;i<mats.size();i++)
	    add2ComboBox((QComboBox*)matTarget,mats[i].data());
	  setComboBoxCurrentItem(((QComboBox*)matTarget),copy.getMaterial(1));
/* 	} */
/*     } */
}
