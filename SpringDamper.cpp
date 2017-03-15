#include "SpringDamper.h"

void SpringDamper::ComputeForce() {
    Vector3 es = P2->getPosition() - P1->getPosition();
    float actualLength = es.Mag();
    float v1 = es.Normalize().Dot(P1->getVelocity());
    float v2 = es.Normalize().Dot(P2->getVelocity());
    float dampingForce = -SpringConstant * (RestLength - actualLength) - DampingFactor * (v1 - v2);
    
    Vector3 f1 = dampingForce * es.Normalize();
    Vector3 f2 = - f1;
    P1->ApplyForce(f1);
    P2->ApplyForce(f2);
}

SpringDamper::SpringDamper() {
    P1 = NULL;
    P2 = NULL;
    RestLength = 0.2;
    DampingFactor = DAMP_CONST;
    SpringConstant = SPRING_CONST;
}

SpringDamper::SpringDamper(Particle * p1, Particle * p2) {
    P1 = p1;
    P2 = p2;
    RestLength = 0.2;
    DampingFactor = DAMP_CONST;
    SpringConstant = SPRING_CONST;
}

SpringDamper::SpringDamper(Particle * p1, Particle * p2, float length) {
    P1 = p1;
    P2 = p2;
    RestLength = length;
    DampingFactor = DAMP_CONST;
    SpringConstant = SPRING_CONST;
}

SpringDamper::SpringDamper(Particle * p1, Particle * p2, float length, int damp, int spring) {
    P1 = p1;
    P2 = p2;
    RestLength = length;
    DampingFactor = damp;
    SpringConstant = spring;
}
