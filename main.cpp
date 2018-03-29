#include <GL/glut.h>
#include <GL/gl.h>

#include <math.h>
#include <string.h>
#include <iostream>

#include "tga.h"

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat mat_ambient[]    = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 0.0f };

TGA *earth, *moon;
GLint earthID, moonID;

int slices = 48;
int stacks = 48;

int GridDiv = 32;

float zoom = 1;

int ccmode = 1;

float mx = 30.0f;
float my = 0.0f;
float mz = -15.0f;

float cx = 0.0f;
float cy = 0.0f;
float cz = 0.0f;

float ux = 0.0f;
float uy = 1.0f;
float uz = 0.0f;

float EarthAngler = 23.44;
bool EarthSpy = false;
bool EarthSpyM = false;

bool showEarthOrbite = true;
bool showMoonOrbite = true;
bool showEarthGrid = false;
bool showMoon = true;

float EarthDistance = 18;
float MoonDistance = 3;

float EarthDist = 18;
float MoonDist = 4;

float EarthR = 1.5;
float MoonR = 0.5;
float SunR = 2;

double alpd = 0;
double alpc = 0;
double alpm = 0;

float msp = 1.0f;
float ksp = 1.0f;
bool playing = true;


static double sin180(double alpha) {
	return sin((alpha * 3.141592653589793238462643)/180);
}
static double cos180(double alpha) {
	return cos((alpha * 3.141592653589793238462643)/180);
}


static void resize(int width, int height) {
	const float ar = (float) width / (float) height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


GLUquadricObj* quadric;

static void display(void) {
	if (playing == true) {
		alpd += 0.004166666666666666666667 * ksp;
		alpc += 0.000011407511727834657152 * ksp;
		alpm += 0.000152625152625152625153 * ksp;
	}
	glColor3d(0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	if (EarthSpy) {
		if (EarthSpyM) {
			glLoadIdentity();
			gluLookAt(EarthDistance*sin180(alpc) + mx, my, EarthDistance*cos180(alpc) + mz, EarthDistance*sin180(alpc), 0, EarthDistance*cos180(alpc), ux, uy, uz);
		} else {
			glLoadIdentity();
			gluLookAt(mx, my, mz, EarthDistance*sin180(alpc), 0, EarthDistance*cos180(alpc), ux, uy, uz);
		}
	} else {
		glLoadIdentity();
		gluLookAt(mx, my, mz, cx, cy, cz, ux, uy, uz);
    }
	EarthDistance = EarthDist * zoom;
	MoonDistance = MoonDist * zoom;

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.5);
	glTranslated(0, 0, 0);
	quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, SunR * zoom, slices, stacks);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslated(EarthDistance*sin180(alpc), 0, EarthDistance*cos180(alpc));
	glRotated(90, 1, 0, 0);
	glRotated(-EarthAngler, 1, 0, 0);
	glRotated(-alpd, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, earthID);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, EarthR * zoom, slices, stacks);
	glPopMatrix();


	if (showMoon) {
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslated(EarthDistance*sin180(alpc) + MoonDistance*sin180(alpm), 0, EarthDistance*cos180(alpc) + MoonDistance*cos180(alpm));
		glRotated(90, 1, 0, 0);
		glRotated(-alpd, 0, 0, 1);
		glBindTexture(GL_TEXTURE_2D, moonID);
		quadric = gluNewQuadric();
		gluQuadricTexture(quadric, true);
		gluQuadricNormals(quadric, GLU_SMOOTH);
		gluSphere(quadric, MoonR * zoom, slices, stacks);
		glPopMatrix();
	}

	if (showEarthOrbite) {
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glColor3d(1,1,1);
		glBegin(GL_LINE_STRIP);
		for (float angle = 0.0f; angle < 6.283185307f; angle += 0.05f){
			glVertex3f(sin(angle) * EarthDistance,  0.0f, cos(angle) * EarthDistance);
		}
		glVertex3f(0.0f, 0.0f, EarthDistance);
		glEnd();
		glPopMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}

	if (showMoonOrbite) {
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glColor3d(1,1,1);
		glBegin(GL_LINE_STRIP);
		for (float angle = 0.0f; angle < 6.283185307f; angle += 0.05f) {
			glVertex3f(EarthDistance*sin180(alpc) + MoonDistance*sin(angle),  0.0f, EarthDistance*cos180(alpc) + MoonDistance*cos(angle));
		}
		glVertex3f(EarthDistance*sin180(alpc), 0.0f, EarthDistance*cos180(alpc) + MoonDistance);
		glEnd();
		glPopMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}

	if (showEarthGrid) {
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glColor3d(1.0, 1.0, 1.0);
		glColor3f(1.0, 1.0, 1.0);
		glTranslated(EarthDistance*sin180(alpc), 0, EarthDistance*cos180(alpc));
		glRotated(90, 1, 0, 0);
		glRotated(-EarthAngler, 1, 0, 0);
		glRotated(-alpd, 0, 0, 1);
		glutWireSphere(EarthR * zoom + 0.1, GridDiv, GridDiv);
		glPopMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}

	glutSwapBuffers();
}


