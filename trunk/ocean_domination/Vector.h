#ifndef Vector_H
#define Vector_H

/*
This class defines the structure of the vertex which holds the x, y and z component of the point

It contains the default constructor, a constructor with 3 components as parameters and a copy constructor
It also contains the destructor and a setter method
There are also operator overload methods for = and ==
*/

class Vector {
public:
	Vector();
	Vector(float a, float b, float c);
	Vector(const Vector& source);
	~Vector();

	void set_vector(float a, float b, float c);
	Vector& operator=(const Vector& rhs);
	const Vector operator+(const Vector& other);
	const Vector operator-(const Vector& other);
	const Vector operator*(const float other);
	Vector& operator+=(const Vector& rhs);
	Vector& operator-=(const Vector& rhs);
	void operator*=(const float value);
	bool operator==(const Vector& rhs);

	float getVectorMagnitude();
	void normalise();
	float getDotProduct(Vector& A, Vector& B);
	float getAngleBetweenVectors(Vector& A, Vector& B);

	void Negate();	
	void Clear();
	Vector getUnitVector();	
	Vector getCrossProduct(Vector& A, Vector& B);
	Vector getNormalVector(Vector& A, Vector& B);
	Vector getNormalVector(Vector& A, Vector& B, Vector& C);
	void doVectorTranslation(Vector& A, Vector& input_vector);
	void addScaledVector(const Vector& vector, float scale);

	void printVector();

	float x, y, z;
};
#endif