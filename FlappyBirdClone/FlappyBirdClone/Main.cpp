#include <stdio.h>
#include <math.h>
#include "GL\freeglut.h"
#include "FreeImage.h"

#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "freeimage.lib")


GLuint BackgroundImage;
//GLuint tex2;

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
//
//void reshapeFunc(int w, int h)
//{
//	//printf("%d %d\n", w, h);
//	glViewport(0, 0, w, h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	//gluOrtho2D(-w/2, w/2, -h/2, h/2);
//	gluPerspective(45.0f, (float)w / h, 1, 1000);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//void drawingFunc()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();	//Reset the modelview matrix
//
//	glTranslatef(0, 0, -15);
//	
//	//Front
//	glNormal3f(0, 0, 1);
//	glBindTexture(GL_TEXTURE_2D, metalTex);
//	glBegin(GL_QUADS);
//	glTexCoord2d(0, 0); 
//	glVertex2d(-5, -7.5); //Down Left
//	
//	glTexCoord2d(1, 0); 
//	glVertex2d(5, -7.5); //Down Right
//	
//	glTexCoord2d(1, 1); 
//	glVertex2d(5, 7.5); //Up Right
//	
//	glTexCoord2d(0, 1); 
//	glVertex2d(-5, 7.5); //Up Left
//	glEnd();
//
//
//	glutSwapBuffers();
//
//	//Drawing goes here
//}
//
////void myTimer(int t)
////{
////	handleKeyboard();
////	glutPostRedisplay();
////	glutTimerFunc(16, myTimer, 0);
////}
//
//
//
//int main(int argc, char* argv[]) {
//
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
//	glutInitWindowSize(600, 800);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("Flappy Bird Clone");
//
//	glutDisplayFunc(drawingFunc);
//	glutReshapeFunc(reshapeFunc);
//	//glutTimerFunc(16, myTimer, 0);
//
//	//glEnable(GL_DEPTH_TEST);	//Draw the objects according to their Z index
//
//	glEnable(GL_TEXTURE_2D);
//
//	//glShadeModel(GL_SMOOTH);
//
//	metalTex = loadTexture("assets/images/backgroundImage.png");
//
//	glutMainLoop();
//
//	return 0;
//}


void renderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Background Image
	glBindTexture(GL_TEXTURE_2D, BackgroundImage);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1, -1); //Down Left
	glTexCoord2f(1, 0); glVertex2f( 1, -1); //Down Right
	glTexCoord2f(1, 1); glVertex2f( 1,  1); //Upper Left
	glTexCoord2f(0, 1); glVertex2f(-1,  1); //Upper Right
	glEnd();

	glutSwapBuffers();
}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(600,700);
	glutCreateWindow("Flappy Bird Clone");

	glutDisplayFunc(renderScene);

	glEnable(GL_TEXTURE_2D);
	BackgroundImage = loadTexture("assets/images/backgroundImage.png");

	glutMainLoop();

}