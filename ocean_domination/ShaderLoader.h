#ifndef Shader_Loader_H
#define Shader_Loader_H

#include <GL/glew.h>
#include <string>

using namespace std;

class ShaderLoader {
public:
	ShaderLoader();
	~ShaderLoader();

	int LoadShader(string& file_name);
	int DetachShader();

private:
	char* ReadShaderFile(string& shader_file_name);
	
	char* vertex_shader;
	char* fragment_shader;
};

#endif