#define STB_IMAGE_IMPLEMENTATION
#include<Windows.h>
#include<fstream>
#include<MMSystem.h>
#include<iostream>
#include <sstream>
using namespace std;
#include<math.h>
#include<cmath>
#include <gl/stb_image.h>
#include<gl/glut.h>

// Variable
int s = 0, max_s = 2;
int w = 0, max_w = 1;
int u = 0, max_u = 10;
int d = 0, max_d = 1;
float eyey, eyez, eyex, upx, upz = -10;
float upy = 0;
unsigned char* im = NULL;
unsigned int texture;
int wallWidth, wallHeight, nrChannels;
float basketz = -5, baskety = 0.5;
bool fullscreen, firstTimeq, space = false, bar_reset = false;
bool inside = false, outside = false, score = false;
float ratio, angle, width_screen, height_screen;
float r = 0.2, r_score = 0.12, x = 0, y = 0, PI = 22 / 7.0, rbasket = 0.5;
float no_score = 3;
float moves;
int no_true = 0;
float xb = 0, yb = 0, zb = 0;
float deltayb = 0.28, sHeight = 1, sWidth = 3.4, deltaxb = 0.1;
float xp = 0, yp = 0, deltayp = 0.05;
bool in_game = false, inst = false;

// Function
void background();
void mydraw();
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void specialkeyboard(int, int, int);
void reset();
void startScreen();
void gameover();
void instructions();
void load_image(int image);
void check(unsigned char* im);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("BasketBall Game");
	glutFullScreen();
	background();
	glutDisplayFunc(startScreen);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	glutMainLoop();
}

void background() {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_TEXTURE_2D);
}

void mydraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// camera

	gluLookAt(eyex, eyey, eyez, upx, upy, upz, 0, 1, 0);
	glTranslatef(0, 0, -5);
	load_image(1);

	//ground

	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-8, -2, -20);

	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-8, -2, 20);

	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(8, -2, 20);

	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(8, -2, -20);

	glEnd();
	
	//top

	glColor3f(0.1, 0.2, 0.3);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-8, 4, -20);

	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-8, 4, 20);

	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(8, 4, 20);

	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(8, 4, -20);

	glEnd();

	//back

	glColor3f(0.1, 0.2, 0.3);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-8, -2, -20);

	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(8, -2, -20);

	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(8, 10, -20);

	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-8, 10, -20);

	glEnd();

	//left

	glBegin(GL_QUADS);

	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-8, -2, -20);

	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-8, -2, 20);

	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-8, 10, 20);

	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-8, 10, -20);

	glEnd();

	//right

	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(8, -2, -20);

	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(8, -2, 20);

	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(8, 10, 20);

	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(8, 10, -20);

	glEnd();

	//sphere  drawing & shading

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// Update the sphere's material properties to make it orange
	GLfloat materialAmbient[] = { 1.0f, 0.5f, 0.0f, 1.0f }; // Ambient color (orange)
	GLfloat materialDiffuse[] = { 1.0f, 0.5f, 0.0f, 1.0f }; // Diffuse color (orange)
	GLfloat materialSpecular[] = { 1.0f, 1.0f, 0.0f, 1.0f }; // Specular color (yellow)
	GLfloat materialShininess = 20; // Shininess for specular highlights

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient); // Ambient property
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse); // Diffuse property
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular); // Specular property
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess); // Shininess

	// Begin transformation
	glPushMatrix();

	// Position and rotate the sphere
	glTranslatef(-2.4 + xb, -1.3 + yb, -1 + zb);
	glRotatef(angle, 1, 1, 1);
	glutWireSphere(r, 50, 50);

	// Disable lighting and light source
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	// End transformation
	glPopMatrix();

	// Basket
	glPushMatrix();
	glLineWidth(5);
	glTranslatef(3.5, 0.7, -1);
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);
	for (float i = 0; i <= 2 * PI; i += PI / 500) {
		glVertex3f(cos(i) * rbasket, 0, sin(i) * rbasket);
	}
	glEnd();
	glPopMatrix();

	//sphere ground
	glPushMatrix();
	glTranslatef(-2.4, -1.53, -1);
	glRotatef(0 - moves, 0, 0, 0.1);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 0, 0);
	for (float i = 0; i <= 2 * PI; i += PI / 500)
		glVertex3f(cos(i) * 0.25, 0, sin(i) * 0.25);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.4, -1.52, -1);
	glRotatef(0 - moves, 0, 0, 0.1);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 1);
	for (float i = 0; i <= 2 * PI; i += PI / 500)
		glVertex3f(cos(i) * 0.2, 0, sin(i) * 0.2);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.4, -1.51, -1);
	glRotatef(0 - moves, 0, 0, 0.1);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 0, 0);
	for (float i = 0; i <= 2 * PI; i += PI / 500)
		glVertex3f(cos(i) * 0.15, 0, sin(i) * 0.15);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.4, -1.5, -1);
	glRotatef(0 - moves, 0, 0, 0.1);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 1);
	for (float i = 0; i <= 2 * PI; i += PI / 500)
		glVertex3f(cos(i) * 0.1, 0, sin(i) * 0.1);
	glEnd();
	glPopMatrix();
    ///////////

	// Score
	x = 0;
	for (int i = no_score; i > 0; i--) {
		glLoadIdentity();
		glTranslatef(-3.4 + x, 1.5, -5);
		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLE_FAN);
		for (float j = 0; j <= 2 * PI; j += PI / 500) {
			glVertex3f(cos(j) * r_score, sin(j) * r_score, 0);
		}
		glEnd();
		x += 0.3;
	}

	glLoadIdentity();
	glRasterPos3f(-3.3, 1.8, -5);
	char score[] = "Score: ";
	stringstream ss;
	ss << no_true;
	string charValue;
	ss >> charValue;
	for (int i = 0; i <= strlen(score); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[i]);
	for (int i = 0; i <= charValue.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, charValue[i]);

	// Point
	glLoadIdentity();
	glTranslatef(-3.5, 0, -5);

	glEnable(GL_POINT_SMOOTH); 
	glPointSize(20);     

	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex2f(0, yp);
	glEnd();

	// triangle
	glLoadIdentity();
	glTranslatef(-3.45, 0, -5);
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 0);
	glVertex2f(0.08, 0.05);
	glVertex2f(0.08, -0.05);
	glEnd();

	// bar
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(-3.5, 0, -5);
	glColor3f(1, 1, 1);

	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0, 0);
	glVertex2f(-0.1, -1);
	glVertex2f(0.1, -1);

	glColor3f(0, 1, 0);
	glVertex2f(-0.1, 0);
	glVertex2f(0.1, 0);

	glColor3f(0, 0, 1);
	glVertex2f(-0.1, 1);
	glVertex2f(0.1, 1);

	glPopMatrix();
	glEnd();


	glLineWidth(3);
	glPushMatrix();
	glLoadIdentity(); 
	glTranslatef(3.2, 0.52, -5); 
	glColor3f(1, 1, 1);

	glPopMatrix();
	glutSwapBuffers(); 
}

void reshape(int w, int h) {
	if (h == 0)
		h = 1;
	ratio = (float)w / h; 
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 

	gluPerspective(45, ratio, 1, 100); 
	glMatrixMode(GL_MODELVIEW);
}

