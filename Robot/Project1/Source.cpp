
/*
*  Curba_spline.c
*  Programul utilizeaza biblioteca GLUT
*  pentru redarea unei curbe spline
*/
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

GLUnurbsObj *theNurb; // curba este un obiect de tipul GLUnurbsObj

void myinit(void)
{
	glShadeModel(GL_FLAT);
	// glEnable(GL_DEPTH_TEST);
	theNurb = gluNewNurbsRenderer(); // obiectul de tip GLUnurbsObj
									 // este creat dinamic in memoria libera prin apelul functiei gluNewNurbsRenderer()
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
	// er0area de esantionare a curbei la redare
	glLineStipple(1, 0x0F0F);
	glPointSize(5);
}



void CALLBACK display(void)
{	// 9 puncte de control

	int i;
	GLfloat ctlpoints[9][3] = {
		{ -3.0, 4.0, 0.0 },
		{ -3.0, 3.0, 0.0 },
		{ -3.0, -0.5, 0.0 },
		{ -1.5, -4.0, 0.0 },
		{ 0.0, -4.0, 0.0 },
		{ 1.5, -4.0, 0.0 },
		{ 3.0, -0.5, 0.0 },
		{ 3.0, 3.0, 0.0 },
		{ 3.0, 4.0, 0.0 },
	};

	// 13 noduri

	GLfloat knots[13] = { 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 6.0, 6.0, 6.0 };

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0); // culoarea curenta de desenare



	glLoadIdentity();
	glTranslated(0.2, 0.0, -1.0);
	glScaled(0.2, 0.2, 0.2);


	// incepe corpul de redare al curbei Spline
	glDisable(GL_LINE_STIPPLE);
	gluBeginCurve(theNurb);
	gluNurbsCurve(theNurb,	// pointer obiect NURBS
		13, knots,			//numar noduri, tablou noduri
		3,					// intervalul de valori dintre doua puncte de control consecutive
		&ctlpoints[0][0],	// vector puncte de control
		4,					// ordinul curbei 
		GL_MAP1_VERTEX_3);	// tip evaluator
	gluEndCurve(theNurb);

	// punctele de control
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POINTS);

	// punctele de control
	for (i = 0; i<9; i++)
		glVertex3fv(&ctlpoints[i][0]);
	glEnd();

	//poligonul caracteristic
	glEnable(GL_LINE_STIPPLE);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i<9; i++)
		glVertex3fv(&ctlpoints[i][0]);
	glEnd();

	glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w,
			1.0 * (GLfloat)h / (GLfloat)w);
	else
		gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h,
			1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*  Main Loop
*/
int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_SINGLE | AUX_RGB);
	auxInitPosition(0, 0, 500, 500);
	auxInitWindow((LPCSTR)"Curba B-spline");
	myinit();
	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return(0);
}
