#include "Rpp_pkl_pkppi.h"
// #include <TH1.h>
// #include <TCanvas.h>
#include "RtwoParticleDecay.h"
#include "fittingRoutines.h"
// TH1F breakReason("break reason", "break reason",100,-1,20);
// TH1F testH("secBestMass","secBestMass",200,-0.5,0.5);
// #include <TFile.h>
// #include <TTree.h>
// TTree *outTree_pkl=NULL;
// TTree *outTree_general_pkl=NULL;
//#define GENERAL_PKL
#ifdef GENERAL_PKL
//#define MOMENTA_USED
typedef struct
{
  Double_t p[6];
  Double_t e[6];
  Double_t m[6];
  Double_t x_c[5];
  Double_t y_c[5];
  Double_t z_c[5];
  Double_t tr[5];
  Double_t lg[5];
  Double_t th[5];
  Double_t ph[5];
  Double_t b[5];
} track_out_general;
track_out_general pro_general_pkl, kao_general_pkl, lbd_general_pkl, dpr_general_pkl,
    dpi_general_pkl;
Double_t missMass_general_pkl[40];
Double_t mInv_general_pkl[40];
Double_t oA_general_pkl[40];
void fillGeneralTree(vector3D* dirs, float* speeds, momentum4D* init, momentum4D* mea,
                     momentum4D* fals, momentum4D* fitted, vector3D lmea, bool swpri, bool swdec,
                     float chim)
{
  kao_general_pkl.m[0] = Eparticles::getMassG(11);
  pro_general_pkl.m[0] = Eparticles::getMassG(14);
  lbd_general_pkl.m[0] = Eparticles::getMassG(18);
  dpr_general_pkl.m[0] = Eparticles::getMassG(14);
  dpi_general_pkl.m[0] = Eparticles::getMassG(9);
  vector3D tmpv;
  momentum4D tmpvv;
  float tmps = 0;
  int DP = (swdec ? 3 : 2), PI = (swdec ? 2 : 3);
  int PR = (swpri ? 1 : 0), KA = (swpri ? 0 : 1);
  track_out_general* ptr;
  momentum4D mmiss_pri = init[0] + init[1] - mea[0] - mea[1];
  momentum4D mmiss_alt = init[0] + init[1] - fals[0] - fals[1];
  momentum4D mInv_beta;
  momentum4D mInv_pri =
      RtwoParticleDecay::getPrimary(pro_general_pkl.m[0], dpi_general_pkl.m[0], lmea, dirs[DP],
                                    dirs[PI], mmiss_pri.Momentum().R(), 1);
  if (speeds[DP] > 0)
    mInv_beta =
        RtwoParticleDecay::getPrimary(pro_general_pkl.m[0], dpi_general_pkl.m[0], lmea, dirs[DP],
                                      dirs[PI], (speeds[DP] > 1 ? 0.9998 : speeds[DP]), 0);
  else
    mInv_beta = mInv_pri;
  momentum4D pI = init[0] + init[1];
  for (int jj = 0; jj < 5; jj++) {
    for (int ii = 0; ii < 2; ii++) {
      ptr = NULL;
      switch (jj) {
      case 0:
        ptr = &pro_general_pkl;
        tmpv = dirs[swpri ? 1 - ii : ii];
        tmps = speeds[swpri ? 1 - ii : ii];
        tmpvv = (ii == 0 ? mea[jj] : fals[jj]);
        break;
      case 1:
        ptr = &kao_general_pkl;
        tmpv = dirs[swpri ? ii : 1 - ii];
        tmps = speeds[swpri ? ii : 1 - ii];
        tmpvv = (ii == 0 ? mea[jj] : fals[jj]);
        break;
      case 2:
        ptr = &dpr_general_pkl;
        tmpv = dirs[swdec ? 3 - ii : 2 + ii];
        tmps = speeds[swdec ? 3 + ii : 2 + ii]; // speeds[swdec?3-ii:2+ii];
        tmpvv = (ii == 0 ? RtwoParticleDecay::getSecondary(
                               pro_general_pkl.m[0], dpi_general_pkl.m[0], lmea, dirs[DP], dirs[PI],
                               (speeds[DP] > 1 ? 0.9998 : speeds[DP]), 0)
                         : RtwoParticleDecay::getSecondary(
                               pro_general_pkl.m[0], dpi_general_pkl.m[0],
                               mmiss_pri.Momentum() / mmiss_pri.Momentum().R(), dirs[DP], dirs[PI],
                               (speeds[DP] > 1 ? 0.9998 : speeds[DP]), 0));
        break;
      case 3:
        tmpv = dirs[swdec ? 2 + ii : 3 - ii];
        ptr = &dpi_general_pkl;
        tmps = speeds[swdec ? 2 + ii : 3 - ii]; //=speeds[swdec?3-ii:2+ii];
        tmpvv = (ii == 0 ? RtwoParticleDecay::getSecondary(
                               dpi_general_pkl.m[0], pro_general_pkl.m[0], lmea, dirs[PI], dirs[DP],
                               (speeds[DP] > 1 ? 0.9998 : speeds[DP]), 3)
                         : RtwoParticleDecay::getSecondary(
                               dpi_general_pkl.m[0], pro_general_pkl.m[0],
                               mmiss_pri.Momentum() / mmiss_pri.Momentum().R(), dirs[PI], dirs[DP],
                               (speeds[DP] > 1 ? 0.9998 : speeds[DP]), 3));
        break;
      case 4:
        ptr = &lbd_general_pkl;
        tmpv = (ii == 0 ? lmea : mmiss_pri.Momentum() / mmiss_pri.Momentum().R());
        tmps = (ii == 0 ? mInv_beta.Beta() : mmiss_pri.Beta());
        tmpvv = (ii == 0 ? mmiss_pri : mInv_beta);
        break;
      }
      if (ptr == NULL)
        continue;
      ptr->x_c[ii] = tmpv.X() / tmpv.R();
      ptr->y_c[ii] = tmpv.Y() / tmpv.R();
      ptr->z_c[ii] = tmpv.Z() / tmpv.R();
      ptr->th[ii] = tmpv.Theta();
      ptr->ph[ii] = tmpv.Phi();
      ptr->b[ii] = tmps;
      ptr->tr[ii] = tmpv.Rho() / tmpv.R();
      ptr->p[ii] = tmpvv.Momentum().R();
      ptr->e[ii] = tmpvv.Energy();
      ptr->lg[ii] = tmpv.Z() / tmpv.R();
      if (ii == 0 || jj == 4) {
        tmpvv = tmpvv.boost(momentum4D::CM(init[0], init[1]).Velocity() * -1);
        tmpv = tmpvv.Momentum();
        ptr->x_c[3] = tmpv.X() / tmpv.R();
        ptr->y_c[3] = tmpv.Y() / tmpv.R();
        ptr->z_c[3] = tmpv.Z() / tmpv.R();
        ptr->th[3] = tmpv.Theta();
        ptr->ph[3] = tmpv.Phi();
        ptr->b[3] = tmpvv.Beta();
        ptr->e[3] = tmpvv.Energy();
        ptr->tr[3] = tmpvv.Pt();
        ptr->lg[3] = tmpvv.Pl();
        // ptr->b[3]=tmpvv.Beta();
      }
    }
    if (ptr == NULL)
      continue;
    if (jj < 4)
      tmpvv = fitted[jj];
    else
      tmpvv = fitted[2] + fitted[3];
    tmpv = tmpvv.Momentum();
    ptr->p[4] = tmpv.R();
    ptr->e[4] = tmpvv.Energy();
    ptr->x_c[2] = tmpv.X() / tmpv.R();
    ptr->y_c[2] = tmpv.Y() / tmpv.R();
    ptr->z_c[2] = tmpv.Z() / tmpv.R();
    ptr->tr[2] = tmpvv.Pt();
    ptr->lg[2] = tmpvv.Pl();
    ptr->th[2] = tmpv.Theta();
    ptr->ph[2] = tmpv.Phi();
    ptr->b[2] = tmpvv.Beta();
    tmpvv = tmpvv.boost(momentum4D::CM(init[0], init[1]).Velocity() * -1);
    tmpv = tmpvv.Momentum();
    ptr->p[5] = tmpv.R();
    ptr->e[5] = tmpvv.Energy();
    ptr->x_c[4] = tmpv.X() / tmpv.R();
    ptr->y_c[4] = tmpv.Y() / tmpv.R();
    ptr->z_c[4] = tmpv.Z() / tmpv.R();
    ptr->tr[4] = tmpvv.Pt();
    ptr->lg[4] = tmpvv.Pl();
    ptr->th[4] = tmpv.Theta();
    ptr->ph[4] = tmpv.Phi();
    ptr->b[4] = tmpvv.Beta();
  } // 0 mltop, 1 mlsec, 2 fl, 3 mctop, 4 fc
  // 0 mltop, 1 mlsec, 2 pri(dec), 3 mass(dec), 4 fl, 5 fc
  lbd_general_pkl.m[1] = mmiss_pri.Mass();
  lbd_general_pkl.m[2] = mInv_pri.Mass();
  lbd_general_pkl.m[3] = mInv_beta.Mass();
  tmpvv = RtwoParticleDecay::getSecondary(pro_general_pkl.m[0], dpi_general_pkl.m[0], lmea,
                                          dirs[DP], dirs[PI], mmiss_pri.Momentum().R(), 3);
  dpr_general_pkl.p[2] = tmpvv.Momentum().R();
  dpr_general_pkl.e[2] = tmpvv.Energy();
  tmpvv = RtwoParticleDecay::getSecondary(dpi_general_pkl.m[0], pro_general_pkl.m[0], lmea,
                                          dirs[PI], dirs[DP], mmiss_pri.Momentum().R(), 3);
  dpi_general_pkl.p[2] = tmpvv.Momentum().R();
  dpi_general_pkl.e[2] = tmpvv.Energy();
  tmpvv = RtwoParticleDecay::getSecondary(pro_general_pkl.m[0], dpi_general_pkl.m[0], lmea,
                                          dirs[DP], dirs[PI], lbd_general_pkl.m[0], 2);
  dpr_general_pkl.p[3] = tmpvv.Momentum().R();
  dpr_general_pkl.e[3] = tmpvv.Energy();
  tmpvv = RtwoParticleDecay::getSecondary(dpi_general_pkl.m[0], pro_general_pkl.m[0], lmea,
                                          dirs[PI], dirs[DP], lbd_general_pkl.m[0], 2);
  dpi_general_pkl.p[3] = tmpvv.Momentum().R();
  dpi_general_pkl.e[3] = tmpvv.Energy();
  missMass_general_pkl[0] = mmiss_pri.Mass();
  missMass_general_pkl[1] = mmiss_alt.Mass();
  missMass_general_pkl[2] = (mmiss_pri - mInv_beta) * (mmiss_pri - mInv_beta);
  missMass_general_pkl[3] = (mmiss_alt - mInv_beta) * (mmiss_alt - mInv_beta);
  missMass_general_pkl[4] = (mmiss_pri - mInv_pri) * (mmiss_pri - mInv_pri);
  missMass_general_pkl[5] = (pI - mea[0]).Mass();
  missMass_general_pkl[6] = (pI - fals[0]).Mass();
  missMass_general_pkl[7] = (pI - mea[1]).Mass();
  missMass_general_pkl[8] = (pI - fals[1]).Mass();
  missMass_general_pkl[9] = (pI - mInv_beta).Mass();
  missMass_general_pkl[10] = (pI - mInv_pri).Mass();
  missMass_general_pkl[11] = (mInv_beta - mInv_pri).Mass();
  missMass_general_pkl[12] = (pI - mea[0] - fals[0]) * (pI - mea[0] - fals[0]);
  momentum4D pi[2];
  pi[0].setVM(speeds[PR] * dirs[PR], dpi_general_pkl.m[0]);
  pi[1].setVM(speeds[KA] * dirs[KA], dpi_general_pkl.m[0]);
  missMass_general_pkl[13] = (pI - mea[0] - pi[1]) * (pI - mea[0] - pi[1]);
  missMass_general_pkl[14] = (pI - pi[0] - fals[0]) * (pI - pi[0] - fals[0]);

  missMass_general_pkl[0 + 20] = (pI - fitted[0] - fitted[1]).Mass();
  pi[0].setVM(fitted[1].Velocity(), pro_general_pkl.m[0]);
  pi[1].setVM(fitted[0].Velocity(), kao_general_pkl.m[0]);
  missMass_general_pkl[1 + 20] = (pI - pi[0] - pi[1]).Mass();
  missMass_general_pkl[2 + 20] = (pI - fitted[0] - fitted[1] - (fitted[2] + fitted[3])) *
                                 (pI - fitted[0] - fitted[1] - (fitted[2] + fitted[3]));
  missMass_general_pkl[3 + 20] = (pI - pi[0] - pi[1] - (fitted[2] + fitted[3])) *
                                 (pI - pi[0] - pi[1] - (fitted[2] + fitted[3]));
  missMass_general_pkl[4 + 20] = (pI - fitted[0] - fitted[1] - (fitted[2] + fitted[3])) *
                                 (pI - fitted[0] - fitted[1] - (fitted[2] + fitted[3]));
  missMass_general_pkl[5 + 20] = (pI - fitted[0]).Mass();
  missMass_general_pkl[6 + 20] = (pI - pi[0]).Mass();
  missMass_general_pkl[7 + 20] = (pI - fitted[1]).Mass();
  missMass_general_pkl[8 + 20] = (pI - pi[1]).Mass();
  missMass_general_pkl[9 + 20] = (pI - (fitted[2] + fitted[3])).Mass();
  missMass_general_pkl[10 + 20] = (pI - (fitted[2] + fitted[3])).Mass();
  missMass_general_pkl[11 + 20] = 0;
  missMass_general_pkl[12 + 20] = (pI - fitted[0] - pi[0]) * (pI - fitted[0] - pi[0]);
  pi[1].setVM(fitted[1].Velocity(), dpi_general_pkl.m[0]);
  missMass_general_pkl[13 + 20] = (pI - fitted[0] - pi[1]) * (pI - fitted[0] - pi[1]);
  pi[1].setVM(fitted[0].Velocity(), dpi_general_pkl.m[0]);
  missMass_general_pkl[14 + 20] = (pI - pi[1] - pi[0]) * (pI - pi[1] - pi[0]);

  mInv_general_pkl[0] = (mea[0] + mea[1]).Mass();
  mInv_general_pkl[1] = (fals[0] + fals[1]).Mass();
  mInv_general_pkl[2] = (mea[0] + mInv_beta).Mass();
  mInv_general_pkl[3] = (fals[0] + mInv_beta).Mass();
  mInv_general_pkl[4] = (mea[1] + mInv_beta).Mass();
  mInv_general_pkl[5] = (fals[1] + mInv_beta).Mass();
  mInv_general_pkl[6] = (mea[0] + mInv_pri).Mass();
  mInv_general_pkl[7] = (fals[0] + mInv_pri).Mass();
  mInv_general_pkl[8] = (mea[1] + mInv_pri).Mass();
  mInv_general_pkl[9] = (fals[1] + mInv_pri).Mass();
  mInv_general_pkl[10] = mInv_pri.Mass();
  mInv_general_pkl[11] = mInv_beta.Mass();
  mInv_general_pkl[12] =
      RtwoParticleDecay::getPrimary(dpi_general_pkl.m[0], dpi_general_pkl.m[0], lmea, dirs[DP],
                                    dirs[PI], (speeds[DP] > 1 ? 0.9998 : speeds[DP]), 0)
          .Mass();

  pi[0].setVM(fitted[1].Velocity(), pro_general_pkl.m[0]);
  pi[1].setVM(fitted[0].Velocity(), kao_general_pkl.m[0]);
  mInv_general_pkl[0 + 20] = (fitted[0] + fitted[1]).Mass();
  mInv_general_pkl[1 + 20] = (pi[0] + pi[1]).Mass();
  mInv_general_pkl[2 + 20] = (fitted[0] + (fitted[2] + fitted[3])).Mass();
  mInv_general_pkl[3 + 20] = (pi[0] + (fitted[2] + fitted[3])).Mass();
  mInv_general_pkl[4 + 20] = (fitted[1] + (fitted[2] + fitted[3])).Mass();
  mInv_general_pkl[5 + 20] = (pi[1] + (fitted[2] + fitted[3])).Mass();
  mInv_general_pkl[6 + 20] = (fitted[0] + (fitted[2] + fitted[3])).Mass();
  mInv_general_pkl[7 + 20] = (pi[0] + (fitted[2] + fitted[3])).Mass();
  mInv_general_pkl[8 + 20] = (fitted[1] + (fitted[2] + fitted[3])).Mass();
  mInv_general_pkl[9 + 20] = (pi[1] + (fitted[2] + fitted[3])).Mass();
  mInv_general_pkl[10 + 20] = (fitted[2] + fitted[3]).Mass();
  mInv_general_pkl[11 + 20] = (fitted[2] + fitted[3]).Mass();
  mInv_general_pkl[12 + 20] =
      RtwoParticleDecay::getPrimary(dpi_general_pkl.m[0], dpi_general_pkl.m[0],
                                    (fitted[2] + fitted[3]).Momentum(), fitted[2].Momentum(),
                                    fitted[3].Momentum(), fitted[2].Beta(), 0)
          .Mass();

  oA_general_pkl[0] = acos((dirs[0] * dirs[1]) / dirs[0].R() / dirs[1].R());
  oA_general_pkl[1] = acos((dirs[DP] * dirs[PI]) / dirs[DP].R() / dirs[PI].R());
  oA_general_pkl[2] =
      acos((mmiss_pri.Momentum() * dirs[PR]) / mmiss_pri.Momentum().R() / dirs[PR].R());
  oA_general_pkl[3] =
      acos((mmiss_pri.Momentum() * dirs[KA]) / dirs[KA].R() / mmiss_pri.Momentum().R());
  oA_general_pkl[4] = acos((lmea * dirs[PR]) / lmea.R() / dirs[PR].R());
  oA_general_pkl[5] = acos((lmea * dirs[KA]) / dirs[KA].R() / lmea.R());
  oA_general_pkl[6] = acos((mmiss_pri.Momentum() * lmea) / lmea.R() / mmiss_pri.Momentum().R());
  oA_general_pkl[7] = acos((lmea * dirs[DP]) / lmea.R() / dirs[DP].R());
  oA_general_pkl[8] = acos((lmea * dirs[PI]) / dirs[PI].R() / lmea.R());
  oA_general_pkl[9] =
      acos((mmiss_pri.Momentum() * dirs[DP]) / mmiss_pri.Momentum().R() / dirs[DP].R());
  oA_general_pkl[10] =
      acos((mmiss_pri.Momentum() * dirs[PI]) / dirs[PI].R() / mmiss_pri.Momentum().R());

  oA_general_pkl[0 + 20] = acos((fitted[0].Momentum() * fitted[1].Momentum()) /
                                fitted[0].Momentum().R() / fitted[1].Momentum().R());
  oA_general_pkl[1 + 20] = acos((fitted[2].Momentum() * fitted[3].Momentum()) /
                                fitted[2].Momentum().R() / fitted[3].Momentum().R());
  oA_general_pkl[2 + 20] =
      acos(((pI - fitted[0] - fitted[1]).Momentum() * fitted[0].Momentum()) /
           (pI - fitted[0] - fitted[1]).Momentum().R() / fitted[0].Momentum().R());
  oA_general_pkl[3 + 20] =
      acos(((pI - fitted[0] - fitted[1]).Momentum() * fitted[1].Momentum()) /
           fitted[1].Momentum().R() / (pI - fitted[0] - fitted[1]).Momentum().R());
  oA_general_pkl[4 + 20] = acos(((fitted[2] + fitted[3]).Momentum() * fitted[0].Momentum()) /
                                (fitted[2] + fitted[3]).Momentum().R() / fitted[0].Momentum().R());
  oA_general_pkl[5 + 20] = acos(((fitted[2] + fitted[3]).Momentum() * fitted[1].Momentum()) /
                                fitted[1].Momentum().R() / (fitted[2] + fitted[3]).Momentum().R());
  oA_general_pkl[6 + 20] =
      acos(((pI - fitted[0] - fitted[1]).Momentum() * (fitted[2] + fitted[3]).Momentum()) /
           (fitted[2] + fitted[3]).Momentum().R() / (pI - fitted[0] - fitted[1]).Momentum().R());
  oA_general_pkl[7 + 20] = acos(((fitted[2] + fitted[3]).Momentum() * fitted[2].Momentum()) /
                                (fitted[2] + fitted[3]).Momentum().R() / fitted[2].Momentum().R());
  oA_general_pkl[8 + 20] = acos(((fitted[2] + fitted[3]).Momentum() * fitted[3].Momentum()) /
                                fitted[3].Momentum().R() / (fitted[2] + fitted[3]).Momentum().R());
  oA_general_pkl[9 + 20] =
      acos(((pI - fitted[0] - fitted[1]).Momentum() * fitted[2].Momentum()) /
           (pI - fitted[0] - fitted[1]).Momentum().R() / fitted[2].Momentum().R());
  oA_general_pkl[10 + 20] =
      acos(((pI - fitted[0] - fitted[1]).Momentum() * fitted[3].Momentum()) /
           fitted[3].Momentum().R() / (pI - fitted[0] - fitted[1]).Momentum().R());
  oA_general_pkl[39] = chim;
  outTree_general_pkl->Fill();
}
#endif
void divideTracks(int num, TTrack** trackIn, int& numP, TTrack** trP, int& numS1, TTrack** trS1,
                  int& numS2, TTrack** trS2)
{
  numP = 0;
  numS1 = 0;
  numS2 = 0;
  for (int i = 0; i < num; i++) {
    if (trackIn[i] == NULL)
      continue;
    if (trackIn[i]->isSecondary())
      continue;
    switch (trackIn[i]->getNumberOfSecondaryTracks()) {
    case 0: {
      if (trackIn[i]->getCharge() != 0) {
        trP[numP] = trackIn[i];
        numP++;
      }
      break;
    }
    case 1:
      trS1[numS1] = trackIn[i];
      numS1++;
      break;
    case 2:
      trS2[numS2] = trackIn[i];
      numS2++;
      break;
    }
  }
}
Rpp_pkl_pkppi::Rpp_pkl_pkppi() : RbaseReaction(NULL, 5, 2)
{
  fname = "Rpp_pkl_pkppi";
  numberOfParticles = 5;
  particles = new momentum4D[numberOfParticles];
  cmparticles = new momentum4D[numberOfParticles];
  particleIDs = new int[5];
  particleIDs[0] = 14;
  particleIDs[1] = 11;
  particleIDs[2] = 18;
  particleIDs[3] = 14;
  particleIDs[4] = 9;
}
Rpp_pkl_pkppi::Rpp_pkl_pkppi(AFitAlgorithm* kinFitIn, momentum4D p1, momentum4D p2)
    : RbaseReaction(kinFitIn, 5, 2)
{
  fname = "Rpp_pkl_pkppi";
  numberOfParticles = 5;
  particles = new momentum4D[numberOfParticles];
  cmparticles = new momentum4D[numberOfParticles];
  momentum4D p[2];
  p[0] = p1;
  p[1] = p2;
  setInitParticles(2, p);
  particleIDs = new int[5];
  particleIDs[0] = 14;
  particleIDs[1] = 11;
  particleIDs[2] = 18;
  particleIDs[3] = 14;
  particleIDs[4] = 9;
  exactRequirement = false;
  useMultiLineFit = false;
  useKinfit = false;
}

