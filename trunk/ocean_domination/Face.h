#ifndef Face_H
#define Face_H

#include <vector>

using namespace std;

/*
This class defines the structure of the Face which is a vector of integers which stores all the face values. The number_of_vertices variable stores the number
of vertices that make up the face

It has a default constructor, a constructor with one parameter and a copy constructor along with a destructor
It has a pair of getters and setters for the vector of integers and number_of_vertices
*/

class Face {
public:
	Face();
	Face(int size);
	Face(const Face& source);
	~Face();

	vector<int> face;
	string texture_material;
};
#endif