#include "AHitClusterSearch.h"
#include "logger.h"
#include <stdlib.h>
AHitClusterSearch::AHitClusterSearch(TSetup& setupIn, THitCluster*** clustersIn,
                                     int** numberOfCluster, TCalibHit*** hitsIn, int** numberOfHits,
                                     const algorithm_parameter& ap)
    : AAlgorithm("Search hit-Clusters"), setup(setupIn)
{
  auto ids = ap.value(0).value<std::vector<int>>();
  numDets = ids.size();
  hits = new TCalibHit**[numDets];
  clusters = new THitCluster**[numDets];
  nClusters = new int*[numDets];
  nHits = new int*[numDets];
  circular = new bool[numDets];
  nEl = new int[numDets];
  for (unsigned int i = 0; i < ids.size(); i++) {
    anaLog << ids.at(i) << ",";
    hits[i] = hitsIn[ids.at(i)];
    clusters[i] = clustersIn[ids.at(i)];
    nHits[i] = numberOfHits[ids.at(i)];
    nClusters[i] = numberOfCluster[ids.at(i)];
    circular[i] = setup.getDetectorr(ids.at(i)).isCircular();
    nEl[i] = setup.getDetectorr(ids.at(i)).getNumberOfElements();
  }
}
AHitClusterSearch::~AHitClusterSearch()
{
  delete[] nHits;
  delete[] nClusters;
  delete[] clusters;
  delete[] hits;
  delete[] circular;
  delete[] nEl;
}

void AHitClusterSearch::process()
{
  int nCl;
  float weightedSum, sumOfWeights;
  int pos, mean, dEl;
  for (int i = 0; i < numDets; i++) {
    int used[(*(nHits[i]))];
    nCl = 0;
    // cout<<(*(nHits[i])) <<" hits"<<endl;
    for (int j = 0; j < *(nHits[i]); j++) {
      used[j] = -1;
      if (!hits[i][j]->isValid() || hits[i][j]->getHitShape() == NULL)
        used[j] = 1000;
    }
    // find agglomerations
    for (int j = 0; j < *(nHits[i]); j++) {
      if (!hits[i][j]->isValid())
        continue;
      if (hits[i][j]->getHitShape() == NULL)
        continue;
      if (used[j] > -1)
        continue;
      for (int k = 0; k < nCl; k++) {
        for (int l = 0; l < clusters[i][k]->getNumberOfHits(); l++) {
          dEl = (int)abs(hits[i][j]->getElement() - clusters[i][k]->getHit(l)->getElement());
          if (dEl <= 1) {
            clusters[i][k]->addHit(hits[i][j]);
            used[j] = k;
          } else if (circular[i] && abs(dEl - nEl[i] + 1) == 0) {
            clusters[i][k]->addHit(hits[i][j]);
            used[j] = k;
          }
          if (used[j] >= 0)
            break;
        }
        if (used[j] >= 0)
          break;
      }
      if (used[j] >= 0)
        continue;
      used[j] = nCl;
      clusters[i][nCl]->reset();
      (*(clusters[i][nCl])) = *(hits[i][j]);
      clusters[i][nCl]->addHit(hits[i][j]);
      for (int k = j + 1; k < *(nHits[i]); k++) {
        if (used[k] >= 0)
          continue;
        for (int l = 0; l < clusters[i][nCl]->getNumberOfHits(); l++) {
          if (abs(hits[i][k]->getElement() - clusters[i][nCl]->getHit(l)->getElement()) <= 1 &&
              used[k] == -1) {
            clusters[i][nCl]->addHit(hits[i][k]);
            used[k] = nCl;
          } else if (circular[i] &&
                     abs(abs(hits[i][k]->getElement() - clusters[i][nCl]->getHit(l)->getElement()) -
                         nEl[i] + 1) &&
                     used[k] == -1) {
            clusters[i][nCl]->addHit(hits[i][k]);
            used[k] = nCl;
          }
        }
      }
      nCl++;
    }
    // bind agglometations together
    bool obsolete;
    for (int j = 0; j < nCl; j++) {
      for (int k = j + 1; k < nCl; k++) {
        obsolete = false;
        for (int m = 0; m < clusters[i][j]->getNumberOfHits(); m++)
          if (!obsolete)
            for (int n = 0; n < clusters[i][k]->getNumberOfHits(); n++) {
              if (abs(clusters[i][j]->getHit(m)->getElement() -
                      clusters[i][k]->getHit(n)->getElement()) <= 1) {
                obsolete = true;
                for (int l = 0; l < clusters[i][k]->getNumberOfHits(); l++)
                  clusters[i][j]->addHit(clusters[i][k]->getHit(l));
                clusters[i][k]->reset();
              }
            }
      }
    }
    // calculate adc, tdc, hitshape
    for (int j = 0; j < nCl; j++) {
      if (clusters[i][j]->getNumberOfHits() == 0)
        continue;
      weightedSum = 0;
      sumOfWeights = 0;
      for (int k = 0; k < clusters[i][j]->getNumberOfHits(); k++) {
        weightedSum +=
            clusters[i][j]->getHit(k)->getElement() * clusters[i][j]->getHit(k)->getADC();
        sumOfWeights += clusters[i][j]->getHit(k)->getADC();
      }
      if (sumOfWeights != 0) {
        pos = (int)(weightedSum / sumOfWeights + 0.5);
        mean = 0;
        for (int k = 0; k < clusters[i][j]->getNumberOfHits(); k++)
          if (clusters[i][j]->getHit(k)->getElement() == pos)
            mean = k;
      } else {
        pos = clusters[i][j]->getHit(0)->getElement();
        mean = 0;
      }
      clusters[i][j]->getRaw().setElement(pos);
      clusters[i][j]->setADC(sumOfWeights);
      clusters[i][j]->setTDC(clusters[i][j]->getHit(mean)->getTDC());
      clusters[i][j]->setHitShape(clusters[i][j]->getHit(mean)->getHitShape());
    }
    // elliminate bad clusters
    for (int j = 0; j < nCl; j++) {
      if (clusters[i][j]->getNumberOfHits() == 0) {
        for (int k = j; k < nCl - 1; k++)
          (*(clusters[i][k])) = (*(clusters[i][k + 1]));
        nCl--;
      }
    }
    (*(nClusters[i])) = nCl;
    // cout<<nCl<<" hitClusters generated"<<endl;
  }
  // cout<<endl;
}
algorithm_parameter AHitClusterSearch::getDescription()
{
  algorithm_parameter ap("HitCluster search", algorithm_parameter::Category::PIXEL, 0);
  string des = "This algorithm can be used instead of or in addition to "
               "a pixel-cluster-search. Intuitutionally, a cluster search "
               "is done on pixels, but it can also be done prior to pixel "
               "calculation for the hits themselves generating hitCluster "
               "instead of cluster. It searches all elements of one detector, "
               "looking for neighbouring elements (element numbers). The "
               "timing of a hit-cluster and the volumeShape are the one "
               "of the element with the largest adc-value in the bundle; "
               "the adc is added up. Circular detectors are taken into "
               "account.\n"
               "Give the algorithm a vector of detector IDs, containing "
               "the hits that should be morved to clusters. ";
  ap.setDescription(des);
  vector<int> tmp;
  ap.addValue("detector IDs", tmp);
  return ap;
}
