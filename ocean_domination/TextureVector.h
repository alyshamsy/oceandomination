#ifndef Texture_Vertex_H
#define Texture_Vertex_H

/*
This class defines the structure of the vector which holds the x and y component of the point

It contains the default constructor, a constructor with 2 components as parameters and a copy constructor
It also contains the destructor and a setter method
*/

class TextureVector {
public:
	TextureVector();
	TextureVector(float a, float b);
	TextureVector(const TextureVector& source);
	~TextureVector();

	float u, v;
};
#endif