void write_detector_parameter_1(ostream& o, const detector_parameter& d)
{
  o << d.getNumberOfElements() << " " << d.getStackType() << " " << d.getID() << " "
    << d.getMaterial() << " " << d.getMaxDistance() << d.getName().data() << endl;
  shape_parameter sh;
  sh = d.getShape();
  int zahl = sh.NumberOfParams<point3D>();
  o << (zahl) << " ";
  zahl = sh.NumberOfParams<vector3D>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<int>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<float>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<string>();
  o << zahl;
  o << sh.getName().data();
  o << endl;
  point3D p;
  vector3D v;
  int pec = o.precision();
  o.precision(10);
  for (int k = 0; k < sh.NumberOfParams<point3D>(); k++) {
    p = sh.getParam<point3D>(k);
    o << p.X() << " " << p.Y() << " " << p.Z() << sh.getParamName<point3D>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<vector3D>(); k++) {
    v = sh.getParam<vector3D>(k);
    o << v.Theta() << " " << v.Phi() << " " << v.R() << sh.getParamName<vector3D>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<int>(); k++) {
    o << sh.getParam<int>(k) << sh.getParamName<int>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<float>(); k++) {
    o << sh.getParam<float>(k) << sh.getParamName<float>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<string>(); k++) {
    o << sh.getParam<string>(k).data() << "\n" << sh.getParamName<string>(k).data() << endl;
  }
  o.precision(pec);
}
void read_detector_parameter_1(istream& i, detector_parameter& d)
{

  int zahl1, zahl2, zahl3, zahl4, zahl5, zahl;
  float value1, value2, value3;
  char li[100];
  i >> zahl;
  d.setNumberOfElements(zahl);
  i >> zahl;
  d.setStackType(zahl);
  i >> zahl;
  d.setID(zahl);
  i >> zahl;
  d.setMaterial(zahl);
  i >> value1;
  d.setMaxDistance(value1);
  i.getline(li, 100);
  d.setName(string(li));
  shape_parameter sh;
  i >> zahl1;
  i >> zahl2;
  i >> zahl3;
  i >> zahl4;
  i >> zahl5;
  i.getline(li, 100);
  sh.setName(string(li));
  point3D p;
  vector3D v;
  for (int k = 0; k < zahl1; k++) {
    i >> value1;
    i >> value2;
    i >> value3;
    p.setValues(value1, value2, value3);
    i.getline(li, 100);
    sh.addParam<point3D>(p, string(li));
  }
  for (int k = 0; k < zahl2; k++) {
    i >> value1;
    i >> value2;
    i >> value3;
    i.getline(li, 100);
    v.setValues(sin(value1) * cos(value2) * value3, sin(value1) * sin(value2) * value3,
                cos(value1) * value3);
    sh.addParam<vector3D>(v, string(li));
  }
  for (int k = 0; k < zahl3; k++) {
    i >> zahl;
    i.getline(li, 100);
    sh.addParam<int>(zahl, string(li));
  }
  for (int k = 0; k < zahl4; k++) {
    i >> value1;
    i.getline(li, 100);
    sh.addParam<float>(value1, string(li));
  }
  char li1[100];
  for (int k = 0; k < zahl5; k++) {
    i.getline(li, 100);
    i.getline(li1, 100);
    sh.addParam<string>(string(li), string(li1));
  }
  d.setShape(sh);
}
void write_detector_parameter_0(ostream& o, const detector_parameter& d)
{
  o << d.getNumberOfElements() << " " << d.getStackType() << " " << d.getID() << " "
    << d.getMaterial() << " " << d.getMaxDistance() << d.getName().data() << endl;
  shape_parameter sh;
  sh = d.getShape();
  int zahl = sh.NumberOfParams<point3D>();
  o << (zahl) << " ";
  zahl = sh.NumberOfParams<vector3D>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<int>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<float>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<string>();
  o << zahl;
  o << sh.getName().data();
  o << endl;
  point3D p;
  vector3D v;
  for (int k = 0; k < sh.NumberOfParams<point3D>(); k++) {
    p = sh.getParam<point3D>(k);
    o << p.X() << " " << p.Y() << " " << p.Z() << sh.getParamName<point3D>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<vector3D>(); k++) {
    v = sh.getParam<vector3D>(k);
    o << v.X() << " " << v.Y() << " " << v.Z() << sh.getParamName<vector3D>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<int>(); k++) {
    o << sh.getParam<int>(k) << sh.getParamName<int>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<float>(); k++) {
    o << sh.getParam<float>(k) << sh.getParamName<float>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<string>(); k++) {
    o << sh.getParam<string>(k).data() << "\n" << sh.getParamName<string>(k).data() << endl;
  }
}
void read_detector_parameter_0(istream& i, detector_parameter& d)
{
  int zahl1, zahl2, zahl3, zahl4, zahl5, zahl;
  float value1, value2, value3;
  char li[100];
  i >> zahl;
  d.setNumberOfElements(zahl);
  i >> zahl;
  d.setStackType(zahl);
  i >> zahl;
  d.setID(zahl);
  i >> zahl;
  d.setMaterial(zahl);
  i >> value1;
  d.setMaxDistance(value1);
  i.getline(li, 100);
  d.setName(string(li));
  shape_parameter sh;
  i >> zahl1;
  i >> zahl2;
  i >> zahl3;
  i >> zahl4;
  i >> zahl5;
  i.getline(li, 100);
  sh.setName(string(li));
  point3D p;
  vector3D v;
  for (int k = 0; k < zahl1; k++) {
    i >> value1;
    i >> value2;
    i >> value3;
    p.setValues(value1, value2, value3);
    i.getline(li, 100);
    sh.addParam<point3D>(p, string(li));
  }
  for (int k = 0; k < zahl2; k++) {
    i >> value1;
    i >> value2;
    i >> value3;
    i.getline(li, 100);
    v.setValues(value1, value2, value3);
    sh.addParam<vector3D>(v, string(li));
  }
  for (int k = 0; k < zahl3; k++) {
    i >> zahl;
    i.getline(li, 100);
    sh.addParam<int>(zahl, string(li));
  }
  for (int k = 0; k < zahl4; k++) {
    i >> value1;
    i.getline(li, 100);
    sh.addParam<float>(value1, string(li));
  }
  char li1[100];
  for (int k = 0; k < zahl5; k++) {
    i.getline(li, 100);
    i.getline(li1, 100);
    sh.addParam<string>(string(li), string(li1));
  }
  d.setShape(sh);
}
