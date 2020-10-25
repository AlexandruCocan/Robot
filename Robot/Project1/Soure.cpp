/*
Eliminarea fetelor functie de orientare
Se va afisa fara activarea eliminarii fetelor. Cu orientarea directa
implicita se vor schimba fetele care se elimina -cele fata sau cele spate-
cu functia glCullFace.
Se va activa sau se va dezactiva testul de adancime
*/
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <math.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);
void CALLBACK corpRotAdd(void);
void CALLBACK corpRotSub(void);

void CALLBACK bratzAddDr(void);
void CALLBACK bratzSubDr(void);
void CALLBACK bratzAddSt(void);
void CALLBACK bratzSubSt(void);
void CALLBACK bratxAddDr(void);
void CALLBACK bratxSubDr(void);
void CALLBACK bratxAddSt(void);
void CALLBACK bratxSubSt(void);

void CALLBACK antebratzAddDr(void);
void CALLBACK antebratzSubDr(void);
void CALLBACK antebratzAddSt(void);
void CALLBACK antebratzSubSt(void);
void CALLBACK antebratxAddDr(void);
void CALLBACK antebratxSubDr(void);
void CALLBACK antebratxAddSt(void);
void CALLBACK antebratxSubSt(void);
void CALLBACK antebratyAddDr(void);
void CALLBACK antebratySubDr(void);
void CALLBACK antebratyAddSt(void);
void CALLBACK antebratySubSt(void);


static int corpRot = 0;
static int bratxDr = 0, bratzDr = 0, bratxSt = 0, bratzSt = 0;
static int antebratxDr = 0, antebratzDr = 0, antebratyDr = 0, antebratxSt = 0, antebratzSt = 0, antebratySt = 0;
boolean umbra = 0;
GLfloat position[] = { 0, 2.5, -0.5, 1.0 };

void myinit(void)
{
	GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 0.3f };
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 22.0f };

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	glClearColor(0.254, 0.901, 0.839, 1.0);
	glEnable(GL_DEPTH_TEST); 
}

void calcCoeficientiPlan(float P[3][3], float coef[4]) {
	float v1[3], v2[3];
	float length;
	static const int x = 0, y = 1, z = 2;

	// calculeaza 2 vectori din 3 pct
	v1[x] = P[0][x] - P[1][x];
	v1[y] = P[0][y] - P[1][y];
	v1[z] = P[0][z] - P[1][z];
	v2[x] = P[1][x] - P[2][x];
	v2[y] = P[1][y] - P[2][y];
	v2[z] = P[1][z] - P[2][z];

	//se calc produsul vectorial al celor 2 vectori => al3lea vector cu componentele A,B,C chiar coef din ec. planului
	coef[x] = v1[y] * v2[z] - v1[z] * v2[y];
	coef[y] = v1[z] * v2[x] - v1[x] * v2[z];
	coef[z] = v1[x] * v2[y] - v1[y] - v2[x];

	//normalizare vector
	length = (float)sqrt(coef[x] * coef[x] + coef[y] * coef[y] + coef[z] * coef[z]);
	coef[x] /= length;
	coef[y] /= length;
	coef[z] /= length;
}

void MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4]) {
	// creeaza matricea care da umbra cunoscandu-se coef planului + poz sursei
	// IN mat SE SALVEAZA MATRICEA

	GLfloat coefPlan[4], temp;

	//determina coef planului
	calcCoeficientiPlan(puncte, coefPlan);

	// determinam D
	coefPlan[3] = -(coefPlan[0] * puncte[2][0] + coefPlan[1] * puncte[2][1] + coefPlan[2] * puncte[2][2]);

	// temp= AxL + ByL + CzL + Dw
	temp = coefPlan[0] * pozSursa[0] + coefPlan[1] * pozSursa[1] + coefPlan[2] * pozSursa[2] + coefPlan[3] * pozSursa[3];

	//prima coloana
	mat[0][0] = temp - coefPlan[0] * pozSursa[0];
	mat[1][0] = 0.0f - coefPlan[1] * pozSursa[0];
	mat[2][0] = 0.0f - coefPlan[2] * pozSursa[0];
	mat[3][0] = 0.0f - coefPlan[3] * pozSursa[0];
	//a 2a coloana
	mat[0][1] = 0.0f - coefPlan[0] * pozSursa[1];
	mat[1][1] = temp - coefPlan[1] * pozSursa[1];
	mat[2][1] = 0.0f - coefPlan[2] * pozSursa[1];
	mat[3][1] = 0.0f - coefPlan[3] * pozSursa[1];
	//a 3a coloana
	mat[0][2] = 0.0f - coefPlan[0] * pozSursa[2];
	mat[1][2] = 0.0f - coefPlan[1] * pozSursa[2];
	mat[2][2] = temp - coefPlan[2] * pozSursa[2];
	mat[3][2] = 0.0f - coefPlan[3] * pozSursa[2];
	//a4a coloana
	mat[0][3] = 0.0f - coefPlan[0] * pozSursa[3];
	mat[1][3] = 0.0f - coefPlan[1] * pozSursa[3];
	mat[2][3] = 0.0f - coefPlan[2] * pozSursa[3];
	mat[3][3] = temp - coefPlan[3] * pozSursa[3];
}


void CALLBACK corpRotSub(void)
{
	corpRot = corpRot - 15;
}

void CALLBACK corpRotAdd(void)
{
	corpRot = corpRot + 15;
}

void CALLBACK bratzAddDr(void)
{
	bratzDr = (bratzDr + 5) % 360;
}

void CALLBACK bratzSubDr(void)
{
	bratzDr = (bratzDr - 5) % 360;
}

void CALLBACK bratzAddSt(void)
{
	bratzSt = (bratzSt + 5) % 360;
}

void CALLBACK bratzSubSt(void)
{
	bratzSt = (bratzSt - 5) % 360;
}

void CALLBACK bratxAddDr(void)
{
	bratxDr = (bratxDr + 5) % 360;
}

void CALLBACK bratxSubDr(void)
{
	bratxDr = (bratxDr - 5) % 360;
}

void CALLBACK bratxAddSt(void)
{
	bratxSt = (bratxSt + 5) % 360;
}

void CALLBACK bratxSubSt(void)
{
	bratxSt = (bratxSt - 5) % 360;
}



void CALLBACK antebratzAddDr(void)
{
	antebratzDr = (antebratzDr + 5) % 360;
}

void CALLBACK antebratzSubDr(void)
{
	antebratzDr = (antebratzDr - 5) % 360;
}

void CALLBACK antebratzAddSt(void)
{
	antebratzSt = (antebratzSt + 5) % 360;
}

void CALLBACK antebratzSubSt(void)
{
	antebratzSt = (antebratzSt - 5) % 360;
}

void CALLBACK antebratxAddDr(void)
{
	antebratxDr = (antebratxDr + 5) % 360;
}

void CALLBACK antebratxSubDr(void)
{
	antebratxDr = (antebratxDr - 5) % 360;
}

void CALLBACK antebratxAddSt(void)
{
	antebratxSt = (antebratxSt + 5) % 360;
}

