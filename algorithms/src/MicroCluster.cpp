#include "MicroCluster.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include <stdlib.h>
AMicroCluster::AMicroCluster(int IDIn, int& numberOfPixelsIn, TPixel** pixelIn, int maxClusterIn,
                             int& numberOfClustersIn, TCluster** clusterIn, int patternIn)
    : AAlgorithm("Cluster search in Micro strip detector"), ID(IDIn), maxCluster(maxClusterIn),
      numberOfClusters(numberOfClustersIn), numberOfPixels(numberOfPixelsIn)
{
  pixel = pixelIn;
  cluster = clusterIn;
  pattern = patternIn;
  for (int i = 0; i < maxCluster; i++)
    cluster[i]->setID(ID);
}

AMicroCluster::~AMicroCluster() {}

void* AMicroCluster::process(void* ptr)
{
  bool used[numberOfPixels];
  float energy[numberOfPixels];
  int pixelLeft = numberOfPixels;
  // pixelLog<<"begin cluster search: "<<numberOfPixels<<" pixels"<<endli;
  for (int i = 0; i < numberOfPixels; i++) {
    used[i] = false;
    energy[i] = 0;
    for (int j = 0; j < pixel[i]->getNumberOfActualElements(); j++)
      energy[i] += pixel[i]->getElementr(j).getADC();
  }
  bool stillPixelLeft = (numberOfPixels > 0);
  numberOfClusters = 0;
  int maxPixel = -1;
  int n1;
  vector3D mean;
  float normer;
  while (stillPixelLeft) {
    maxPixel = -1;
    for (int i = 0; i < numberOfPixels; i++)
      if (!used[i]) {
        if (maxPixel == -1)
          maxPixel = i;
        else if (energy[i] > energy[maxPixel])
          maxPixel = i;
      }
    // pixelLog<<"max energy pixel: "<<maxPixel<<endli;
    if (maxPixel == -1)
      stillPixelLeft = false;
    else {
      pixelLeft--;
      mean = (pixel[maxPixel]->getShape()->getCenter() - point3D(0, 0, 0)) * energy[maxPixel];
      normer = energy[maxPixel];
      cluster[numberOfClusters]->reset();
      cluster[numberOfClusters]->addPixel(*pixel[maxPixel]);
      used[maxPixel] = true;
      // pixelLog<<"search ring with pattern: "<<pattern;
      pixelLeft -= searchRing(cluster[numberOfClusters], normer, mean, used, energy,
                              (pattern & 1 ? 1 : (pattern & 2 ? 2 : 0)), maxPixel);
      if (pattern & 4) {
        for (int i = 1; i < cluster[numberOfClusters]->getNumberOfPixels(); i++) {
          if (pixelLeft <= 0)
            break;
          n1 = -1;
          for (int j = 0; j < numberOfPixels; j++)
            if (&cluster[numberOfClusters]->getPixelr(i) == pixel[j])
              n1 = j;
          if (n1 >= 0)
            pixelLeft -= searchRing(cluster[numberOfClusters], normer, mean, used, energy,
                                    (pattern & 1 ? 1 : (pattern & 2 ? 2 : 0)), n1);
        }
      }
      mean = mean * (1 / normer);
      // pixelLog<<" "<<pixelLeft<<" left; center point of cluster:
      // "<<mean<<endli;
      cluster[numberOfClusters]->setCenter(point3D(0, 0, 0) + mean);
      //	  cout<<ID<<"
      //"<<cluster[numberOfClusters]->getPixelr(0).getID()<<cluster[numberOfClusters]->getCenter().Z()<<"
      //"<<cluster[numberOfClusters]->getPixelr(0).getShape()->getCenter().Z()<<"
      //"<<cluster[numberOfClusters]->getPixelr(0).getElement(0).getHitShape()->getCenter().Z()<<endl;
      cluster[numberOfClusters]->calculateClusterRadius();
      if (numberOfClusters < maxCluster)
        numberOfClusters++;
    }
  }
  // pixelLog<<numberOfClusters<<" clusters found"<<endli;
  return ptr;
}

/*searchRing searches in all pixels the ones which are within an element
 range (size) of the centerpixel. It returns the number of pixels added
 to the cluster.*/
int AMicroCluster::searchRing(TCluster* clust, float& normer_f, vector3D& mean_f, bool* used_f,
                              float* energy_f, int size, int centerP)
{
  bool take;
  int n1, n2;
  float d = 0;
  int ret = 0;
  for (int i = 0; i < numberOfPixels; i++) {
    take = true;
    d = 0;
    if ((!used_f[i]))
      for (int j = 0; j < pixel[i]->getNumberOfActualElements(); j++)
        for (int k = 0; k < pixel[centerP]->getNumberOfActualElements(); k++)
          if (pixel[i]->getElementr(j).getDetector() ==
              pixel[centerP]->getElementr(k).getDetector()) {
            n1 = pixel[centerP]->getElementr(k).getElement();
            n2 = pixel[i]->getElementr(j).getElement();
            d = d + abs(n1 - n2) * abs(n1 - n2);
            if (!(n1 == n2 || (abs(n1 - n2) <= size)))
              take = false;
          }
    if (take && (!used_f[i]) && clust->getMaxNumberOfPixels() > clust->getNumberOfPixels()) {
      ret++;
      clust->addPixel(*pixel[i]);
      used_f[i] = true;
      normer_f = normer_f + energy_f[i];
      mean_f = mean_f + ((pixel[i]->getShape()->getCenter() - point3D(0, 0, 0)) * energy_f[i]);
    }
  }
  return ret;
}
// void AMicroCluster::Streamer(TBuffer &b)
//{
//}