static void outCoordinates(void) {
	std::cout << "[  CAM  ] Coords change: " << "\n M: " << mx << " ; " << my << " ; " << mz << "\n C: " << cx << " ; " << cy << " ; " << cz << "\n U: " << ux << " ; " << uy << " ; " << uz << "\n";

}

static void skey(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_F1:
			showEarthOrbite = !showEarthOrbite;
			break;
		case GLUT_KEY_F2:
			showEarthGrid = !showEarthGrid;
			break;
		case GLUT_KEY_F3:
			showMoonOrbite = !showMoonOrbite;
			break;
		case GLUT_KEY_F4:
			showMoon = !showMoon;
			break;

		case GLUT_KEY_F9:
			GridDiv -= 1;
			std::cout << "[ VIEW  ] Divide grid of Earth change: " << GridDiv << "\n";
			break;
		case GLUT_KEY_F10:
			GridDiv += 1;
			std::cout << "[ VIEW  ] Divide grid of Earth change: " << GridDiv << "\n";
			break;

		case GLUT_KEY_F11:
			zoom -= 0.05;
			std::cout << "[ VIEW  ] Zoom change: " << zoom << "\n";
			break;
		case GLUT_KEY_F12:
			zoom += 0.05;
			std::cout << "[ VIEW  ] Zoom change: " << zoom << "\n";
			break;

		case GLUT_KEY_PAGE_UP:
			alpd += 0.004166666666666666666667 * ksp;
			alpc += 0.000011407511727834657152 * ksp;
			alpm += 0.000152625152625152625153 * ksp;
			break;
		case GLUT_KEY_PAGE_DOWN:
			alpd -= 0.004166666666666666666667 * ksp;
			alpc -= 0.000011407511727834657152 * ksp;
			alpm -= 0.000152625152625152625153 * ksp;
			break;
		case GLUT_KEY_HOME:
			EarthSpy = !EarthSpy;
			break;
		case GLUT_KEY_END:
			EarthSpyM = !EarthSpyM;
			break;
	}
}


