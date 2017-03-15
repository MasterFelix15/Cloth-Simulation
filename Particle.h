#ifndef __cse169__Particle__
#define __cse169__Particle__

#include "Vector3.h"

class Particle {
public:
    Particle();
    void Update(float deltaTime);
    void Draw();
    void Reset();
    
    void ApplyForce(Vector3 &f);
    void updatePrevious(Vector3 prev);
    Vector3& getPrevPosition();
    Vector3& getPosition();
    void setPosition(float x, float y, float z);
    Vector3 getVelocity();
    void setVelocity(float x, float y, float z);
    void setFixed(bool f);
    void updatePos(float x, float y);
    float getMass();
    Vector3 normal; // for smooth rendering
    
private:
    float Mass; // Constant
    Vector3 Position;
    Vector3 Velocity;
    Vector3 Force;
    Vector3 prevPos; 
    bool fixed;
};

#endif /* defined(__cse169__Particle__) */