void CALLBACK antebratxSubSt(void)
{
	antebratxSt = (antebratxSt - 5) % 360;
}

void CALLBACK antebratyAddDr(void)
{
	antebratyDr = (antebratyDr + 5) % 360;
}

void CALLBACK antebratySubDr(void)
{
	antebratyDr = (antebratyDr - 5) % 360;
}

void CALLBACK antebratyAddSt(void)
{
	antebratySt = (antebratySt + 5) % 360;
}

void CALLBACK antebratySubSt(void)
{
	antebratySt = (antebratySt - 5) % 360;
}

void CALLBACK soareDr(void) {
	if (position[0] < 8) {
		position[0] += 0.4;

	}
}
void CALLBACK soareSt(void) {
	if (position[0] > -8) {
		position[0] -= 0.4;

	}
}

void Corp(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.152, 0.552, 0.925);

	glBegin(GL_QUADS);
	glNormal3f(-0.35f, -0.1f, 0.0f);
	glVertex3f(-0.35f, -0.1f, 0.0f);
	glNormal3f(-0.35f, 0.9f, 0.0f);
	glVertex3f(-0.35f, 0.9f, 0.0f);
	glNormal3f(0.35f, 0.9f, 0.0f);
	glVertex3f(0.35f, 0.9f, 0.0f);
	glNormal3f(0.35f, -0.1f, 0.0f);
	glVertex3f(0.35f, -0.1f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.35f, -0.1f, 0.0f);
	glVertex3f(0.35f, -0.1f, 0.0f);
	glNormal3f(0.35f, 0.9f, 0.0f);
	glVertex3f(0.35f, 0.9f, 0.0f);
	glNormal3f(0.35f, 0.9f, 0.5f);
	glVertex3f(0.35f, 0.9f, 0.5f);
	glNormal3f(0.35f, -0.1f, 0.5f);
	glVertex3f(0.35f, -0.1f, 0.5f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.35f, -0.1f, 0.5f);
	glVertex3f(0.35f, -0.1f, 0.5f);
	glNormal3f(0.35f, 0.9f, 0.5f);
	glVertex3f(0.35f, 0.9f, 0.5f);
	glNormal3f(-0.35f, 0.9f, 0.5f);
	glVertex3f(-0.35f, 0.9f, 0.5f);
	glNormal3f(-0.35f, -0.1f, 0.5f);
	glVertex3f(-0.35f, -0.1f, 0.5f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.35f, -0.1f, 0.5f);
	glVertex3f(-0.35f, -0.1f, 0.5f);
	glNormal3f(-0.35f, 0.9f, 0.5f);
	glVertex3f(-0.35f, 0.9f, 0.5f);
	glNormal3f(-0.35f, 0.9f, 0.0f);
	glVertex3f(-0.35f, 0.9f, 0.0f);
	glNormal3f(-0.35f, -0.1f, 0.0f);
	glVertex3f(-0.35f, -0.1f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.35f, 0.9f, 0.0f);
	glVertex3f(-0.35f, 0.9f, 0.0f);
	glNormal3f(-0.35f, 0.9f, 0.5f);
	glVertex3f(-0.35f, 0.9f, 0.5f);
	glNormal3f(0.35f, 0.9f, 0.5f);
	glVertex3f(0.35f, 0.9f, 0.5f);
	glNormal3f(0.35f, 0.9f, 0.0f);
	glVertex3f(0.35f, 0.9f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.35f, -0.1f, 0.0f);
	glVertex3f(-0.35f, -0.1f, 0.0f);
	glNormal3f(-0.35f, -0.1f, 0.5f);
	glVertex3f(-0.35f, -0.1f, 0.5f);
	glNormal3f(0.35f, -0.1f, 0.5f);
	glVertex3f(0.35f, -0.1f, 0.5f);
	glNormal3f(0.35f, -0.1f, 0.0f);
	glVertex3f(0.35f, -0.1f, 0.0f);
	glEnd();
}

void PiciorSt(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.152, 0.552, 0.925);

	glBegin(GL_QUADS);
	glNormal3f(-0.3f, -0.18f, 0.125f);
	glVertex3f(-0.3f, -0.18f, 0.125f);
	glNormal3f(-0.3f, -0.18f, 0.125f);
	glVertex3f(-0.3f, -0.18f, 0.125f);
	glNormal3f(-0.05f, -0.18f, 0.125f);
	glVertex3f(-0.05f, -0.18f, 0.125f);
	glNormal3f(-0.05f, -0.8f, 0.125f);
	glVertex3f(-0.05f, -0.8f, 0.125f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.05f, -0.8f, 0.125f);
	glVertex3f(-0.05f, -0.8f, 0.125f);
	glNormal3f(-0.05f, -0.18f, 0.125f);
	glVertex3f(-0.05f, -0.18f, 0.125f);
	glNormal3f(-0.05f, -0.18f, 0.375f);
	glVertex3f(-0.05f, -0.18f, 0.375f);
	glNormal3f(-0.05f, -0.8f, 0.375f);
	glVertex3f(-0.05f, -0.8f, 0.375f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.05f, -0.8f, 0.375f);
	glVertex3f(-0.05f, -0.8f, 0.375f);
	glNormal3f(-0.05f, -0.18f, 0.375f);
	glVertex3f(-0.05f, -0.18f, 0.375f);
	glNormal3f(-0.3f, -0.18f, 0.375f);
	glVertex3f(-0.3f, -0.18f, 0.375f);
	glNormal3f(-0.3f, -0.8f, 0.375f);
	glVertex3f(-0.3f, -0.8f, 0.375f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.3f, -0.8f, 0.375f);
	glVertex3f(-0.3f, -0.8f, 0.375f);
	glNormal3f(-0.3f, -0.18f, 0.375f);
	glVertex3f(-0.3f, -0.18f, 0.375f);
	glNormal3f(-0.3f, -0.18f, 0.125f);
	glVertex3f(-0.3f, -0.18f, 0.125f);
	glNormal3f(-0.3f, -0.8f, 0.125f);
	glVertex3f(-0.3f, -0.8f, 0.125f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.3f, -0.8f, 0.125f);
	glVertex3f(-0.3f, -0.8f, 0.125f);
	glNormal3f(-0.3f, -0.8f, 0.375f);
	glVertex3f(-0.3f, -0.8f, 0.375f);
	glNormal3f(-0.05f, -0.8f, 0.375f);
	glVertex3f(-0.05f, -0.8f, 0.375f);
	glNormal3f(-0.05f, -0.8f, 0.125f);
	glVertex3f(-0.05f, -0.8f, 0.125f);
	glEnd();

}

