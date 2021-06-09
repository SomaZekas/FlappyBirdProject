#include <stdio.h>
#include <math.h>
#include "GL\freeglut.h"
#include "FreeImage.h"

#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "freeimage.lib")


GLuint BackgroundImage, Ground, Pipes;
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


class PipeObject {
public:

	struct Pipe {
		float x1, x2;
		float y1, y2;
		GLuint texture;
	};

	Pipe* pipes;
	int count;
	float leftEdge = -1;
	
public:
	PipeObject(int count) {
		this->count = count;
		pipes = new Pipe[count];
	}

	void renderPipes()
	{
		for (int i = 0; i < count; i++) {
			
			//will draw another pipe with rotation 90 degrees with same x but opposite y-axis, with random gap.
			//on condition that two pipes appear on screen.

			//Pipes
			glBindTexture(GL_TEXTURE_2D, pipes[i].texture);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPushMatrix();
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(0.2, -0.7); //Down Left
			glTexCoord2f(1, 0); glVertex2f(0.3, -0.7); //Down Right
			glTexCoord2f(1, 1); glVertex2f(0.3, 0.0); //Upper Left
			glTexCoord2f(0, 1); glVertex2f(0.2, 0.0); //Upper Right
			glEnd();
			glPopMatrix();

			glDisable(GL_BLEND);
		}
	}

	void initializePipeObject() {
		for (int i = 0; i < count; i++)
		{
			pipes[i].texture = loadTexture("assets/images/pipe.png");
			//t3rf 2l x bel initial value bta3o wa tzwd 2l i.
		}
		
		
	}


};

PipeObject thePipe(3);

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

		y2 = y1 + (float)0.12;

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
		//dimming when lost.
		//rotate when up or down.

		//if (y1 < groundLevelBird) {
		//	gameOver = true;
		//}
		//else {
			if (GetAsyncKeyState(VK_SPACE) != 0)
				y1 += 0.03;
			else
				y1 -= 0.01;
		//}
	}

};

BirdObject theBird;

void handleKeyboard() {

	if (GetAsyncKeyState(VK_SPACE) != 0)
		theBird.y1 += 0.03;
	else
		theBird.y1 -= 0.01;
}

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

	//render Pipe
	thePipe.renderPipes();

	//Pipes
	//glBindTexture(GL_TEXTURE_2D, Pipes);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPushMatrix();
	//glBegin(GL_QUADS);
	//glTexCoord2f(0, 0); glVertex2f(0.2, -0.7); //Down Left
	//glTexCoord2f(1, 0); glVertex2f(0.3, -0.7); //Down Right
	//glTexCoord2f(1, 1); glVertex2f(0.3, 0.0); //Upper Left
	//glTexCoord2f(0, 1); glVertex2f(0.2, 0.0); //Upper Right
	//glEnd();
	//glPopMatrix();

	//glDisable(GL_BLEND);



	glutSwapBuffers();
}

void main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 700);

	glutCreateWindow("Flappy Bird Clone");

	glutDisplayFunc(renderScene);

	glutTimerFunc(16, myTimer, 0);
	glEnable(GL_TEXTURE_2D);
	BackgroundImage = loadTexture("assets/images/backgroundImage.png");
	Ground = loadTexture("assets/images/ground.png");
	theBird.initializeBirdObject();
	//Pipes = loadTexture("assets/images/pipe.png");
	thePipe.initializePipeObject();

	glutMainLoop();

}