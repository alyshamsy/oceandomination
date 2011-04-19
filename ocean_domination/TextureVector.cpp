#include <iostream>
#include "TextureVector.h"

using namespace std;

/*
The default constructor of the vertex which initiates the x, y and z components to be 0
*/
TextureVector::TextureVector() {
	this->u = 0.0;
	this->v = 0.0;
}

/*
The parameter constructor which takes in 3 parameters a, b and c and assigns them to x, y and z respectively
*/
TextureVector::TextureVector(float a, float b) {
	this->u = a;
	this->v = b;
}

/*
The copy constructor which copies the contents of the source vertex to the local vertex
*/
TextureVector::TextureVector(const TextureVector& source):
u(source.u),
v(source.v) {
}

/*
The default constructor
*/
TextureVector::~TextureVector() {
}