void PiciorDr(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.152, 0.552, 0.925);

	glBegin(GL_QUADS); 
	glNormal3f(0.3f, -0.18f, 0.125f);
	glVertex3f(0.3f, -0.18f, 0.125f);
	glNormal3f(0.3f, -0.18f, 0.125f);
	glVertex3f(0.3f, -0.18f, 0.125f);
	glNormal3f(0.05f, -0.18f, 0.125f);
	glVertex3f(0.05f, -0.18f, 0.125f);
	glNormal3f(0.05f, -0.8f, 0.125f);
	glVertex3f(0.05f, -0.8f, 0.125f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.05f, -0.8f, 0.125f);
	glVertex3f(0.05f, -0.8f, 0.125f);
	glNormal3f(0.05f, -0.18f, 0.125f);
	glVertex3f(0.05f, -0.18f, 0.125f);
	glNormal3f(0.05f, -0.18f, 0.375f);
	glVertex3f(0.05f, -0.18f, 0.375f);
	glNormal3f(0.05f, -0.8f, 0.375f);
	glVertex3f(0.05f, -0.8f, 0.375f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.05f, -0.8f, 0.375f);
	glVertex3f(0.05f, -0.8f, 0.375f);
	glNormal3f(0.05f, -0.18f, 0.375f);
	glVertex3f(0.05f, -0.18f, 0.375f);
	glNormal3f(0.3f, -0.18f, 0.375f);
	glVertex3f(0.3f, -0.18f, 0.375f);
	glNormal3f(0.3f, -0.8f, 0.375f);
	glVertex3f(0.3f, -0.8f, 0.375f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.3f, -0.8f, 0.375f);
	glVertex3f(0.3f, -0.8f, 0.375f);
	glNormal3f(0.3f, -0.18f, 0.375f);
	glVertex3f(0.3f, -0.18f, 0.375f);
	glNormal3f(0.3f, -0.18f, 0.125f);
	glVertex3f(0.3f, -0.18f, 0.125f);
	glNormal3f(0.3f, -0.8f, 0.125f);
	glVertex3f(0.3f, -0.8f, 0.125f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.3f, -0.8f, 0.125f);
	glVertex3f(0.3f, -0.8f, 0.125f);
	glNormal3f(0.3f, -0.8f, 0.375f);
	glVertex3f(0.3f, -0.8f, 0.375f);
	glNormal3f(0.05f, -0.8f, 0.375f);
	glVertex3f(0.05f, -0.8f, 0.375f);
	glNormal3f(0.05f, -0.8f, 0.125f);
	glVertex3f(0.05f, -0.8f, 0.125f);
	glEnd();
	glEnd();

}

void PapucSt(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.133, 0.305, 0.466);

	glBegin(GL_QUADS);
	glNormal3f(-0.33f, -1.0f, 0.075f);
	glVertex3f(-0.33f, -1.0f, 0.075f);
	glNormal3f(-0.33f, -0.8f, 0.075f);
	glVertex3f(-0.33f, -0.8f, 0.075f);
	glNormal3f(-0.02f, -0.8f, 0.075f);
	glVertex3f(-0.02f, -0.8f, 0.075f);
	glNormal3f(-0.02f, -1.0f, 0.075f);
	glVertex3f(-0.02f, -1.0f, 0.075f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.02f, -1.0f, 0.075f);
	glVertex3f(-0.02f, -1.0f, 0.075f);
	glNormal3f(-0.02f, -0.8f, 0.075f);
	glVertex3f(-0.02f, -0.8f, 0.075f);
	glNormal3f(-0.02f, -0.8f, 0.425f);
	glVertex3f(-0.02f, -0.8f, 0.425f);
	glNormal3f(-0.02f, -1.0f, 0.425f);
	glVertex3f(-0.02f, -1.0f, 0.425f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.02f, -1.0f, 0.425f);
	glVertex3f(-0.02f, -1.0f, 0.425f);
	glNormal3f(-0.02f, -0.8f, 0.425f);
	glVertex3f(-0.02f, -0.8f, 0.425f);
	glNormal3f(-0.33f, -0.8f, 0.425f);
	glVertex3f(-0.33f, -0.8f, 0.425f);
	glNormal3f(-0.33f, -1.0f, 0.425f);
	glVertex3f(-0.33f, -1.0f, 0.425f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.33f, -1.0f, 0.425f);
	glVertex3f(-0.33f, -1.0f, 0.425f);
	glNormal3f(-0.33f, -0.8f, 0.425f);
	glVertex3f(-0.33f, -0.8f, 0.425f);
	glNormal3f(-0.33f, -0.8f, 0.075f);
	glVertex3f(-0.33f, -0.8f, 0.075f);
	glNormal3f(-0.33f, -1.0f, 0.075f);
	glVertex3f(-0.33f, -1.0f, 0.075f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.33f, -0.8f, 0.075f);
	glVertex3f(-0.33f, -0.8f, 0.075f);
	glNormal3f(-0.33f, -0.8f, 0.425f);
	glVertex3f(-0.33f, -0.8f, 0.425f);
	glNormal3f(-0.02f, -0.8f, 0.425f);
	glVertex3f(-0.02f, -0.8f, 0.425f);
	glNormal3f(-0.02f, -0.8f, 0.075f);
	glVertex3f(-0.02f, -0.8f, 0.075f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.33f, -1.0f, 0.075f);
	glVertex3f(-0.33f, -1.0f, 0.075f);
	glNormal3f(-0.33f, -1.0f, 0.425f);
	glVertex3f(-0.33f, -1.0f, 0.425f);
	glNormal3f(-0.02f, -1.0f, 0.425f);
	glVertex3f(-0.02f, -1.0f, 0.425f);
	glNormal3f(-0.02f, -1.0f, 0.075f);
	glVertex3f(-0.02f, -1.0f, 0.075f);
	glEnd();
}

void PapucDr(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.133, 0.305, 0.466);

	glBegin(GL_QUADS);
	glNormal3f(0.33f, -1.0f, 0.075f);
	glVertex3f(0.33f, -1.0f, 0.075f);
	glNormal3f(0.33f, -0.8f, 0.075f);
	glVertex3f(0.33f, -0.8f, 0.075f);
	glNormal3f(0.02f, -0.8f, 0.075f);
	glVertex3f(0.02f, -0.8f, 0.075f);
	glNormal3f(0.02f, -1.0f, 0.075f);
	glVertex3f(0.02f, -1.0f, 0.075f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.02f, -1.0f, 0.075f);
	glVertex3f(0.02f, -1.0f, 0.075f);
	glNormal3f(0.02f, -0.8f, 0.075f);
	glVertex3f(0.02f, -0.8f, 0.075f);
	glNormal3f(0.02f, -0.8f, 0.425f);
	glVertex3f(0.02f, -0.8f, 0.425f);
	glNormal3f(0.02f, -1.0f, 0.425f);
	glVertex3f(0.02f, -1.0f, 0.425f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.02f, -1.0f, 0.425f);
	glVertex3f(0.02f, -1.0f, 0.425f);
	glNormal3f(0.02f, -0.8f, 0.425f);
	glVertex3f(0.02f, -0.8f, 0.425f);
	glNormal3f(0.33f, -0.8f, 0.425f);
	glVertex3f(0.33f, -0.8f, 0.425f);
	glNormal3f(0.33f, -1.0f, 0.425f);
	glVertex3f(0.33f, -1.0f, 0.425f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.33f, -1.0f, 0.425f);
	glVertex3f(0.33f, -1.0f, 0.425f);
	glNormal3f(0.33f, -0.8f, 0.425f);
	glVertex3f(0.33f, -0.8f, 0.425f);
	glNormal3f(0.33f, -0.8f, 0.075f);
	glVertex3f(0.33f, -0.8f, 0.075f);
	glNormal3f(0.33f, -1.0f, 0.075f);
	glVertex3f(0.33f, -1.0f, 0.075f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.33f, -0.8f, 0.075f);
	glVertex3f(0.33f, -0.8f, 0.075f);
	glNormal3f(0.33f, -0.8f, 0.425f);
	glVertex3f(0.33f, -0.8f, 0.425f);
	glNormal3f(0.02f, -0.8f, 0.425f);
	glVertex3f(0.02f, -0.8f, 0.425f);
	glNormal3f(0.02f, -0.8f, 0.075f);
	glVertex3f(0.02f, -0.8f, 0.075f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.33f, -1.0f, 0.075f);
	glVertex3f(0.33f, -1.0f, 0.075f);
	glNormal3f(0.33f, -1.0f, 0.425f);
	glVertex3f(0.33f, -1.0f, 0.425f);
	glNormal3f(0.02f, -1.0f, 0.425f);
	glVertex3f(0.02f, -1.0f, 0.425f);
	glNormal3f(0.02f, -1.0f, 0.075f);
	glVertex3f(0.02f, -1.0f, 0.075f);
	glEnd();
}

