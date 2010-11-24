#include "TextureLoader.h"

TextureLoader::TextureLoader() {
}

TextureLoader::~TextureLoader() {
}

GLuint* TextureLoader::LoadTextures(string* textures, int number_of_textures, GLuint* texture_image) {
	glGenTextures(number_of_textures, texture_image); // Texture name generation

	GLFWimage current_image;
	GLuint success;

	for(int i = 0; i < number_of_textures; i++) {
		success = glfwReadImage(textures[i].c_str(), &current_image, 0);

		if (success) {
			glBindTexture(GL_TEXTURE_2D, texture_image[i]); // Binding of texture name

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, current_image.Format, current_image.Width, current_image.Height, 0, current_image.Format, GL_UNSIGNED_BYTE, current_image.Data); // Texture specification
		} else {
			// Error occured
			//log error
			break;
		}

		glfwFreeImage(&current_image);
	}

	return texture_image;
}

