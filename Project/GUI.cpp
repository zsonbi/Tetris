#include <GL/glut.h>
#include "GUI.h"
#include "tetris.h"
#include <vector>
#include <string>
#include <GL/freeglut.h>

#define WIDTH 1000
#define HEIGHT 1000

//---------------------------------------------------
//Color. no need for further explanations
class Color {
public:
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;

	Color(float red, float green, float blue) {
		this->red = red;
		this->green = green;
		this->blue = blue;
	}
	Color() {
	}
};

//Varriables
//The colors which the pieces can take
Color* colors[7];
//The speed which the pieces fall
const int speed = 5;
//Determines the two rectangles size which drawn by the DrawWalls method
constexpr int wallSize = (rows - cols) / 2;
//The tetris game
Tetris game;
//The number which the speed is multiplied (used for the soft drop)
int speedMultiplier = 1;

void display_callback();
void reshape_callback(int, int);
void timer_callback(int);
void unit(int, int);
void drawGrid();
void DrawWalls();
void keyboard_callback(int, int, int);
void SpecialKeyUpFunc(int, int, int);
void NormalKeysDetection(unsigned char key, int x, int y);
//----------------------------------------------------------------
//Adds the main colors and sets the background color
void InitWindow() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	colors[0] = new Color(0.67, 0.85, 1.0);//light blue
	colors[1] = new Color(0.0, 0.0, 1.0);//blue
	colors[2] = new Color(1.0, 0.65, 0.0);//orange
	colors[3] = new Color(1.0, 1.0, 0.0);//yellow
	colors[4] = new Color(0.0, 1.0, 0.0);//green
	colors[5] = new Color(1.0, 0.0, 1.0);//purple
	colors[6] = new Color(1.0, 0.0, 0.0);//red
}

//------------------------------------------------------------------
//Initializes the window and the Tetris game
void Init(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Tetris");
	glutDisplayFunc(display_callback);
	glutKeyboardFunc(NormalKeysDetection);
	glutReshapeFunc(reshape_callback);
	glutTimerFunc(0, timer_callback, 0);
	glutSpecialFunc(keyboard_callback);
	glutSpecialUpFunc(SpecialKeyUpFunc);
	InitWindow();
	glutMainLoop();
}

//-------------------------------------------------------
//Detects keyboard releases
void SpecialKeyUpFunc(int key, int, int) {
	switch (key)
	{
	case GLUT_KEY_DOWN:
		//Turn it back to normal
		speedMultiplier = 1;
		game.SoftDropOff();
		break;
	default:
		break;
	}
}

//-------------------------------------------------------
//Detects keyboard presses for the arrows
void keyboard_callback(int key, int, int) {
	switch (key)
	{
		//Left movement
	case GLUT_KEY_LEFT:
		game.Move(LEFT);
		break;
		//Right movement
	case GLUT_KEY_RIGHT:
		game.Move(RIGHT);
		break;
		//Soft drop
	case GLUT_KEY_DOWN:
		game.SoftDropOn();
		speedMultiplier = 4;
		break;

	default:
		break;
	}
}

//----------------------------------------------------------
//Detects keyboard presses for the other keys
void NormalKeysDetection(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'r':
		game.Rotate(RIGHT);
		break;
		//space
	case 32:
		game.HardDrop();
		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------
//Runs every frame
void display_callback() {
	glClear(GL_COLOR_BUFFER_BIT);

	//Draws the pieces
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			if (game.board[i][j].color != -1) {
				glColor3f(colors[game.board[i][j].color]->red, colors[game.board[i][j].color]->green, colors[game.board[i][j].color]->blue);

				glRectd(GLdouble(j + (double)wallSize), i, ((GLdouble)j + 1 + wallSize), ((GLdouble)i + 1));
			}
		}
	}

	for (size_t i = 0; i < game.fallingPieces->pieces.size(); i++)
	{
		glColor3f(colors[game.fallingPieces->color]->red, colors[game.fallingPieces->color]->green, colors[game.fallingPieces->color]->blue);
		glRectd((GLdouble)(game.fallingPieces->pieces[i][1] + (double)wallSize), game.fallingPieces->pieces[i][0], ((GLdouble)game.fallingPieces->pieces[i][1] + 1 + wallSize), ((GLdouble)game.fallingPieces->pieces[i][0] + 1));
	}

	DrawWalls();
	drawGrid();

	//-----------------------------------------
	//Writes out the score ty stackoverflow guy <3
	std::string text = "score: " + std::to_string(game.GetScore());
	glRasterPos2i(matrixSize - wallSize, matrixSize - 1);
	for (std::string::iterator i = text.begin(); i != text.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

	glutSwapBuffers();
}

//-----------------------------------------------------------------
//Determines the fps
void timer_callback(int) {
	glutPostRedisplay();
	game.Fall();
	glutTimerFunc(1000 / (speed * speedMultiplier), timer_callback, 0);
}

//-----------------------------------------------------------------
//Runs when the window is resized
void reshape_callback(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, matrixSize, 0.0, matrixSize, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW_MATRIX);
}

//---------------------------------------------------------------------
//Draws a rectangle
void unit(int x, int y) {
	glLineWidth(1.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + 1, y);
	glVertex2f(x + 1, y + 1);
	glVertex2f(x, y + 1);
	glEnd();
}

//-------------------------------------------------------------------
//Draws the grid
void drawGrid() {
	for (int i = 0; i < rows; i++)
	{
		for (int j = wallSize; j < matrixSize - wallSize; j++)
		{
			unit(j, i);
		}
	}
}

//-------------------------------------------------------------------
//Draws two rectangles at the sides of the screen
void DrawWalls() {
	glColor3f(0.5, 0.5, 0.5);

	glRectd(0, 0, 0 + wallSize, matrixSize);
	glRectd(matrixSize, matrixSize, (GLdouble)(matrixSize - wallSize), 0);
}