void Gat(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.980, 0.788, 0.019);

	glBegin(GL_QUADS);
	glNormal3f(-0.15f, 0.9f, 0.1f);
	glVertex3f(-0.15f, 0.9f, 0.1f);
	glNormal3f(-0.15f, 1.0f, 0.1f);
	glVertex3f(-0.15f, 1.0f, 0.1f);
	glNormal3f(0.15f, 1.0f, 0.1f);
	glVertex3f(0.15f, 1.0f, 0.1f);
	glNormal3f(0.15f, 0.9f, 0.1f);
	glVertex3f(0.15f, 0.9f, 0.1f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.15f, 0.9f, 0.1f);
	glVertex3f(0.15f, 0.9f, 0.1f);
	glNormal3f(0.15f, 1.0f, 0.1f);
	glVertex3f(0.15f, 1.0f, 0.1f);
	glNormal3f(0.15f, 1.0f, 0.4f);
	glVertex3f(0.15f, 1.0f, 0.4f);
	glNormal3f(0.15f, 0.9f, 0.4f);
	glVertex3f(0.15f, 0.9f, 0.4f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.15f, 0.9f, 0.4f);
	glVertex3f(0.15f, 0.9f, 0.4f);
	glNormal3f(0.15f, 1.0f, 0.4f);
	glVertex3f(0.15f, 1.0f, 0.4f);
	glNormal3f(-0.15f, 1.0f, 0.4f);
	glVertex3f(-0.15f, 1.0f, 0.4f);
	glNormal3f(-0.15f, 0.9f, 0.4f);
	glVertex3f(-0.15f, 0.9f, 0.4f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.15f, 0.9f, 0.4f);
	glVertex3f(-0.15f, 0.9f, 0.4f);
	glNormal3f(-0.15f, 1.0f, 0.4f);
	glVertex3f(-0.15f, 1.0f, 0.4f);
	glNormal3f(-0.15f, 1.0f, 0.1f);
	glVertex3f(-0.15f, 1.0f, 0.1f);
	glNormal3f(-0.15f, 0.9f, 0.1f);
	glVertex3f(-0.15f, 0.9f, 0.1f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.15f, 1.0f, 0.1f);
	glVertex3f(-0.15f, 1.0f, 0.1f);
	glNormal3f(-0.15f, 1.0f, 0.4f);
	glVertex3f(-0.15f, 1.0f, 0.4f);
	glNormal3f(0.15f, 1.0f, 0.4f);
	glVertex3f(0.15f, 1.0f, 0.4f);
	glNormal3f(0.15f, 1.0f, 0.1f);
	glVertex3f(0.15f, 1.0f, 0.1f);
	glEnd();
}

void Cap(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.980, 0.788, 0.019);

	glBegin(GL_QUADS);
	glNormal3f(-0.30f, 1.0f, 0.05f);
	glVertex3f(-0.30f, 1.0f, 0.05f);
	glNormal3f(-0.30f, 1.45f, 0.05f);
	glVertex3f(-0.30f, 1.45f, 0.05f);
	glNormal3f(0.30f, 1.45f, 0.05f);
	glVertex3f(0.30f, 1.45f, 0.05f);
	glNormal3f(0.30f, 1.0f, 0.05f);
	glVertex3f(0.30f, 1.0f, 0.05f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.30f, 1.0f, 0.05f);
	glVertex3f(0.30f, 1.0f, 0.05f);
	glNormal3f(0.30f, 1.45f, 0.05f);
	glVertex3f(0.30f, 1.45f, 0.05f);
	glNormal3f(0.30f, 1.45f, 0.45f);
	glVertex3f(0.30f, 1.45f, 0.45f);
	glNormal3f(0.30f, 1.0f, 0.45f);
	glVertex3f(0.30f, 1.0f, 0.45f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.30f, 1.0f, 0.4f);
	glVertex3f(0.30f, 1.0f, 0.4f);
	glNormal3f(0.30f, 1.45f, 0.4f);
	glVertex3f(0.30f, 1.45f, 0.4f);
	glNormal3f(-0.30f, 1.45f, 0.4f);
	glVertex3f(-0.30f, 1.45f, 0.4f);
	glNormal3f(-0.30f, 1.0f, 0.4f);
	glVertex3f(-0.30f, 1.0f, 0.4f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.30f, 1.0f, 0.45f);
	glVertex3f(-0.30f, 1.0f, 0.45f);
	glNormal3f(-0.30f, 1.45f, 0.45f);
	glVertex3f(-0.30f, 1.45f, 0.45f);
	glNormal3f(-0.30f, 1.45f, 0.05f);
	glVertex3f(-0.30f, 1.45f, 0.05f);
	glNormal3f(-0.30f, 1.0f, 0.05f);
	glVertex3f(-0.30f, 1.0f, 0.05f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.30f, 1.45f, 0.05f);
	glVertex3f(-0.30f, 1.45f, 0.05f);
	glNormal3f(-0.30f, 1.45f, 0.45f);
	glVertex3f(-0.30f, 1.45f, 0.45f);
	glNormal3f(0.30f, 1.45f, 0.45f);
	glVertex3f(0.30f, 1.45f, 0.45f);
	glNormal3f(0.30f, 1.45f, 0.05f);
	glVertex3f(0.30f, 1.45f, 0.05f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.30f, 1.0f, 0.05f);
	glVertex3f(-0.30f, 1.0f, 0.05f);
	glNormal3f(-0.30f, 1.0f, 0.45f);
	glVertex3f(-0.30f, 1.0f, 0.45f);
	glNormal3f(0.30f, 1.0f, 0.45f);
	glVertex3f(0.30f, 1.0f, 0.45f);
	glNormal3f(0.30f, 1.0f, 0.05f);
	glVertex3f(0.30f, 1.0f, 0.05f);
	glEnd();
}

