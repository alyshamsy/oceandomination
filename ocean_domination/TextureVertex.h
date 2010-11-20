#ifndef Texture_Vertex_H
#define Texture_Vertex_H

/*
This class defines the structure of the vertex which holds the x, y and z component of the point

It contains the default constructor, a constructor with 3 components as parameters and a copy constructor
It also contains the destructor and a setter method
There are also operator overload methods for = and ==
*/

class TextureVertex {
public:
	TextureVertex();
	TextureVertex(float a, float b);
	TextureVertex(const TextureVertex& source);
	~TextureVertex();

	float u, v;
};
#endif