/*
	Computer Graphics Project - Flappy Bird Clone
	Made By:
		Ismail Ahmed Zekry
		Rany Hatem
		Mostafa Mohamed Sami
*/
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include "GL\freeglut.h"
#include "FreeImage.h"


#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "freeimage.lib")


//Define texture variables
GLuint BackgroundImage, Ground, Pipes, GameOver, Score;

//Define light variables
GLfloat diffuse[] = { 0.0f, 0.0f, 0.1f, 1.0f };
GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat position[] = { 0.0f, 0.0f, 2.0f, 1.0f };

bool gameOver = false;
bool lightsOn = false; //To check if lights on or not

//Define score variables
int score = 0;
bool scoreBool = true;
char scoreString[10][50];
float scoreX1 = -0.9, scoreY1 = 0.8; //Left edge of texture and lower edge of texture

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

void reshapeFunc(int w, int h) {
	glViewport(0, 0, w, h);

	//Defines the properties of the camera that views the objects in the world coordinate frame.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //Essentially resets the matrix back to its default state.

	gluOrtho2D(-w / 600, w / 600, -h / 700, h / 700);

	//Defines how your objects are transformed in your world coordinate frame.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

class PipeObject {
public:

	struct Pipe {
		float x1, x2; //Left, right edges of the pipe
		float y; //Lower Y of upper pipe
	};

	//Object variables
	int count;
	float gap = 0.5;
	float leftEdge = -1;
	float rightEdge = 1;
	GLuint texture;

	Pipe* pipes;

	//Functions
	PipeObject(int count) {
		this->count = count;
		pipes = new Pipe[count];
		
	}

	void renderPipes() {

		for (int i = 0; i < count; i++) {

			if (pipes[i].x1 > leftEdge && pipes[i].x2 < rightEdge) {
				
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
			pipes[i].y = rand() / (float)RAND_MAX;
		} while (pipes[i].y > -0.2 && pipes[i].y < 0.5);

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

		if (lightsOn) //If lights are on (scene is dark) then disable it and render bird texture
			glDisable(GL_LIGHTING);

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

		if (lightsOn)
			glEnable(GL_LIGHTING);

	}


	void update() {
		
		for (int i = 0; i < thePipe.count; i++) {
			if (
				y1 < groundLevelBird ||
				(!(y2 < thePipe.pipes[i].y && y1 > thePipe.pipes[i].y - thePipe.gap) && (thePipe.pipes[i].x1 <= 0.06 && thePipe.pipes[i].x2 > -0.06))
				)
			{
				gameOver = true;
				scoreX1 = -0.05;
				scoreY1 = 0.0;

				if (GetAsyncKeyState(VK_BACK)) {
					gameOver = false;
					score = 0;
					scoreBool = true;
					scoreX1 = -0.9;
					scoreY1 = 0.8;
					thePipe.initialize();
					initializeBirdObject();
					
				}
				
			}
			else if (!gameOver) {
				if (GetAsyncKeyState(VK_SPACE) != 0)
					y1 += 0.02;
				else
					y1 -= 0.01;

			}

			if (thePipe.pipes[i].x1 <= -0.25 && thePipe.pipes[i].x2 >= -0.15)
				scoreBool = true;

			if ((y2 < thePipe.pipes[i].y && y1 > thePipe.pipes[i].y - thePipe.gap) && (thePipe.pipes[i].x1 <= 0.06 && thePipe.pipes[i].x2 > -0.06) && scoreBool) {
				score++;
				scoreBool = false;
				if (score > 9)
					score = 0;

			}
		}
	}

};

BirdObject theBird;

void dayOrNight() {
	if (GetAsyncKeyState(VK_UP)) {
		BackgroundImage = loadTexture("assets/images/backgroundImage-Day.png");
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		lightsOn = false;
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		BackgroundImage = loadTexture("assets/images/backgroundImage-Night.png");
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		lightsOn = true;

	}
}

void myTimer(int t) {
	dayOrNight();
	theBird.update();
	thePipe.update();
	glutPostRedisplay(); //If you don’t tell the mainloop it needs to draw the next frame with a glutPostRedisplay then your animation will look as if it is stuck (no animation)
	glutTimerFunc(1000/60, myTimer, 0); //This should ask for a refresh every 60th of a seccond. Acts like a refresh operation.
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

	//Render GameOver Text
	if (gameOver) {
		if (lightsOn) //If lights are on (scene is dark) then disable it and render gameOver texture
			glDisable(GL_LIGHTING);

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

		if (lightsOn)
			glEnable(GL_LIGHTING);
	}

	//Render Score
	Score = loadTexture(scoreString[score]);
	if (lightsOn) //If lights are on (scene is dark) then disable it and render score texture
		glDisable(GL_LIGHTING);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glBindTexture(GL_TEXTURE_2D, Score);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(scoreX1, scoreY1); //Down Left
	glTexCoord2f(1, 0); glVertex2f(scoreX1 + 0.1, scoreY1); //Down Right
	glTexCoord2f(1, 1); glVertex2f(scoreX1 + 0.1, scoreY1 + 0.1); //Upper Right
	glTexCoord2f(0, 1); glVertex2f(scoreX1, scoreY1 + 0.1); //Upper Left
	glEnd();
	glPopMatrix();

	if (lightsOn)
		glEnable(GL_LIGHTING);

	glutSwapBuffers(); //Performs the actual buffer swap for animation.
}


void main(int argc, char* argv[]) {
	//Print Commands
	printf("Press BackSpace Key to reset.\n");
	printf("Press Upper arrow Key to change to Day.\n");
	printf("Press Lower arrow Key to change to Night.\n");
	printf("Press BackSpace Key to play again.\n\n\n");

	//Print Credits
	printf(" _____              _ _ _					\n");
	printf("/  __ \\            | (_) |        _		\n");
	printf("| /  \\/_ __ ___  __| |_| |_ ___  (_)		\n");
	printf("| |   | '__/ _ \\/ _` | | __/ __|			\n");
	printf("| \\__/\\ | |  __/ (_| | | |_\\__ \\  _		\n");
	printf(" \\____/_|  \\___|\\__,_|_|\\__|___/ (_)	\n\n\n");
	printf("\t  Ismail Ahmed Zekry	\n");
	printf("\t  Rany Hatem fathy	\n");
	printf("\t  Mostafa Mohamed Sami\n\n\n");

	//Location of score pictures
	strcpy_s(scoreString[0], "assets/images/0.png");
	strcpy_s(scoreString[1], "assets/images/1.png");
	strcpy_s(scoreString[2], "assets/images/2.png");
	strcpy_s(scoreString[3], "assets/images/3.png");
	strcpy_s(scoreString[4], "assets/images/4.png");
	strcpy_s(scoreString[5], "assets/images/5.png");
	strcpy_s(scoreString[6], "assets/images/6.png");
	strcpy_s(scoreString[7], "assets/images/7.png");
	strcpy_s(scoreString[8], "assets/images/8.png");
	strcpy_s(scoreString[9], "assets/images/9.png");

	//Initialize theme song
	PlaySound(TEXT("assets/audio/theme.wav"), NULL, SND_LOOP | SND_ASYNC);

	//Initializing Glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 700);

	glutCreateWindow("Flappy Bird Clone - Computer Graphics Project");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeFunc);
	glutTimerFunc(1000/60, myTimer, 0); ////////CHRCK


	glEnable(GL_TEXTURE_2D);

	//Initialize Textures and objects
	BackgroundImage = loadTexture("assets/images/backgroundImage-Day.png");
	Ground = loadTexture("assets/images/ground.png");
	GameOver = loadTexture("assets/images/gameover.png");
	
	//Initializing Objects
	thePipe.initialize();
	theBird.initializeBirdObject();

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glutMainLoop();

}