Rpp_pkl_pkppi::Rpp_pkl_pkppi(const Rpp_pkl_pkppi& rIn) : RbaseReaction(rIn)
{
  fname = "Rpp_pkl_pkppi";
  particles = new momentum4D[rIn.getNumberOfSecondaries()];
  cmparticles = new momentum4D[rIn.getNumberOfSecondaries()];
  if (rIn.getLambdaDecay() != NULL)
    setLambdaDecay(*(rIn.getLambdaDecay()));
  momentum4D p[2];
  p[0] = rIn.getInitParticle(0);
  p[1] = rIn.getInitParticle(1);
  setInitParticles(2, p);
  particleIDs = new int[5];
  particleIDs[0] = 14;
  particleIDs[1] = 11;
  particleIDs[2] = 18;
  particleIDs[3] = 14;
  particleIDs[4] = 9;
  exactRequirement = rIn.getExactRequirement();
  useMultiLineFit = rIn.getMultiLineFit();
  useKinfit = rIn.getUseKinfit();
  maximumProtonAngle = rIn.getMaximumProtonAngle();
  useDecay = rIn.UseDecay();
  setTOFResolution(rIn.getTOFResolution(0), rIn.getTOFResolution(1), rIn.getTOFResolution(2),
                   rIn.getTOFResolution(3));
  setRepresentation(rIn.getRepresentation(0), rIn.getRepresentation(1), rIn.getRepresentation(2),
                    rIn.getRepresentation(3));
}

