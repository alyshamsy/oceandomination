#include "ModelLoader.h"
#include <iostream>
#include <sstream>

using namespace std;

/*
The constructor which allocates memory on the heap for the vertex and the face vectors. Because no values have been provided, it initially creates a vector of size 1
*/
ModelLoader::ModelLoader() {
	current_model.vertices = new vector<Vertex>();
	current_model.texture_vertices = new vector<TextureVertex>();
	current_model.normal_vertices = new vector<Vertex>();
	current_model.faces = new vector<Face>();
	current_model.materials = new vector<MtlHandler>();
}

/*
The destructor which clears the vectors and the deletes them from the heap
*/
ModelLoader::~ModelLoader() {
	current_model.vertices->clear();
	current_model.texture_vertices->clear();
	current_model.normal_vertices->clear();
	current_model.faces->clear();
	current_model.materials->clear();

	delete current_model.vertices;
	delete current_model.texture_vertices;
	delete current_model.normal_vertices;
	delete current_model.faces;
	delete current_model.materials;
}

/*
The file loader method which clears the current vectors of faces and vertices and loads them with new values provided in the file 

It then calls the ParseFile method to carry out the load
*/
int ModelLoader::LoadModel(string& file_name) {
	current_model.vertices->clear();
	current_model.texture_vertices->clear();
	current_model.normal_vertices->clear();
	current_model.faces->clear();
	current_model.materials->clear();

	current_file = file_name;
	int success = ParseFile(current_file);

	return success;
}

/*
This method takes the file_name and loads it into the corresponding vertices and faces vectors.

It opens the file and reads the first line. If the first line says vertices it starts to load the vector<Vertex> until it hits a bad input.
At that point, if the line reads faces it starts to store the values into the vector<Face>.

The assumption is no preceding new line characters before the word vertices.
Also assumes vertices and faces are spelt with lower cases
*/
int ModelLoader::ParseFile(string& model_file_name) {
	stringstream face_values(stringstream::in | stringstream::out);
	string current_definition, current_face_values, file_name = "../models/";
	int bad_input = 0, array_size;

	Vertex model_vertices;
	TextureVertex model_texture_vertices;
	Vertex model_normal_vertices;
	Face current_face;
	MtlHandler model_materials;
	file_name.append(model_file_name);

	read_model_file.open(file_name, ios::in);
	
	if(!read_model_file) {
		return 1;
	}

	read_model_file >> current_definition;

	if(current_definition.compare("mtllib") == 0) {
		//handle the mtl file
		string materials_file_name = "../models/", temp_string;

		read_model_file >> temp_string;
		materials_file_name.append(temp_string);

		fstream read_material_file;

		read_material_file.open(materials_file_name, ios::in);

		if(!read_material_file) {
			return 1;
		}

		read_material_file.ignore(1024, '\n');
		read_material_file >> temp_string;

		while(!read_material_file.eof()) {
			read_material_file >> model_materials.newmtl;
			read_material_file >> temp_string;

			while(temp_string.compare("newmtl") != 0 && !read_material_file.eof()) {				
				if(temp_string.compare("Ka") == 0) {
					for(int i = 0; i < number_of_colors; i++)
						read_material_file >> model_materials.Ka[i];
				} else if(temp_string.compare("Kd") == 0) {
					for(int i = 0; i < number_of_colors; i++)
						read_material_file >> model_materials.Kd[i];
				} else if(temp_string.compare("Ks") == 0) {
					for(int i = 0; i < number_of_colors; i++)
						read_material_file >> model_materials.Ks[i];
				} else if(temp_string.compare("d") == 0) {
					read_material_file >> model_materials.d;
				} else if(temp_string.compare("illum") == 0) {
					read_material_file >> model_materials.illum;
				} else if(temp_string.compare("Ns") == 0) {
					read_material_file >> model_materials.Ns;
				} else if(temp_string.compare("map_Ka") == 0) {
					read_material_file >> model_materials.map_Ka;
				} else if(temp_string.compare("map_Kd") == 0) {
					read_material_file >> model_materials.map_Kd;
				} else if(temp_string.compare("map_Ks") == 0) {
					read_material_file >> model_materials.map_Ks;
				} else if(temp_string.compare("map_d") == 0) {
					read_material_file >> model_materials.map_d;
				} else if(temp_string.compare("map_bump") == 0 || temp_string.compare("bump") == 0) {
					read_material_file >> model_materials.map_bump;
				}

				read_material_file >> temp_string;
			}

			current_model.materials->push_back(model_materials);
		}

		read_material_file.close();
	}
	
	read_model_file >> current_definition;

	while(!bad_input && current_definition.compare("v") == 0) {
		read_model_file >> model_vertices.x >> model_vertices.y >> model_vertices.z; 
					
		if(!read_model_file) {
			bad_input = 1;
			read_model_file.clear();
		} else {
			current_model.vertices->push_back(model_vertices);
		}

		read_model_file >> current_definition;
	}
	bad_input = 0;

	while(!bad_input && current_definition.compare("vt") == 0) {
		read_model_file >> model_texture_vertices.u >> model_texture_vertices.v; 
					
		if(!read_model_file) {
			bad_input = 1;
			read_model_file.clear();
		} else {
			current_model.texture_vertices->push_back(model_texture_vertices);
		}

		read_model_file >> current_definition;
	}
	bad_input = 0;

	while(!bad_input && current_definition.compare("vn") == 0) {
		read_model_file >> model_normal_vertices.x >> model_normal_vertices.y >> model_normal_vertices.z; 
					
		if(!read_model_file) {
			bad_input = 1;
			read_model_file.clear();
		} else {
			current_model.normal_vertices->push_back(model_normal_vertices);
		}

		read_model_file >> current_definition;
	}
	bad_input = 0;

	size_t position;
	int f;
	string smooth_value;
	while(!read_model_file.eof()) {
		if(current_definition.compare("usemtl") == 0)
			read_model_file >> current_face.texture_material;

		if(current_definition.compare("s") == 0) {
			read_model_file >> smooth_value;
			if(smooth_value.compare("off") == 0) {
				current_model.smooth_shading = 0;
			} else {
				current_model.smooth_shading = atof(smooth_value.c_str());
			}
		}

		read_model_file >> current_definition;
		while(current_definition.compare("f") == 0 && !read_model_file.eof()) {
			getline(read_model_file, current_face_values);

			array_size = 0;
			position = 0;
			while(current_face_values.find(' ', position) != string::npos) {
				++array_size;
				position = current_face_values.find(' ', position+1);
			}

			array_size*=3;

			position = 0;
			while(current_face_values.find('/', position) != string::npos) {
				current_face_values.replace(position, 1, " ");
				position = current_face_values.find('/', position+1);
			}

			face_values << current_face_values;

			for(int i = 0; i < array_size; i++) {
				face_values >> f;
				current_face.face.push_back(f);
			}
			current_model.faces->push_back(current_face);
			current_face.face.clear();
			read_model_file >> current_definition;
			face_values.clear();
		}
	}

	read_model_file.close();

	return 0;
}