static void key(unsigned char key, int x, int y) {
	switch (key) {
		case 27 :
			exit(0);
			break;

		case '+':
			msp+=0.1;
			std::cout << "[  CAM  ] Speed Change coords of Cam change: " << msp << "\n";
			break;
		case '-':
			msp-=0.1;
			std::cout << "[  CAM  ] Speed Change coords of Cam change: " << msp << "\n";
			break;

		case '7':
			if (ccmode == 1) {
				mx += msp;
			} else if (ccmode == 2) {
				cx += msp;
			} else if (ccmode == 3) {
				ux += msp;
			}
			outCoordinates();
			break;

		case '1':
			if (ccmode == 1) {
				mx -= msp;
			} else if (ccmode == 2) {
				cx -= msp;
			} else if (ccmode == 3) {
				ux -= msp;
			}
			outCoordinates();
			break;

		case '8':
			if (ccmode == 1) {
				my += msp;
			} else if (ccmode == 2) {
				cy += msp;
			} else if (ccmode == 3) {
				uy += msp;
			}
			outCoordinates();
			break;
		case '2':
			if (ccmode == 1) {
				my -= msp;
			} else if (ccmode == 2) {
				cy -= msp;
			} else if (ccmode == 3) {
				uy -= msp;
			}
			outCoordinates();
			break;
		case '9':
			if (ccmode == 1) {
				mz += msp;
			} else if (ccmode == 2) {
				cz += msp;
			} else if (ccmode == 3) {
				uz += msp;
			}
			outCoordinates();
			break;
		case '3':
			if (ccmode == 1) {
				mz -= msp;
			} else if (ccmode == 2) {
				cz -= msp;
			} else if (ccmode == 3) {
				uz -= msp;
			}
			outCoordinates();
			break;

		case '4':
			ccmode = 1;
			break;
		case '5':
			ccmode = 2;
			break;
		case '6':
			ccmode = 3;
			break;
		case '0':
			mx = 30.0;
			my = 0.0;
			mz = 20.0;
			cx = 0.0;
			cy = 0.0;
			cz = 0.0;
			ux = 0.0;
			uy = 1.0;
			uz = 0.0;
			break;
		case '*':
			ksp *= 2;
			std::cout << "[  CAM  ] Animate Speed change: " << ksp << "\n";
			break;
		case '/':
			ksp /= 2;
			std::cout << "[  CAM  ] Animate Speed change: " << ksp << "\n";
			break;

		case ' ':
			playing = !playing;
			break;

		case 'q':
			EarthDist -= 0.1;
			std::cout << "[ MATH  ] Distance Sun-Earth change: " << EarthDist << "\n";
			break;
		case 'w':
			EarthDist += 0.1;
			std::cout << "[ MATH  ] Distance Sun-Earth change: " << EarthDist << "\n";
			break;

		case 'e':
			MoonDist -= 0.1;
			std::cout << "[ MATH  ] Distance Earth-Moon change: " << MoonDist << "\n";
			break;
		case 'r':
			MoonDist += 0.1;
			std::cout << "[ MATH  ] Distance Earth-Moon change: " << MoonDist << "\n";
			break;

		case 'a':
			SunR -= 0.1;
			std::cout << "[ MATH  ] Radius Sun change: " << SunR << "\n";
			break;
		case 's':
			SunR += 0.1;
			std::cout << "[ MATH  ] Radius Sun change: " << SunR << "\n";
			break;

		case 'd':
			EarthR -= 0.1;
			std::cout << "[ MATH  ] Radius Earth change: " << EarthR << "\n";
			break;
		case 'f':
			EarthR += 0.1;
			std::cout << "[ MATH  ] Radius Earth change: " << EarthR << "\n";
			break;

		case 'g':
			MoonR -= 0.1;
			std::cout << "[ MATH  ] Radius Moon change: " << MoonR << "\n";
			break;
		case 'h':
			MoonR += 0.1;
			std::cout << "[ MATH  ] Radius Moon change: " << MoonR << "\n";
			break;

		case 'z':
			EarthAngler = 23.44;
			break;
		case 'x':
			EarthAngler = 0;
			break;
		case 'c':
			EarthAngler = 45;
			break;
		case 'v':
			EarthAngler = 90;
			break;
    }
	glutSwapBuffers();
}

static void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	std::cout << "Simple Interactive Modeles: The motion of the Earth\n";
	std::cout << "version 1.0.0\n\n";
	std::cout << "Created by iner7\n\n";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Simple Interactive Modeles: The motion of the Earth v. 1.0.0");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutSpecialFunc(skey);
	glutIdleFunc(idle);

	glEnable(GL_TEXTURE_2D);
	glClearColor(0,0,0,1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);

	earth = new TGA("earth.tga");
	moon = new TGA("moon.tga");
	earthID = earth->getTextureHandle();
	moonID = moon->getTextureHandle();

	glutMainLoop();
	return 0;
}
