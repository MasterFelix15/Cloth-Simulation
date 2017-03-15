#ifndef __cse169__Cloth__
#define __cse169__Cloth__

#include "SpringDamper.h"
#include "Triangle.h"
#include "Plane.h"

//change size of cloth here
static const int clothSize = 15;
static const float ratio = 0.06;
static const int specialSpring = 400;
static const int specialDamp = 20;

class Cloth {
    
public:
    Cloth();
    void Draw(bool mode);
    void Update(Vector3 vair);
    void Reset();
    void smoothenSurface();
    void drag(float x, float y, float z);
    
private:
    Particle * particles[clothSize][clothSize];
    float deltaTime;
    vector<SpringDamper*> springs;
    vector<SpringDamper*> bendingSprings;
    vector<Triangle> triangles;
    
    Plane ground;
    
};

#endif /* defined(__cse169__Cloth__) */
