#ifndef File_Loader_H
#define File_Loader_H

#include "Helper.h"
#include "Vector.h"
#include "TextureVector.h"
#include "Face.h"
#include "MtlHandler.h"
#include <GL/glfw.h>
#include <GL/glext.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

/*
This class is used to load and save files along with generating normals and printing the vertices and faces of the object
There are 2 vectors, one of type Vertex and one of type Face which stores the vertices and the faces
*/

class ModelLoader {
private:
	struct model {
		vector<Vector> vertices;
		vector<TextureVector> texture_vertices;
		vector<Vector> normal_vertices;
		vector<Face> faces;
		//vector<MtlHandler> materials;
		vector<int> start_index;
		vector<string> objects;

		int end_index;
		int smooth_shading;
	};

	vector<model>* game_models;
	vector<MtlHandler>* materials;

	string current_file;
	fstream read_model_file;
	int array_size;

	string TrimSpaces(string& current_line);
	//int ParseModels(vector<string>* models, model current_model);

public:
	ModelLoader();
	~ModelLoader();

	int LoadModel(vector<string>* models);
	int LoadMaterials(vector<string>* material_files);
	int GenerateModelDisplayList(vector<GLuint>* model_display_list, GLuint* texture_images, vector<string>* texture_file_names);
	int GenerateModelVBO(vector<GLuint>* model_vbo_id, GLuint* texture_images, vector<string>* texture_file_names);
};
#endif