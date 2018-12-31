void read_algorithm_parameter_0(istream& i, algorithm_parameter& a)
{
  int zahl, z2, z3;
  float fz1, fz2, fz3;
  char c, _tmp[100];
  i >> zahl;
  a.setCategory(zahl);
  i >> zahl;
  a.setLevel(zahl);
  i >> zahl;
  a.setUse(zahl == 1);
  i >> zahl;
  a.setID(zahl);
  i >> zahl;
  for (int j = 0; j < zahl; j++) {
    i >> z2;
    i.get(c);
    i.getline(_tmp, 100);
    a.addParam<bool>(single_parameter<bool>(string(_tmp), z2 == 1));
  }
  i >> zahl;
  //    cout<<"point3D:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    i >> fz1 >> fz2 >> fz3;
    i.get(c);
    i.getline(_tmp, 100);
    a.addParam<point3D>(single_parameter<point3D>(string(_tmp), point3D(fz1, fz2, fz3)));
  }
  i >> zahl;
  //    cout<<"vector3D:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    i >> fz1 >> fz2 >> fz3;
    i.get(c);
    i.getline(_tmp, 100);
    vector3D tmpVector(fz1, fz2, fz3);
    a.addParam<vector3D>(
        single_parameter<vector3D>(string(_tmp), tmpVector)); // vector3D(fz1,fz2,fz3)));
  }
  i >> zahl;
  //    cout<<"int:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    i >> z2;
    i.get(c);
    i.getline(_tmp, 100);
    a.addParam<int>(single_parameter<int>(string(_tmp), z2));
  }
  i >> zahl;
  //    cout<<"float:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    i >> fz1;
    i.get(c);
    i.getline(_tmp, 100);
    a.addParam<float>(single_parameter<float>(string(_tmp), fz1));
  }
  i >> zahl;
  //    cout<<"string:"<<zahl<<endl;
  char _tmp2[100];
  i.get(c);
  for (int j = 0; j < zahl; j++) {
    i.getline(_tmp2, 100);
    i.getline(_tmp, 100);
    a.addParam<string>(single_parameter<string>(string(_tmp), string(_tmp2)));
  }
  i >> zahl;
  //    cout<<"vector<int>:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    vector<int> tmp;
    i >> z2;
    for (int k = 0; k < z2; k++) {
      i >> z3;
      tmp.push_back(z3);
    }
    i.get(c);
    i.getline(_tmp, 100);
    a.addParam<vector<int>>(single_parameter<vector<int>>(string(_tmp), tmp));
  }
  i >> zahl;
  //    cout<<"vector<float>:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    vector<float> tmp;
    i >> z2;
    for (int k = 0; k < z2; k++) {
      i >> fz1;
      tmp.push_back(fz1);
    }
    i.get(c);
    i.getline(_tmp, 100);
    a.addParam<vector<float>>(single_parameter<vector<float>>(string(_tmp), tmp));
  }
  i >> zahl;
  //    cout<<"vector<string>:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    char tmp_s[1000];
    vector<string> tmp;
    i >> z2;
    i.get(c);
    for (int k = 0; k < z2; k++) {
      i.getline(tmp_s, 1000);
      tmp.push_back(string(tmp_s));
    }
    i.getline(_tmp, 100);
    a.addParam<vector<string>>(single_parameter<vector<string>>(string(_tmp), tmp));
  }
  i >> zahl;
  cout << "algorithm_parameter:" << zahl << endl;
  for (int j = 0; j < zahl; j++) {
    algorithm_parameter ap;
    //      i.getline(_tmp,100);
    i >> ap;
    i.getline(_tmp, 100);
    a.addParam<algorithm_parameter>(single_parameter<algorithm_parameter>(ap.getName(), ap));
  }
  i.get(c);
  i.getline(_tmp, 100);
  cout << _tmp << endl;
  a.setName(string(_tmp));
  i >> zahl;
  cout << zahl;
  i.get(c);
  vector<string> d;
  for (int j = 0; j < zahl; j++) {
    i.getline(_tmp, 100);
    cout << " " << _tmp << endl;
    d.push_back(string(_tmp));
  }
  a.setDescription(d);
}