Rpp_pkl_pkppi::~Rpp_pkl_pkppi() { delete[] cmparticles; }

int Rpp_pkl_pkppi::getNumberOfPrimaries() const { return 2; }

int Rpp_pkl_pkppi::getNumberOfSecondaries() const { return 5; }

// TH1F checkHistoBest("best","best",100,0,1.5);
// TH1F checkHistoSecondBest("2.best","2.best",100,0,1.5);

void printtestH()
{
  //   TCanvas *canvas=new TCanvas();
  //   testH.Draw();
  //   canvas->Print("altMissM.ps(");
  //   checkHistoBest.Draw();
  //   canvas->Print("altMissM.ps");
  //   checkHistoSecondBest.Draw();
  //   canvas->Print("altMissM.ps)");
  //   delete canvas;
}

float Rpp_pkl_pkppi::multiLineFit(TTrack& v1, TTrack& v2, TTrack& v3, TTrack& v4, vector3D& r1,
                                  vector3D& r2, vector3D& r3, vector3D& r4, point3D& p1,
                                  point3D& p2)
{
  vertexFit* vFit[2];
  vFit[0] = new vertexFit(3, v1.getNumberOfCalibHits() + v2.getNumberOfCalibHits() + 5);
  vFit[1] = new vertexFit(3, v3.getNumberOfCalibHits() + v3.getNumberOfCalibHits() + 5);
  TTrack* ttr[4] = {&v1, &v2, &v3, &v4};
  Vector p(13);
  for (int k = 0; k < 4; k++) {
    for (int i = 0; i < ttr[k]->getNumberOfCalibHits(-2); i++)
      for (int j = 0; j < ttr[k]->getNumberOfCalibHits(i); j++) {
        if (ttr[k]->getCalibSrcr(i, j)->getHitShape() != NULL) {
          p = ttr[k]->getCalibSrcr(i, j)->getHitShape()->HitParams(ttr[k]->getPath());
          vFit[k / 2]->addPoint(vector3D(p[7], p[8], p[3]), vector3D(p[4], p[5], 0), p[0], 0);
        }
      }
  }
  vector2D c(vFit[0]->fit(), vFit[1]->fit());
  r1 = vFit[0]->Line(0);
  r2 = vFit[0]->Line(1);
  p1 = vFit[0]->Vertex();
  r3 = vFit[1]->Line(0);
  r4 = vFit[1]->Line(1);
  p2 = vFit[1]->Vertex();
  delete vFit[0];
  delete vFit[1];
  return c.R();
}

