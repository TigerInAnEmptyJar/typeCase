#ifndef __PAINTPARAM
#define __PAINTPARAM
#include "baseparameter.h"
using namespace std;

class paint_parameter:public base_parameter
{
private:
    int ID;
    int color;
    int marker;
    bool painted;
public:
    paint_parameter(const string &nameIn=string(), int IDIn=0, int colorIn=1, bool toPaint=false, int markerIn=20);
    ~paint_parameter();
    paint_parameter(const paint_parameter&p);
    paint_parameter &operator=(const paint_parameter&p);
    int getID()const;
    int getMarker()const;
    int getColor()const;
    bool isToPaint()const;
    void setID(int IDIn);
    void setColor(int colorIn);
    void setMarker(int markerIn);
    void setToPaint(bool to=true);
};
ostream & operator <<(ostream & o, const paint_parameter &p);
istream & operator >>(istream &i, paint_parameter &p);
template<>bool takeItemFromVector<paint_parameter>(vector<paint_parameter> &vect,paint_parameter &item);
template<>bool moveItemToPosVector<paint_parameter>(vector<paint_parameter>& vect,paint_parameter&item,int pos);
#endif
