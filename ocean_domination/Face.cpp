#include <iostream>
#include "Face.h"

using namespace std;

/*
The default constructor which sets the number of vertices to a minimum size of 3 and resizes the face vector to the minimum size
*/
Face::Face() {
	this->texture_material = "null";
}

/*
Parameter constructor sets the number of vertices to a the size provided and resizes the face vector to the the size
*/
Face::Face(int size) {
	this->texture_material = size;
	this->face.resize(size);
}

/*
The copy constructor copies the values of the source Face to the local Face by setting the number of vertices and the vector<int> to the source vector
*/
Face::Face(const Face& source): 
face(source.face),
texture_material(source.texture_material) {
}

/*
The default constructor
*/
Face::~Face() {
}