float Rpp_pkl_pkppi::applyAKinfit(const vector3D& v1, const vector3D& v2, const vector3D& v3,
                                  const vector3D& v4, float beta1, float beta2, float beta3,
                                  float beta4, momentum4D* res, vector3D* errors)
{
  if ((!(v1.getState() == _regular_ || v1.getState() == _zero_)) ||
      (!(v2.getState() == _regular_ || v2.getState() == _zero_)) ||
      (!(v3.getState() == _regular_ || v3.getState() == _zero_)) ||
      (!(v4.getState() == _regular_ || v4.getState() == _zero_)))
    return 1e+30;

  vector3D input; //,error(0,M_PI/180./2.,M_PI/180.);

  int nIterations = 200;
  float epsilon = 1e-4;
  vector3D inits[2];
  inits[0] = initParticles[0].Momentum();
  inits[1] = initParticles[1].Momentum();
  float masses[4] = {Eparticles::getMassG(particleIDs[0]), Eparticles::getMassG(particleIDs[1]),
                     Eparticles::getMassG(particleIDs[3]), Eparticles::getMassG(particleIDs[4])};
  vector3D vcs[4] = {v1, v2, v3, v4};
  float bt[4] = {beta1, beta2, beta3, beta4};
#ifdef MOMENTA_USED
  for (int i = 0; i < 4; i++)
    bt[i] = bt[i] / sqrt(bt[i] * bt[i] + masses[i] * masses[i]);
#endif
  for (int i = 0; i < 4; i++) {
    if (bt[i] < 0)
      bt[i] = -bt[i];
    if (bt[i] > 1)
      bt[i] = 0.995;
  }
  kinFit kfit(nIterations, 7, epsilon);
  kfit.add(inits[0], vector3D(0, 0, 0), vector3D(2, 2, 2), initParticles[0].Mass(), 1, -1);
  kfit.add(inits[1], vector3D(0, 0, 0), vector3D(2, 2, 2), initParticles[1].Mass(), 1, -1);

  momentum4D tmp;
  vector3D value, error;
  int ass;
  float dp[4], de[4], dx[4], dy[4];
  for (int j = 0; j < 4; j++) {
    float g = 1 / sqrt(1 - bt[j] * bt[j]);
    dp[j] = g * g * (1 / g + bt[j] * bt[j] * g) * errors[j].X() * reso[j];
    de[j] = g * g * g * bt[j] * errors[j].X() * reso[j];
    g = sqrt(vcs[j].X() / vcs[j].Z() * vcs[j].X() / vcs[j].Z() +
             vcs[j].Y() / vcs[j].Z() * vcs[j].Y() / vcs[j].Z());
    float t1 = cos(vcs[j].Theta()) * cos(vcs[j].Phi());
    float t2 = sin(vcs[j].Theta()) * sin(vcs[j].Phi());
    dx[j] = sqrt(t1 * t1 * errors[j].Y() * errors[j].Y() + t2 * t2 * errors[j].Z() * errors[j].Z());
    g = sqrt(vcs[j].X() / vcs[j].Z() * vcs[j].X() / vcs[j].Z() +
             vcs[j].Y() / vcs[j].Z() * vcs[j].Y() / vcs[j].Z() + 1);
    t1 = cos(vcs[j].Theta()) * sin(vcs[j].Phi());
    t2 = sin(vcs[j].Theta()) * cos(vcs[j].Phi());
    dy[j] = sqrt(t1 * t1 * errors[j].Y() * errors[j].Y() + t2 * t2 * errors[j].Z() * errors[j].Z());
  }
  for (int j = 0; j < 4; j++) {
    ass = j;
    tmp.setVM(vcs[j] * bt[j], masses[ass]);
    switch (useRep[j]) {
    case 1: {
      value = tmp.Momentum();
      error = (dp[j] * masses[ass] / value.R()) * value;
      break;
    }
    case 2: {
      value.setValues(tmp.Energy() - masses[ass], vcs[j].Theta(), vcs[j].Phi());
      error.setValues(de[j] * masses[ass], errors[j].Y(), errors[j].Z());
      break;
    }
    case 3: {
      value.setValues(bt[j], vcs[j].Theta(), vcs[j].Phi());
      error.setValues(errors[j].X() * reso[j], errors[j].Y(), errors[j].Z());
      break;
    }
    case 4: {
      value.setValues(tmp.Momentum().R(), vcs[j].Theta(), vcs[j].Phi());
      error.setValues(dp[j] * masses[ass], errors[j].Y(), errors[j].Z());
      break;
    }
    case 5: {
      value.setValues(tmp.Energy() - masses[ass], vcs[j].X() / vcs[j].Z(), vcs[j].Y() / vcs[j].Z());
      error.setValues(de[j] * masses[ass], dx[j], dy[j]);
      break;
    }
    case 6: {
      value.setValues(tmp.Momentum().R(), vcs[j].X() / vcs[j].Z(), vcs[j].Y() / vcs[j].Z());
      error.setValues(de[j] * masses[ass], dx[j], dy[j]);
      break;
    }
    case 7: {
      value.setValues(bt[j], vcs[j].X() / vcs[j].Z(), vcs[j].Y() / vcs[j].Z());
      error.setValues(errors[j].X() * reso[j], dx[j], dy[j]);
      break;
    }
    }
    kfit.add(value, error, vector3D(1, 1, 1), masses[ass], useRep[j], 1);
  }

  float c = kfit.fit();

  for (int i = 0; i < 4; i++)
    res[i] = kfit.getMomentum(2 + i, kfit.FittedParticle(2 + i));

  if (kfit.OutInfo() < 0)
    c = -c;
  return c;
}
float Rpp_pkl_pkppi::applyKinfit(const vector3D& v1, const vector3D& v2, const vector3D& v3,
                                 const vector3D& v4, float beta1, float beta2, float beta3,
                                 float beta4, int& combination, momentum4D* res, vector3D* err)
{
  if ((!(v1.getState() == _regular_ || v1.getState() == _zero_)) ||
      (!(v2.getState() == _regular_ || v2.getState() == _zero_)) ||
      (!(v3.getState() == _regular_ || v3.getState() == _zero_)) ||
      (!(v4.getState() == _regular_ || v4.getState() == _zero_)))
    return 1e+30;
  momentum4D m11, m12, m21, m22, ii(0, 0, 0, 0);
#ifdef MOMENTA_USED
  m11.setPM(v1 * beta1, Eparticles::getMassG(particleIDs[0]));
  m12.setPM(v1 * beta1, Eparticles::getMassG(particleIDs[1]));
  m21.setPM(v2 * beta2, Eparticles::getMassG(particleIDs[0]));
  m22.setPM(v2 * beta2, Eparticles::getMassG(particleIDs[1]));
#else
  m11.setVM(v1 * beta1, Eparticles::getMassG(particleIDs[0]));
  m12.setVM(v1 * beta1, Eparticles::getMassG(particleIDs[1]));
  m21.setVM(v2 * beta2, Eparticles::getMassG(particleIDs[0]));
  m22.setVM(v2 * beta2, Eparticles::getMassG(particleIDs[1]));
#endif
  ii = initParticles[0] + initParticles[1];
  if (fabs((ii - m11 - m22).Mass() - Eparticles::getMassG(particleIDs[2])) > 1.5 &&
      fabs((ii - m12 - m21).Mass() - Eparticles::getMassG(particleIDs[2])) > 1.5)
    return 1e+30;
#ifdef OUTPUT
  cout << "pk:" << (ii - m11 - m22).Mass() - Eparticles::getMassG(particleIDs[2])
       << " kp:" << (ii - m12 - m21).Mass() - Eparticles::getMassG(particleIDs[2]) << endl;
  if (beta3 > 0 && beta4 > 0)
    cout << "beta\'s decay valid" << endl;
#endif
  momentum4D rets[4][4];
  float c[4];
  vector3D errors[4];
  errors[0] = err[0];
  errors[1] = err[1];
  errors[2] = err[2];
  errors[3] = err[3];
  //   cout<<"\t"<<v1<<beta1<<"\n\t"<<v2<<beta2<<"\n\t"<<v3<<beta3<<"\n\t"<<v4<<beta4<<endl;
  c[0] = applyAKinfit(v1, v2, v3, v4, beta1, beta2, beta3, beta4, rets[0], errors);
  errors[2] = err[3];
  errors[3] = err[2];
  c[1] = applyAKinfit(v1, v2, v4, v3, beta1, beta2, beta4, beta3, rets[1], errors);
  errors[0] = err[1];
  errors[1] = err[0];
  c[3] = applyAKinfit(v2, v1, v4, v3, beta2, beta1, beta4, beta3, rets[3], errors);
  errors[2] = err[3];
  errors[3] = err[2];
  c[2] = applyAKinfit(v2, v1, v3, v4, beta2, beta1, beta3, beta4, rets[2], errors);
  //   cout<<"kinfit"<<endl;
  //  tr->setErrorSpeed(0.1);
  //   cout<<errors[0]<<errors[1]<<errors[2]<<errors[3]<<endl;
  //   cout<<c[0]<<" "<<c[1]<<" "<<c[2]<<" "<<c[3]<<endl;
  int best = 0;
  for (int i = 1; i < 4; i++)
    if ((c[i] > 0 && c[i] < c[best]) || c[best] < 0)
      best = i;
  combination = best;
  for (int i = 0; i < 4; i++)
    res[i] = rets[best][i];
  return c[best];
}

