#include "Cloth.h"
#include <math.h>

Cloth::Cloth() {
    deltaTime = 0.005f;
    ground = Plane(Vector3(0.0f, -1.0f, 0.0f));
    
    //create particles
    for (int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            Particle * p = new Particle();
            p->setPosition(i*ratio-(ratio*clothSize/2.0f), 0.2, j*ratio-(ratio*clothSize/2.0f));
            p->updatePrevious(p->getPosition());
            particles[i][j] = p;
        }
    }
    for (int i = 0; i < clothSize-1; i++) {
        for (int j = 0; j < clothSize-1; j++) {
            //adjacent side
            SpringDamper * s1 = new SpringDamper(particles[i][j],
                                                 particles[i+1][j],
                                                 ratio);
            //adjacent below
            SpringDamper * s2 = new SpringDamper(particles[i][j],
                                                 particles[i][j+1],
                                                 ratio);
            //diagonal positive
            SpringDamper * s3 = new SpringDamper(particles[i][j],
                                                 particles[i+1][j+1],
                                                 sqrt(2) * ratio);
            springs.push_back(s1);
            springs.push_back(s2);
            springs.push_back(s3);
        }
    }
    for (int i = 1; i < clothSize; i++) {
        for (int j = 0; j < clothSize-1; j++) {
            //diagonal negative
            SpringDamper * s1 = new SpringDamper(particles[i][j],
                                                 particles[i-1][j+1],
                                                 sqrt(2) * ratio);
            springs.push_back(s1);
        }
    }
    for (int j = 0; j < clothSize-1; j++) {
        //last column
        SpringDamper * s1 = new SpringDamper(particles[clothSize-1][j],
                                             particles[clothSize-1][j+1],
                                             ratio);
        //last row
        SpringDamper * s2 = new SpringDamper(particles[j][clothSize-1],
                                             particles[j+1][clothSize-1],
                                             ratio);
        springs.push_back(s1);
        springs.push_back(s2);
    }
    
    //set triangles
    for (int i = 0; i < clothSize-1; i++) {
        for (int j = 0; j < clothSize-1; j++) {
            Triangle c = Triangle();
            c.setVertex(particles[i][j],
                        particles[i][j+1],
                        particles[i+1][j+1]);
            triangles.push_back(c);
        }
    }
    for (int i = 1; i < clothSize; i++) {
        for (int j = 1; j < clothSize; j++) {
            Triangle c = Triangle();
            c.setVertex(particles[i][j],
                        particles[i][j-1],
                        particles[i-1][j-1]);
            triangles.push_back(c);
        }
    }
    
    // bending springs
    
    for (int i = 0; i < clothSize-2; i += 2) {
        for (int j = 0; j < clothSize-2; j += 2) {
            //adjacent side
            SpringDamper * s1 = new SpringDamper(particles[i][j],
                                                 particles[i+2][j],
                                                 ratio*3, specialDamp, specialSpring);
            //adjacent below
            SpringDamper * s2 = new SpringDamper(particles[i][j],
                                                 particles[i][j+2],
                                                 ratio*3, specialDamp, specialSpring);
            bendingSprings.push_back(s1);
            bendingSprings.push_back(s2);
        }
    }
    for (int j = 0; j < clothSize - 2; j += 2) {
        
        //last column
        SpringDamper * s1 = new SpringDamper(particles[clothSize-1][j],
                                             particles[clothSize-1][j+2],
                                             ratio*3, specialDamp, specialSpring);
        //last row
        SpringDamper * s2 = new SpringDamper(particles[j][clothSize-1],
                                             particles[j+2][clothSize-1],
                                             ratio*3, specialDamp, specialSpring);
        bendingSprings.push_back(s1);
        bendingSprings.push_back(s2);
    }

    // first row will be fixed
    for (int i = 0; i < clothSize; i++) {
        particles[i][clothSize-1]->setFixed(true);
    }
    
}