void timer(int v) {
	glutPostRedisplay(); 
	glutTimerFunc(30, timer, 0); 
	angle += 1.9; 

	if (space) {
		deltayb = abs(deltayb);
		xb += deltaxb;

		sWidth = 3.3 + yp;

		if (xb >= sWidth) {
			yb -= deltayb;
		}
		else {
			yb += deltayb;
		}

		if ((yp >= -0.15 && yp <= 0) || (yp >= 0 && yp <= 0.15)) {
			inside = true;
			outside = false;
		}
		else {
			inside = false;
			outside = true;
		}
		moves += 50;
		if (moves >= 50)
			moves = 50;
		//to make collision
		if (yb < 0) {
			yb = 0;
		}
		if (xb >= 5.1 && xb <= 5.4 && yb >= 1.4 && yb <= 1.7) {
			yb += 0.5;
			xb = 4;
		}
		if (xb >= 6.3 && xb <= 6.5 && yb >= 1.9 && yb <= 2.2) {
			yb += 0.5;
			xb = 6.6;
		}
		if (xb >= 5.4 && xb <= 5.6 && yb >= 1.5 && yb <= 1.8) {
			yb += 0.5;
			xb = 5.9;
			no_true += 1;
			score = true;
		}
		else if (xb >= 6.1 && xb <= 6.3 && yb >= 1.8 && yb <= 2.1) {
			yb += 0.5;
			xb = 5.9;
			no_true += 1;
			score = true;
		}
	}
	else {
		if (yp >= sHeight || yp <= -sHeight)
			deltayp = -deltayp;
		yp += deltayp;
	}
	if (bar_reset) {
		if (yp >= sHeight || yp <= -sHeight)
			deltayp = -deltayp;
		yp += deltayp;
	}

	if (xb >= 8.5)
		reset();

	if (no_score == 0) {
		glutDisplayFunc(gameover);
		glutIdleFunc(gameover);
	}

}

void keyboard(unsigned char key, int x, int y) {
	if (key == 27)  // Esc to Exit the game
		exit(0);
	if (key == 32)  // space key to play the game
	{	
		space = true;
		bar_reset = false;
	}

	if (key == 13) //Enter key : start game
	{
		glutDisplayFunc(mydraw);
		glutIdleFunc(mydraw);
		in_game = true;
	}
	if (key == 'w' && w < max_w)
	{
		s--;
		w++;
		eyez -= 2;
		eyey = .3 * abs(cos(angle));
		baskety -= 0.3;
		basketz += 0.05;
	}
	if (key == 's' && s < max_s) {
		s++;
		w--;
		eyez += 2;
		eyey = .1 * abs(cos(angle));
		basketz -= 0.05;
		baskety += 0.3;
	}

	if (key == 'a')
	{
		upx -= .9 * cos(.1);
		upz -= .9 * sin(.1);
	}
	if (key == 'd') {
		upx += .9 * cos(.1);
		upz += .9 * sin(.1);
	}
}

void specialkeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_F1) // F1 to exit fullscreen
	{ 
		fullscreen = !fullscreen;
		if (fullscreen)
			glutFullScreen();
		else { 
			glutReshapeWindow(width_screen / 2, height_screen / 2);
			glutPositionWindow(width_screen / 2 - width_screen / 4
				, height_screen / 2 - height_screen / 4);
		}
	}
	if (key == GLUT_KEY_F2 && in_game) // F2 to setting the game
	{
		if (!inst) {
			glutDisplayFunc(instructions);
			glutIdleFunc(instructions);
			inst = true;
		}
		else
		{
			glutDisplayFunc(mydraw);
			glutIdleFunc(mydraw);
			inst = false;
		}
	}
	if (key == GLUT_KEY_UP && u < max_u) {
		eyey += .1 * abs(cos(angle));
		u++;
		d--;
	}
	if (key == GLUT_KEY_DOWN && d < max_d) {
		eyey -= .1 * abs(cos(angle));
		u--;
		d++;
	}
	if (key == GLUT_KEY_RIGHT) {
		upx += .9 * cos(.1);
		upz += .9 * sin(.1);
	}
	if (key == GLUT_KEY_LEFT) {
		upx -= .9 * cos(.1);
		upz -= .9 * sin(.1);
	}
}

void reset()
{
	// Reset ball position and any other necessary variables
	xb = 0;
	yb = 0;
	zb = 0;
	yp = 0;
	moves = 0;
	if (outside && score == false)
		no_score--;
	if (inside)
		no_true += 1;
	inside = false;
	outside = false;
	score = false;
	bar_reset = true;
	space = false;
	sWidth = 3.3;
}

void startScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	// Hexadecimal color
	unsigned int hexColor = 0xee6c4d;
	// Extract R , G , B from hexa
	float r = ((hexColor >> 16) & 0xFF) / 255.0f;  // Red 
	float g = ((hexColor >> 8) & 0xFF) / 255.0f;   // Green 
	float b = (hexColor & 0xFF) / 255.0f;          // Blue 

	glColor3f(r, g, b); 
	glRasterPos3f(-3, 2, -30);
	char Title[] = "BasketBall Game";

	for (int i = 0; i <= strlen(Title); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Title[i]);

	glRasterPos3f(-5, -1, -30);
	char msg1[] = "1. Press Enter to start the game";
	for (int i = 0; i <= strlen(msg1); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg1[i]);

	glRasterPos3f(-5, -3, -30);
	char msg2[] = "2. Press F1 to exit the Full Screen";
	for (int i = 0; i <= strlen(msg2); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg2[i]);

	glRasterPos3f(-5, -5, -30);
	char msg3[] = "3. Press F2 to show instructions in-game";
	for (int i = 0; i <= strlen(msg3); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg3[i]);

	glRasterPos3f(-5, -7, -30);
	char msg4[] = "4. Press ESC to exit the game";
	for (int i = 0; i <= strlen(msg4); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg4[i]);

	glutSwapBuffers();
}

void gameover() {
	no_score = 3;
	no_true = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	// Hexadecimal color
	//unsigned int hexColor = 0xeb5e28;
	unsigned int hexColor = 0xee6c4d;
	// Extract R , G , B from hexa
	float r = ((hexColor >> 16) & 0xFF) / 255.0f;  // Red 
	float g = ((hexColor >> 8) & 0xFF) / 255.0f;   // Green 
	float b = (hexColor & 0xFF) / 255.0f;          // Blue 

	glColor3f(r, g, b); 


	glRasterPos3f(-3, 1, -30);
	char finish[] = "Game Over";
	for (int i = 0; i <= strlen(finish); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, finish[i]);

	glRasterPos3f(-4, -1, -30);
	char restart[] = "Press Enter to restart";
	for (int i = 0; i <= strlen(restart); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, restart[i]);

	glutSwapBuffers();
}

void instructions()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	// Hexadecimal color
	unsigned int hexColor = 0xee6c4d;
	// Extract R , G , B from hexa
	float r = ((hexColor >> 16) & 0xFF) / 255.0f;  // Red 
	float g = ((hexColor >> 8) & 0xFF) / 255.0f;   // Green 
	float b = (hexColor & 0xFF) / 255.0f;          // Blue 

	glColor3f(r, g, b); 

	glRasterPos3f(-2.7, 2, -10);
	char m1[] = "Instructions to play";
	for (int i = 0; i <= strlen(m1); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, m1[i]);

	glRasterPos3f(-2.7, 0.6, -10);
	char m2[] = "Shoot      :     Space";
	for (int i = 0; i <= strlen(m2); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, m2[i]);

	glRasterPos3f(-2.7, -0.2, -10);
	char m3[] = "Move Camera       :     Arrows & WASD";
	for (int i = 0; i <= strlen(m3); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, m3[i]);

	glRasterPos3f(-2.7, -1, -10);
	char m4[] = "Exit the game      :     Esc";
	for (int i = 0; i <= strlen(m4); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, m4[i]);

	glutSwapBuffers();
}

void load_image(int image)
{
	if (image == 1) {

		im = stbi_load("backgroud.jpg", &wallWidth, &wallHeight, &nrChannels, 0);//name of photo/width and height for photo/color chanel/load with the same number of channels
		check(im);
	}

}

void check(unsigned char* im)
{
	if (im)// if im != null 
	{	//make id 
		glGenTextures(1, &texture);//first Argument : the number of texture names to generate ,second : the generated texture name 
		glBindTexture(GL_TEXTURE_2D, texture);//mapping the photo to 0 1 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wallWidth, wallHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, im);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	stbi_image_free(im);
}
