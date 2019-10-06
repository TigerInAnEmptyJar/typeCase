#include "AKinkSearch.h"
#include "algorithmparameter.h"
#include "logger.h"
void AKinkSearch::process()
{
  point3D vertex(0, 0, 0);
  float maxDistanceLines = 10;
  tmpNum = 0;
  int PID;
  int dPID;
  int pPID;
  int sNum, pNum, dNum;
  int *sID, *pID, *dID;
  int **nP, **nS, **nD;
  sLine3D promptLine, decayLine;
  Vector* PromptParams = new Vector[numPromptDet];
  for (int i = 0; i < numPromptDet; i++)
    PromptParams[i].reDimension(7);
  int numPromptParams = 0;
  // prompt loop
  for (int i2 = 0; i2 < 2; i2++) // switch prompt pixel/cluster
  {
    if (i2 == 0) {
      pNum = numPromptPix;
      pID = definePromptPixIDs;
      nP = numberOfPixels;
    } else {
      pNum = numPromptCl;
      pID = definePromptClIDs;
      nP = numberOfClusters;
    }
    for (int i = 0; i < pNum; i++) // loop over all prompt point IDs
    {
      pPID = pID[i];                           // get prompt point ID
      for (int ii = 0; ii < (*nP)[pPID]; ii++) // loop over all prompt points of that ID
      {
        if (i2 == 0) {
          if (!pixels[pPID][ii]->getShape())
            continue;
        } else {
          if (clusters[pPID][ii]->getNumberOfPixels() <= 0)
            continue;
        }
        promptLine.newLine(vertex, (i2 == 0 ? pixels[pPID][ii]->getShape()->getCenter()
                                            : clusters[pPID][ii]->getCenter()) -
                                       vertex);
        if (!checkLine(promptLine)) // check if detectors just allowed for decay
                                    // tracks lie on the line
          continue;
        tmpPrompt->reset();
        tmpPrompt->setPath(promptLine);
        tmpPrompt->setOrder(false);
        if (i2 == 0)
          tmpPrompt->addPixel(pixels[pPID][ii]);
        else
          for (int l = 0; l < clusters[pPID][ii]->getNumberOfPixels(); l++)
            tmpPrompt->addPixel(&clusters[pPID][ii]->getPixelr(l));
        if (!searchPrompt(tmpPrompt,
                          (i2 == 0 ? (void*)pixels[pPID][ii] : (void*)clusters[pPID][ii]), i2,
                          PromptParams, numPromptParams))
          continue;
        if (tmpPrompt->getNumberOfCalibHits() < minElementsP[i])
          continue;
        // stop loop
        for (int j2 = 0; j2 < 2; j2++) // switch stop pixel/cluster
        {
          if (j2 == 0) {
            sNum = numStopPixels;
            sID = stopPixelIDs;
            nS = numberOfPixels;
          } else {
            sNum = numStopCluster;
            sID = stopClusterIDs;
            nS = numberOfClusters;
          }
          for (int j = 0; j < sNum; j++) // loop over all stop IDs
          {
            PID = sID[j];
            for (int jj = 0; jj < (*nS)[PID]; jj++) // loop over all stop points of that ID
            {
              if (j2 == 0) {
                if (!pixels[PID][jj]->getShape())
                  continue;
              } else {
                if (clusters[PID][jj]->getNumberOfPixels() <= 0)
                  continue;
              }
              // decay loop
              for (int k2 = 0; k2 < 2; k2++) // switch decay pixel/cluster
              {
                if (k2 == 0) {
                  dNum = numDecPix;
                  dID = defineDecPixIDs;
                  nD = numberOfPixels;
                } else {
                  dNum = numDecCl;
                  dID = defineDecClIDs;
                  nD = numberOfClusters;
                }
                for (int k = 0; k < dNum; k++) // loop over all decay IDs
                {
                  dPID = dID[k];
                  for (int kk = 0; kk < (*nD)[dPID]; kk++) // loop over all decay points of that ID
                  {
                    if (k2 == 0) {
                      if (!pixels[dPID][kk]->getShape())
                        continue;
                    } else {
                      if (clusters[dPID][kk]->getNumberOfPixels() <= 0)
                        continue;
                    }
                    decayLine.newLine((k2 == 0 ? pixels[dPID][kk]->getShape()->getCenter()
                                               : clusters[dPID][kk]->getCenter()),
                                      (j2 == 0 ? pixels[PID][jj]->getShape()->getCenter()
                                               : clusters[PID][jj]->getCenter()) -
                                          (k2 == 0 ? pixels[dPID][kk]->getShape()->getCenter()
                                                   : clusters[dPID][kk]->getCenter()));

                    lLine3D poca = decayLine - promptLine;
                    if (poca.length() > maxDistanceLines)
                      continue;
                    vertexFit vFit(2, 100);
                    for (int iii = 0; iii < numPromptParams; iii++)
                      vFit.addPoint(vector3D(PromptParams[iii][7], PromptParams[iii][8],
                                             PromptParams[iii][9]),
                                    vector3D(PromptParams[iii][4], PromptParams[iii][5],
                                             PromptParams[iii][6]),
                                    PromptParams[iii][0], 0);
                    (*tmpTracks[tmpNum]) = *tmpPrompt;
                    tmpTracks[tmpNum + 1]->reset();
                    tmpTracks[tmpNum + 1]->setPath(sLine3D(poca.Q(), decayLine.Direction()));
                    if (i2 == 0)
                      tmpTracks[tmpNum + 1]->addPixel(pixels[PID][ii]);
                    else
                      for (int l = 0; l < clusters[PID][ii]->getNumberOfPixels(); l++)
                        tmpTracks[tmpNum + 1]->addPixel(&clusters[PID][ii]->getPixelr(l));
                    if (j2 == 0)
                      tmpTracks[tmpNum + 1]->addPixel(pixels[dPID][jj]);
                    else
                      for (int l = 0; l < clusters[dPID][jj]->getNumberOfPixels(); l++)
                        tmpTracks[tmpNum + 1]->addPixel(&clusters[dPID][jj]->getPixelr(l));
                    tmpTracks[tmpNum + 1]->setOrder(true);
                    if (!searchTrack(
                            tmpNum + 1, decDetIDs, numDecDet,
                            (i2 == 0 ? (void*)pixels[PID][ii] : (void*)clusters[PID][ii]),
                            (j2 == 0 ? (void*)pixels[dPID][jj] : (void*)clusters[dPID][jj]), i2,
                            j2))
                      continue;
                    if (tmpTracks[tmpNum + 1]->getNumberOfCalibHits() < minElementsD[i])
                      continue;
                    float chi = vFit.fit();
                    if (chi > maxChiKink)
                      continue;
                    tmpTracks[tmpNum]->setPath(sLine3D(vertex, vFit.Line(0)));
                    tmpTracks[tmpNum + 1]->setPath(sLine3D(vFit.Vertex(), vFit.Line(1)));
                    tmpTracks[tmpNum]->setChiSquared(chi);
                    tmpTracks[tmpNum + 1]->setChiSquared(chi);
                    tmpTracks[tmpNum]->setCharge(1);
                    tmpTracks[tmpNum + 1]->setCharge(1);
                    if (tmpNum + 2 <= maxTrack)
                      tmpNum = tmpNum + 2;
                    else
                      trackLog << "too many tracks found in Kink search" << endli;
                  } // loop over all decay points of that ID
                }   // loop over all decay IDs
              }     // switch decay pixel/cluster
            }       // loop over all stop points of that ID
          }         // loop over all stop IDs
        }           // switch stop pixel/cluster
      }             // loop over all prompt points of that ID
    }               // loop over all prompt point IDs
  }                 // switch prompt pixel/cluster

  delete[] PromptParams;
  //  return ptr;
  //   for(int i2=0;i2<2;i2++)
  //     {
  //       if(i2==0)
  // 	{sNum=numStopPixels;sID=stopPixelIDs;nS=numberOfPixels;}
  //       else
  // 	{sNum=numStopCluster;sID=stopClusterIDs;nS=numberOfClusters;}
  //       for(int i=0;i<sNum;i++)
  // 	{
  // 	  PID=sID[i];
  // 	  for(int ii=0;ii<(*nS)[PID];ii++)
  // 	    {
  // 	      if(i2==0)
  // 		{if(!pixels[PID][ii]->getShape())continue;}
  // 	      else
  // 		{if(clusters[PID][ii]->getNumberOfPixels()<=0)continue;}
  // 	      for(int j2=0;j2<2;j2++)
  // 		{
  // 		  if(j2==0)
  // 		    {dNum=numDecPix;dID=defineDecPixIDs;nD=numberOfPixels;}
  // 		  else
  // 		    {dNum=numDecCl;dID=defineDecClIDs;nD=numberOfClusters;}
  // 		  for(int j=0;j<dNum;j++)
  // 		    {
  // 		      dPID=dID[j];
  // 		      for(int jj=0;jj<(*nD)[dPID];jj++)
  // 			{
  // 			  if(j2==0)
  // 			    {if(!pixels[dPID][jj]->getShape())continue;}
  // 			  else
  // 			    {if(clusters[dPID][jj]->getNumberOfPixels()<=0)continue;}
  // 			  switch(i2+2*j2)
  // 			    {
  // 			    case 0:
  // decayLine=sLine3D(pixels[dPID][jj]->getShape()->getCenter(),pixels[PID][ii]->getShape()->getCenter()-pixels[dPID][jj]->getShape()->getCenter());
  // 			      break;
  // 			    case 1:
  // decayLine=sLine3D(pixels[dPID][jj]->getShape()->getCenter(),clusters[PID][ii]->getCenter()-pixels[dPID][jj]->getShape()->getCenter());
  // 			      break;
  // 			    case 2:
  // decayLine=sLine3D(clusters[dPID][jj]->getCenter(),pixels[PID][ii]->getShape()->getCenter()-clusters[dPID][jj]->getCenter());
  // 			      break;
  // 			    case 3:
  // decayLine=sLine3D(clusters[dPID][jj]->getCenter(),clusters[PID][ii]->getCenter()-clusters[dPID][jj]->getCenter());
  // 			      break;
  // 			    }
  // 			  for(int k2=0;k2<2;k2++)
  // 			    {
  // 			      if(k2==0)
  // 				{pNum=numPromptPix;pID=definePromptPixIDs;nP=numberOfPixels;}
  // 			      else
  // 				{pNum=numPromptCl;pID=definePromptClIDs;nP=numberOfClusters;}
  // 			      for(int k=0;k<pNum;k++)
  // 				{
  // 				  pPID=dID[k];
  // 				  for(int kk=0;kk<(*nP)[pPID];kk++)
  // 				    {
  // 				      if(k2==0)
  // 					{
  // 					  if(!pixels[pPID][kk]->getShape())continue;
  // 					  promptLine=sLine3D(vertex,pixels[pPID][kk]->getShape()->getCenter()-vertex);
  // 					}
  // 				      else
  // 					{
  // 					  if(clusters[pPID][kk]->getNumberOfPixels()<=0)continue;
  // 					  promptLine=sLine3D(vertex,clusters[pPID][kk]->getCenter()-vertex);
  // 					}

  // 				      lLine3D poca=decayLine-promptLine;
  // 				      if(poca.length()>maxDistanceLines)continue;
  // 				      vFit->clear();
  // 				      tmpTracks[tmpNum]->reset();
  // 				      tmpTracks[tmpNum]->setPath(promptLine);
  // 				      tmpTracks[tmpNum]->setOrder(false);
  // 				      if(k2==0)
  // 					tmpTracks[tmpNum]->addPixel(pixels[pPID][kk]);
  // 				      else
  // 					for(int
  // l=0;l<clusters[pPID][kk]->getNumberOfPixels();l++)
  // 					  tmpTracks[tmpNum]->addPixel(&clusters[pPID][kk]->getPixelr(l));
  // 				      if(!searchTrack(tmpNum,promptDetIDs,numPromptDet,(k2==0?(void*)pixels[pPID][kk]:(void*)clusters[pPID][kk]),NULL,k2,-1))continue;
  // 				      if(tmpTracks[tmpNum]->getNumberOfCalibHits()<minElementsP[k])continue;
  // 				      tmpTracks[tmpNum+1]->reset();
  // 				      tmpTracks[tmpNum+1]->setPath(sLine3D(poca.Q(),promptLine.Direction()));
  // 				      if(i2==0)
  // 					tmpTracks[tmpNum+1]->addPixel(pixels[PID][ii]);
  // 				      else
  // 					for(int
  // l=0;l<clusters[PID][ii]->getNumberOfPixels();l++)
  // 					  tmpTracks[tmpNum+1]->addPixel(&clusters[PID][ii]->getPixelr(l));
  // 				      if(j2==0)
  // 					tmpTracks[tmpNum+1]->addPixel(pixels[dPID][jj]);
  // 				      else
  // 					for(int
  // l=0;l<clusters[dPID][jj]->getNumberOfPixels();l++)
  // 					  tmpTracks[tmpNum+1]->addPixel(&clusters[dPID][jj]->getPixelr(l));

  // 				      tmpTracks[tmpNum+1]->setOrder(true);
  // 				      if(!searchTrack(tmpNum+1,decDetIDs,numDecDet,(i2==0?(void*)pixels[PID][ii]:(void*)clusters[PID][ii]),(j2==0?(void*)pixels[dPID][jj]:(void*)clusters[dPID][jj]),i2,j2))continue;
  // 				      if(tmpTracks[tmpNum+1]->getNumberOfCalibHits()<minElementsD[i])continue;
  // 				      float chi=vFit->fit();
  // 				      if(chi>maxChiKink)continue;
  // 				      Vector res(vFit->getResult());
  // 				      tmpTracks[tmpNum]->setPath(sLine3D(vertex,vector3D(res.getValue(3),res.getValue(4),res.getValue(5))));
  // 				      tmpTracks[tmpNum+1]->setPath(sLine3D(point3D(res.getValue(0),res.getValue(1),res.getValue(2)),vector3D(res.getValue(6),res.getValue(7),res.getValue(8))));
  // 				      tmpTracks[tmpNum]->setChiSquared(chi);
  // 				      tmpTracks[tmpNum+1]->setChiSquared(chi);
  // 				      tmpTracks[tmpNum]->setCharge(1);
  // 				      tmpTracks[tmpNum+1]->setCharge(1);
  // 				      if(tmpNum+2<=maxTrack)
  // 					tmpNum=tmpNum+2;
  // 				      else
  // 					trackLog<<"too many tracks found in Kink
  // search"<<endli;
  // 				    }
  // 				}
  // 			    }
  // 			}
  // 		    }
  // 		}
  // 	    }
  // 	}
  //     }
  if (tmpNum + numberOfTracks > maxTrack)
    tmpNum = maxTrack - numberOfTracks;
  for (int i = 0; i < tmpNum; i++) {
    (*tracks[numberOfTracks + i]) = (*tmpTracks[i]);
    tracks[numberOfTracks + i]->setOrder(i & 1);
  }
  numberOfTracks = numberOfTracks + tmpNum;
  //   for(int i=0;i<maxTracks;i++)
  //     tmpTracks[i]=trackSave[i];
  //   *numTracks=tmpnum;
}
bool AKinkSearch::searchTrack(int trNum, int* dets, int numDets, void* p1, void* p2, int d1, int d2)
{
  sLine3D path = tmpTracks[trNum]->getPath();
  bool InPixel = false;
  bool falsePixel = false;
  float pR = 0, tR = 0;
  TPixel** tmpPix;
  int numTmpP = 0;
  int elnum;
  if (d1 == 0) {
    numTmpP = 1;
    tmpPix = new TPixel*[numTmpP];
    tmpPix[0] = (TPixel*)p1;
    pR = ((TPixel*)p1)->getShape()->angularRange_Phi(path.Foot());
    tR = ((TPixel*)p1)->getShape()->angularRange_Theta(path.Foot());
  } else if (d1 == 1) {
    float phis[((TCluster*)p1)->getNumberOfPixels()];
    float thetas[((TCluster*)p1)->getNumberOfPixels()];
    float minT = M_PI, maxT = 0, minP = M_PI, maxP = -M_PI;
    //      float pc=((TCluster*)p1)->getCenter().Phi();
    vector3D z(0, 0, 1);
    vector3D project, projc = (((TCluster *)p1)->getCenter() - path.Foot());
    projc = projc - ((projc * z) * z);
    projc.normalize();
    point3D p;
    numTmpP = ((TCluster*)p1)->getNumberOfPixels();
    tmpPix = new TPixel*[numTmpP];
    for (int i = 0; i < numTmpP; i++) {
      tmpPix[i] = &((TCluster*)p1)->getPixelr(i);
      p = tmpPix[i]->getShape()->getCenter();
      thetas[i] = p.Theta();
      project = (p - path.Foot()) - (((p - path.Foot()) * z) * z);
      project.normalize();
      phis[i] = acos(project * projc);
      if ((project ^ projc) * tmpPix[i]->getShape()->getNormal() < 0)
        phis[i] = -phis[i];
      if (thetas[i] < minT)
        minT = thetas[i];
      if (thetas[i] > maxT)
        maxT = thetas[i];
      if (phis[i] < minP)
        minP = phis[i];
      if (phis[i] > maxP)
        maxP = phis[i];
    }
    pR = maxP + minP + tmpPix[0]->getShape()->angularRange_Phi(path.Foot());
    tR = maxT - minT + tmpPix[0]->getShape()->angularRange_Theta(path.Foot());
  } else
    tmpPix = new TPixel*[1];
  TPixel** tmpP2;
  int numTP2 = 0;
  if (d2 == 0) {
    numTP2 = 1;
    float pr = ((TPixel*)p2)->getShape()->angularRange_Phi(path.Foot());
    float tr = ((TPixel*)p2)->getShape()->angularRange_Theta(path.Foot());
    if (pr < pR)
      pR = pr;
    if (tr < tR)
      tR = tr;
    tmpP2 = new TPixel*[numTP2];
    tmpP2[0] = ((TPixel*)p2);
  } else if (d2 == 1) {
    float pr, tr;
    float ph, th;
    float minT = M_PI, maxT = 0, minP = M_PI, maxP = -M_PI;
    //      float pc=((TCluster*)p2)->getCenter().Phi();
    vector3D z(0, 0, 1);
    vector3D project, projc = (((TCluster *)p2)->getCenter() - path.Foot());
    projc = projc - ((projc * z) * z);
    projc.normalize();
    point3D p;
    numTP2 = ((TCluster*)p2)->getNumberOfPixels();
    tmpP2 = new TPixel*[numTP2];
    for (int i = 0; i < numTP2; i++) {
      tmpP2[i] = &((TCluster*)p2)->getPixelr(i);
      p = tmpP2[i]->getShape()->getCenter();
      th = p.Theta();
      project = (p - path.Foot()) - (((p - path.Foot()) * z) * z);
      project.normalize();
      ph = acos(project * projc);
      if ((project ^ projc) * tmpP2[i]->getShape()->getNormal() < 0)
        ph = -ph;
      if (th < minT)
        minT = th;
      if (th > maxT)
        maxT = th;
      if (ph < minP)
        minP = ph;
      if (ph > maxP)
        maxP = ph;
    }
    pr = maxP + minP + tmpP2[0]->getShape()->angularRange_Phi(path.Foot());
    tr = maxT - minT + tmpP2[0]->getShape()->angularRange_Theta(path.Foot());
    if (pr < pR)
      pR = pr;
    if (tr < tR)
      tR = tr;
  } else
    tmpP2 = new TPixel*[1];
  int k;
  int snum = (trNum % 2);
  Vector bestHit(13);
  for (int i = 0; i < numDets; i++) // for all detectors
  {
    k = dets[i];
    if (*numberOfHits[k] <= 0)
      continue;
    InPixel = false;
    for (int j = 0; j < numTmpP; j++) {
      for (int kk = 0; kk < tmpPix[j]->getNumberOfActualElements(); kk++) {
        if (tmpPix[j]->getElementr(kk).getDetector() == k) {
          InPixel = true;
          tmpTracks[trNum]->addCalibSrc(&tmpPix[j]->getElementr(kk));
          bestHit = tmpPix[j]->getElementr(kk).getHitShape()->HitParams(path);
          vector3D dis(bestHit.getValue(10), bestHit.getValue(11), bestHit.getValue(12));
          if (dis.length() > tmpPix[j]->getElementr(kk).getHitShape()->getMaxDistance() * 0.1) {
            delete[] tmpP2;
            delete[] tmpPix;
            return false;
          }
          Vector tmp(7);
          tmp.setValue(0, (bestHit.getValue(4) * bestHit.getValue(7) +
                           bestHit.getValue(5) * bestHit.getValue(8)) /
                              bestHit.getValue(0));
          tmp.setValue(1, bestHit.getValue(4) / bestHit.getValue(0));
          tmp.setValue(2, bestHit.getValue(5) / bestHit.getValue(0));
          tmp.setValue(3, bestHit.getValue(4) * bestHit.getValue(3) / bestHit.getValue(0));
          tmp.setValue(4, bestHit.getValue(5) * bestHit.getValue(3) / bestHit.getValue(0));
          tmp.setValue(5, bestHit.getValue(3));
          tmp.setValue(6, snum);
          vFit.addPoint(vector3D(bestHit[7], bestHit[8], bestHit[9]),
                        vector3D(bestHit[4], bestHit[5], bestHit[6]), bestHit[0], 1);
        }
      }
    }
    if (p2 != NULL) {
      for (int j = 0; j < numTP2; j++) {
        for (int kk = 0; kk < tmpP2[j]->getNumberOfActualElements(); kk++) {
          if (tmpP2[j]->getElementr(kk).getDetector() == k) {
            InPixel = true;
            tmpTracks[trNum]->addCalibSrc(&tmpP2[j]->getElementr(kk));
            bestHit = tmpP2[j]->getElementr(kk).getHitShape()->HitParams(path);
            vector3D dis(bestHit.getValue(10), bestHit.getValue(11), bestHit.getValue(12));
            if (dis.length() > tmpP2[j]->getElementr(kk).getHitShape()->getMaxDistance() * 0.1) {
              delete[] tmpP2;
              delete[] tmpPix;
              return false;
            }
            Vector tmp(7);
            tmp.setValue(0, (bestHit.getValue(4) * bestHit.getValue(7) +
                             bestHit.getValue(5) * bestHit.getValue(8)) /
                                bestHit.getValue(0));
            tmp.setValue(1, bestHit.getValue(4) / bestHit.getValue(0));
            tmp.setValue(2, bestHit.getValue(5) / bestHit.getValue(0));
            tmp.setValue(3, bestHit.getValue(4) * bestHit.getValue(3) / bestHit.getValue(0));
            tmp.setValue(4, bestHit.getValue(5) * bestHit.getValue(3) / bestHit.getValue(0));
            tmp.setValue(5, bestHit.getValue(3));
            tmp.setValue(6, snum);
            vFit.addPoint(vector3D(bestHit[7], bestHit[8], bestHit[9]),
                          vector3D(bestHit[4], bestHit[5], bestHit[6]), bestHit[0], 1);
          }
        }
      }
    }
    if (falsePixel) {
      delete[] tmpP2;
      delete[] tmpPix;
      return false;
    }
    if (InPixel) {
      continue;
    }
    //       if(Iscalo[k])
    // 	{
    // 	  getBestThick(k);
    // 	}
    // 	      else
    {
      elnum = bestElement(tmpTracks[trNum], k, /*maxDistPix[ii]*/ pR,
                          /*maxDistPix[ii]*/ tR,
                          /*completePixel[ii]*/ false, bestHit);
      if ((elnum >= 0) && (elnum < *(numberOfHits[k]))) {
        tmpTracks[trNum]->addCalibSrc(hits[k][elnum]);
        Vector tmp(7);
        tmp.setValue(0, (bestHit.getValue(4) * bestHit.getValue(7) +
                         bestHit.getValue(5) * bestHit.getValue(8)) /
                            bestHit.getValue(0));
        tmp.setValue(1, bestHit.getValue(4) / bestHit.getValue(0));
        tmp.setValue(2, bestHit.getValue(5) / bestHit.getValue(0));
        tmp.setValue(3, bestHit.getValue(4) * bestHit.getValue(3) / bestHit.getValue(0));
        tmp.setValue(4, bestHit.getValue(5) * bestHit.getValue(3) / bestHit.getValue(0));
        tmp.setValue(5, bestHit.getValue(3));
        tmp.setValue(6, snum);
        vFit.addPoint(vector3D(bestHit[7], bestHit[8], bestHit[9]),
                      vector3D(bestHit[4], bestHit[5], bestHit[6]), bestHit[0], 1);
      }
    }
  }
  delete[] tmpP2;
  delete[] tmpPix;
  return true;
}

