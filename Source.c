/*
*** FILE NAME   : csl_clipping.c
*** DESCRIPTION : This program is an example program using OpenGL.
*** DATE        : Jan. 2015
*** WRITTEN By  : farheen
*/

/*
Program performs Clipping using Cohen Sutherland Algorithm

Uses Outcode to determine if line is Above, Below, Left, Right of a Clipping window.
Data structures used:

Methods used:

draw_rosette() --> draws the 7-rosette
draw_rectangle --> takes the cooridnates of lower and uppper boundary after user selects with mouse and draws a clipping boundary.

cohen_sutherland_clipping() --> takes the two end points of a line as parameters and determines if it is inside, outside, partially in or partially outside.
compute_outcode() --> takes x and y coordinates of a line and finds the outcode (above, below, left, right)
draw_line() --> after the clipping, draws the clipped line segment
display_text() --> displays the instructions on screen.

Mode of Execution:

1. Draw the rosette --> draw_rosette()
2. Prompt the user to select lower and upper boundary and then draw the clipping window. --> draw_rectangle()
3. Clip the lines against the clipping window --> cohen_sutherland_clipping(). This function internally calls compute_outcode().
4. Draw the clipped lines --> draw_line()

*/

#include <stdio.h>              // standard C libraries
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <GL/glut.h>            // GLUT library
#include "cs_graphics_setup.h"  // Header for CS4250/5250/6250 courses

//@@***********************************************************************************@@
// Constants
#define WINDOW_XS 550					// Window size
#define WINDOW_YS 550
#define WINDOW_NAME "Cohen Sutherland Cliping"	// Window name

//@@***********************************************************************************@@
// Structure
typedef struct pt2d
{
	GLint x, y;
}My2DPoint;

//@@***********************************************************************************@@
// Function prototypes
void display_func(void);
void keyboard_func(unsigned char c, int x, int y);
void mouse_func(int button, int state, int x, int y);

void draw_rosette();
void draw_rectangle();

void cohen_sutherland_clipping(int, int, int, int);
void compute_outcode(int[], int, int);
void draw_line(int, int, int, int);
void display_text(char *string, int x, int y);

//@@***********************************************************************************@@
// Global Variables
My2DPoint strtPt, endPt;
My2DPoint recPt1, recPt2;

int pt[7][2] = { { 130, 280 }, { 100, 150 }, { 175, 20 }, { 325, 20 }, { 400, 150 }, { 370, 280 }, { 250, 410 } };
int count_mouse_click = 1;
int flag_draw_rect = 0;
//@@***********************************************************************************@@
int main(int argc, char **argv)
{
	recPt1.x = 1, recPt1.y = 2, recPt2.x = 1, recPt2.y = 1;

	glutInit(&argc, argv);

	init_setup(WINDOW_XS, WINDOW_YS, WINDOW_NAME);

	glutDisplayFunc(display_func);			// call back for display event
	glutKeyboardFunc(keyboard_func);		// call back for keyboard event
	glutMouseFunc(mouse_func);				// call back for mouse event

	glutMainLoop();

	return 1;
}	// end of main()

//@@***********************************************************************************@@
void display_func(void)
{
	glClearColor(0.0, 0.0, 1.0, 1.0);   // background color (blue)
	glClear(GL_COLOR_BUFFER_BIT);       // clearing the buffer not to keep the color

	display_text("Select a lower and upper boundary of clipping window", 20, 470);
	draw_rosette();
	if (flag_draw_rect == 1) //when the user selects lower and upper boundary , flag is set to 1
	{
		draw_rectangle();
		draw_rosette();
	}
	glFlush();
}	// end of display_func()

//@@***********************************************************************************@@
//this function draws a 7- rosette . the coordinates are stored in an array pt.
void draw_rosette()
{
	int i = 0, j = 0;
	glColor3f(1.0, 0.0, 0.0);			// setting pen color (red)

	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 7; j++)
		{
			if (j == i)
				continue;
			else
			{
				glBegin(GL_LINES);
					glVertex2i(pt[i][0], pt[i][1]);
					glVertex2i(pt[j][0], pt[j][1]);
				glEnd();
				cohen_sutherland_clipping(pt[i][0], pt[i][1], pt[j][0], pt[j][1]);
			}
		}
	}
}