void Cloth::Update(Vector3 vair) {
    // add gravity force
    Vector3 gravity(0,-9.8,0);
    for(int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            Vector3 force = gravity * particles[i][j]->getMass(); // f=mg
            particles[i][j]->ApplyForce(force);
        }
    }
    //add spring damper force
    for (int i = 0; i < springs.size(); i++) {
        springs[i]->ComputeForce();
    }
    //add spring damper force for elasticity deformation fix
    for (int i = 0; i < bendingSprings.size(); i++) {
        bendingSprings[i]->ComputeForce();
    }
    //add aerodynamic force
    for (int i = 0; i < triangles.size(); i++) {
        triangles[i].ComputeForce(vair);
    }
    
    // Integrate
    for(int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            particles[i][j]->Update(deltaTime);
        }
    }
    
    //Check for collisions
    for (int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            Segment seg;
            Intersection inter;
            seg.A = particles[i][j]->getPrevPosition();
            seg.B = particles[i][j]->getPosition();

            //check for collision with ground
            if (ground.TestSegment(seg, inter)) {
                particles[i][j]->setPosition(inter.Position.x, inter.Position.y, inter.Position.z);
                Vector3 v = particles[i][j]->getVelocity();
                v.x = 0.7 * v.x;
                v.y = 0;
                v.z = 0.7 * v.z;
                particles[i][j]->setVelocity(v.x, v.y, v.z);
            }
            particles[i][j]->updatePrevious(particles[i][j]->getPosition());
        }
    }
}

void Cloth::smoothenSurface() {
    // step one: Loop through all particles and zero out the normal
    for (int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            particles[i][j]->normal = Vector3(0,0,0);
        }
    }
    
    // step two: Loop through all triangles and add the triangle normal to the normal of each of the three particles it connects
    for (int i = 0; i < triangles.size(); i++) {
        Vector3 triangleNormal = triangles[i].getNormal();
        triangles[i].getVertex(0)->normal += triangleNormal;
        triangles[i].getVertex(1)->normal += triangleNormal;
        triangles[i].getVertex(2)->normal += triangleNormal;
    }
    
    // step three: Loop through all the particles again and normalize the normal
    for (int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            particles[i][j]->normal = particles[i][j]->normal.Normalize();
        }
    }
}

void Cloth::Draw(bool mode) {
    
    if (!mode) {
        glBegin(GL_TRIANGLES);
        GLfloat purple[] = {0.545f, 0, 0, 1.0f};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
        for (int i = 0; i < triangles.size(); i++) {
            Vector3 p0 = triangles[i].getVertex(0)->getPosition();
            Vector3 p1 = triangles[i].getVertex(1)->getPosition();
            Vector3 p2 = triangles[i].getVertex(2)->getPosition();
            smoothenSurface();
            Vector3 n0 = triangles[i].getVertex(0)->normal;
            Vector3 n1 = triangles[i].getVertex(1)->normal;
            Vector3 n2 = triangles[i].getVertex(2)->normal;
            
            glNormal3f(n0.x, n0.y, n0.z);
            glVertex3f(p0.x, p0.y, p0.z);
            glNormal3f(n1.x, n1.y, n1.z);
            glVertex3f(p1.x, p1.y, p1.z);
            glNormal3f(n2.x, n2.y, n2.z);
            glVertex3f(p2.x, p2.y, p2.z);
        }
        GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glEnd();
        
        ground.DrawSolid();
        return;
        
    }
    
    glDisable(GL_LIGHTING);
    glColor3f(0, 0, 1);
    for (int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            particles[i][j]->Draw();
        }
    }
    
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    for (int i = 0; i < springs.size(); i++) {
        Vector3 p1 = springs[i]->P1->getPosition();
        Vector3 p2 = springs[i]->P2->getPosition();
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
    }
    glEnd();
    
    ground.DrawSolid();
    glEnable(GL_LIGHTING);
}

void Cloth::Reset() {
    for (int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            particles[i][j]->Reset();
            particles[i][j]->setPosition(i*ratio-(ratio*clothSize/2.0f), 0.2, j*ratio-(ratio*clothSize/2.0f));
        }
    }
}

void Cloth::drag(float x, float y, float z) {
    for (int i = 0; i < clothSize; i++) {
        Vector3 prev =particles[i][clothSize-1]->getPosition();
        particles[i][clothSize-1]->setPosition(prev.x+x, prev.y+y, prev.z+z);
    }
}


