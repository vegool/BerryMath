#ifndef OS_LIBRARY_H
#define OS_LIBRARY_H

#include <BerryMath.h>
#include <vector>
#include <map>
using std::vector;
using std::map;
using BM::Object;
using BM::Scope;
using BM::String;
using BM::Number;
using BM::NativeFunction;
using BM::Function;

Object* system(Scope*, vector<Object*>);
extern "C" Object* initModule();

#endif