// copied from ATuTrackSearch
#include "fiber.h"
int AKinkSearch::bestElement(TTrack* tracki, int det, float maxDistP, float maxDistT, bool cp,
                             Vector& bestHit)
{
  /******************************************
   * changed in 12.09.2005 by K.Ehrhardt
   *
   * no absolute maximum distances to track
   * but maximum distances in angles.
   * takes pixel resolution into account.
 ********************************************/
  if (*(numberOfHits[det]) <= 0)
    return -1;
  int elnum = -1;
  vector3D bestDist = vector3D(1000000, 100000000, 100000000);
  vector3D distance(bestDist);
  Vector hitParam(13);
  float detDis;
  sLine3D path(tracki->getPath());
  sLine3D middleLineFiber;
  lLine3D dista;
  vector3D v(path.Direction());
  vector3D z(0, 0, 1);
  vector3D n = v - z * (v * z);
  n.normalize();
  float tphi = acos(n * vector3D(1, 0, 0));
  float tth = acos(v * z);
  volumeShape* shape;
  planeShape* pshape = tracki->getPixel(0, 0).getShape();
  float phiD, thetaD;
  for (int l = 0; l < *(numberOfHits[det]); l++) {
    shape = hits[det][l]->getHitShape();
    if ((shape) && (hits[det][l]->isValid())) {
      detDis = shape->getMaxDistance();
      if (shape->getName() == "fiber") {
        middleLineFiber =
            sLine3D(((fiber*)shape)->getCorner() + ((fiber*)shape)->getDirection(1) * 0.5 +
                        ((fiber*)shape)->getDirection(2) * 0.5,
                    ((fiber*)shape)->getDirection(0));
        dista = middleLineFiber - path;
        if (dista.length() > (((fiber*)shape)->getDirection(1).length() + detDis) * 5)
          continue;
      }
      if (cp)
        hitParam = shape->HitParams(*pshape, path.Foot()); // vertex);//ath);
      else
        hitParam = shape->HitParams(path);
      distance.setValues(hitParam.getValue(10), hitParam.getValue(11), hitParam.getValue(12));
      v = point3D(hitParam.getValue(7), hitParam.getValue(8),
                  hitParam.getValue(9)) -
          path.Foot(); // vertex;
      n = v - z * (v * z);
      v.normalize();
      n.normalize();
      if (distance.length() <= 0.0001) {
        bestHit = hitParam;
        return l;
      }
      phiD = abs(tphi - acos(n * vector3D(1, 0, 0)));
      thetaD = abs(tth - acos(v * z));
      if (phiD >= 2 * M_PI)
        phiD = phiD - 2 * M_PI;
      if (phiD > M_PI)
        phiD = 2 * M_PI - phiD;
      if (thetaD > M_PI) {
        thetaD = thetaD - M_PI;
        phiD = M_PI - phiD;
      }
      if ((phiD < maxDistP) && (thetaD < maxDistT) && (distance.length() < bestDist.length()) &&
          (distance.length() < detDis)) {
        elnum = l;
        bestDist = distance;
        bestHit = hitParam;
      }
    }
  }
  return elnum;
}
bool AKinkSearch::checkLine(const sLine3D& promptLine) // check if detectors
                                                       // just allowed for decay
                                                       // tracks lie on the line
{
  float pr, tr;
  vector3D c;
  for (int i = 0; i < numStopPixels; i++)
    for (int j = 0; j < (*numberOfPixels[stopPixelIDs[i]]); j++) {
      if (!pixels[stopPixelIDs[i]][j]->getShape())
        continue;
      pr = pixels[stopPixelIDs[i]][j]->getShape()->angularRange_Phi();
      tr = pixels[stopPixelIDs[i]][j]->getShape()->angularRange_Theta();
      c = pixels[stopPixelIDs[i]][j]->getShape()->getCenter() - promptLine.Foot();
      if (promptLine.Direction() * c < pr && promptLine.Direction() * c < tr)
        return true;
    }
  for (int i = 0; i < numStopCluster; i++)
    for (int j = 0; j < (*numberOfClusters[stopClusterIDs[i]]); j++) {
      for (int k = 0; k < clusters[stopClusterIDs[i]][j]->getNumberOfPixels(); k++) {
        if (!clusters[stopClusterIDs[i]][j]->getPixelr(k).getShape())
          continue;
        pr = clusters[stopClusterIDs[i]][j]->getPixelr(k).getShape()->angularRange_Phi();
        tr = clusters[stopClusterIDs[i]][j]->getPixelr(k).getShape()->angularRange_Theta();
        c = clusters[stopClusterIDs[i]][j]->getPixelr(k).getShape()->getCenter() -
            promptLine.Foot();
        if (promptLine.Direction() * c < pr && promptLine.Direction() * c < tr)
          return true;
      }
    }
  return false;
}
bool AKinkSearch::searchPrompt(TTrack* tmpPrompt, void* pix, int d1, Vector* PromptParams,
                               int& numPromptP)
{
  numPromptP = 0;
  sLine3D path = tmpPrompt->getPath();
  bool InPixel = false;
  bool falsePixel = false;
  float pR = 0, tR = 0;
  TPixel** tmpPix;
  int numTmpP = 0;
  int elnum;
  if (d1 == 0) {
    numTmpP = 1;
    tmpPix = new TPixel*[numTmpP];
    tmpPix[0] = (TPixel*)pix;
    pR = ((TPixel*)pix)->getShape()->angularRange_Phi(path.Foot());
    tR = ((TPixel*)pix)->getShape()->angularRange_Theta(path.Foot());
  } else if (d1 == 1) {
    float phis[((TCluster*)pix)->getNumberOfPixels()];
    float thetas[((TCluster*)pix)->getNumberOfPixels()];
    float minT = M_PI, maxT = 0, minP = M_PI, maxP = -M_PI;
    //      float pc=((TCluster*)p1)->getCenter().Phi();
    vector3D z(0, 0, 1);
    vector3D project, projc = (((TCluster *)pix)->getCenter() - path.Foot());
    projc = projc - ((projc * z) * z);
    projc.normalize();
    point3D p;
    numTmpP = ((TCluster*)pix)->getNumberOfPixels();
    tmpPix = new TPixel*[numTmpP];
    for (int i = 0; i < numTmpP; i++) {
      tmpPix[i] = &((TCluster*)pix)->getPixelr(i);
      p = tmpPix[i]->getShape()->getCenter();
      thetas[i] = p.Theta();
      project = (p - path.Foot()) - (((p - path.Foot()) * z) * z);
      project.normalize();
      phis[i] = acos(project * projc);
      if ((project ^ projc) * tmpPix[i]->getShape()->getNormal() < 0)
        phis[i] = -phis[i];
      if (thetas[i] < minT)
        minT = thetas[i];
      if (thetas[i] > maxT)
        maxT = thetas[i];
      if (phis[i] < minP)
        minP = phis[i];
      if (phis[i] > maxP)
        maxP = phis[i];
    }
    pR = maxP + minP + tmpPix[0]->getShape()->angularRange_Phi(path.Foot());
    tR = maxT - minT + tmpPix[0]->getShape()->angularRange_Theta(path.Foot());
  } else
    tmpPix = new TPixel*[1];
  int k;
  Vector bestHit(13);
  for (int i = 0; i < numPromptDet; i++) // for all detectors
  {
    k = promptDetIDs[i];
    if (*numberOfHits[k] <= 0)
      continue;
    InPixel = false;
    for (int j = 0; j < numTmpP; j++) {
      for (int kk = 0; kk < tmpPix[j]->getNumberOfActualElements(); kk++) {
        if (tmpPix[j]->getElementr(kk).getDetector() == k) {
          InPixel = true;
          tmpPrompt->addCalibSrc(&tmpPix[j]->getElementr(kk));
          bestHit = tmpPix[j]->getElementr(kk).getHitShape()->HitParams(path);
          vector3D dis(bestHit.getValue(10), bestHit.getValue(11), bestHit.getValue(12));
          if (dis.length() > tmpPix[j]->getElementr(kk).getHitShape()->getMaxDistance() * 0.1) {
            delete[] tmpPix;
            return false;
          }
          Vector tmp(7);
          tmp.setValues((bestHit.getValue(4) * bestHit.getValue(7) +
                         bestHit.getValue(5) * bestHit.getValue(8)) /
                            bestHit.getValue(0),
                        bestHit.getValue(4) / bestHit.getValue(0),
                        bestHit.getValue(5) / bestHit.getValue(0),
                        bestHit.getValue(4) * bestHit.getValue(3) / bestHit.getValue(0),
                        bestHit.getValue(5) * bestHit.getValue(3) / bestHit.getValue(0),
                        bestHit.getValue(3), 0);
          PromptParams[numPromptP] = tmp;
          numPromptP++;
        }
      }
    }
    if (falsePixel) {
      delete[] tmpPix;
      return false;
    }
    if (InPixel) {
      continue;
    }
    //       if(Iscalo[k])
    // 	{
    // 	  getBestThick(k);
    // 	}
    // 	      else
    {
      elnum = bestElement(tmpPrompt, k, /*maxDistPix[ii]*/ pR,
                          /*maxDistPix[ii]*/ tR,
                          /*completePixel[ii]*/ false, bestHit);
      if ((elnum >= 0) && (elnum < *(numberOfHits[k]))) {
        tmpPrompt->addCalibSrc(hits[k][elnum]);
        Vector tmp(7);
        tmp.setValues((bestHit.getValue(4) * bestHit.getValue(7) +
                       bestHit.getValue(5) * bestHit.getValue(8)) /
                          bestHit.getValue(0),
                      bestHit.getValue(4) / bestHit.getValue(0),
                      bestHit.getValue(5) / bestHit.getValue(0),
                      bestHit.getValue(4) * bestHit.getValue(3) / bestHit.getValue(0),
                      bestHit.getValue(5) * bestHit.getValue(3) / bestHit.getValue(0),
                      bestHit.getValue(3), 0);
        PromptParams[numPromptP] = tmp;
        numPromptP++;
      }
    }
  }
  delete[] tmpPix;
  return true;
}

