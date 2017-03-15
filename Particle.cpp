#include "Particle.h"

Particle::Particle() {
    Mass = 1.0f;
    Position.Zero();
    Velocity.Zero();
    Force.Zero();
    fixed = false;
}

void Particle::Particle::Update(float deltaTime) {
    if (fixed) return;
    
    // Compute acceleration
    Vector3 Accel=(1.0/Mass) * Force;
    
    // Compute new p and v
    Velocity += Accel * deltaTime;
    Position += Velocity * deltaTime;
    
    // Zero out Force vector
    Force.Zero();
}

void Particle::Draw() {
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex3f(Position.x, Position.y, Position.z);
    glEnd();
}

void Particle::Reset() {
    Force.Zero();
    Velocity.Zero();
}

void Particle::ApplyForce(Vector3 &f) {
    Force += f;
}

void Particle::updatePos(float x, float y) {
    Position.x = Position.x + x;
    Position.y = Position.y + y;
}

void Particle::updatePrevious(Vector3 prev) {
    prevPos = prev;
}

Vector3& Particle::getPrevPosition() {
    return prevPos;
}

void Particle::setVelocity(float x, float y, float z) {
    Velocity.Set(x, y, z);
}

void Particle::setPosition(float x, float y, float z) {
    Position.Set(x, y, z);
}

void Particle::setFixed(bool f) {
    fixed = f;
}

Vector3 Particle::getVelocity() {
    return Velocity;
}

Vector3& Particle::getPosition() {
    return Position;
}

float Particle::getMass() {
    return Mass;
}
