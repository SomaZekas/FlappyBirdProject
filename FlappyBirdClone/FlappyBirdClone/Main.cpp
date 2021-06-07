#include <stdio.h>
#include <math.h>
#include "GL\freeglut.h"
#include "FreeImage.h"

#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "freeimage.lib")


GLuint BackgroundImage;
GLuint bird;
double birdy = 0;
bool spaceKeyPressed = false;

int loadTexture(const char* location)
{
	GLuint texture;


	FIBITMAP* myImage = FreeImage_Load(FreeImage_GetFileType(location, 0), location);
	myImage = FreeImage_ConvertTo32Bits(myImage);

	int width = FreeImage_GetWidth(myImage);
	int height = FreeImage_GetHeight(myImage);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_BGRA_EXT, GL_UNSIGNED_BYTE, FreeImage_GetBits(myImage));

	return texture;

}

void renderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//Background Image
	glBindTexture(GL_TEXTURE_2D, BackgroundImage);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1, -1); //Down Left
	glTexCoord2f(1, 0); glVertex2f( 1, -1); //Down Right
	glTexCoord2f(1, 1); glVertex2f( 1,  1); //Upper Left
	glTexCoord2f(0, 1); glVertex2f(-1,  1); //Upper Right
	glEnd();
	glPopMatrix();

	//Bird Image
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, bird);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-0.06, -0.06 + birdy); //Down Left
	glTexCoord2f(1, 0); glVertex2f(0.06, -0.06 + birdy); //Down Right
	glTexCoord2f(1, 1); glVertex2f(0.06, 0.06 + birdy); //Upper Left
	glTexCoord2f(0, 1); glVertex2f(-0.06, 0.06 + birdy); //Upper Right
	glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);


	glutSwapBuffers();
}

void handleKeyboard() {

	if (GetAsyncKeyState(VK_SPACE) != 0) {
		birdy += 0.03;
	}
	else {
		birdy -= 0.01;
	}
}

void myTimer(int t)
{
	handleKeyboard();
	glutPostRedisplay();
	glutTimerFunc(16, myTimer, 0);
}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(600,700);

	glutCreateWindow("Flappy Bird Clone");

	glutDisplayFunc(renderScene);
	glutTimerFunc(16, myTimer, 0);

	glEnable(GL_TEXTURE_2D);
	BackgroundImage = loadTexture("assets/images/backgroundImage.png");
	bird = loadTexture("assets/images/bird0.png");

	glutMainLoop();

}