#include "GameWorld.h"
#include <GL/glfw.h>
#include <FTGL/ftgl.h>

using namespace std;

GameWorld current_game;

//displays the text on the screen
void load_text(string text, string font_type, FTPoint& position) {
	FTGLPixmapFont font(font_type.c_str());
	font.FaceSize(72);
	font.Render(text.c_str(), -1, position);
}

//initialize the game
void init() {
	int window_width = 1280, window_height = 720;
	float aspect_ratio = (float)window_width/window_height;

	// Initialise GLFW
	if( !glfwInit() ) {
		exit( EXIT_FAILURE );
	}
	
	// Open an OpenGL window in Full Screen mode
	if( !glfwOpenWindow( window_width, window_height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwSetWindowTitle("Ocean Domination");

	//initialize GLEW
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0")) {
		cout << "OpenGL 2.0 not supported" << endl;
		exit(1);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	 
    //glShadeModel (GL_SMOOTH); 
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//set up the camera and the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, aspect_ratio, 1.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);

	//write loading... on the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		//add a texture image for the game
		//add flashing for the loading sign
	FTPoint text_position(window_width*0.36, window_height*0.25);
	load_text("Loading...", "../fonts/CAMBRIA.ttf", text_position);
	glfwSwapBuffers();

	current_game.InitializeGameWorld();
}

//exits the game
void exit() {
	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit( EXIT_SUCCESS );
}

int main() {
	int running = GL_TRUE;

	//initialize the openGL window
	init();

	//create a function to setup the world
	current_game.CreateGameWorld();

	// Main loop
	while( running ) {
		
		//updates the world
		current_game.UpdateGameWorld();

		int window_width = 1280, window_height = 720;
		float aspect_ratio = (float)window_width/window_height;


		//single player view
		glViewport(0,0, window_width, window_height);
		glScissor(0,0, window_width, window_height);
		glEnable(GL_SCISSOR_TEST);
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//set up the camera and the viewport
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(75.0, aspect_ratio, 1.0, 10000.0);

		glMatrixMode(GL_MODELVIEW);

		current_game.draw_world();

		/*	
			//2nd view
			glViewport(0,0, window_width/4.0f, window_height/4.0f);
			glScissor(0,0, window_width/4.0f, window_height/4.0f);
			glEnable(GL_SCISSOR_TEST);// OpenGL rendering goes here ...
			glClear( GL_DEPTH_BUFFER_BIT );

			//set up the camera and the viewport
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(35.0, aspect_ratio, 1.0, 10000.0);

			glMatrixMode(GL_MODELVIEW);

			//transform for second player			
			current_game.draw_world();
		*/

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