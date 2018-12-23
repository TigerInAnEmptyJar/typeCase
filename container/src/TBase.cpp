#include "TBase.h"
TBase::TBase(const string& nme):fname(nme)
{}
TBase::~TBase(){}
string TBase::name()const{return fname;}
