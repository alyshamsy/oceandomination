#include "Helper.h"

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

GLfloat random_number_generator(int size) {
	GLfloat random_number = 0.0;

	return random_number;
}