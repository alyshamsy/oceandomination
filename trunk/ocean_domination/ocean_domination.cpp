#include "GameWorld.h"
#include "Threads.h"
#include "Mutex.h"
#include <GL/glfw.h>
#include <FTGL/ftgl.h>

//#define THREAD_LOADING

using namespace std;

GameWorld current_game;
Mutex init_mutex;

int window_width = 1280, window_height = 720;

//initialize the game
void init() {	
	GLuint logo_texture;
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
	glShadeModel (GL_SMOOTH); 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

#ifdef THREAD_LOADING
	HGLRC mainContext = wglGetCurrentContext();
	HDC hdc = wglGetCurrentDC();
	wglMakeCurrent(hdc, mainContext);
	HGLRC initialization_context = wglCreateContext(hdc);
	
	bool error = wglShareLists(mainContext, initialization_context);

	if(error == FALSE) {
		cout << "failed to share resources" << endl;

		DWORD errorCode = GetLastError();
		LPVOID lpMsgBuf;
		
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf, 0, NULL);
		MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree(lpMsgBuf);

		//Destroy the GL context and just use 1 GL context
		wglDeleteContext(initialization_context);
	}

	InitializationThread init_thread(initialization_context, hdc);
	init_thread.start();
	init_thread.resume();
#endif

	//set up the camera and the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, aspect_ratio, 1.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);

	//Load the logo as a texture
    glGenTextures(1, &logo_texture);
    GLFWimage current_image;

    GLuint success = glfwReadImage("bin/images/company.tga", &current_image, 0);

    if(success) {
        glBindTexture(GL_TEXTURE_2D, logo_texture);
                        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, current_image.Format, current_image.Width, current_image.Height, 0, current_image.Format, GL_UNSIGNED_BYTE, current_image.Data); // Texture specification

        glfwFreeImage(&current_image);
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
    if(success) {
        glBindTexture(GL_TEXTURE_2D, logo_texture);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-3.0, -0.5, 0.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(-3.0, 0.5, 0.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(3.0, 0.5, 0.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(3.0, -0.5, 0.0);
        glEnd();
    } 

	glfwSwapBuffers();

#ifdef THREAD_LOADING
	init_thread.join();
#endif

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
	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF);
	
	double loading_time = glfwGetTime();
	init();
	cout << "Initialization took: " << glfwGetTime() - loading_time << endl;

	//create a function to setup the world
	current_game.CreateGameWorld();

	// Main loop
	while( running ) {
		//sets up the camera for the game
		current_game.setup_camera();

		// Swap front and back rendering buffers
		glFlush();
		glfwSwapBuffers();

		//updates the world
		game_end = current_game.UpdateGameWorld();

		if(game_end != 0) {
			running = GL_FALSE;
			break;
		}

		// Check if ESC key	was pressed or window was closed
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	while(quit) {
		//write loading... on the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			//add a texture image for the game
			//add flashing for the loading sign
		if(game_end == 0) {
			current_game.setFontSize(48);
			FTPoint text_position(window_width*0.2, window_height*0.5);
			current_game.load_text("Thank you for Playing Ocean Domination", text_position);
		} else {
			current_game.setFontSize(72);
			FTPoint text_position(window_width*0.35, window_height*0.75);
			current_game.load_text("Game Over", text_position);

			if(game_end == 1) {
				FTPoint text_position(window_width*0.38, window_height*0.35);
				current_game.load_text("You Lose!", text_position);
			} else if(game_end == 2) {
				FTPoint text_position(window_width*0.39, window_height*0.35);
				current_game.load_text("You Win!", text_position);
			}
		}

		current_game.setFontSize(20);
		FTPoint exit_text_position(window_width*0.45, window_height*0.1);
		current_game.load_text("Press Enter to Exit", exit_text_position);

		glfwSwapBuffers();

		quit = !glfwGetKey( GLFW_KEY_ENTER ) && glfwGetWindowParam( GLFW_OPENED );
	}
	
	//exit the openGL window
	exit();
	
	//_CrtDumpMemoryLeaks();

	return 0;
}

InitializationThread::InitializationThread(HGLRC renderContext, HDC currentDC):context(renderContext), hdc(currentDC) {

}

void InitializationThread::run() {
	wglMakeCurrent(hdc, context);

	init_mutex.lock_mutex();
		current_game.InitializeGameWorld();
	init_mutex.unlock_mutex();
}