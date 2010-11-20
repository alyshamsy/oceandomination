#include <iostream>
#include "Vertex.h"

using namespace std;

/*
The default constructor of the vertex which initiates the x, y and z components to be 0
*/
Vertex::Vertex() {
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

/*
The parameter constructor which takes in 3 parameters a, b and c and assigns them to x, y and z respectively
*/
Vertex::Vertex(float a, float b, float c) {
	this->x = a;
	this->y = b;
	this->z = c;
}

/*
The copy constructor which copies the contents of the source vertex to the local vertex
*/
Vertex::Vertex(const Vertex& source):
x(source.x),
y(source.y),
z(source.z) {
}

/*
The default constructor
*/
Vertex::~Vertex() {
}

/*
The assignment operator is used to equate the local vertex to the value provided by the parameter and returns the local vertex
*/
Vertex& Vertex::operator=(const Vertex& rhs) {
	if(this == &rhs)
		return (*this);

	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;

	return (*this);
}

/*
The equals operation which checks if the values of the parameter vertex is the same as the local vertex and returns true else returns false
*/
bool Vertex::operator==(const Vertex& rhs) {
	if(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z) {
		return true;
	} else {
		return false;
	}
}