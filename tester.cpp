////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

////////////////////////////////////////////////////////////////////////////////

string appName = "animator";
string APP_PATH = "Cloth-Simulation-master";

static Tester *TESTER;

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_position[] = {-1.0, 2.5, -1.0, 0.0};

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}
static void specialkeys(int key, int x, int y)          {TESTER->specialKeys(key, x, y); }

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=1000;
	WinY=645;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;
    
    // Create the window
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( WinX, WinY );
    glutInitWindowPosition( 0, 0 );
    WindowHandle = glutCreateWindow( "Cloth Simulation -- Tianrui Zhang" );
    glutSetWindow( WindowHandle );
    
    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
    //glEnable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
    
    // Background color
    glClearColor( 0., 0., 0., 1. );
    
    // Callbacks
    glutDisplayFunc( display );
    glutIdleFunc( idle );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mousebutton );
    glutMotionFunc( mousemotion );
    glutPassiveMotionFunc( mousemotion );
    glutReshapeFunc( resize );
    glutSpecialFunc( specialkeys );
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_diffuse);
    
	// Initialize components
	Cam.SetAspect(float(WinX)/float(WinY));
    showText = true;
    clothSim = true;
    airVelocity.Zero();
    cloth = Cloth();
    
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
    cloth.Update(airVelocity);
    
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
    cloth.Reset();
    airVelocity.Zero();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
    
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Draw components
    
	Cam.Draw();
    if (showText)  {displayText();}
    if (clothSim)  {cloth.Draw(skelMode);}
    
    
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::displayText() {
    //setup window for 2d drawing
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, WinX, 0.0, WinY);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    

    drawText(WinX/2 - 140, WinY - 45, "Programming Project Four: Cloth Simulation", 1.0f, 1.0f, 1.0f);
    drawText(WinX/2 - 100, WinY - 60, "Use WASDZX to move cloth", 0, 0, 1.0f);
    drawText(WinX/2 - 100, WinY - 75, "Use arrow keys to adjust wind", 0, 0, 1.0f);
    
    float toggle = 1.0f;
    sprintf(buffer, "%f", airVelocity.x);
    if (selectedDof == 0) toggle = 0.0f;
    drawText(WinX-120, WinY - 45, "X wind: " + string(buffer), 1.0f, toggle, toggle);
    toggle = 1.0f;
    sprintf(buffer, "%f", airVelocity.y);
    if (selectedDof == 1) toggle = 0.0f;
    drawText(WinX-120, WinY - 60, "Y wind: " + string(buffer), 1.0f, toggle, toggle);
    toggle = 1.0f;
    sprintf(buffer, "%f", airVelocity.z);
    if (selectedDof == 2) toggle = 0.0f;
    drawText(WinX-120, WinY - 75, "Z wind: " + string(buffer), 1.0f, toggle, toggle);

    //finish 2d drawing
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::drawText(int x, int y, string text, float r, float g, float b) {
    
    glColor3f(r, g, b);//needs to be called before RasterPos
    glRasterPos2i(x, y);
    void * font = GLUT_BITMAP_HELVETICA_12;
    
    for (std::string::iterator i = text.begin(); i != text.end(); ++i) {
        char c = *i;
        //this does nothing, color is fixed for Bitmaps when calling glRasterPos
        //glColor3f(1.0, 0.0, 1.0);
        glutBitmapCharacter(font, c);
    }


}

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
        case 'h':
            showText = !showText;
            break;
        case 'a':
            if (clothSim) cloth.drag(0.1f, 0.0f, 0.0f);
            break;
        case 'd':
            if (clothSim) cloth.drag(-0.1f, 0.0f, 0.0f);
            break;
        case 'w':
            if (clothSim) cloth.drag(0.0f, 0.1f, 0.0f);
            break;
        case 's':
            if (clothSim) cloth.drag(0.0f, -0.1f, 0.0f);
            break;
        case 'z':
            if (clothSim) cloth.drag(0.0f, 0.0f, 0.1f);
            break;
        case 'x':
            if (clothSim) cloth.drag(0.0f, 0.0f, -0.1f);
            break;
    }
}

void Tester::specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_LEFT:
            if (selectedDof == 0) airVelocity.x -= 2.0f;
            if (selectedDof == 1) airVelocity.y -= 2.0f;
            if (selectedDof == 2) airVelocity.z -= 2.0f;
            break;
        case GLUT_KEY_RIGHT:
            if (selectedDof == 0) airVelocity.x += 2.0f;
            if (selectedDof == 1) airVelocity.y += 2.0f;
            if (selectedDof == 2) airVelocity.z += 2.0f;
            break;
        case GLUT_KEY_UP:
            if (selectedDof > 0) selectedDof--;
            else selectedDof = 2;
            break;
        case GLUT_KEY_DOWN:
            if (selectedDof < 2) selectedDof++;
            else selectedDof = 0;
            break;
    }
}

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
        const float rate=1.0f;
        Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
        Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////

