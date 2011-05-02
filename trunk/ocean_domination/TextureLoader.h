#ifndef Texture_loader_H
#define Texture_loader_H

#include <GL/glfw.h>
#include <string>
#include <vector>

using namespace std;

class TextureLoader {
public:
	TextureLoader();
	~TextureLoader();

	void LoadTextures(vector<string>* textures, int number_of_textures, GLuint* texture_image);
	GLuint LoadTexture(string current_texture, GLuint &texture_image);
};
#endif