//@@***********************************************************************************@@
//this function draws the clipping window. The coordinates are provided by the user.
void draw_rectangle()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
		glVertex2i(recPt1.x, recPt2.y);
		glVertex2i(recPt1.x, recPt1.y);
		glVertex2i(recPt2.x, recPt1.y);
		glVertex2i(recPt2.x, recPt2.y);
	glEnd();
}

//@@***********************************************************************************@@
void keyboard_func(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'q':
	case 'Q':
		exit(1);
		break;

	}  // end of switch
}	// end of keyboard_func()

//@@***********************************************************************************@@
void mouse_func(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (count_mouse_click == 1)
		{
			recPt1.x = x;
			recPt1.y = WINDOW_YS - y;
			count_mouse_click++;
		}
		else if (count_mouse_click == 2)
		{
			recPt2.x = x;
			recPt2.y = WINDOW_YS - y;
			count_mouse_click++;
			flag_draw_rect = 1;
		}
	}
	glutPostRedisplay();
}	// end of mouse_func()

//@@***********************************************************************************@@
void cohen_sutherland_clipping(int x1, int y1, int x2, int y2)
{
	int accept = 0; //0->false , 1->true
	int done = 0; //0->false , 1->true
	int k = 0;
	
	int outcode1[4] = { 0, 0, 0, 0 }, outcode2[4] = { 0, 0, 0, 0 };
	
	int outcode[4]; //Stores the Logical And output outcode - Trivial Reject case
	int count_and = 0; //counts the number of zeroes in the logical And operation for Trivial Reject case
	int count_or = 0;

	int x=0, y=0;
	
	while (!done)
	{
		compute_outcode(outcode1, x1, y1);
		compute_outcode(outcode2, x2, y2);

		//Trivial Reject case
		for (k = 0; k < 4; k++)
		{
			if (outcode1[k] & outcode2[k] == 0)
			{
				count_and++;
			}
		}
		if (count_and != 4)
		{
			done = 1;
			break;
		}
			
		//Trivial Accept case
		for (k = 0; k < 4; k++)
		{
			if (outcode1[k] | outcode2[k] == 0)
			{
				count_or++;
			}
		}
		if (count_or == 4)
		{
			done = 1;
			accept = 1;
			draw_line(x1, y1, x2, y2);
		}
		
		//Line is partially in and partially out
		//Find the intersection point of Line with the clip window.
		
		//point is out and above the clip window
		if (outcode1[0] != 0)
		{
			x1 = x1 + ((x2 - x1) *(recPt2.y - y1))/ (y2 - y1);
			y1 = recPt2.y;
		}

		//point is out and below the clip window
		else if (outcode1[1] != 0)
		{
			x1 = x1 + (x2 - x1) / (y2 - y1)*(recPt1.y - y1);
			y1 = recPt1.y;
		}

		//point is out and left of the clip window
		else if (outcode1[2] != 0)
		{
			y1 = y1 + (y2 - y1) / (x2 - x1)*(recPt2.x - x1);
			x1 = recPt1.x;
		}

		//point is out and right of the clip window
		else
		{
			y1 = y1 + (y2 - y1) / (x2 - x1)*(recPt2.x - x1);
			x1 = recPt1.x;
		}

		compute_outcode(outcode1, x1, y1);
		draw_line(x1, y1, x2, y2);

	}//end while
}
//@@***********************************************************************************@@
void  compute_outcode(int outcode[4], int x, int y)
{
	//Above Below Left Right
	//rectPt1 is lower left point so, rectPt1.X and rectPt1.Y are min
	//rectPt2 is upper right point. So, rectPt2.x and rectPt2.y are max

	for (int i = 0; i<4; i++)
		outcode[i] = 0;

	//Check Above
	if (y > recPt2.y)
	{
		outcode[0] = 1;
	}

	//Check Below
	if (y < recPt1.y)
	{
		outcode[1] = 1;
	}

	//Check Left
	if (x < recPt1.x)
	{
		outcode[2] = 1;
	}

	//Check Right
	if (x > recPt2.x)
	{
		outcode[3] = 1;
	}
}
//@@***********************************************************************************@@
void draw_line(int x1, int y1, int x2, int y2)
{
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}
//@@***********************************************************************************@@
void display_text(char *string, int x, int y)
{
	glColor3f(1.0, 1.0, 0.0);
	int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(string); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}