void UrecheSt(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.913, 0.627, 0.047);

	glBegin(GL_QUADS);
	glNormal3f(-0.35f, 1.15f, 0.15f);
	glVertex3f(-0.35f, 1.15f, 0.15f);
	glNormal3f(-0.35f, 1.3f, 0.15f);
	glVertex3f(-0.35f, 1.3f, 0.15f);
	glNormal3f(-0.30f, 1.3f, 0.15f);
	glVertex3f(-0.30f, 1.3f, 0.15f);
	glNormal3f(-0.30f, 1.15f, 0.15f);
	glVertex3f(-0.30f, 1.15f, 0.15f);
	glEnd();

	//glBegin(GL_QUADS);
	//glVertex3f(-0.30f, 1.0f, 0.05f);
	//glVertex3f(-0.30f, 1.45f, 0.05f);
	//glVertex3f(0.30f, 1.45f, 0.35f);
	//glVertex3f(0.30f, 1.0f, 0.35f);
	//glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.30f, 1.15f, 0.35f);
	glVertex3f(-0.30f, 1.15f, 0.35f);
	glNormal3f(-0.30f, 1.35f, 0.35f);
	glVertex3f(-0.30f, 1.35f, 0.35f);
	glNormal3f(-0.35f, 1.35f, 0.35f);
	glVertex3f(-0.35f, 1.35f, 0.35f);
	glNormal3f(-0.35f, 1.15f, 0.35f);
	glVertex3f(-0.35f, 1.15f, 0.35f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.35f, 1.15f, 0.35f);
	glVertex3f(-0.35f, 1.15f, 0.35f);
	glNormal3f(-0.35f, 1.35f, 0.35f);
	glVertex3f(-0.35f, 1.35f, 0.35f);
	glNormal3f(-0.35f, 1.35f, 0.15f);
	glVertex3f(-0.35f, 1.35f, 0.15f);
	glNormal3f(-0.35f, 1.15f, 0.15f);
	glVertex3f(-0.35f, 1.15f, 0.15f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.35f, 1.35f, 0.15f);
	glVertex3f(-0.35f, 1.35f, 0.15f);
	glNormal3f(-0.35f, 1.35f, 0.35f);
	glVertex3f(-0.35f, 1.35f, 0.35f);
	glNormal3f(-0.30f, 1.35f, 0.35f);
	glVertex3f(-0.30f, 1.35f, 0.35f);
	glNormal3f(-0.30f, 1.35f, 0.15f);
	glVertex3f(-0.30f, 1.35f, 0.15f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.35f, 1.15f, 0.15f);
	glVertex3f(-0.35f, 1.15f, 0.15f);
	glNormal3f(-0.35f, 1.15f, 0.35f);
	glVertex3f(-0.35f, 1.15f, 0.35f);
	glNormal3f(-0.30f, 1.15f, 0.35f);
	glVertex3f(-0.30f, 1.15f, 0.35f);
	glNormal3f(-0.30f, 1.15f, 0.15f);
	glVertex3f(-0.30f, 1.15f, 0.15f);
	glEnd();
}

void UrecheDr(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.913, 0.627, 0.047);

	glBegin(GL_QUADS);
	glNormal3f(0.30f, 1.15f, 0.15f);
	glVertex3f(0.30f, 1.15f, 0.15f);
	glNormal3f(0.30f, 1.35f, 0.15f);
	glVertex3f(0.30f, 1.35f, 0.15f);
	glNormal3f(0.35f, 1.35f, 0.15f);
	glVertex3f(0.35f, 1.35f, 0.15f);
	glNormal3f(0.35f, 1.15f, 0.15f);
	glVertex3f(0.35f, 1.15f, 0.15f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.35f, 1.15f, 0.15f);
	glVertex3f(0.35f, 1.15f, 0.15f);
	glNormal3f(0.35f, 1.35f, 0.15f);
	glVertex3f(0.35f, 1.35f, 0.15f);
	glNormal3f(0.35f, 1.35f, 0.35f);
	glVertex3f(0.35f, 1.35f, 0.35f);
	glNormal3f(0.35f, 1.15f, 0.35f);
	glVertex3f(0.35f, 1.15f, 0.35f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.35f, 1.15f, 0.35f);
	glVertex3f(0.35f, 1.15f, 0.35f);
	glNormal3f(0.35f, 1.35f, 0.35f);
	glVertex3f(0.35f, 1.35f, 0.35f);
	glNormal3f(0.30f, 1.35f, 0.35f);
	glVertex3f(0.30f, 1.35f, 0.35f);
	glNormal3f(0.30f, 1.15f, 0.35f);
	glVertex3f(0.30f, 1.15f, 0.35f);
	glEnd();

	/*glBegin(GL_QUADS);
	glVertex3f(0.35f, 1.15f, 0.35f);
	glVertex3f(0.35f, 1.35f, 0.35f);
	glVertex3f(0.30f, 1.35f, 0.15f);
	glVertex3f(0.30f, 1.15f, 0.15f);
	glEnd();*/

	glBegin(GL_QUADS);
	glNormal3f(0.30f, 1.35f, 0.15f);
	glVertex3f(0.30f, 1.35f, 0.15f);
	glNormal3f(0.30f, 1.35f, 0.35f);
	glVertex3f(0.30f, 1.35f, 0.35f);
	glNormal3f(0.35f, 1.35f, 0.35f);
	glVertex3f(0.35f, 1.35f, 0.35f);
	glNormal3f(0.35f, 1.35f, 0.15f);
	glVertex3f(0.35f, 1.35f, 0.15f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.30f, 1.15f, 0.15f);
	glVertex3f(0.30f, 1.15f, 0.15f);
	glNormal3f(0.30f, 1.15f, 0.35f);
	glVertex3f(0.30f, 1.15f, 0.35f);
	glNormal3f(0.35f, 1.15f, 0.35f);
	glVertex3f(0.35f, 1.15f, 0.35f);
	glNormal3f(0.35f, 1.15f, 0.15f);
	glVertex3f(0.35f, 1.15f, 0.15f);
	glEnd();
}

