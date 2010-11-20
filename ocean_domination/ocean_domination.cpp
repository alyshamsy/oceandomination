#include <GL/glfw.h>
#include <stdlib.h>

#include "ModelLoader.h"

float translation_value = 0.0;

float red = 0.5,
	  blue = 0.5,
	  green = 0.5;

void init() {
	int window_width = 800, window_height = 600;
	float aspect_ratio = (float)window_width/window_height;

	// Initialise GLFW
	if( !glfwInit() ) {
		exit( EXIT_FAILURE );
	}

	// Open an OpenGL window
	if( !glfwOpenWindow( window_width, window_height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect_ratio, 1.0, 50.0);

	glMatrixMode(GL_MODELVIEW);
}

void exit() {
	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit( EXIT_SUCCESS );
}

void draw_model(float red, float blue, float green) {
	glBegin(GL_QUADS);
		glColor3f(red, blue, green);
		glVertex3f(0.5, 0.5, 0);   //A
		glVertex3f(0.5, -0.5, 0);  //B
		glVertex3f(-0.5, -0.5, 0); //C
		glVertex3f(-0.5, 0.5, 0);  //D
		glColor3f(red*0.2, blue*0.2, green*0.2);
		glVertex3f(0.5, 0.5, 0);     //A
		glVertex3f(0.5, 0.5, -1.0);  //F
		glVertex3f(0.5, -0.5, -1.0); //G
		glVertex3f(0.5, -0.5, 0);    //B
		glColor3f(red*0.4, blue*0.4, green*0.4);
		glVertex3f(-0.5, 0.5, -1.0);  //E
		glVertex3f(0.5, 0.5, -1.0);   //F
		glVertex3f(0.5, -0.5, -1.0);  //G
		glVertex3f(-0.5, -0.5, -1.0); //H
		glColor3f(red*0.6, blue*0.6, green*0.6);
		glVertex3f(-0.5, 0.5, 0);     //D
		glVertex3f(-0.5, 0.5, -1.0);  //E
		glVertex3f(-0.5, -0.5, -1.0); //H
		glVertex3f(-0.5, -0.5, 0);    //C
		glColor3f(red*0.8, blue*0.8, green*0.8);
		glVertex3f(-0.5, -0.5, -1.0); //H
		glVertex3f(0.5, -0.5, -1.0);  //G
		glVertex3f(0.5, -0.5, 0);     //B
		glVertex3f(-0.5, -0.5, 0);    //C
		glColor3f(red*0.95, blue*0.95, green*0.95);
		glVertex3f(-0.5, 0.5, -1.0);  //E
		glVertex3f(0.5, 0.5, -1.0);   //F
		glVertex3f(0.5, 0.5, 0);      //A		
		glVertex3f(-0.5, 0.5, 0);     //D
	glEnd();
}

void draw_wall() {
	glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(2.5, 2.5, -10);   //A
		glVertex3f(2.5, -2.5, -10);  //B
		glVertex3f(-2.5, -2.5, -10); //C
		glVertex3f(-2.5, 2.5, -10);  //D
	glEnd();
}

void render() {
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.0);

	if(glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
		translation_value += 0.2f;
	}

	if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
		translation_value -= 0.2f;
	}

	glPushMatrix();
	
	draw_model(red, blue, green);

	glTranslatef(0.0, 0.0, translation_value);
	draw_wall();

	glPopMatrix();
}

int main() {
	int running = GL_TRUE;

	ModelLoader test;
	string current_model = "../models/ship.obj";
	test.LoadModel(current_model);

	//initialize the openGL window
	init();

	// Main loop
	while( running ) {
		// OpenGL rendering goes here ...
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		render();

		// Swap front and back rendering buffers
		glfwSwapBuffers();

		// Check if ESC key	was pressed or window was closed
		running = !glfwGetKey( GLFW_KEY_ESC ) &&
					glfwGetWindowParam( GLFW_OPENED );
	}

	//exit the openGL window
	exit();

	return 0;
}