void Rpp_pkl_pkppi::setInitParticles(int num, momentum4D* p)
{
  RbaseReaction::setInitParticles(num, p);
  excessEnergy = cm.Energy() -
                 (Eparticles::getMassG(14) + Eparticles::getMassG(18) + Eparticles::getMassG(11));
}

momentum4D Rpp_pkl_pkppi::getPromptProton() const { return particles[0]; }

momentum4D Rpp_pkl_pkppi::getDecayProton() const { return particles[3]; }

momentum4D Rpp_pkl_pkppi::getKaon() const { return particles[1]; }
momentum4D Rpp_pkl_pkppi::getDecayPion() const { return particles[4]; }

momentum4D Rpp_pkl_pkppi::getLambda() const { return particles[2]; }

int Rpp_pkl_pkppi::getPromptProtonID() const { return 0; }

int Rpp_pkl_pkppi::getDecayProtonID() const { return 3; }

int Rpp_pkl_pkppi::getKaonID() const { return 1; }

int Rpp_pkl_pkppi::getLambdaID() const { return 2; }

int Rpp_pkl_pkppi::getDecayPionID() const { return 4; }

RbaseReaction* Rpp_pkl_pkppi::getClone()
{
  Rpp_pkl_pkppi* ret = new Rpp_pkl_pkppi(*this);
  ret->setAngleLimit(angleLimit);
  ret->setMissingMassLimit(missMassLimit);
  ret->setChiLimit(chiLimit);
  ret->setExact(exactRequirement);
  ret->setMomentumLimit(momentumLimit);
  ret->setAngleRangesPK(angleRanges[0], angleRanges[1]);
  ret->setAngleRangesDecay(angleRanges[2], angleRanges[3]);
  ret->setMaximumProtonAngle(maximumProtonAngle);
  ret->setUseDecay(useDecay);
  ret->setTOFResolution(reso[0], reso[1], reso[2], reso[3]);
  ret->setRepresentation(useRep[0], useRep[1], useRep[2], useRep[3]);
  return ret;
}

RbaseReaction* Rpp_pkl_pkppi::getLambdaDecay() const { return Ldecay; }

