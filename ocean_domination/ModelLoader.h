#ifndef File_Loader_H
#define File_Loader_H

#include "Vertex.h"
#include "TextureVertex.h"
#include "Face.h"
#include "MtlHandler.h"
#include <string>
#include <fstream>
#include <vector>

using namespace std;

/*
This class is used to load and save files along with generating normals and printing the vertices and faces of the object
There are 2 vectors, one of type Vertex and one of type Face which stores the vertices and the faces
*/

class ModelLoader {
public:
	ModelLoader();
	~ModelLoader();

	int LoadModel(string& model_file_name);

	struct Model {
		vector<Vertex>* vertices;
		vector<TextureVertex>* texture_vertices;
		vector<Vertex>* normal_vertices;
		vector<Face>* faces;
		vector<MtlHandler>* materials;
		int smooth_shading;
	} current_model;

private:
	string current_file;
	fstream read_model_file;
	int array_size;
	int ParseFile(string& file_name);

};
#endif