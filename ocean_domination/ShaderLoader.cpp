#include "ShaderLoader.h"
#include <fstream>
#include <iostream>

using namespace std;

ShaderLoader::ShaderLoader() {
	program_object = 0;
	v_shader = 0;
	f_shader = 0;
}

ShaderLoader::~ShaderLoader() {

}

int ShaderLoader::LoadShader() {
	GLint status = 0;

	char* vertex_shader_source = NULL;
	char* fragment_shader_source = NULL;

	vertex_shader_source = ReadShaderFile("test.vert");
	if(vertex_shader_source == NULL) {
		cout << "Failed to read the vertex shader" << endl;
		status = 1;
		return status;
	}

	fragment_shader_source = ReadShaderFile("test.frag");
	if(fragment_shader_source == NULL) {
		cout << "Failed to read the fragment shader" << endl;
		status = 1;
		return status;
	}

	v_shader = glCreateShader(GL_VERTEX_SHADER);
	f_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(v_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
	glShaderSource(f_shader, 1, (const GLchar**) &fragment_shader_source, NULL);

	glCompileShader(v_shader);
	glGetShaderiv(v_shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE) {
		cout << "Failed to compile the vertex shader" << endl;
		PrintInfoLog(1);
		return status;
	}

	glCompileShader(f_shader);
	glGetShaderiv(f_shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE) {
		cout << "Failed to compile the fragment shader" << endl;
		PrintInfoLog(2);
		return status;
	}

	program_object = glCreateProgram();

	glAttachShader(program_object, v_shader);
	glAttachShader(program_object, f_shader);

	glLinkProgram(program_object);
	glGetProgramiv(program_object, GL_LINK_STATUS, &status);
	if(status != GL_TRUE) {
		cout << "Failed to link the shader program object" << endl;
		PrintInfoLog(3);
		return status;
	}

	glUseProgram(program_object);

	return status;
}

void ShaderLoader::DetachShader() {
	glAttachShader(program_object, NULL);
}


char* ShaderLoader::ReadShaderFile(char* shader_file_name) {
	char* shader_contents = NULL;
	long size = 0;

	FILE* shader_file;
	
	shader_file = fopen(shader_file_name, "rt");

	if (shader_file != NULL) { 
		fseek(shader_file, 0, SEEK_END);
		size = ftell(shader_file);
		rewind(shader_file);
			
		shader_contents = new char[size];
		size = fread(shader_contents, sizeof(char), size, shader_file);
		shader_contents[size] = '\0';

		fclose(shader_file);
	}

	return shader_contents;
}

void ShaderLoader::PrintInfoLog(int object) {
	int log_length = 0;
	int characters_written = 0;
	char* log_info;

	if(object == 1) {
		glGetShaderiv(v_shader, GL_INFO_LOG_LENGTH, &log_length);

		if(log_length > 0) {
			log_info = new char[log_length];
			glGetShaderInfoLog(v_shader, log_length, &characters_written, log_info);

			cout << log_info << endl;
		}
	} else if(object == 2) {
		glGetShaderiv(f_shader, GL_INFO_LOG_LENGTH, &log_length);

		if(log_length > 0) {
			log_info = new char[log_length];
			glGetShaderInfoLog(f_shader, log_length, &characters_written, log_info);

			cout << log_info << endl;
		}
	} else {
		glGetProgramiv(program_object, GL_INFO_LOG_LENGTH, &log_length);

		if(log_length > 0) {
			log_info = new char[log_length];
			glGetProgramInfoLog(program_object, log_length, &characters_written, log_info);

			cout << log_info << endl;
		}
	}

	delete log_info;
	log_info = NULL;
}