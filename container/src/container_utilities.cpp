#include "geometry.h"
#include "container_utilities.h"
#include "track.h"
#include "shape_utilities.h"
vector3D getPath(int nstart, int *start, int nstop, int *stop, TTrack* tr, const sLine3D &path)
{
  if(start==NULL||stop==NULL)return vector3D();
  if(nstart==0||nstop==0)return vector3D();
  if(tr==NULL)return vector3D();
  int ns=0,nt=0;
  for(int i=0;i<nstart;i++)
    ns+=tr->getNumberOfCalibHits(start[i]);
  for(int i=0;i<nstop;i++)
    nt+=tr->getNumberOfCalibHits(stop[i]);
  TCalibHit *hs[ns],*ht[nt];
  ns=0;nt=0;
  for(int i=0;i<nstart;i++)
    for(int j=0;j<tr->getNumberOfCalibHits(start[i]);j++)
      if(tr->getCalibSrcr(start[i],j)->getHitShape()!=NULL)
	hs[ns++]=tr->getCalibSrcr(start[i],j);
  for(int i=0;i<nstop;i++)
    for(int j=0;j<tr->getNumberOfCalibHits(stop[i]);j++)
      if(tr->getCalibSrcr(stop[i],j)->getHitShape()!=NULL)
	ht[nt++]=tr->getCalibSrcr(stop[i],j);
  return getPath(ns,hs,nt,ht,path);
//   vector3D sa,so;
//   sa.setValues(0,0,0);so.setValues(0,0,0);
//   vector3D tmp;
//   for(int i=0;i<nstart;i++)
//     {
//       if(tr->getNumberOfCalibHits(start[i])==0)continue;
//       for(int j=0;j<tr->getNumberOfCalibHits(start[i]);j++)
// 	{
// 	  if(tr->getCalibSrcr(start[i],j)->getHitShape()==NULL)continue;
// 	  tmp=getEntrance(tr->getCalibSrcr(start[i],j)->getHitShape(),path);
// 	  if(tmp.getState()!=_undefined_)
// 	    {
// 	      ns++;
// 	      sa=sa+tmp;
// 	    }
// 	}
//     }
//   if(ns==0)return vector3D();
//   for(int i=0;i<nstop;i++)
//     {
//       if(tr->getNumberOfCalibHits(stop[i])==0)continue;
//       for(int j=0;j<tr->getNumberOfCalibHits(stop[i]);j++)
// 	{
// 	  if(tr->getCalibSrcr(stop[i],j)->getHitShape()==NULL)continue;
// 	  tmp=getEntrance(tr->getCalibSrcr(stop[i],j)->getHitShape(),path);
// 	  if(tmp.getState()!=_undefined_)
// 	    {
// 	      nt++;
// 	      so=so+tmp;
// 	    }
// 	}
//     }
//   if(nt==0)return vector3D();
//   sa=sa*(1./ns);
//   so=so*(1./nt);
//   return so-sa;
}
vector3D getPath(int nstart, int *start, int nstop, int *stop, int nhits, TCalibHit** hits, const sLine3D &path)
{
  if(start==NULL||stop==NULL)return vector3D();
  if(nstart==0||nstop==0)return vector3D();
  if(nhits==0)return vector3D();
  if(hits==NULL)return vector3D();
  int ns=0,nt=0;
  for(int i=0;i<nstart;i++)
    for(int j=0;j<nhits;j++)
      if(start[i]==hits[j]->getDetector()&&hits[j]->getHitShape()!=NULL)ns++;
  for(int i=0;i<nstop;i++)
    for(int j=0;j<nhits;j++)
      if(stop[i]==hits[j]->getDetector()&&hits[j]->getHitShape()!=NULL)nt++;
  TCalibHit *hs[ns],*ht[nt];
  ns=0;nt=0;
  for(int i=0;i<nstart;i++)
    for(int j=0;j<nhits;j++)
      if(start[i]==hits[j]->getDetector()&&hits[j]->getHitShape()!=NULL)
	hs[ns++]=hits[j];
  for(int i=0;i<nstop;i++)
    for(int j=0;j<nhits;j++)
      if(stop[i]==hits[j]->getDetector()&&hits[j]->getHitShape()!=NULL)
	ht[nt++]=hits[j];
  return getPath(ns,hs,nt,ht,path);

//   vector3D sa,so;
//   sa.setValues(0,0,0);so.setValues(0,0,0);
//   vector3D tmp;
//   for(int j=0;j<nhits;j++)
//     {
//       if(hits[j]==NULL)continue;
//       if(hits[j]->getHitShape()==NULL)continue;
//       for(int i=0;i<nstart;i++)
// 	{
// 	  if(hits[j]->getDetector()==start[i])
// 	    {
// 	      tmp=getEntrance(hits[j]->getHitShape(),path);
// 	      if(tmp.getState()!=_undefined_)
// 		{
// 		  ns++;
// 		  sa=sa+tmp;
// 		}
// 	      break;
// 	    }
// 	}
//       for(int i=0;i<nstop;i++)
// 	{
// 	  if(hits[i]->getDetector()==stop[i])
// 	    {
// 	      tmp=getEntrance(hits[j]->getHitShape(),path);
// 	      if(tmp.getState()!=_undefined_)
// 		{
// 		  nt++;
// 		  so=so+tmp;
// 		}
// 	      break;
// 	    }
// 	}
//     }
//   if(ns==0)return vector3D();
//   if(nt==0)return vector3D();
//   sa=sa*(1./ns);
//   so=so*(1./nt);
//   return so-sa;
}
vector3D getPath(int nstart, TCalibHit **start, int nstop, TCalibHit** stop, const sLine3D &path)
{
  if(start==NULL||stop==NULL)return vector3D();
  if(nstart==0||nstop==0)return vector3D();
  int ns=1,nt=1;
  vector3D sa,so;
  sa.setValues(0,0,1e20);so.setValues(0,0,1e20);
  vector3D tmp;
  for(int i=0;i<nstart;i++)
    {
      if(start[i]==NULL)continue;
      if(start[i]->getHitShape()==NULL)continue;
      tmp=getEntrance(start[i]->getHitShape(),path);
      if(tmp.getState()!=_undefined_)
	{
	  if(tmp.Z()<sa.Z())sa=tmp;
 	  ns++;
// 	  sa=sa+tmp;
	}
    }
  if(ns==0)return vector3D();
  for(int i=0;i<nstop;i++)
    {
      if(stop[i]==NULL)continue;
      if(stop[i]->getHitShape()==NULL)continue;
      tmp=getEntrance(stop[i]->getHitShape(),path);
      if(tmp.getState()!=_undefined_)
	{
	  if(tmp.Z()<so.Z())so=tmp;
 	  nt++;
// 	  so=so+tmp;
	}
    }
  if(nt==0)return vector3D();
//   sa=sa*(1./ns);
//   so=so*(1./nt);
  return so-sa;
}
