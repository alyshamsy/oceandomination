#ifndef Shader_Loader_H
#define Shader_Loader_H

#include <GL/glew.h>
#include <string>

using namespace std;

class ShaderLoader {
public:
	ShaderLoader();
	~ShaderLoader();

	int LoadShader(string& vertex_shader_file, string& fragment_shader_file, GLint current_program);
	void DetachShader(GLint program);

	GLuint program_object, v_shader, f_shader;

private:
	char* ReadShaderFile(string& shader_file_name);
	void PrintInfoLog(int object);
};

#endif