#ifndef Helper_H
#define Helper_H

#include <GL/glfw.h>
#include <string>

using namespace std;

GLfloat** generate_vector(int rows, int cols);
void delete_vector(GLfloat** my_vector, int row);

//string** generate_arrays(int rows, int cols);
//void delete_array(string** my_vector, int row);

GLfloat random_number_generator(int size);

#endif