void Gura(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.921, 0.070, 0.058);

	glBegin(GL_QUADS);
	glNormal3f(-0.10f, 1.1f, 0.45f);
	glVertex3f(-0.10f, 1.1f, 0.45f);
	glNormal3f(-0.10f, 1.2f, 0.45f);
	glVertex3f(-0.10f, 1.2f, 0.45f);
	glNormal3f(0.10f, 1.2f, 0.45f);
	glVertex3f(0.10f, 1.2f, 0.45f);
	glNormal3f(0.10f, 1.1f, 0.45f);
	glVertex3f(0.10f, 1.1f, 0.45f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.10f, 1.1f, 0.45f);
	glVertex3f(0.10f, 1.1f, 0.45f);
	glNormal3f(0.10f, 1.2f, 0.45f);
	glVertex3f(0.10f, 1.2f, 0.45f);
	glNormal3f(0.10f, 1.2f, 0.50f);
	glVertex3f(0.10f, 1.2f, 0.50f);
	glNormal3f(0.10f, 1.1f, 0.50f);
	glVertex3f(0.10f, 1.1f, 0.50f);
	glEnd();

	/*glBegin(GL_QUADS);
	glVertex3f(0.30f, 1.0f, 0.4f);
	glVertex3f(0.30f, 1.45f, 0.4f);
	glVertex3f(-0.30f, 1.45f, 0.4f);
	glVertex3f(-0.30f, 1.0f, 0.4f);
	glEnd();*/

	glBegin(GL_QUADS);
	glNormal3f(-0.10f, 1.1f, 0.50f);
	glVertex3f(-0.10f, 1.1f, 0.50f);
	glNormal3f(-0.10f, 1.2f, 0.50f);
	glVertex3f(-0.10f, 1.2f, 0.50f);
	glNormal3f(-0.10f, 1.2f, 0.45f);
	glVertex3f(-0.10f, 1.2f, 0.45f);
	glNormal3f(-0.10f, 1.1f, 0.45f);
	glVertex3f(-0.10f, 1.1f, 0.45f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.10f, 1.2f, 0.45f);
	glVertex3f(-0.10f, 1.2f, 0.45f);
	glNormal3f(-0.10f, 1.2f, 0.50f);
	glVertex3f(-0.10f, 1.2f, 0.50f);
	glNormal3f(0.10f, 1.2f, 0.50f);
	glVertex3f(0.10f, 1.2f, 0.50f);
	glNormal3f(0.10f, 1.2f, 0.45f);
	glVertex3f(0.10f, 1.2f, 0.45f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.10f, 1.1f, 0.45f);
	glVertex3f(-0.10f, 1.1f, 0.45f);
	glNormal3f(-0.10f, 1.1f, 0.50f);
	glVertex3f(-0.10f, 1.1f, 0.50f);
	glNormal3f(0.10f, 1.1f, 0.50f);
	glVertex3f(0.10f, 1.1f, 0.50f);
	glNormal3f(0.10f, 1.1f, 0.45f);
	glVertex3f(0.10f, 1.1f, 0.45f);
	glEnd();
}

void CapTop(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.980, 0.788, 0.019);

	glBegin(GL_QUADS);
	glNormal3f(-0.20f, 1.45f, 0.1f);
	glVertex3f(-0.20f, 1.45f, 0.1f);
	glNormal3f(-0.20f, 1.5f, 0.1f);
	glVertex3f(-0.20f, 1.5f, 0.1f);
	glNormal3f(0.20f, 1.5f, 0.1f);
	glVertex3f(0.20f, 1.5f, 0.1f);
	glNormal3f(0.20f, 1.45f, 0.1f);
	glVertex3f(0.20f, 1.45f, 0.1f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.20f, 1.45f, 0.1f);
	glVertex3f(0.20f, 1.45f, 0.1f);
	glNormal3f(0.20f, 1.5f, 0.1f);
	glVertex3f(0.20f, 1.5f, 0.1f);
	glNormal3f(0.20f, 1.5f, 0.4f);
	glVertex3f(0.20f, 1.5f, 0.4f);
	glNormal3f(0.20f, 1.45f, 0.4f);
	glVertex3f(0.20f, 1.45f, 0.4f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.20f, 1.45f, 0.4f);
	glVertex3f(0.20f, 1.45f, 0.4f);
	glNormal3f(0.20f, 1.5f, 0.4f);
	glVertex3f(0.20f, 1.5f, 0.4f);
	glNormal3f(-0.20f, 1.5f, 0.4f);
	glVertex3f(-0.20f, 1.5f, 0.4f);
	glNormal3f(-0.20f, 1.45f, 0.4f);
	glVertex3f(-0.20f, 1.45f, 0.4f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.20f, 1.45f, 0.4f);
	glVertex3f(-0.20f, 1.45f, 0.4f);
	glNormal3f(-0.20f, 1.5f, 0.4f);
	glVertex3f(-0.20f, 1.5f, 0.4f);
	glNormal3f(-0.20f, 1.5f, 0.1f);
	glVertex3f(-0.20f, 1.5f, 0.1f);
	glNormal3f(-0.20f, 1.45f, 0.1f);
	glVertex3f(-0.20f, 1.45f, 0.1f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.20f, 1.5f, 0.1f);
	glVertex3f(-0.20f, 1.5f, 0.1f);
	glNormal3f(-0.20f, 1.5f, 0.4f);
	glVertex3f(-0.20f, 1.5f, 0.4f);
	glNormal3f(0.20f, 1.5f, 0.4f);
	glVertex3f(0.20f, 1.5f, 0.4f);
	glNormal3f(0.20f, 1.5f, 0.1f);
	glVertex3f(0.20f, 1.5f, 0.1f);
	glEnd();

	/*glBegin(GL_QUADS);
	glVertex3f(-0.30f, 1.0f, 0.05f);
	glVertex3f(-0.30f, 1.0f, 0.45f);
	glVertex3f(0.30f, 1.0f, 0.45f);
	glVertex3f(0.30f, 1.0f, 0.05f);
	glEnd();*/
}

void OchiSt(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.121, 0.019, 0.019);

	glTranslatef(-0.13, 1.35, 0.40);
	auxSolidSphere(0.08);
	glPopMatrix();
}

void CercOchiSt(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.988, 0.890, 0.890);

	glTranslatef(-0.13, 1.33, 0.45);
	auxSolidSphere(0.035);
	glPopMatrix();
}

void OchiDr(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.121, 0.019, 0.019);

	glTranslatef(0.13, 1.35, 0.40);
	auxSolidSphere(0.08);
	glPopMatrix();
}

void CercOchiDr(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.988, 0.890, 0.890);
	glTranslatef(0.13, 1.33, 0.45);
	auxSolidSphere(0.035);
	glPopMatrix();
}

void CercManaSt(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.454, 0.545, 0.925);

	glTranslatef(-0.38, 0.78, 0.25);
	auxSolidSphere(0.11);
	glPopMatrix();
}

void CercManaDr(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.454, 0.545, 0.925);

	glTranslatef(0.38, 0.78, 0.25);
	auxSolidSphere(0.11);
	glPopMatrix();
}

void CercAntebratSt(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.454, 0.545, 0.925);

	glTranslatef(-0.08, -0.32, 0.0);
	auxSolidSphere(0.09);
	glPopMatrix();
}

void CercAntebratDr(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.454, 0.545, 0.925);

	glTranslatef(0.08, -0.32, 0.0);
	auxSolidSphere(0.09);
	glPopMatrix();
}

void CercPiciorSt(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.454, 0.545, 0.925);

	glTranslatef(-0.175, -0.125, 0.25);
	auxSolidSphere(0.11);
	glPopMatrix();
}

void CercPiciorDr(void) {

	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.454, 0.545, 0.925);

	glTranslatef(0.175, -0.125, 0.25);
	auxSolidSphere(0.11);
	glPopMatrix();
}

