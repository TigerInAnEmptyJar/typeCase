#include "algorithm.h"
#include "event.h"
#include "setup.h"
#include <TTree.h>
class algorithm_parameter;
using namespace std;
const int MAX_TRACKS = 1000;
const int MAX_H_P_T = 100;
typedef struct
{
  Short_t det[MAX_TRACKS * MAX_H_P_T];
  Short_t el[MAX_TRACKS * MAX_H_P_T];
  Float_t theta[MAX_TRACKS];
  Float_t beta[MAX_TRACKS];
  Float_t phi[MAX_TRACKS];
  Float_t theta_kf[MAX_TRACKS];
  Float_t beta_kf[MAX_TRACKS];
  Float_t phi_kf[MAX_TRACKS];
  Float_t vertex_x[MAX_TRACKS];
  Float_t vertex_y[MAX_TRACKS];
  Float_t vertex_z[MAX_TRACKS];
  Float_t chi[MAX_TRACKS];
  Short_t id[MAX_TRACKS];
  Short_t nel[MAX_TRACKS];
} prompt_struct;
typedef struct
{
  Short_t det[MAX_TRACKS * MAX_H_P_T];
  Short_t el[MAX_TRACKS * MAX_H_P_T];
  Float_t theta1[MAX_TRACKS];
  Float_t beta1[MAX_TRACKS];
  Float_t phi1[MAX_TRACKS];
  Float_t theta1_kf[MAX_TRACKS];
  Float_t beta1_kf[MAX_TRACKS];
  Float_t phi1_kf[MAX_TRACKS];
  Float_t theta2[MAX_TRACKS];
  Float_t beta2[MAX_TRACKS];
  Float_t phi2[MAX_TRACKS];
  Float_t theta2_kf[MAX_TRACKS];
  Float_t beta2_kf[MAX_TRACKS];
  Float_t phi2_kf[MAX_TRACKS];
  Float_t vertex_x[MAX_TRACKS];
  Float_t vertex_y[MAX_TRACKS];
  Float_t vertex_z[MAX_TRACKS];
  Float_t chi[MAX_TRACKS];
  Short_t id1[MAX_TRACKS];
  Short_t nel1[MAX_TRACKS];
  Short_t id2[MAX_TRACKS];
  Short_t nel2[MAX_TRACKS];
} vee_struct;
typedef struct
{
  Short_t det[MAX_TRACKS * MAX_H_P_T];
  Short_t el[MAX_TRACKS * MAX_H_P_T];
  Float_t theta_p[MAX_TRACKS];
  Float_t beta_p[MAX_TRACKS];
  Float_t phi_p[MAX_TRACKS];
  Float_t theta_p_kf[MAX_TRACKS];
  Float_t beta_p_kf[MAX_TRACKS];
  Float_t phi_p_kf[MAX_TRACKS];
  Float_t theta_d[MAX_TRACKS];
  Float_t beta_d[MAX_TRACKS];
  Float_t phi_d[MAX_TRACKS];
  Float_t theta_d_kf[MAX_TRACKS];
  Float_t beta_d_kf[MAX_TRACKS];
  Float_t phi_d_kf[MAX_TRACKS];
  Float_t vertex_p_x[MAX_TRACKS];
  Float_t vertex_p_y[MAX_TRACKS];
  Float_t vertex_p_z[MAX_TRACKS];
  Float_t vertex_d_x[MAX_TRACKS];
  Float_t vertex_d_y[MAX_TRACKS];
  Float_t vertex_d_z[MAX_TRACKS];
  Float_t chi[MAX_TRACKS];
  Short_t id_p[MAX_TRACKS];
  Short_t nel_p[MAX_TRACKS];
  Short_t id_d[MAX_TRACKS];
  Short_t nel_d[MAX_TRACKS];
} kink_struct;

class ATrackTreeOutput : public AAlgorithm
{
private:
  TEvent& EventStructure;
  TTrack** tracks;                           //!
  int& numberOfTracks;                       //!
  int& EventNumber;                          //!
  int& RunNumber;                            //!
  int& Trigger;                              //!
  int maxDet;                                //!
  TTree* trackTree;                          //!
  TTree* pklTree;                            //!
  prompt_struct outPrompt;                   //!
  vee_struct outVee;                         //!
  kink_struct outKink;                       //!
  Float_t Chi;                               //!
  Int_t Event;                               //!
  Int_t Run;                                 //!
  Int_t Trig;                                //!
  Int_t nPrompt;                             //!
  Int_t nVee;                                //!
  Int_t nKink;                               //!
  Int_t nPromptHit;                          //!
  Int_t nVeeHit;                             //!
  Int_t nKinkHit;                            //!
  Float_t trackTdcs[MAX_TRACKS * MAX_H_P_T]; //!
  Float_t trackQdcs[MAX_TRACKS * MAX_H_P_T]; //!
  Float_t trackpaths[MAX_TRACKS];            //!
  Float_t trackpaths1[MAX_TRACKS];           //!
  int numReactions;                          //!
  RbaseReaction** referReact;                //!
  bool localDirectory;                       //!
  string directory;                          //!
  string outFile;                            //!
  string copyFile;                           //!
  bool writePattern;                         //!
  int nPattern;                              //!
  int* pattern;                              //!
  void initialize(const string& filename);
  void finalize();
  void setPrompt(int num, TTrack* tr);
  void setVee(int num, TTrack* tr);
  void setKink(int num, TTrack* tr);

public:
  ATrackTreeOutput(TEvent& eventIn, TTrack** trackIn, int& nTrack, int& evtNr, int& rnNr, int& trg,
                   int maxD, const algorithm_parameter& param);
  ~ATrackTreeOutput() override;
  void process() override;
  static algorithm_parameter getDescription();
};
