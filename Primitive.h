#ifndef cse169_Primitive_h
#define cse169_Primitive_h

#include "Vector3.h"

class Segment {
public:
    Vector3 A,B;
};

class Intersection {
public:
    Vector3 Position;
    Vector3 Normal;
};

class Primitive {
    virtual bool TestSegment(const Segment &s, Intersection &i)=0;
};

#endif
