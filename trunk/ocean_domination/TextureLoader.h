#ifndef Texture_loader_H
#define Texture_loader_H

#include <GL/glfw.h>
#include <string>

using namespace std;

class TextureLoader {
public:
	TextureLoader();
	~TextureLoader();

	GLuint* LoadTextures(string* textures, int number_of_textures, GLuint* texture_image);
};
#endif