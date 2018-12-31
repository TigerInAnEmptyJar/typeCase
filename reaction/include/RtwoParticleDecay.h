#include "TBase.h"
#include "geometry.h"
#include "track.h"
using namespace std;

class RtwoParticleDecay : public TBase
{
private:
  double p_1_cm, p_2_cm;       // constructor
  double E_1_cm, E_2_cm;       // constructor
  double beta_1_cm, beta_2_cm; // constructor
  double m_1, m_2, m_i;        // constructor
  double p_1_lab, p_2_lab;
  double E_1_lab, E_2_lab;
  double beta_1_lab, beta_2_lab;
  double beta_i_lab;
  double p_i_lab;
  double E_i_lab;
  double theta_cm;
  double excess;
  vector3D initDirection;
  vector3D direction1, direction2;
  vector3D dir1CM;
  bool valid_angles;
  bool possible;

public:
  RtwoParticleDecay();
  RtwoParticleDecay(const RtwoParticleDecay& react);
  RtwoParticleDecay(double mass1, double mass2, double mass3);
  void setMasses(double mass1, double mass2, double mass3);
  void check(const vector3D& v_in, const vector3D& v_out1, const vector3D& v_out2);
  void check(float theta_1, float theta_2);
  void check(TTrack* decayingTrack);
  float betaInit() const;
  float beta1Lab() const;
  float beta2Lab() const;
  float beta1CM() const;
  float beta2CM() const;
  float pCM() const;
  float p1Lab() const;
  float p2Lab() const;
  float pInit() const;
  bool valid() const;
  momentum4D P2CM(const vector3D& dir2 = vector3D(), const vector3D& initDir = vector3D()) const;
  momentum4D P1CM(const vector3D& dir1 = vector3D(), const vector3D& initDir = vector3D()) const;
  momentum4D initCM() const;
  momentum4D P2Lab(const vector3D& direction = vector3D()) const;
  momentum4D P1Lab(const vector3D& direction = vector3D()) const;
  momentum4D initLab(const vector3D& direction = vector3D()) const;
  float mass(int m) const;
  float massI() const;
  float mass2() const;
  float mass1() const;
  bool isPossible() const;
  static float getInvariantMass(float mass1, float mass2, float theta1, float theta2,
                                float decayingMomentum);
  static momentum4D getPrimary(float mass_p, float mass1, float mass2, vector3D dir_p,
                               vector3D dir1, vector3D dir2);
  static momentum4D getPrimary(float mass1, float mass2, vector3D dir_p, vector3D dir1,
                               vector3D dir2, float beta1, int ptt = 0);
  static momentum4D getSecondary(float mass1, float mass2, vector3D dir_p, vector3D dir1,
                                 vector3D dir2, float value, int ptt = 0);
};
