#ifndef Helper_H
#define Helper_H

#include <GL/glfw.h>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

GLfloat** generate_vector(int rows, int cols);
void delete_vector(GLfloat** my_vector, int row);

//string** generate_arrays(int rows, int cols);
//void delete_array(string** my_vector, int row);

GLfloat random_number_generator(int low_bound, int high_bound);
float getHypotenuse(float a, float b);

float getSmallestValue(vector<float> arr, int& index);

#endif