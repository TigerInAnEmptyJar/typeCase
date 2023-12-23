#include "tmyclass.h"
#include <fstream>
extern std::string knownTypesDoc(const std::string& type);
extern TMyClass* getKnownClass(const std::string& name);
extern void minimizeString(std::string& s);
#include "string_utilities.h"
nameTypeDoc::nameTypeDoc(std::string nameIn, std::string typeIn, int typeIdIn, std::string specifierIn,
                         std::string docIn, std::string typeDocIn)
{
  name = nameIn;
  type = typeIn;
  typeId = typeIdIn;
  specifier = specifierIn;
  doc = docIn;
  typeDoc = typeDocIn;
  defaultvalue = "";
  minimizeString(name);
  minimizeString(type);
  minimizeString(specifier);
  minimizeString(defaultvalue);
}
nameTypeDoc::nameTypeDoc(const nameTypeDoc& n)
{
  name = n.Name();
  type = n.Type();
  typeId = n.TypeId();
  specifier = n.Specifier();
  doc = n.Doc();
  typeDoc = n.TypeDoc();
  clearDescription();
  defaultvalue = n.Default();
  for (int i = 0; i < n.DescriptionLines(); i++)
    description.push_back(n.Description(i));
}
nameTypeDoc::nameTypeDoc(const std::string& s)
{
  name = "";
  type = "";
  doc = "";
  typeId = -1;
  typeDoc = "";
  specifier = "";
  defaultvalue = "";
  parse(s);
  minimizeString(name);
  minimizeString(type);
  minimizeString(specifier);
  minimizeString(defaultvalue);
}
nameTypeDoc::nameTypeDoc()
{
  name = "";
  type = "";
  doc = "";
  typeId = -1;
  typeDoc = "";
  defaultvalue = "";
  specifier = "";
}
nameTypeDoc::~nameTypeDoc() {}
nameTypeDoc& nameTypeDoc::operator=(const nameTypeDoc& nt)
{
  name = nt.Name();
  type = nt.Type();
  typeId = nt.TypeId();
  specifier = nt.Specifier();
  doc = nt.Doc();
  typeDoc = nt.TypeDoc();
  clearDescription();
  defaultvalue = nt.Default();
  for (int i = 0; i < nt.DescriptionLines(); i++)
    description.push_back(nt.Description(i));
  return *this;
}
bool nameTypeDoc::operator==(const nameTypeDoc& nt)
{
  if (name == nt.Name() && type == nt.Type() && specifier == nt.Specifier())
    return true;
  return false;
}
std::string nameTypeDoc::Name() const { return name; }
std::string nameTypeDoc::Type() const { return type; }
std::string nameTypeDoc::Doc() const { return doc; }
std::string nameTypeDoc::TypeDoc() const { return typeDoc; }
std::string nameTypeDoc::Description(int i) const
{
  if (i < 0 || i >= (int)description.size())
    return "";
  return description[i];
}
int nameTypeDoc::DescriptionLines() const { return description.size(); }
std::string nameTypeDoc::Specifier() const { return specifier; }
std::string nameTypeDoc::Default() const { return defaultvalue; }
int nameTypeDoc::TypeId() const { return typeId; }
void nameTypeDoc::setType(const std::string& s)
{
  type = s;
  minimizeString(type);
}
void nameTypeDoc::setName(const std::string& s)
{
  name = s;
  minimizeString(name);
}
void nameTypeDoc::setDoc(const std::string& s) { doc = s; }
void nameTypeDoc::setTypeId(int v) { typeId = v; }
void nameTypeDoc::setTypeDoc(const std::string& s) { typeDoc = s; }
void nameTypeDoc::setDescription(const std::vector<std::string>& input)
{
  clearDescription();
  for (unsigned int i = 0; i < input.size(); i++)
    description.push_back(input[i]);
}
void nameTypeDoc::setSpecifier(const std::string& s)
{
  specifier = s;
  minimizeString(specifier);
}
void nameTypeDoc::setDefault(const std::string& s)
{
  defaultvalue = s;
  minimizeString(defaultvalue);
}
void nameTypeDoc::clearDescription()
{
  while (!description.empty())
    description.pop_back();
}
void nameTypeDoc::addDescriptionLine(const std::string& s) { description.push_back(s); }
std::string nameTypeDoc::toString(int ptt) const
{
  std::string ret = type + " " + name;
  if ((ptt == 1 && hasA(name, "tmp") && hasA(name, lower(type))) || ptt == 2)
    ret = type;
  if (specifier != "")
    ret = specifier + " " + ret;
  return ret;
}
std::string nameTypeDoc::htmlHREF() const
{
  std::string ret = "";
  if (specifier != "")
    ret = specifier + " ";
  std::string t = type;
  t = replace(t, "<", "&lt;");
  t = replace(t, ">", "&gt;");
  std::string td = typeDoc, tp = type;
  tp = remove(tp, "*");
  tp = remove(tp, "&");
  tp = remove(tp, "[");
  tp = remove(tp, "]");
  if (td == "")
    td = knownTypesDoc(tp);
  if (td == "") {
    TMyClass* tmp = getKnownClass(tp);
    if (tmp != NULL)
      td = tmp->Doc();
  }
  if (td != "")
    ret += std::string("<A HREF=\"") + td + "\">" + t + "</A> ";
  else
    ret += std::string("<vtype>") + t + "</vtype> ";
  if (doc != "")
    ret += std::string("<A HREF=\"") + doc + "\">" + name + "</A>";
  else
    ret += name;
  if (defaultvalue != "")
    ret += std::string(" = ") + defaultvalue;
  return ret;
}
std::vector<std::string> nameTypeDoc::htmlNAME(const std::string& /*classname*/) const
{
  std::vector<std::string> ret;
  std::string td = typeDoc;
  if (td == "")
    td = knownTypesDoc(type);
  if (td == "") {
    TMyClass* tmp = getKnownClass(type);
    if (tmp != NULL)
      td = tmp->Doc();
  }
  if (td != "")
    ret.push_back(specifier + "<A HREF=\"" + td + "\">" + type + "</A> <A NAME=\"" + doc + "\">" +
                  name + "</A>");
  else
    ret.push_back(std::string("<A NAME=\"") + doc + "\">" + toString() + "</A>");
  ret.push_back("<BR/><deff>");
  for (unsigned int i = 0; i < description.size(); i++)
    ret.push_back(description[i]);
  if (defaultvalue != "")
    ret.push_back(std::string("Default: ") + defaultvalue);
  ret.push_back("</deff>");
  return ret;
}
extern ofstream logg;
void nameTypeDoc::parse(const std::string& expression)
{
  std::vector<std::string> parts;
  std::string word;
  std::string tmps;
  int i = 0;
  bool finished;
  std::string s = expression;
  std::string comment = "";
  if (hasA(s, "/*")) {
    if (!hasA(s, "*/"))
      s = s.substr(0, s.find("/*"));
    else {
      std::string tmp = s.substr(0, s.find("/*"));
      s = s.substr(s.find("/*") + 2, s.length() - s.find("/*") - 2);
      s = tmp + s.substr(s.find("*/") + 2, s.length() - s.find("*/") - 2);
    }
  }
  if (hasA(s, "//")) {
    comment = s.substr(s.find("//") + 2, s.length() - s.find("//") - 2);
    s = s.substr(0, s.find("//"));
  }
  if (hasA(s, ";"))
    s = s.substr(0, s.find(";"));
  if (hasA(s, "=")) {
    if (s[s.find("=") + 1] != '=') {
      defaultvalue = s.substr(s.find("=") + 1, s.length() - s.find("=") - 1);
      if (defaultvalue != "")
        s = s.substr(0, s.find("="));
    }
  }
  int def = 0;
  while (s[i] != ';' && (int)s.length() > i) {
    def = 0;
    word = "";
    finished = false;
    while ((!finished || (finished && (isSpace(s[i]) || s[i] == '*' || s[i] == '&'))) &&
           s[i] != ';' && s[i] != ',' && (int)s.length() > i) {
      if (!(isSpace(s[i]) || s[i] == '*' || s[i] == '&') || def != 0) {
        if (s[i] == '>') {
          def--;
          if (def == 0 && (int)s.length() > i + 1)
            if (!(isSpace(s[i + 1])))
              finished = true;
        }
        if (s[i] == '<')
          def++;
        word += s[i];
      } else if (s[i] == '<') {
        def++;
        word += s[i];
      } else if (s[i] == '*' || s[i] == '&') {
        word += s[i];
        finished = true;
      } else
        finished = true;
      i++;
    }
    if (word != "")
      parts.push_back(word);
  }
  if (parts.size() <= 0)
    return;
  name = parts[parts.size() - 1];
  minimizeString(name);
  if (parts.size() <= 1)
    return;
  type = parts[parts.size() - 2];
  minimizeString(type);
  typeId = nameTypeDoc::typeIDS(type);
  tmps = type;
  tmps = remove(tmps, "*");
  tmps = remove(tmps, "&");
  tmps = remove(tmps, "[");
  tmps = remove(tmps, "]");
  typeDoc = knownTypesDoc(tmps);
  if (parts.size() <= 2)
    return;
  specifier = "";
  for (unsigned int i = 0; i < parts.size() - 2; i++)
    specifier += parts[i] + " ";
  minimizeString(specifier);
}
ostream& operator<<(ostream& o, const nameTypeDoc& t)
{
  // old version
  /*  o<<t.Name().data()<<endl;
  o<<t.TypeId()<<" "<<t.Type().data()<<endl;
  o<<t.Specifier().data()<<endl;
  o<<t.Doc().data()<<endl;
  o<<t.TypeDoc().data()<<endl;*/
  // new version
  o << t.Name().data() << "\n" << t.TypeId() << " " << t.Type().data() << endl;
  if (t.Specifier() != "")
    o << "S" << t.Specifier().data() << endl;
  if (t.Doc() != "")
    o << "O" << smallerPath(t.Doc()).data() << endl;
  if (t.TypeDoc() != "")
    o << "T" << smallerPath(t.TypeDoc()).data() << endl;
  if (t.Default() != "")
    o << "V" << t.Default().data() << endl;
  // o<<"V"<<t.Default().data()<<endl;
  for (int i = 0; i < t.DescriptionLines(); i++)
    o << "D" << t.Description(i).data() << endl;
  o << "=" << endl;
  return o;
}
istream& operator>>(istream& o, nameTypeDoc& t)
{
  char li[1000], c;
  int n;
  o.getline(li, 1000);
  t.setName(li);
  o >> n;
  o.get(c);
  o.getline(li, 1000);
  t.setTypeId(n);
  t.setType(li);
  t.setSpecifier("");
  t.setDoc("");
  t.setTypeDoc("");
  t.setDefault("");
// old version
#ifdef OLDINPUT
  o.getline(li, 1000);
  t.setSpecifier(li);
  o.getline(li, 1000);
  t.setDoc(li);
  o.getline(li, 1000);
  t.setTypeDoc(li);
#endif
  // new
  o.get(c);
  t.clearDescription();
  while (c == 'D' || c == 'S' || c == 'O' || c == 'T' || c == 'V') {
    if (c == 'V') {
      o.getline(li, 1000);
      t.setDefault(li);
    } else if (c == 'S') {
      o.getline(li, 1000);
      t.setSpecifier(li);
    } else if (c == 'O') {
      o.getline(li, 1000);
      t.setDoc(li);
    } else if (c == 'T') {
      o.getline(li, 1000);
      t.setTypeDoc(expandPath(li));
    } else if (c == 'D') {
      o.getline(li, 1000);
      t.addDescriptionLine(li);
    }
    o.get(c);
  }
  o.getline(li, 1000);
  return o;
}
nameTypeDoc nameTypeDoc::parseIt(const std::string& expression)
{
  nameTypeDoc ret(expression);
  return ret;
}
//#include "container.h"
//#include "geometry.h"
//#include "parameter.h"
//#include "algorithm.h"
int nameTypeDoc::typeIDS(const std::string& s1)
{
  std::string s = s1;
  s = remove(s, "*");
  s = remove(s, "&");
  if (s == "int")
    return 0; // typeid(int);
  if (s == "float")
    return 1; // typeid(float);
  if (s == "long")
    return 2; // typeid(long);
  if (s == "double")
    return 3; // typeid(double);
  if (s == "long double")
    return 4; // typeid(long double);
  if (s == "char")
    return 5; // typeid(char);
  if (s == "bool")
    return 6; // typeid(bool);
  if (s == "void")
    return 7; // typeid(int);
  if (s == "ostream")
    return 8; // typeid(ostream);
  if (s == "istream")
    return 9; // typeid(istream);
  if (s == "vector")
    return 10; // typeid(vector);
  if (s == "geomObj")
    return 11; // typeid(geomObj);
  if (s == "point")
    return 12; // typeid(point);
  if (s == "Vector")
    return 13; // typeid(Vector);
  if (s == "point2D")
    return 14; // typeid(point2D);
  if (s == "vector2D")
    return 15; // typeid(vector2D);
  if (s == "point3D")
    return 16; // typeid(point3D);
  if (s == "vector3D")
    return 17; // typeid(vector3D);
  if (s == "point4D")
    return 18; // typeid(point4D);
  if (s == "vector4D")
    return 19; // typeid(vector4D);
  if (s == "momentum4D")
    return 20; // typeid(momentum4D);
  if (s == "sLine3D")
    return 21; // typeid(sLine3D);
  if (s == "lLine3D")
    return 22; // typeid(lLine3D);
  if (s == "plane3D")
    return 23; // typeid(plane3D);
  if (s == "matrix3D")
    return 24; // typeid(matrix3D);
  if (s == "matrixNxM3D")
    return 25; // typeid(matrixNxM3D);
  if (s == "TRawHit")
    return 100; // typeid(TRawHit);
  if (s == "TCalibHit")
    return 101; // typeid(TCalibHit);
  if (s == "TTrack")
    return 102; // typeid(TTrack);
  if (s == "TPixel")
    return 103; // typeid(TPixel);
  if (s == "TCluster")
    return 104; // typeid(TCluster);
  if (s == "TEvent")
    return 105; // typeid(TEvent);
  if (s == "TMaterial")
    return 106; // typeid(TMaterial);
  if (s == "TDetector")
    return 107; // typeid(TDetector);
  if (s == "TSetup")
    return 108; // typeid(TSetup);
  if (s == "TBeam")
    return 109; // typeid(TBeam);
  if (s == "TTarget")
    return 110; // typeid(TTarget);
  if (s == "AAlgorithm")
    return 111; // typeid(AAlgorithm);
  if (s == "base_parameter")
    return 120; // typeid(base_parameter);
  if (s == "algorithm_parameter")
    return 121; // typeid(algorithm_parameter);
  if (s == "detector_parameter")
    return 122; // typeid(detector_parameter);
  if (s == "reaction_parameter")
    return 123; // typeid(reaction_parameter);
  if (s == "material_parameter")
    return 124; // typeid(material_parameter);
  if (s == "beamTime_parameter")
    return 125; // typeid(beamTime_parameter);
  if (s == "run_parameter")
    return 126; // typeid(run_parameter);
  if (s == "element_parameter")
    return 128; // typeid(element_parameter);
  if (s == "shape_parameter")
    return 129; // typeid(shape_parameter);
  if (s == "single_parameter")
    return 130; // typeid(single_parameter);
  if (s == "paint_parameter")
    return 131; // typeid(paint_parameter);
  if (s == "baseShape")
    return 150; // typeid(baseShape);
  if (s == "volumeShape")
    return 151; // typeid(volumeShape);
  if (s == "planeShape")
    return 152; // typeid(planeShape);
  return -1;
}
