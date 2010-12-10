#include "GameWorld.h"
#include <GL/glfw.h>
#include <FTGL/ftgl.h>

using namespace std;

GameWorld current_game;
int window_width = 1280, window_height = 720;
//int window_width = 800, window_height = 600;

//displays the text on the screen
void load_text(string text, string font_type, FTPoint& position, unsigned int fontsize) {
	FTGLPixmapFont font(font_type.c_str());
	font.FaceSize(fontsize);
	font.Render(text.c_str(), -1, position);
}

//initialize the game
void init() {
	
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
	FTPoint game_name_1(window_width*0.4, window_height*0.75);
	load_text("Ocean", "../fonts/ALGER.ttf", game_name_1, 72);

	FTPoint game_name_2(window_width*0.33, window_height*0.65);
	load_text("Domination", "../fonts/ALGER.ttf", game_name_2, 72);

	FTPoint text_position(window_width*0.36, window_height*0.25);
	load_text("Loading...", "../fonts/CAMBRIA.ttf", text_position, 72);
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
	int quit = GL_TRUE;
	int game_end = 0;

	//initialize the openGL window
	init();

	//create a function to setup the world
	current_game.CreateGameWorld();

	// Main loop
	while( running ) {
		
		//updates the world
		game_end = current_game.UpdateGameWorld();

		if(game_end != 0) {
			running = GL_FALSE;
			break;
		}

		//sets up the camera for the game
		current_game.setup_camera();

		// Swap front and back rendering buffers
		glfwSwapBuffers();

		// Check if ESC key	was pressed or window was closed
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	while(quit) {
		//write loading... on the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			//add a texture image for the game
			//add flashing for the loading sign
		if(game_end == 0) {
			FTPoint text_position(window_width*0.1, window_height*0.5);
			load_text("Thank you for Playing Ocean Domination", "../fonts/ALGER.ttf", text_position, 48);
		} else {
			FTPoint text_position(window_width*0.35, window_height*0.75);
			load_text("Game Over", "../fonts/ALGER.ttf", text_position, 72);

			if(game_end == 1) {
				FTPoint text_position(window_width*0.38, window_height*0.35);
				load_text("You Lose!", "../fonts/ALGER.ttf", text_position, 72);
			} else if(game_end == 2) {
				FTPoint text_position(window_width*0.39, window_height*0.35);
				load_text("You Win!", "../fonts/ALGER.ttf", text_position, 72);
			}
		}

		FTPoint exit_text_position(window_width*0.45, window_height*0.1);
		load_text("Press Enter to Exit", "../fonts/ALGER.ttf", exit_text_position, 20);

		glfwSwapBuffers();

		quit = !glfwGetKey( GLFW_KEY_ENTER ) && glfwGetWindowParam( GLFW_OPENED );
	}
	
	//exit the openGL window
	exit();
	
	return 0;
}