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

GLfloat random_number_generator(int low_bound, int high_bound, float seed) {
	GLfloat random_number = 0.0;
	double time = glfwGetTime();

	if(seed == 0)
		seed = time;

	srand(seed);
	random_number = (float)((rand() % (high_bound - low_bound + 1)) + low_bound);

	return random_number;
}

float getHypotenuse(float a, float b) {
	float h = 0.0;

	h = sqrtf(a*a + b*b);

	return h;
}

float getSmallestValue(vector<float> arr, int& index) {
	int size = arr.size();

	float smallest_value = arr[0];
	index = 0;

	for(int i = 1; i < size; i++) {
		if(arr[i] < smallest_value) {
			smallest_value = arr[i];
			index = i;
		}
	}

	return smallest_value;
}

string intToString(int a) {
	string output;
	stringstream out;
	out << a;
	output = out.str();

	return output;
}

string floatToString(float a) {
	string output;
	stringstream out;
	out << a;
	output = out.str();

	return output;
}