void Rpp_pkl_pkppi::setLambdaDecay(RbaseReaction& reaction)
{
  if (Ldecay)
    delete Ldecay;
  Ldecay = reaction.getClone();
}
void Rpp_pkl_pkppi::setMissingMassLimit(float v) { missMassLimit = v; }
void Rpp_pkl_pkppi::setAngleLimit(float v) { angleLimit = v; }
bool Rpp_pkl_pkppi::calculatePrimary(TTrack* t1, TTrack* t2, momentum4D lambda, float* m,
                                     momentum4D* results)
{
  momentum4D pI = initParticles[0] + initParticles[1]; //-lambda;
  bool useGeometry = false;
  if (useGeometry) {
    RtwoParticleDecay pri((pI - lambda).Mass(), m[0], m[1]);
    pri.check(pI.Direction(), t1->getPath().Direction(), t2->getPath().Direction());
    results[0] = pri.P1Lab();
    results[1] = pri.P2Lab();
  } else {
    results[0].setVM(t1->getPath().Direction() * t1->getSpeed(), m[0]);
    results[1].setVM(t2->getPath().Direction() * t2->getSpeed(), m[1]);
  }
  return true;
}
bool Rpp_pkl_pkppi::calculateDecay(TTrack* t1, TTrack* t2, TTrack* t3, float* m,
                                   momentum4D* results)
{
  RtwoParticleDecay decay(m[0], m[1], m[2]);
  decay.check(t1->getPath().Direction(), t2->getPath().Direction(), t3->getPath().Direction());
  results[0] = decay.initLab();
  results[1] = decay.P1Lab();
  results[2] = decay.P2Lab();
  return decay.valid();
}
void Rpp_pkl_pkppi::setExact(bool e) { exactRequirement = e; }
void Rpp_pkl_pkppi::setMomentumLimit(float v) { momentumLimit = v; }
void Rpp_pkl_pkppi::setAngleRangesPK(float mn, float mx)
{
  angleRanges[0] = mn;
  angleRanges[1] = mx;
}
void Rpp_pkl_pkppi::setAngleRangesDecay(float mn, float mx)
{
  angleRanges[2] = mn;
  angleRanges[3] = mx;
}
bool Rpp_pkl_pkppi::getExactRequirement() const { return exactRequirement; }
bool Rpp_pkl_pkppi::getMultiLineFit() const { return useMultiLineFit; }
bool Rpp_pkl_pkppi::getUseKinfit() const { return useKinfit; }
void Rpp_pkl_pkppi::setMultiLineFit(bool value) { useMultiLineFit = value; }
void Rpp_pkl_pkppi::setUseKinfit(bool value) { useKinfit = value; }
void Rpp_pkl_pkppi::setMaximumProtonAngle(float ang) { maximumProtonAngle = ang; }
float Rpp_pkl_pkppi::getMaximumProtonAngle() const { return maximumProtonAngle; }
void Rpp_pkl_pkppi::setUseDecay(bool value) { useDecay = value; }
bool Rpp_pkl_pkppi::UseDecay() const { return useDecay; }
float Rpp_pkl_pkppi::getTOFResolution(int n) const
{
  if (n < 0 || n >= 4)
    return 0;
  return reso[n];
}
void Rpp_pkl_pkppi::setTOFResolution(float f1, float f2, float f3, float f4)
{
  reso[0] = f1;
  reso[1] = f2;
  reso[2] = f3;
  reso[3] = f4;
}
void Rpp_pkl_pkppi::setTOFResolution(int n, float f)
{
  if (n < 0 || n >= 4)
    return;
  reso[n] = f;
}
int Rpp_pkl_pkppi::getRepresentation(int n) const
{
  if (n < 0 || n >= 4)
    return 1;
  return useRep[n];
}
void Rpp_pkl_pkppi::setRepresentation(int n, int rep)
{
  if (n < 0 || n >= 4)
    return;
  useRep[n] = rep;
}
void Rpp_pkl_pkppi::setRepresentation(int rep1, int rep2, int rep3, int rep4)
{
  useRep[0] = rep1;
  useRep[1] = rep2;
  useRep[2] = rep3;
  useRep[3] = rep4;
}

float Rpp_pkl_pkppi::check(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                           TTrack** tracks_2s, int num_2s)
{
  chi2 = 1e+5;
  if (tracks_nd == NULL || tracks_2s == NULL)
    return chi2;
  if (num_nd < 2 || num_2s < 1)
    return chi2;
  int index[5];
  momentum4D result[5];
  float chi = justCheck(tracks_nd, num_nd, tracks_1s, num_1s, tracks_2s, num_2s, index, result);
  chi2 = chi;
  if (chi > chiLimit || chi < 0) {
    return chi;
  }
  for (int i = 0; i < 2; i++) {
    if (index[i] < 0 || index[i] >= num_nd)
      return chi2;
    setTrack(i, tracks_nd[index[i]]);
  }
  if (index[2] < 0 || index[2] >= num_2s)
    return chi2;
  setTrack(2, tracks_2s[index[2]]);
  for (int i = 0; i < 2; i++) {
    if (index[3 + i] < 0 || index[i + 3] > getTrack(2)->getNumberOfSecondaryTracks())
      return chi2;
    setTrack(i + 3, getTrack(2)->getSecondary(index[i + 3]));
  }
  for (int i = 0; i < 5; i++)
    particles[i] = result[i];
  return chi2;
}

