#include <stdio.h>
#include <math.h>
#include "GL\freeglut.h"
#include "FreeImage.h"

#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "freeimage.lib")


GLuint BackgroundImage, Ground;
bool gameOver = false;

int loadTexture(const char* location) {

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

class BirdObject {
public:

	float y1, y2;
	GLuint texture;
	float groundLevelBird = -0.7;

	void initializeBirdObject() {
		texture = loadTexture("assets/images/bird0.png");
		y1 = -0.06;
		
	}

	void renderBird() {

		y2 = y1 + (float) 0.12;
		
		if (!gameOver) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			glBindTexture(GL_TEXTURE_2D, texture);
			glPushMatrix();
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(-0.06, y1); //Down Left
			glTexCoord2f(1, 0); glVertex2f(0.06, y1); //Down Right
			glTexCoord2f(1, 1); glVertex2f(0.06, y2); //Upper Left
			glTexCoord2f(0, 1); glVertex2f(-0.06, y2); //Upper Right
			glEnd();
			glPopMatrix();

			glDisable(GL_BLEND);
		}
	}

	void update() {
		if (y1 < groundLevelBird) {
			gameOver = true;
		}
		else {
			if (GetAsyncKeyState(VK_SPACE) != 0)
				y1 += 0.03;
			else
				y1 -= 0.01;
		}
	}

};

BirdObject theBird;

//class PipeObject {
//	struct Pipe {
//		float x, y;
//		float speed;
//		float lifetime;
//		GLuint texture;
//	};
//
//	Pipe* pipes;
//	int count;
//	float topLevel, GroundLevel;
//	GLuint pipeTexture[2];
//
//public:
//	PipeObject() {
//		pipes = new Pipe[2];
//	}
//};

//void handleKeyboard() {
//
//	if (GetAsyncKeyState(VK_SPACE) != 0)
//		theBird.y1 += 0.03;
//	else
//		theBird.y1 -= 0.01;
//}

void myTimer(int t) {
	//handleKeyboard();
	theBird.update();
	glutPostRedisplay();
	glutTimerFunc(16, myTimer, 0);
}

void renderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Background Image
	glBindTexture(GL_TEXTURE_2D, BackgroundImage);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1, -1); //Down Left
	glTexCoord2f(1, 0); glVertex2f(1, -1); //Down Right
	glTexCoord2f(1, 1); glVertex2f(1, 1); //Upper Left
	glTexCoord2f(0, 1); glVertex2f(-1, 1); //Upper Right
	glEnd();
	glPopMatrix();

	//Ground
	glBindTexture(GL_TEXTURE_2D, Ground);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1, -1); //Down Left
	glTexCoord2f(1, 0); glVertex2f(1, -1); //Down Right
	glTexCoord2f(1, 1); glVertex2f(1, -0.7); //Upper Left
	glTexCoord2f(0, 1); glVertex2f(-1, -0.7); //Upper Right
	glEnd();
	glPopMatrix();

	//Render Bird
	theBird.renderBird();


	glutSwapBuffers();
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
	Ground = loadTexture("assets/images/ground.png");

	theBird.initializeBirdObject();

	glutMainLoop();

}