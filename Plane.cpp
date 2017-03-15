#include "Plane.h"

Plane::Plane() {
    point = Vector3::ORIGIN;
    normal = Vector3::YAXIS;
}

Plane::Plane(Vector3 p) {
    point = p;
    normal = Vector3::YAXIS;
    distance = normal.Dot(point);
}

bool Plane::TestSegment(const Segment &s, Intersection &i) {
    Vector3 v = s.B - point;
    
    float l = v.Dot(normal);
    
    //behind the plane
    if (l < 0) {
        Vector3 dir = s.B - s.A;
        dir.Normalize();
        float dn = dir.Dot(normal);
        float t = (distance - s.A.Dot(normal)) / dn;
        Vector3 x = s.A + (t * dir);
        
        i.Position.Set(x.x, x.y, x.z);
        i.Normal = v;
        return true;
    }
    
    return false;
}

void Plane::SetPoint(Vector3 &p) {
    point = p;
    distance = normal.Dot(point);
}

void Plane::SetNormal(Vector3 &n) {
    normal = n;
    distance = normal.Dot(point);
}

Vector3 Plane::GetNormal() {
    return normal;
}

float Plane::getD() {
    return distance;
}

void Plane::DrawWire() {
    glPushMatrix();
    glTranslatef(point.x, point.y, point.z);
    glBegin(GL_LINES);
    glVertex3f(len, 0.0, len);
    glVertex3f(len, 0.0, -len);
    
    glVertex3f(-len, 0.0, len);
    glVertex3f(-len, 0.0, -len);
    
    glVertex3f(-len, 0.0, len);
    glVertex3f(len, 0.0, len);
    
    glVertex3f(-len, 0.0, -len);
    glVertex3f(len, 0.0, -len);
    
    glEnd();
    glPopMatrix();
}

void Plane::DrawSolid() {
    glPushMatrix();
    glTranslatef(point.x, point.y, point.z);
    glBegin(GL_QUADS);
    glNormal3f(normal.x, normal.y, normal.z);
    glColor3f(0.5f, 0.0f, 0.5f);
    glVertex3f(len, -0.001, len);
    glVertex3f(len, -0.001, -len);
    glVertex3f(-len, -0.001, -len);
    glVertex3f(-len, -0.001, len);
    glEnd();
    glPopMatrix();
}
