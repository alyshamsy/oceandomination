#include <iostream>
#include "Vector.h"

using namespace std;

/*
The default constructor of the vertex which initiates the x, y and z components to be 0
*/
Vector::Vector() {
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

/*
The parameter constructor which takes in 3 parameters a, b and c and assigns them to x, y and z respectively
*/
Vector::Vector(float a, float b, float c) {
	this->x = a;
	this->y = b;
	this->z = c;
}

/*
The copy constructor which copies the contents of the source vertex to the local vertex
*/
Vector::Vector(const Vector& source):
x(source.x),
y(source.y),
z(source.z) {
}

/*
The default constructor
*/
Vector::~Vector() {
}

/*
The setter method which takes in 3 parameters a, b and c and assigns them to x, y and z respectively
*/
void Vector::set_vector(float a, float b, float c) {
	this->x = a;
	this->y = b;
	this->z = c;
}

/*
The assignment operator is used to equate the local vector to the value provided by the parameter and returns the local vector
*/
Vector& Vector::operator=(const Vector& rhs) {
	if(this == &rhs)
		return (*this);

	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;

	return (*this);
}

/*
the addition operation adds the 2 vectors provided and returns a vector with the respective x, y and z variables
*/
const Vector Vector::operator+(const Vector& other) {
	Vector added_vector;

	added_vector.x = this->x + other.x;
	added_vector.y = this->y + other.y;
	added_vector.z = this->z + other.z;

	return added_vector;
}

/*
the subtraction operation subtracts the 2 vectors provided and returns the vector with the respective x, y and z variables
*/
const Vector Vector::operator-(const Vector& other) {
	Vector subtracted_vector;

	subtracted_vector.x = this->x - other.x;
	subtracted_vector.y = this->y - other.y;
	subtracted_vector.z = this->z - other.z;

	return subtracted_vector;
}

/*
the multiplication operation multiplies the 2 vectors provided and returns the vector with the respective x, y and z variables
*/
const Vector Vector::operator*(const float value) {
	Vector multiplied_vector;

	multiplied_vector.x = this->x * value;
	multiplied_vector.y = this->y * value;
	multiplied_vector.z = this->z * value;

	return multiplied_vector;
}

/*
the addition operation adds the 2 vectors provided and returns a vertex with the respective x, y and z variables added
*/
Vector& Vector::operator+=(const Vector& rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return (*this);
}

Vector& Vector::operator-=(const Vector& rhs) {
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return (*this);
}

/** Multiplies this vector by the given scalar. */
void Vector::operator*=(const float value) {
    this->x *= value;
    this->y *= value;
    this->z *= value;
}


/*
The equals operation which checks if the values of the parameter vertex is the same as the local vertex and returns true else returns false
*/
bool Vector::operator==(const Vector& rhs) {
	if(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z) {
		return true;
	} else {
		return false;
	}
}

/*
returns the magnitude of the vector by using the pythagorean formula
*/
float Vector::getVectorMagnitude() {
	float magnitude, x, y, z;

	x = this->x;
	y = this->y;
	z = this->z;

	magnitude = sqrt((x*x) + (y*y) + (z*z));

	return magnitude;
}

/** Turns a non-zero vector into a vector of unit length. */
void Vector::normalise() {
    float l = getVectorMagnitude();
    if (l > 0) {
        (*this) *= 1.0f/l;
    }
}

/*
returns the dot product as a double value of the 2 vectors by multiplying the respective components
*/
float Vector::getDotProduct(Vector& A, Vector& B) {
	float dot_product;

	dot_product = ((A.x*B.x) + (A.y*B.y) + (A.z*B.z));

	return dot_product;
}

/*
returns the angle between the 2 vectors as a double value by calculating the dot product and the magnitudes and getting the arc cos of the dot product divided by the multiplication of the magnitudes of the 2 vectors
*/
float Vector::getAngleBetweenVectors(Vector& A, Vector& B) {
	float angle, dot_product, magnitude_A, magnitude_B;

	dot_product = getDotProduct(A, B);
	magnitude_A = A.getVectorMagnitude();
	magnitude_B = B.getVectorMagnitude();

	float a = (dot_product)/(magnitude_A*magnitude_B);
	if(a>1)
	{
		a = 1;
	}
	if(a< -1)
	{
		a = -1;
	}
	
	angle = acos(a);

	return angle;
}

/*
returns the negation of the vector provided as the parameter 
*/
void Vector::Negate() {
	this->x = -(this->x);
	this->y = -(this->y);
	this->z = -(this->z);
}

/*
clears the current vector
*/
void Vector::Clear() {
	this->x = this->y = this->z = 0;
}

/*
return the unit vector of the parameter vector provided by calculating the magnitude of the vector and dividing each of the coordinates by the magnitude
*/
Vector Vector::getUnitVector() {
	Vector unit_vector;
	float magnitude = this->getVectorMagnitude();

	unit_vector.x = this->x/magnitude;
	unit_vector.y = this->y/magnitude;
	unit_vector.z = this->z/magnitude;

	return unit_vector;
}

/*
calculates the cross product of the 2 vectors and returns a vector
*/
Vector Vector::getCrossProduct(Vector& A, Vector& B) {
	Vector cp_vector;

	cp_vector.x = ((A.y*B.z) - (A.z*B.y));
	cp_vector.y = ((A.z*B.x) - (A.x*B.z));
	cp_vector.z = ((A.x*B.y) - (A.y*B.x));

	return cp_vector;
}

/*
calculates the normal vector of the 2 vectors provided by calculating the cross product and angle between them and their respective magnitudes
*/
Vector Vector::getNormalVector(Vector& A, Vector& B) {
	float angle, denominator, magnitude_A, magnitude_B;
	Vector normal_vector;

	Vector cross_product = getCrossProduct(A, B);
	magnitude_A = A.getVectorMagnitude();
	magnitude_B = B.getVectorMagnitude();
	angle = getAngleBetweenVectors(A, B);
	
	denominator = (magnitude_A*magnitude_B*sin(angle));

	normal_vector.x = (cross_product.x)/denominator;
	normal_vector.y = (cross_product.y)/denominator;
	normal_vector.z = (cross_product.z)/denominator;

	return normal_vector;
}

/*
uses method overloading

calculates the normal of the 3 vectors provided. It first checks if any 2 vectors are the same. if yes then returns 0, 0, 0 else calculates 2 vectors and getting the normal vector between 2 vectors
*/
Vector Vector::getNormalVector(Vector& A, Vector& B, Vector& C) {
	Vector normal_unit_vector;

	if(!(A == B || A == C || B == C)) {
		Vector X;
		Vector Y;

		X = A - B;
		Y = B - C;

		normal_unit_vector = getNormalVector(X, Y).getUnitVector();
	}

	return normal_unit_vector;
}

/*
computes vector translation by adding the 2 vectors provided
*/
void Vector::doVectorTranslation(Vector& A, Vector& input_vector) {
	A = A + input_vector;
}

/**
* Adds the given vector to this, scaled by the given amount.
*/
void Vector::addScaledVector(const Vector& vector, float scale) {
    this->x += vector.x * scale;
    this->y += vector.y * scale;
    this->z += vector.z * scale;
}

void Vector::printVector() {
	cout << '(' << this->x << ", " << this->y << ", " << this->z << ')' << endl;
}