float Rpp_pkl_pkppi::justCheck(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                               TTrack** tracks_2s, int num_2s, int* indexes, momentum4D* results)
{
  if (num_nd < 2 || num_2s < 1)
    return 1e+5;
  if (tracks_nd == NULL || tracks_2s == NULL)
    return 1e+5;
  if (exactRequirement && (num_nd != 2 || num_2s != 1 || num_1s != 0)) {
    cout << "no exact requrement" << endl;
    return 1e+5;
  }
  float bchi = 1e+5;
  int sub = 0, best = 0;
  if (useKinfit) {
    vector3D err[4];
    vector3D d[4];
    float s[4];
    point3D p[2];
    for (int i = 0; i < num_nd; i++) {
      d[0] = tracks_nd[i]->getPath().Direction();
      s[0] = tracks_nd[i]->getSpeed();
      if (s[0] > 0.995)
        s[0] = 0.995;
      err[0] = vector3D(tracks_nd[i]->getErrorSpeed(), tracks_nd[i]->getErrorTheta(),
                        tracks_nd[i]->getErrorPhi());
      p[0] = tracks_nd[i]->getPath().Foot();
      for (int j = i + 1; j < num_nd; j++) {
        d[1] = tracks_nd[j]->getPath().Direction();
        s[1] = tracks_nd[j]->getSpeed();
        err[1] = vector3D(tracks_nd[j]->getErrorSpeed(), tracks_nd[j]->getErrorTheta(),
                          tracks_nd[j]->getErrorPhi());
        if (s[1] > 0.995)
          s[1] = 0.995;
        for (int k = 0; k < num_2s; k++) {
          p[1] = tracks_2s[k]->getSecondary(0)->getPath().Foot();
          d[2] = tracks_2s[k]->getSecondary(0)->getPath().Direction();
          d[3] = tracks_2s[k]->getSecondary(1)->getPath().Direction();
          s[2] = tracks_2s[k]->getSecondary(0)->getSpeed();
          if (s[2] > 0.995)
            s[2] = 0.995;
          s[3] = tracks_2s[k]->getSecondary(1)->getSpeed();
          if (s[3] > 0.995)
            s[3] = 0.995;
          if (useMultiLineFit) {
            multiLineFit(*tracks_nd[i], *tracks_nd[j], *tracks_2s[k]->getSecondary(0),
                         *tracks_2s[k]->getSecondary(1), d[0], d[1], d[2], d[3], p[0],
                         p[1]); //<<endl;
          }
          err[2] = vector3D(tracks_2s[k]->getSecondary(0)->getErrorSpeed(),
                            tracks_2s[k]->getSecondary(0)->getErrorTheta(),
                            tracks_2s[k]->getSecondary(0)->getErrorPhi());
          err[3] = vector3D(tracks_2s[k]->getSecondary(1)->getErrorSpeed(),
                            tracks_2s[k]->getSecondary(1)->getErrorTheta(),
                            tracks_2s[k]->getSecondary(1)->getErrorPhi());
          //		  cout<<err[0]<<err[1]<<err[2]<<err[3]<<endl;
          int combo;
          float cc;
          momentum4D resul[4];
#ifdef MOMENTA_USED
          float s_2[4];
          vector3D xxld = p[1] - p[0];
          xxld.normalize();
          vector3D xx = solve(d[0], d[1], xxld, (initParticles[0] + initParticles[1]).Momentum());
          float tt1 = tan(acos((d[2] * xxld) / d[2].R() / xxld.R())),
                tt2 = tan(acos((d[3] * xxld) / d[3].R() / xxld.R()));
          s_2[0] = xx[0];
          s_2[1] = xx[1];
          s_2[2] = xx[2] / (tt1 + tt2) * sqrt(tt2 * tt2 + tt1 * tt1 * tt2 * tt2);
          s_2[3] = xx[2] / (tt1 + tt2) * sqrt(tt1 * tt1 + tt1 * tt1 * tt2 * tt2);
          cc = applyKinfit(d[0], d[1], d[2], d[3], s_2[0], s_2[1], s_2[2], s_2[3], combo, resul,
                           err);
#else
          cc = applyKinfit(d[0], d[1], d[2], d[3], s[0], s[1], s[2], s[3], combo, resul, err);
#endif
          momentum4D l_sup = (initParticles[0] + initParticles[1]) - (resul[0] + resul[1]);
          if (cc < 0 || combo < 0)
            continue;
          if (cc > bchi)
            continue;
          bchi = cc;
          if (results != NULL) {
            results[0] = resul[0];
            results[1] = resul[1];
            results[2] = l_sup;
            results[3] = resul[2];
            results[4] = resul[3];
          }
          switch (combo) {
          case 0:
            best = i + j * num_nd + k * num_nd * num_nd;
            sub = 0;
            break;
          case 1:
            best = i + j * num_nd + k * num_nd * num_nd;
            sub = 1;
            break;
          case 2:
            best = j + i * num_nd + k * num_nd * num_nd;
            sub = 0;
            break;
          case 3:
            best = j + i * num_nd + k * num_nd * num_nd;
            sub = 1;
            break;
          }
        }
      }
    }
    //      cout<<"chi"<<bchi<<endl;
    if (bchi < chiLimit) {
      if (indexes != NULL) {
        indexes[0] = best % (num_nd);
        best = best / num_nd;
        indexes[1] = best % num_nd;
        best = best / num_nd;
        indexes[2] = best;
        indexes[3] = sub;
        indexes[4] = 1 - sub;
      }
      return bchi;
    }
    return 1e+5;
  } else // use kinfit
  {
    bool useGeometry = false;
    momentum4D pI(initParticles[0] + initParticles[1]), ka, pr, l1;
    vector3D dir[6];
    float sp[5], p1, p2, chi, ma, bestMass = 1e+5, secBestMass = 1e+5; //, secBest=0;
    //      float mass[num_nd*num_nd*num_2s];
    float mP = Eparticles::getMassG(particleIDs[0]), mK = Eparticles::getMassG(particleIDs[1]),
          mL = Eparticles::getMassG(particleIDs[2]);
    vector3D ret, solution[num_2s * 2];
    RtwoParticleDecay decay(Eparticles::getMassG(particleIDs[2]),
                            Eparticles::getMassG(particleIDs[3]),
                            Eparticles::getMassG(particleIDs[4]));
    for (int i = 0; i < num_2s; i++) {
      decay.check(tracks_2s[i]->getPath().Direction(),
                  tracks_2s[i]->getSecondary(0)->getPath().Direction(),
                  tracks_2s[i]->getSecondary(1)->getPath().Direction());
      if (decay.valid())
        solution[i * 2].setValues(decay.pInit(), decay.p1Lab(), decay.p2Lab());
      else
        solution[i * 2].setValues(0, 0, 0);
      decay.check(tracks_2s[i]->getPath().Direction(),
                  tracks_2s[i]->getSecondary(1)->getPath().Direction(),
                  tracks_2s[i]->getSecondary(0)->getPath().Direction());
      if (decay.valid())
        solution[i * 2 + 1].setValues(decay.pInit(), decay.p1Lab(), decay.p2Lab());
      else
        solution[i * 2 + 1].setValues(0, 0, 0);
    }
    for (int i = 0; i < num_nd; i++)
      for (int j = i + 1; j < num_nd; j++) {
        if (i == j)
          continue;
        dir[0] = tracks_nd[i]->getPath().Direction();
        dir[1] = tracks_nd[j]->getPath().Direction();
        if (dir[0].Z() < 0)
          dir[0] = dir[0] * -1;
        if (dir[1].Z() < 0)
          dir[1] = dir[1] * -1;
        sp[0] = tracks_nd[i]->getSpeed();
        sp[1] = tracks_nd[j]->getSpeed();
        if (sp[0] >= 1)
          sp[0] = 0.99;
        if (sp[1] >= 1)
          sp[1] = 0.99;
        if (sp[0] <= 0)
          sp[0] = abs(sp[0]);
        if (sp[1] <= 0)
          sp[1] = abs(sp[1]);
        for (int l = 0; l < 2; l++) {
          // use Time of flight
          pr.setVM(dir[l] * sp[l], mP);
          ka.setVM(dir[1 - l] * sp[1 - l], mK);
          l1 = pI - (pr + ka);
          dir[5] = l1.Momentum();
          dir[5].normalize();
          chi =
              sqrt(abs(l1 * l1)); // abs(mass[2*(i+j*npri+k*npri*npri)+l]-Eparticles::getMassG(18));
          ma = sqrt(abs(l1 * l1)) - mL;
          chi = ma;
          if (abs(ma) > abs(bestMass) && abs(ma) < abs(secBestMass))
            secBestMass = ma;
#ifdef OUTPUT
          cout << l1.toString(4 + _spheric_).ascii() << pI << pr.toString(12 + _spheric_).ascii()
               << ka.toString(12 + _spheric_).ascii() << endl;
#endif
          if (abs(ma) > missMassLimit) {
            continue;
          }
          if (abs(ma) < abs(bestMass)) // if(chi<bchi)
          {
            float p3;
            if (useGeometry)
              p3 = (ret[2]);
            else
              p3 = l1.Momentum().length();
            for (int k = 0; k < num_2s; k++) {
              if (solution[k * 2].R() == 0 && solution[k * 2 + 1].R() == 0)
                continue;
              p1 = fabs(solution[k * 2][0] - p3);
              p2 = fabs(solution[k * 2 + 1][0] - p3);
              if (p1 > momentumLimit && p2 > momentumLimit)
                continue;

              dir[2] = tracks_2s[k]->getSecondary(0)->getPath().Direction();
              dir[3] = tracks_2s[k]->getSecondary(1)->getPath().Direction();
              dir[4] = tracks_2s[k]->getPath().Direction();
              if (dir[4].Z() < 0)
                dir[4] = dir[4] * -1;
              sp[2] = tracks_2s[k]->getSecondary(0)->getSpeed();
              sp[3] = tracks_2s[k]->getSecondary(1)->getSpeed();

              //			mass[2*(i+j*num_nd+k*num_nd*num_nd)+l]=acos((dir[4]*dir[5])/dir[4].R()/dir[5].R());
              if (acos((dir[4] * dir[5]) / dir[4].R() / dir[5].R()) > angleLimit) {
                continue;
              }
              if (l == 0) {
                best = i + j * num_nd + k * num_nd * num_nd;
              } else {
                best = j + i * num_nd + k * num_nd * num_nd;
              }
              bchi = abs(ma);
              bestMass = ma;
              if (solution[k * 2].R() == 0)
                sub = 1;
              else if (solution[2 * k + 1].R() == 0)
                sub = 0;
              else {
                if (p1 < p2)
                  sub = 0;
                else
                  sub = 1;
              }
            }
          }
          if (chi > bchi && chi < secBestMass) {
            secBestMass = chi;
            //		    secBest=acos((dir[4]*dir[5])/dir[4].R()/dir[5].R());
          }
        }
      }
    if (bchi <= chiLimit) {
      int i = best % num_nd;
      best = best / num_nd;
      int j = best % num_nd;
      best = best / num_nd;
      int k = best;
      if (indexes != NULL) {
        indexes[0] = i;
        indexes[1] = j;
        indexes[2] = k;
        indexes[3] = sub;
        indexes[4] = 1 - sub;
      }
      if (results != NULL) {
        float beta = tracks_nd[i]->getSpeed();
        if (beta >= 1)
          beta = 0.99;
        results[0].setVM(tracks_nd[i]->getPath().Direction() * beta, mP);
        beta = tracks_nd[j]->getSpeed();
        if (beta >= 1)
          beta = 0.99;
        results[1].setVM(tracks_nd[j]->getPath().Direction() * beta, mK);
        dir[2] = tracks_1s[k]->getPath().Direction();
        dir[3] = tracks_1s[k]->getSecondary(sub)->getPath().Direction();
        dir[4] = tracks_1s[k]->getSecondary(1 - sub)->getPath().Direction();
        float p3;
        if (useGeometry) {
          vector3D d1 = tracks_nd[i]->getPath().Direction(),
                   d2 = tracks_nd[j]->getPath().Direction();
          matrixNxM mm1(3, 3);
          mm1.setValue(0, 0, d1.X());
          mm1.setValue(0, 1, d2.X());
          mm1.setValue(0, 2, dir[2].X());
          mm1.setValue(1, 0, d1.Y());
          mm1.setValue(1, 1, d2.Y());
          mm1.setValue(1, 2, dir[2].Y());
          mm1.setValue(2, 0, d1.Z());
          mm1.setValue(2, 1, d2.Z());
          mm1.setValue(2, 2, dir[2].Z());
          Vector bb1(3), ret(3);
          bb1.transpose();
          bb1.setValues(pI.X(), pI.Y(), pI.Z());
          ret.transpose();
          ret = (-((!mm1) * mm1)) * ((!mm1) * bb1);
          results[0].setPM(d1 * (ret[0]), mP);
          results[1].setPM(d2 * (ret[1]), mK);
          results[2].setPM(dir[2] * (ret[2]), mL); //=pI-particles[0]-particles[1];
          p3 = (ret[2]);
          results[3].setPM(dir[3] * solution[k * 2 + sub][0], mP);
          results[4].setPM(dir[4] * solution[k * 2 + sub][1], Eparticles::getMassG(particleIDs[4]));
        } else {
          p3 = /*solutions[k*2+sub][0];*/ (pI - results[0] - results[1]).Momentum().length();
          // decay.check(dir[2],dir[3],dir[4]);
          double tanth1 = tan(acos((dir[2] * dir[3]) / dir[2].R() / dir[3].R()));
          double tanth2 = tan(acos((dir[2] * dir[4]) / dir[2].R() / dir[4].R()));
          double sum = tanth1 + tanth2;
          tanth1 = tanth1 * tanth1;
          tanth2 = tanth2 * tanth2;
          results[2].setPM(dir[2] * p3, mL);
          results[3].setPM(dir[3] * p3 * sqrt(tanth2 + tanth1 * tanth2) / sum, mP);
          results[4].setPM(dir[4] * p3 * sqrt(tanth1 + tanth1 * tanth2) / sum,
                           Eparticles::getMassG(particleIDs[4]));
        }
      }
    }
    return bchi;
  }
  return 1e+5;
}
RbaseReaction* Rpp_pkl_pkppi::checkNew(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s,
                                       int num_1s, TTrack** tracks_2s, int num_2s)
{
  if (tracks_nd == NULL || tracks_2s == NULL)
    return NULL;
  if (num_nd < 2 || num_2s < 1)
    return NULL;
  if (exactRequirement && (num_nd != 2 || num_2s != 1 || num_1s != 0)) {
    // cout <<"no exact requrement"<<endl;
    return NULL;
  }
  RbaseReaction* ret = this->getClone();
  float chi = ret->check(tracks_nd, num_nd, tracks_1s, num_1s, tracks_2s, num_2s);
  if (chi > chiLimit || chi < 0) {
    delete ret;
    return NULL;
  }
  return ret;
}
// RbaseReaction* Rpp_pkl_pkppi::checkNew(TTrack **tracks,int num)
// {
//   if(num<5)return NULL;
// #ifdef GENERAL_PKL
//   if(outTree_general_pkl==NULL)
//     {
//       TFile *f=new TFile("pkl_control_general.root","recreate");
//       f->cd();
//       outTree_general_pkl=new TTree("PKL-General","PKL-General");
//       outTree_general_pkl->Branch("pr",&pro_general_pkl,"p[6]/D:e[6]/D:m[6]/D:x_c[5]/D:y_c[5]/D:z_c[5]/D:tr[5]/D:lg[5]/D:th[5]/D:ph[5]/D:b[5]/D");
//       outTree_general_pkl->Branch("ka",&kao_general_pkl,"p[6]/D:e[6]/D:m[6]/D:x_c[5]/D:y_c[5]/D:z_c[5]/D:tr[5]/D:lg[5]/D:th[5]/D:ph[5]/D:b[5]/D");
//       outTree_general_pkl->Branch("ld",&lbd_general_pkl,"p[6]/D:e[6]/D:m[6]/D:x_c[5]/D:y_c[5]/D:z_c[5]/D:tr[5]/D:lg[5]/D:th[5]/D:ph[5]/D:b[5]/D");
//       outTree_general_pkl->Branch("dp",&dpr_general_pkl,"p[6]/D:e[6]/D:m[6]/D:x_c[5]/D:y_c[5]/D:z_c[5]/D:tr[5]/D:lg[5]/D:th[5]/D:ph[5]/D:b[5]/D");
//       outTree_general_pkl->Branch("pi",&dpi_general_pkl,"p[6]/D:e[6]/D:m[6]/D:x_c[5]/D:y_c[5]/D:z_c[5]/D:tr[5]/D:lg[5]/D:th[5]/D:ph[5]/D:b[5]/D");
//       outTree_general_pkl->Branch("missingMass",missMass_general_pkl,"missingMass[40]/D");
//       outTree_general_pkl->Branch("invariantMass",mInv_general_pkl,"invariantMass[40]/D");
//       outTree_general_pkl->Branch("openingAngle",oA_general_pkl,"openingAngle[40]/D");
//     }
// #endif
//   RbaseReaction *ret=new
//   Rpp_pkl_pkppi(fkinFit,initParticles[0],initParticles[1]);
//   ret->setChiLimit(chiLimit);
//   ((Rpp_pkl_pkppi*)ret)->setAngleLimit(angleLimit);
//   ((Rpp_pkl_pkppi*)ret)->setMissingMassLimit(missMassLimit);
//   ((Rpp_pkl_pkppi*)ret)->setExact(exactRequirement);
//   ((Rpp_pkl_pkppi*)ret)->setMomentumLimit(momentumLimit);
//   ((Rpp_pkl_pkppi*)ret)->setAngleRangesPK(angleRanges[0],angleRanges[1]);
//   ((Rpp_pkl_pkppi*)ret)->setAngleRangesDecay(angleRanges[2],angleRanges[3]);
//   ((Rpp_pkl_pkppi*)ret)->setMultiLineFit(useMultiLineFit);
//   ((Rpp_pkl_pkppi*)ret)->setUseKinfit(useKinfit);
//   ((Rpp_pkl_pkppi*)ret)->setMaximumProtonAngle(maximumProtonAngle);
//   ((Rpp_pkl_pkppi*)ret)->setUseDecay(useDecay);
//   ((Rpp_pkl_pkppi*)ret)->setTOFResolution(reso[0], reso[1], reso[2],
//   reso[3]);
//   ((Rpp_pkl_pkppi*)ret)->setRepresentation(useRep[0],useRep[1],useRep[2],useRep[3]);
//   TTrack *trp[num];
//   TTrack *trk[num];
//   TTrack *trv[num];
//   int np,nv,nk;
//   divideTracks(num, tracks, np, trp, nk, trk,nv, trv);
//   float chi1=ret->check(trp, np, trk, nk, trv, nv);
//   if(chi1>getChiLimit()||chi1<0)
//     {
//       delete ret;
//       return NULL;
//     }
//   return ret;
// }
extern vector3D solve(const vector3D& d1, const vector3D& d2, const vector3D& d3,
                      const vector3D& s);
