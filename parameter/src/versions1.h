
void read_shape_parameter_0(istream& i, shape_parameter& sh)
{
  char li[100];
  int zahl[5];
  i >> zahl[0];
  i >> zahl[1];
  i >> zahl[2];
  i >> zahl[3];
  i >> zahl[4];
  i.getline(li, 100);
  sh.setName(li);
  for (int k = 0; k < zahl[0]; k++) {
    i.getline(li, 100);
    sh.addParam<point3D>(point3D(0, 0, 0), string(li));
  }
  for (int k = 0; k < zahl[1]; k++) {
    i.getline(li, 100);
    sh.addParam<vector3D>(vector3D(0, 0, 0), string(li));
  }
  for (int k = 0; k < zahl[2]; k++) {
    i.getline(li, 100);
    sh.addParam<int>(-1, string(li));
  }
  for (int k = 0; k < zahl[3]; k++) {
    i.getline(li, 100);
    sh.addParam<float>(-1, string(li));
  }
  for (int k = 0; k < zahl[4]; k++) {
    i.getline(li, 100);
    sh.addParam<string>(" ", string(li));
  }
}

void write_shape_parameter_0(ostream& o, const shape_parameter& sh)
{
  int zahl = sh.NumberOfParams<point3D>();
  o << (zahl) << " ";
  zahl = sh.NumberOfParams<vector3D>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<int>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<float>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<string>();
  o << zahl << sh.getName().data() << endl;
  for (int k = 0; k < sh.NumberOfParams<point3D>(); k++) {
    o << sh.getParamName<point3D>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<vector3D>(); k++) {
    o << sh.getParamName<vector3D>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<int>(); k++) {
    o << sh.getParamName<int>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<float>(); k++) {
    o << sh.getParamName<float>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<string>(); k++) {
    o << sh.getParamName<string>(k).data() << endl;
  }
}