void BratSt(void) {
	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.078, 0.423, 0.741);

	glTranslatef(-0.08, -0.08, 0.0);
	auxSolidBox(0.18, 0.43, 0.25);
	glPopMatrix();
}

void BratDr(void) {
	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.078, 0.423, 0.741);

	glTranslatef(0.08, -0.08, 0.0);
	auxSolidBox(0.18, 0.43, 0.25);
	glPopMatrix();
}

void AnteBratSt(void) {
	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.078, 0.423, 0.741);

	glTranslatef(0.0, -0.23, 0.0);
	auxSolidBox(0.18, 0.43, 0.25);
	glPopMatrix();
}

void AnteBratDr(void) {
	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.078, 0.423, 0.741);

	glTranslatef(0.0, -0.23, 0.0);
	auxSolidBox(0.18, 0.43, 0.25);
	glPopMatrix();
}

void ManaSt(void) {
	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.980, 0.788, 0.019);

	glTranslatef(0.0, -0.5, 0.0);
	auxSolidBox(0.24, 0.18, 0.30);
	glPopMatrix();
}

void ManaDr(void) {
	glPushMatrix();
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.980, 0.788, 0.019);

	glTranslatef(0.0, -0.5, 0.0);
	auxSolidBox(0.24, 0.18, 0.30);
	glPopMatrix();
}

void BuzunarSt(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.964, 0.545, 0.333);

	glBegin(GL_QUADS);
	glNormal3f(-0.2f, 0.0f, 0.5f);
	glVertex3f(-0.2f, 0.0f, 0.5f);
	glNormal3f(-0.2f, 0.3f, 0.5f);
	glVertex3f(-0.2f, 0.3f, 0.5f);
	glNormal3f(-0.05f, 0.3f, 0.5f);
	glVertex3f(-0.05f, 0.3f, 0.5f);
	glNormal3f(-0.05f, 0.0f, 0.5f);
	glVertex3f(-0.05f, 0.0f, 0.5f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.05f, 0.0f, 0.5f);
	glVertex3f(-0.05f, 0.0f, 0.5f);
	glNormal3f(-0.05f, 0.3f, 0.5f);
	glVertex3f(-0.05f, 0.3f, 0.5f);
	glNormal3f(-0.05f, 0.3f, 0.525f);
	glVertex3f(-0.05f, 0.3f, 0.525f);
	glNormal3f(-0.05f, 0.0f, 0.525f);
	glVertex3f(-0.05f, 0.0f, 0.525f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.05f, 0.0f, 0.525f);
	glVertex3f(-0.05f, 0.0f, 0.525f);
	glNormal3f(-0.05f, 0.3f, 0.525f);
	glVertex3f(-0.05f, 0.3f, 0.525f);
	glNormal3f(-0.3f, 0.3f, 0.525f);
	glVertex3f(-0.3f, 0.3f, 0.525f);
	glNormal3f(-0.3f, 0.0f, 0.525f);
	glVertex3f(-0.3f, 0.0f, 0.525f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.3f, 0.0f, 0.525f);
	glVertex3f(-0.3f, 0.0f, 0.525f);
	glNormal3f(-0.3f, 0.3f, 0.525f);
	glVertex3f(-0.3f, 0.3f, 0.525f);
	glNormal3f(-0.3f, 0.3f, 0.5f);
	glVertex3f(-0.3f, 0.3f, 0.5f);
	glNormal3f(-0.3f, 0.0f, 0.5f);
	glVertex3f(-0.3f, 0.0f, 0.5f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.3f, 0.3f, 0.5f);
	glVertex3f(-0.3f, 0.3f, 0.5f);
	glNormal3f(-0.3f, 0.3f, 0.5f);
	glVertex3f(-0.3f, 0.3f, 0.5f);
	glNormal3f(-0.05f, 0.3f, 0.525f);
	glVertex3f(-0.05f, 0.3f, 0.525f);
	glNormal3f(-0.05f, 0.3f, 0.525f);
	glVertex3f(-0.05f, 0.3f, 0.525f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-0.3f, 0.0f, 0.5f);
	glVertex3f(-0.3f, 0.0f, 0.5f);
	glNormal3f(-0.3f, 0.0f, 0.5f);
	glVertex3f(-0.3f, 0.0f, 0.5f);
	glNormal3f(-0.05f, 0.0f, 0.525f);
	glVertex3f(-0.05f, 0.0f, 0.525f);
	glNormal3f(-0.05f, 0.0f, 0.525f);
	glVertex3f(-0.05f, 0.0f, 0.525f);
	glEnd();
}

void BuzunarDr(void) {
	if (umbra == 1)
		glColor3f(0.05, 0.05, 0.05);
	else
		glColor3f(0.952, 0.360, 0.070);

	glBegin(GL_QUADS);
	glNormal3f(0.2f, 0.0f, 0.5f);
	glVertex3f(0.2f, 0.0f, 0.5f);
	glNormal3f(0.2f, 0.3f, 0.5f);
	glVertex3f(0.2f, 0.3f, 0.5f);
	glNormal3f(0.05f, 0.3f, 0.5f);
	glVertex3f(0.05f, 0.3f, 0.5f);
	glNormal3f(0.05f, 0.0f, 0.5f);
	glVertex3f(0.05f, 0.0f, 0.5f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.05f, 0.0f, 0.5f);
	glVertex3f(0.05f, 0.0f, 0.5f);
	glNormal3f(0.05f, 0.3f, 0.5f);
	glVertex3f(0.05f, 0.3f, 0.5f);
	glNormal3f(0.05f, 0.3f, 0.525f);
	glVertex3f(0.05f, 0.3f, 0.525f);
	glNormal3f(0.05f, 0.0f, 0.525f);
	glVertex3f(0.05f, 0.0f, 0.525f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.05f, 0.0f, 0.525f);
	glVertex3f(0.05f, 0.0f, 0.525f);
	glNormal3f(0.05f, 0.3f, 0.525f);
	glVertex3f(0.05f, 0.3f, 0.525f);
	glNormal3f(0.3f, 0.3f, 0.525f);
	glVertex3f(0.3f, 0.3f, 0.525f);
	glNormal3f(0.3f, 0.0f, 0.525f);
	glVertex3f(0.3f, 0.0f, 0.525f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.3f, 0.0f, 0.525f);
	glVertex3f(0.3f, 0.0f, 0.525f);
	glNormal3f(0.3f, 0.3f, 0.525f);
	glVertex3f(0.3f, 0.3f, 0.525f);
	glNormal3f(0.3f, 0.3f, 0.5f);
	glVertex3f(0.3f, 0.3f, 0.5f);
	glNormal3f(0.3f, 0.0f, 0.5f);
	glVertex3f(0.3f, 0.0f, 0.5f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.3f, 0.3f, 0.5f);
	glVertex3f(0.3f, 0.3f, 0.5f);
	glNormal3f(0.3f, 0.3f, 0.5f);
	glVertex3f(0.3f, 0.3f, 0.5f);
	glNormal3f(0.05f, 0.3f, 0.525f);
	glVertex3f(0.05f, 0.3f, 0.525f);
	glNormal3f(0.05f, 0.3f, 0.525f);
	glVertex3f(0.05f, 0.3f, 0.525f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.3f, 0.0f, 0.5f);
	glVertex3f(0.3f, 0.0f, 0.5f);
	glNormal3f(0.3f, 0.0f, 0.5f);
	glVertex3f(0.3f, 0.0f, 0.5f);
	glNormal3f(0.05f, 0.0f, 0.525f);
	glVertex3f(0.05f, 0.0f, 0.525f);
	glNormal3f(0.05f, 0.0f, 0.525f);
	glVertex3f(0.05f, 0.0f, 0.525f);
	glEnd();
}

