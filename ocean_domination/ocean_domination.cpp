#include <GL/glfw.h>
#include <FTGL/ftgl.h>
#include <stdlib.h>
#include <fstream>
#include "ModelLoader.h"
#include "TextureLoader.h"

float translation_value;

ModelLoader ship;
TextureLoader textures;

GLuint ship_list;
GLuint* texture_images;

string* texture_file_names;

//generates a 2D array with the number of rows and columns
GLfloat** generate_vector(int rows, int cols) {
  GLfloat** my_vector = new GLfloat* [rows];
  for (int j = 0; j < rows; j ++)
     my_vector[j] = new GLfloat[cols];
  
  return my_vector;
}

// Deletes a 2D array pointed by 'my_vector' that has 'row' number rows
void delete_vector(GLfloat** my_vector, int row) {
  for (int j = 0; j < row; j ++)
     delete [] my_vector[j];

  delete [] my_vector;
}

//prints a 2D array pointed by 'my_vector'
void print_vector(GLfloat** my_vector, int rows, int cols) {
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			cout << my_vector[i][j] << "\t";
		}
		cout << "\n";
	}
	cout << "\n";
}

//handle to read models.txt and load models in parallel
int load_models() {
	string ship_model = "../models/ship.obj";
	ship.LoadModel(ship_model);

	return 0;
}

//read the texture file and load all the textures
int load_textures(string& texture_file) {
	int number_of_textures;
	string texture_file_name;
	texture_file_names = new string[number_of_textures];
	fstream texture_loader;

	texture_loader.open(texture_file, ios::in);

	if(!texture_loader) {
		return 1;
	}

	texture_loader.ignore(1024, '\n');
	texture_loader >> number_of_textures;

	for(int i = 0; i < number_of_textures; i++) {
		texture_loader >> texture_file_name;
		texture_file_names[i] = texture_file_name;
	}

	textures.LoadTextures(texture_file_names, number_of_textures, texture_images);

	return 0;
}

//generate the call list from the model and the call list value
int generate_call_list(ModelLoader& model, GLuint model_call_list) {
	if(model_call_list != 0) {
		string current_material;
		string current_texture;

		GLfloat** vertex_vector = generate_vector(3, 3);
		GLfloat** normals_vector = generate_vector(3, 3);
		GLfloat** texture_vector = generate_vector(3, 2);

		glNewList(model_call_list, GL_COMPILE);
			for(int i = 0; i < model.current_model.faces->size(); i++) {
				//load materials and bind textures
				//if the current material is the same as in previous iteration do not relaoad the material values
				if(current_material.compare(model.current_model.faces->at(i).texture_material) != 0) {
					current_material = model.current_model.faces->at(i).texture_material;
					if(current_material.compare("(null)") != 0) {
						int material_index = 0;
						while(current_material.compare(model.current_model.materials->at(material_index).newmtl) != 0)
							material_index++;

						//assign material values
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, model.current_model.materials->at(material_index).Ns);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*) &model.current_model.materials->at(material_index).Ka);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*) &model.current_model.materials->at(material_index).Kd);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) &model.current_model.materials->at(material_index).Ks);

						//bind texture
						current_texture = model.current_model.materials->at(material_index).map_Kd;
						int texture_index = 0;
						while(current_texture.compare(texture_file_names[texture_index]) != 0)
							texture_index++;

						glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
					}
				}
				
				//load vectors
				int j = 0, vertex_index = 0, normals_index = 0, texture_index = 0, index = 0;
				while(j < model.current_model.faces->at(i).face.size()) {
					if(j%3 == 0) {
						index = model.current_model.faces->at(i).face.at(j) - 1;
						
						vertex_vector[vertex_index][0] = model.current_model.vertices->at(index).x;
						vertex_vector[vertex_index][1] = model.current_model.vertices->at(index).y;
						vertex_vector[vertex_index][2] = model.current_model.vertices->at(index).z;

						vertex_index++;
					} else if(j%3 == 1) {
						index = model.current_model.faces->at(i).face.at(j) - 1;

						texture_vector[texture_index][0] = model.current_model.texture_vertices->at(index).u;
						texture_vector[texture_index][1] = model.current_model.texture_vertices->at(index).v;	

						texture_index++;
					} else if(j%3 == 2) {
						index = model.current_model.faces->at(i).face.at(j) - 1;

						normals_vector[normals_index][0] = model.current_model.normal_vertices->at(index).x;
						normals_vector[normals_index][1] = model.current_model.normal_vertices->at(index).y;
						normals_vector[normals_index][2] = model.current_model.normal_vertices->at(index).z;

						normals_index++;
					}
					j++;
				}

				//draw the triangle
				glBegin(GL_TRIANGLES);
					//first point
					glTexCoord2fv(texture_vector[0]);
					glNormal3fv(normals_vector[0]);
					glVertex3fv(vertex_vector[0]);
					//second point
					glTexCoord2fv(texture_vector[1]);
					glNormal3fv(normals_vector[1]);
					glVertex3fv(vertex_vector[1]);
					//third point
					glTexCoord2fv(texture_vector[2]);
					glNormal3fv(normals_vector[2]);
					glVertex3fv(vertex_vector[2]);
				glEnd();
			}
		glEndList();

		delete_vector(vertex_vector, 3);
		delete_vector(normals_vector, 3);
		delete_vector(texture_vector, 3);
	}

	return 0;
}

void load_text(string text, string font_type, FTPoint& position) {
	FTGLPixmapFont font(font_type.c_str());
	font.FaceSize(72);
	font.Render(text.c_str(), -1, position);
}

void init() {
	int window_width = 800, window_height = 600;
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

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING); 
    glEnable (GL_LIGHT0); 
    glShadeModel (GL_SMOOTH); 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//set up the camera and the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(105.0, aspect_ratio, 1.0, 50.0);

	glMatrixMode(GL_MODELVIEW);

	//write loading... on the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	FTPoint text_position(250.0, 300.0);
	load_text("Loading...", "../fonts/CAMBRIA.ttf", text_position);
	glfwSwapBuffers();

	//load all models
	load_models();

	//load all textures
	string texture_file_name = "textures.txt";
	load_textures(texture_file_name);

	//create the call lists
	ship_list = glGenLists(1);

	//generate all the call lists
	generate_call_list(ship, ship_list);
}

void exit() {
	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit( EXIT_SUCCESS );
}

void draw_model(GLuint& model_list) {
	glCallList(model_list);
}

void draw_world() {
	//draw sky
	//draw sun
	//draw water
	//draw islands
	glBegin(GL_QUADS);
		glColor3f(0.53, 0.81, 0.92);
		glVertex3f(5.0, 5.0, -40);   //A
		glVertex3f(5.0, -5.0, -40);  //B
		glVertex3f(-5.0, -5.0, -40); //C
		glVertex3f(-5.0, 5.0, -40);  //D
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
	
	draw_model(ship_list);

	glTranslatef(0.0, 0.0, translation_value);
	draw_world();

	glPopMatrix();
}

int main() {
	int running = GL_TRUE;

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