AKinkSearch::AKinkSearch(TTrack** tracksI, int maxTrackI, int& numberOfTracksI, TPixel*** pixelsI,
                         int** numberOfPixelsI, TCluster*** clustersI, int** numberOfClustersI,
                         TCalibHit*** hitsI, int** numberOfHitsI, vector<int> stopPixelIDsI,
                         vector<int> stopClusterIDsI, vector<int> definePromptPixIDsI,
                         vector<int> definePromptClIDsI, vector<int> defineDecPixIDsI,
                         vector<int> defineDecClIDsI, vector<int> promptDetIDsI,
                         vector<int> decDetIDsI, vector<int> interDetIDsI, int minPromPixI,
                         int minDecPixI, vector<int> minElementsPI, vector<int> minElementsDI,
                         float maxChiKinkI)
    : AAlgorithm("Tracks with Kink search "), maxTrack(maxTrackI), numberOfTracks(numberOfTracksI),
      vFit(2, 100)
{
  tracks = tracksI;
  pixels = pixelsI;
  clusters = clustersI;
  hits = hitsI;
  numberOfPixels = numberOfPixelsI;
  numberOfClusters = numberOfClustersI;
  numberOfHits = numberOfHitsI;
  tmpTracks = new TTrack*[maxTrack];
  for (int i = 0; i < maxTrack; i++)
    tmpTracks[i] = new TTrack(tracks[0]->getNumberOfCalibHits(-2), 50);
  tmpPrompt = new TTrack(tracks[0]->getNumberOfCalibHits(-2), 50);
  numStopPixels = stopPixelIDsI.size();
  stopPixelIDs = new int[numStopPixels];
  for (int i = 0; i < numStopPixels; i++) {
    stopPixelIDs[i] = stopPixelIDsI[i];
  }
  numStopCluster = stopClusterIDsI.size();
  stopClusterIDs = new int[numStopCluster];
  for (int i = 0; i < numStopCluster; i++) {
    stopClusterIDs[i] = stopClusterIDsI[i];
  }
  numPromptPix = definePromptPixIDsI.size();
  definePromptPixIDs = new int[numPromptPix];
  for (int i = 0; i < numPromptPix; i++) {
    definePromptPixIDs[i] = definePromptPixIDsI[i];
  }
  numPromptCl = definePromptClIDsI.size();
  definePromptClIDs = new int[numPromptCl];
  for (int i = 0; i < numPromptCl; i++) {
    definePromptClIDs[i] = definePromptClIDsI[i];
  }
  numDecPix = defineDecPixIDsI.size();
  defineDecPixIDs = new int[numDecPix];
  for (int i = 0; i < numDecPix; i++) {
    defineDecPixIDs[i] = defineDecPixIDsI[i];
  }
  numDecCl = defineDecClIDsI.size();
  defineDecClIDs = new int[numDecCl];
  for (int i = 0; i < numDecCl; i++) {
    defineDecClIDs[i] = defineDecClIDsI[i];
  }
  numPromptDet = promptDetIDsI.size();
  promptDetIDs = new int[numPromptDet];
  for (int i = 0; i < numPromptDet; i++)
    promptDetIDs[i] = promptDetIDsI[i];
  numDecDet = decDetIDsI.size();
  decDetIDs = new int[numDecDet];
  for (int i = 0; i < numDecDet; i++)
    decDetIDs[i] = decDetIDsI[i];
  numInterDet = interDetIDsI.size();
  interDetIDs = new int[numInterDet];
  for (int i = 0; i < numInterDet; i++)
    interDetIDs[i] = interDetIDsI[i];
  minPromPix = minPromPixI;
  minDecPix = minDecPixI;
  minElementsP = new int[numPromptPix + numPromptCl];
  for (int i = 0; i < numPromptPix + numPromptCl; i++)
    if ((int)minElementsPI.size() > i)
      minElementsP[i] = minElementsPI[i];
    else
      minElementsP[i] = 0;
  minElementsD = new int[numStopPixels + numStopCluster];
  for (int i = 0; i < numStopPixels + numStopCluster; i++)
    if ((int)minElementsDI.size() > i)
      minElementsD[i] = minElementsDI[i];
    else
      minElementsD[i] = 0;
  maxChiKink = maxChiKinkI;
}
AKinkSearch::~AKinkSearch()
{
  for (int i = 0; i < maxTrack; i++)
    delete tmpTracks[i];
  delete[] tmpTracks;
  delete tmpPrompt;
  delete[] stopPixelIDs;
  delete[] stopClusterIDs;
  delete[] definePromptPixIDs;
  delete[] definePromptClIDs;
  delete[] defineDecPixIDs;
  delete[] defineDecClIDs;
  delete[] promptDetIDs;
  delete[] decDetIDs;
  delete[] interDetIDs;
  delete[] minElementsP;
  delete[] minElementsD;
}
