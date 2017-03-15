#ifndef __cse169__Triangle__
#define __cse169__Triangle__

#include "Primitive.h"
#include "Particle.h"

class Triangle : public Primitive {
    
public:
    Triangle();
    void setVertex(Particle * v1, Particle * v2, Particle * v3);
    Particle * getVertex(int idx);
    
    void initNormal();
    Vector3 getNormal();
    
    Vector3 getVelocity();
    
    void ComputeForce(Vector3 vair);
    
    bool TestSegment(const Segment &s, Intersection &i);
    bool TestTriangle(Triangle &t, Intersection &i);
    
private:
    Particle * p1;
    Particle * p2;
    Particle * p3;
    Vector3 normal;
    Vector3 velocity;
};

#endif /* defined(__cse169__Triangle__) */