void write_algorithm_parameter_0(ostream& o, const algorithm_parameter& a)
{
  o << a.getCategory() << " ";
  o << a.getLevel() << " ";
  o << (a.IsUsed() ? 1 : 0) << " ";
  o << a.getID() << " ";
  o << a.getNumberOfParam<bool>() << " ";
  for (int i = 0; i < a.getNumberOfParam<bool>(); i++)
    o << (a.getParam<bool>(i).getData() ? "1" : "0") << " "
      << (a.getParam<bool>(i).getName().data()) << endl;
  o << a.getNumberOfParam<point3D>() << " ";
  for (int i = 0; i < a.getNumberOfParam<point3D>(); i++) {
    point3D tmpP = a.getParam<point3D>(i).getData();
    o << tmpP.X() << " " << tmpP.Y() << " " << tmpP.Z() << " "
      << a.getParam<point3D>(i).getName().data() << endl;
  }
  o << a.getNumberOfParam<vector3D>() << " ";
  for (int i = 0; i < a.getNumberOfParam<vector3D>(); i++) {
    vector3D tmpP = a.getParam<vector3D>(i).getData();
    o << tmpP.X() << " " << tmpP.Y() << " " << tmpP.Z() << " "
      << a.getParam<vector3D>(i).getName().data() << endl;
  }
  o << a.getNumberOfParam<int>() << " ";
  for (int i = 0; i < a.getNumberOfParam<int>(); i++)
    o << a.getParam<int>(i).getData() << " " << a.getParam<int>(i).getName().data() << endl;
  o << a.getNumberOfParam<float>() << " ";
  for (int i = 0; i < a.getNumberOfParam<float>(); i++)
    o << a.getParam<float>(i).getData() << " " << a.getParam<float>(i).getName().data() << endl;
  o << a.getNumberOfParam<string>() << " ";
  for (int i = 0; i < a.getNumberOfParam<string>(); i++)
    o << a.getParam<string>(i).getData().data() << "\n"
      << a.getParam<string>(i).getName().data() << endl;
  o << a.getNumberOfParam<vector<int>>() << " ";
  for (int j = 0; j < a.getNumberOfParam<vector<int>>(); j++) {
    a.getParam<vector<int>>(j).getData();
    o << a.getParam<vector<int>>(j).getData().size() << " ";
    for (unsigned int k = 0; k < a.getParam<vector<int>>(j).getData().size(); k++) {
      o << a.getParam<vector<int>>(j).getData().at(k) << " ";
    }
    o << a.getParam<vector<int>>(j).getName().data() << endl;
  }
  o << a.getNumberOfParam<vector<float>>() << " ";
  for (int j = 0; j < a.getNumberOfParam<vector<float>>(); j++) {
    a.getParam<vector<float>>(j).getData();
    o << a.getParam<vector<float>>(j).getData().size() << " ";
    for (unsigned int k = 0; k < a.getParam<vector<float>>(j).getData().size(); k++) {
      o << a.getParam<vector<float>>(j).getData().at(k) << " ";
    }
    o << a.getParam<vector<float>>(j).getName().data() << endl;
  }
  o << a.getNumberOfParam<vector<string>>() << " ";
  for (int j = 0; j < a.getNumberOfParam<vector<string>>(); j++) {
    a.getParam<vector<string>>(j).getData();
    o << a.getParam<vector<string>>(j).getData().size() << " ";
    for (unsigned int k = 0; k < a.getParam<vector<string>>(j).getData().size(); k++) {
      o << a.getParam<vector<string>>(j).getData().at(k).data() << "\n";
    }
    o << a.getParam<vector<string>>(j).getName().data() << endl;
  }
  o << a.getNumberOfParam<algorithm_parameter>() << " ";
  for (int i = 0; i < a.getNumberOfParam<algorithm_parameter>(); i++)
    o << a.getParam<algorithm_parameter>(i).getData() << " "
      << (a.getParam<algorithm_parameter>(i).getName().data()) << endl;

  if (a.getNumberOfParam<algorithm_parameter>() != 0)
    o << " ";
  o << a.getName().data() << endl;
  o << a.getDescription().size() << " ";
  for (unsigned int j = 0; j < a.getDescription().size(); j++) {
    o << a.getDescription(j).data() << endl;
  }
}