void Iarba(void)  {
	glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	glNormal3f(-0.25f, -0.25f, -0.25f);
	glVertex3f(-7.0f, -0.05f, -0.8f);
	glNormal3f(0.25f, -0.25f, -0.25f);
	glVertex3f(7.0f, -0.05f, -0.8f);
	glNormal3f(0.25f, -0.25f, -0.25f);
	glVertex3f(7.0f, -4.0f, -1.0f);
	glNormal3f(-0.25f, -0.25f, -0.25f);
	glVertex3f(-7.0f, -4.0f, -1.0f);

	glEnd();
}

void Pozitie(void) {

	glPushMatrix();
		glRotatef(corpRot, 0.0, 1.0, 0.0);
		glTranslatef(0.0, -0.3, -0.3);
		Corp();
		PiciorSt();
		PiciorDr();
		PapucSt();
		PapucDr();
		Gat();

		glPushMatrix();
			Cap();
			UrecheSt();
			UrecheDr();
			Gura();
			CapTop();
			glEnable(GL_AUTO_NORMAL);
			OchiSt();
			CercOchiSt();
			OchiDr();
			CercOchiDr();
			glDisable(GL_AUTO_NORMAL);
		glPopMatrix();

		glEnable(GL_AUTO_NORMAL);
		CercManaSt();
		CercManaDr();
		CercPiciorSt();
		CercPiciorDr();
		glDisable(GL_AUTO_NORMAL);

		glPushMatrix();
			
			BuzunarSt();
			BuzunarDr();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.45, 0.78, 0.25);
			glRotatef((GLfloat)bratxSt, 1.0, 0.0, 0.0);
			glRotatef((GLfloat)bratzSt, 0.0, 0.0, 1.0);
			glEnable(GL_AUTO_NORMAL);
			BratSt();
			CercAntebratSt();
			glDisable(GL_AUTO_NORMAL);

			glPushMatrix();
				glTranslatef(-0.08, -0.32, 0.0);
				glRotatef((GLfloat)antebratxSt, 1.0, 0.0, 0.0);
				glRotatef((GLfloat)antebratzSt, 0.0, 0.0, 1.0);
				glRotatef((GLfloat)antebratySt, 0.0, 0.0, 1.0);
				glEnable(GL_AUTO_NORMAL);
				AnteBratSt();
				ManaSt();
				glDisable(GL_AUTO_NORMAL);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
				glTranslatef(0.45, 0.78, 0.25);
				glRotatef((GLfloat)bratxDr, 1.0, 0.0, 0.0);
				glRotatef((GLfloat)bratzDr, 0.0, 0.0, 1.0);
				glEnable(GL_AUTO_NORMAL);
				BratDr();
				CercAntebratDr();
				glDisable(GL_AUTO_NORMAL);


			glPushMatrix();
				glTranslatef(0.08, -0.32, 0.0);
				glRotatef((GLfloat)antebratxDr, 1.0, 0.0, 0.0);
				glRotatef((GLfloat)antebratzDr, 0.0, 0.0, 1.0);
				glRotatef((GLfloat)antebratyDr, 0.0, 0.0, 1.0);
				glEnable(GL_AUTO_NORMAL);
				AnteBratDr();
				ManaDr();
				glDisable(GL_AUTO_NORMAL);
			glPopMatrix();
		glPopMatrix();

	glPopMatrix();
}


void CALLBACK display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat matUmbra[4][4];

	GLfloat puncte[3][3] = {
		{ -2.0f, -2.0f, -2.0f },
		{ -2.0f, -2.0f, 2.0f },
		{ 2.0f, -2.0f, 2.0f } };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MatriceUmbra(puncte, position, matUmbra);

	glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		Iarba();
		umbra = 0;
		Pozitie();
	glPopMatrix();

	glPushMatrix();
		glPushAttrib(GL_LIGHTING_BIT);
			glDisable(GL_LIGHTING);
			glPushMatrix();
				glMultMatrixf((GLfloat*)matUmbra);
				umbra = 1;
				glPushMatrix();
				glTranslatef(0, -0.2, 0);
				Pozitie();
				glPopMatrix();
			glPopMatrix();
			
			glPushMatrix();
				glTranslatef(position[0], position[1], position[2]);
				glColor3f(1.0, 0.9, 0);
				auxSolidSphere(0.3);
			glPopMatrix();
			
		glPopAttrib();
	glPopMatrix();

	auxSwapBuffers();
	
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 2.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -4.0);  /*  aduce obiectele in volumul de vizualizare   */

}

/*  Main Loop
*/
int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_DOUBLE | AUX_RGB | AUX_DEPTH);
	auxInitPosition(0, 0, 500, 400);
	auxInitWindow("ROBOT");
	myinit();
	auxKeyFunc(AUX_x, corpRotSub);
	auxKeyFunc(AUX_z, corpRotAdd);
	auxKeyFunc(AUX_1, bratzAddDr);
	auxKeyFunc(AUX_2, bratzSubDr);
	auxKeyFunc(AUX_3, bratxAddDr);
	auxKeyFunc(AUX_4, bratxSubDr);
	auxKeyFunc(AUX_5, bratzAddSt);
	auxKeyFunc(AUX_6, bratzSubSt);
	auxKeyFunc(AUX_7, bratxAddSt);
	auxKeyFunc(AUX_8, bratxSubSt);

	auxKeyFunc(AUX_q, antebratzAddDr);
	auxKeyFunc(AUX_w, antebratzSubDr);
	auxKeyFunc(AUX_e, antebratxAddDr);
	auxKeyFunc(AUX_r, antebratxSubDr);
	auxKeyFunc(AUX_t, antebratyAddDr);
	auxKeyFunc(AUX_y, antebratySubDr);
	auxKeyFunc(AUX_u, antebratzAddSt);
	auxKeyFunc(AUX_i, antebratzSubSt);
	auxKeyFunc(AUX_o, antebratxAddSt);
	auxKeyFunc(AUX_p, antebratxSubSt);
	auxKeyFunc(AUX_k, antebratyAddSt);
	auxKeyFunc(AUX_l, antebratySubSt);
	auxKeyFunc(AUX_RIGHT, soareDr);
	auxKeyFunc(AUX_LEFT, soareSt);


	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return(0);
}






