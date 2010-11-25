#ifndef Shader_Loader_H
#define Shader_Loader_H

#include <GL/glew.h>
#include <string>

using namespace std;

class ShaderLoader {
public:
	ShaderLoader();
	~ShaderLoader();

	int LoadShader();
	void DetachShader();

	GLuint program_object, v_shader, f_shader;

private:
	char* ReadShaderFile(char* shader_file_name);
	void PrintInfoLog(int object);
};

#endif