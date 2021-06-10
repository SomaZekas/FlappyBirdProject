#include <stdio.h>
#include <math.h>
#include "GL\freeglut.h"
#include "FreeImage.h"

#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "freeimage.lib")


GLuint BackgroundImage, Ground, Pipes, GameOver;
bool gameOver = false;
//int score;

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
		float x1, x2; //Left, right edges of the pipe
		float y; //Lower Y of upper pipe
	};

	int count;
	float gap = 0.3;
	float leftEdge = -1;
	GLuint texture;
	Pipe* pipes;

public:
	PipeObject(int count) {
		this->count = count;
		pipes = new Pipe[count];
		
	}

	void renderPipes() {

		for (int i = 0; i < count; i++) {

			if (pipes[i].x2 > leftEdge) {
				
				//Upper Pipe
				glBindTexture(GL_TEXTURE_2D, texture);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glPushMatrix();
				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex2f(pipes[i].x1, 1); //Upper Left
				glTexCoord2f(1, 0); glVertex2f(pipes[i].x2, 1); //Upper Right
				glTexCoord2f(1, 1); glVertex2f(pipes[i].x2, pipes[i].y); //Down Right
				glTexCoord2f(0, 1); glVertex2f(pipes[i].x1, pipes[i].y); //Down Left
				glEnd();
				glPopMatrix();

				glDisable(GL_BLEND);

				//Lower Pipe
				glBindTexture(GL_TEXTURE_2D, texture);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glPushMatrix();
				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex2f(pipes[i].x1, -0.7); //Down Left
				glTexCoord2f(1, 0); glVertex2f(pipes[i].x2, -0.7); //Down Right
				glTexCoord2f(1, 1); glVertex2f(pipes[i].x2, pipes[i].y - gap); //Upper RIght
				glTexCoord2f(0, 1); glVertex2f(pipes[i].x1, pipes[i].y - gap); //Upper Left
				glEnd();
				glPopMatrix();

				glDisable(GL_BLEND);
			}

		}
	}

	void initializePipeObject(int i, bool firstInit) {
		texture = loadTexture("assets/images/pipe.png");

		//0.925 is the distance between each pipe and the new pipe.
		if (firstInit) {
			//Used ONLY when initalizing to separate the pipes.
			pipes[i].x1 = 1 + i * 0.925;
			pipes[i].x2 = 1.15 + i * 0.925;
		}
		else {
			//Used when updating the frames so each pipe maintains its distance from each other.
			pipes[i].x1 = 1 * 0.925;
			pipes[i].x2 = 1.15 * 0.925;
		}

		//Generating a random number to determine the location of the gap.
		do {
			pipes[i].y = rand() / (double)RAND_MAX;
		} while (pipes[i].y > -0.7 && pipes[i].y < 0.5);

	}

	void initialize() {
		for (int i = 0; i < count; i++)
			initializePipeObject(i, true);

	}

	void update() {
		
		for (int i = 0; i < count; i++) {
			if (!gameOver) {
				if (pipes[i].x2 > leftEdge) {
					pipes[i].x1 -= 0.008;
					pipes[i].x2 -= 0.008;

				}
				else
					initializePipeObject(i, false);
			}
		}
	}

};

PipeObject thePipe(2);

class BirdObject {
public:

	float y1, y2; //Lower, upper edge of bird.
	float groundLevelBird = -0.7;
	GLuint texture;

	void initializeBirdObject() {
		texture = loadTexture("assets/images/bird0.png");
		y1 = -0.06;

	}

	void renderBird() {

		y2 = y1 + (float) 0.12;

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, texture);
		glPushMatrix();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(-0.06, y1); //Down Left
		glTexCoord2f(1, 0); glVertex2f(0.06, y1); //Down Right
		glTexCoord2f(1, 1); glVertex2f(0.06, y2); //Upper Right
		glTexCoord2f(0, 1); glVertex2f(-0.06, y2); //Upper Left
		glEnd();
		glPopMatrix();

		glDisable(GL_BLEND);

	}


	void update() {
		//dimming when lost.
		//rotate when up or down.

		for (int i = 0; i < thePipe.count; i++) {
			if (y1 < groundLevelBird) {
				gameOver = true;
			}
			else if (!gameOver) {
				if (GetAsyncKeyState(VK_SPACE) != 0)
					y1 += 0.03;
				else
					y1 -= 0.01;

			}
		}
	}

};

BirdObject theBird;

void myTimer(int t) {
	theBird.update();
	thePipe.update();
	glutPostRedisplay();
	glutTimerFunc(16, myTimer, 0);
}

void renderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render Background Image
	glBindTexture(GL_TEXTURE_2D, BackgroundImage);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1, -1); //Down Left
	glTexCoord2f(1, 0); glVertex2f(1, -1); //Down Right
	glTexCoord2f(1, 1); glVertex2f(1, 1); //Upper Right
	glTexCoord2f(0, 1); glVertex2f(-1, 1); //Upper Left
	glEnd();
	glPopMatrix();

	//Render Pipes
	thePipe.renderPipes();

	//Render Bird
	theBird.renderBird();

	//Render GameOver Text
	if (gameOver) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, GameOver);
		glPushMatrix();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(-0.6, 0.3); //Down Left
		glTexCoord2f(1, 0); glVertex2f(0.6, 0.3); //Down Right
		glTexCoord2f(1, 1); glVertex2f(0.6, 0.6); //Upper Right
		glTexCoord2f(0, 1); glVertex2f(-0.6, 0.6); //Upper Left
		glEnd();
		glPopMatrix();

		glDisable(GL_BLEND);
	}

	//Render Ground
	glBindTexture(GL_TEXTURE_2D, Ground);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1, -1); //Down Left
	glTexCoord2f(1, 0); glVertex2f(1, -1); //Down Right
	glTexCoord2f(1, 1); glVertex2f(1, -0.7); //Upper Right
	glTexCoord2f(0, 1); glVertex2f(-1, -0.7); //Upper Left
	glEnd();
	glPopMatrix();


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

	//Initialize Textures and objects
	BackgroundImage = loadTexture("assets/images/backgroundImage.png");
	Ground = loadTexture("assets/images/ground.png");
	GameOver = loadTexture("assets/images/gameover.png");
	
	theBird.initializeBirdObject();
	thePipe.initialize();

	glutMainLoop();

}
