#ifndef Vertex_H
#define Vertex_H

/*
This class defines the structure of the vertex which holds the x, y and z component of the point

It contains the default constructor, a constructor with 3 components as parameters and a copy constructor
It also contains the destructor and a setter method
There are also operator overload methods for = and ==
*/

class Vertex {
public:
	Vertex();
	Vertex(float a, float b, float c);
	Vertex(const Vertex& source);
	~Vertex();

	void set_vertex(double a, double b, double c);
	Vertex& operator=(const Vertex& rhs);
	friend Vertex operator+(Vertex& A, Vertex& B);
	friend Vertex operator-(Vertex& A, Vertex& B);
	bool operator==(const Vertex& rhs);

	float x, y, z;
};
#endif