#include "TextureLoader.h"

TextureLoader::TextureLoader() {
}

TextureLoader::~TextureLoader() {
}

GLuint* TextureLoader::LoadTextures(vector<string>* textures, int number_of_textures, GLuint* texture_image) {
	glGenTextures(number_of_textures, texture_image); // Texture name generation

	GLFWimage current_image;
	GLuint success;
	string texture_file_name;

	for(int i = 0; i < number_of_textures; i++) {
		texture_file_name = "bin/images/";
		texture_file_name.append(textures->at(i));
		success = glfwReadImage(texture_file_name.c_str(), &current_image, 0);

		if (success) {
			glBindTexture(GL_TEXTURE_2D, texture_image[i]); // Binding of texture name

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	glTexImage2D(GL_TEXTURE_2D, 0, current_image.Format, current_image.Width, current_image.Height, 0, current_image.Format, GL_UNSIGNED_BYTE, current_image.Data); // Texture specification

			//Create and use Mipmaps
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR); // Mipmap Linear Filtering
			gluBuild2DMipmaps(GL_TEXTURE_2D, current_image.BytesPerPixel, current_image.Width, current_image.Height, current_image.Format, GL_UNSIGNED_BYTE, current_image.Data);
			
		} else {
			// Error occured
			//log error
			break;
		}

		glfwFreeImage(&current_image);
	}

	return texture_image;
}

GLuint TextureLoader::LoadTexture(string current_texture, GLuint &texture_image) {
	glGenTextures(1, &texture_image); // Texture name generation

	GLFWimage current_image;
	GLuint success;
	string texture_file_name;
	
	texture_file_name = "bin/images/";
	texture_file_name.append(current_texture);
	success = glfwReadImage(texture_file_name.c_str(), &current_image, 0);

	if (success) {
		glBindTexture(GL_TEXTURE_2D, texture_image); // Binding of texture name

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, current_image.Format, current_image.Width, current_image.Height, 0, current_image.Format, GL_UNSIGNED_BYTE, current_image.Data); // Texture specification
	} else {
		// Error occured
		//log error
	}

	glfwFreeImage(&current_image);

	return texture_image;
}