#include "ModelLoader.h"

/*
The constructor which allocates memory on the heap for the vertex and the face vectors. Because no values have been provided, it initially creates a vector of size 1
*/
ModelLoader::ModelLoader() {
	game_models = new vector<model>();
	materials = new vector<MtlHandler>();
}

/*
The destructor which clears the vectors and the deletes them from the heap
*/
ModelLoader::~ModelLoader() {
	game_models->clear();
	materials->clear();

	delete game_models;
	delete materials;
}

/*
The file loader method which clears the current vectors of faces and vertices and loads them with new values provided in the file 

It then calls the ParseFile method to carry out the load
*/
int ModelLoader::LoadModel(vector<string>* models) {
	model current_model;
	
	stringstream face_values(stringstream::in | stringstream::out);
	
	string current_definition, current_face_values, object_name, model_name, file_name, current_model_name;
	
	int bad_input = 0, array_size;
	int number_of_models = models->size();

	vector<string> object_models;

	Vector model_vertices;
	TextureVector model_texture_vertices;
	Vector model_normal_vertices;
	Face current_face;

	for(int i = 0; i < number_of_models; i++) {
		file_name = "bin/models/";
		model_name = models->at(i);
		file_name.append(model_name).append(".obj");

		read_model_file.open(file_name, ios::in);
	
		if(read_model_file) {
			
			read_model_file.ignore(1024, '\n');

			read_model_file >> current_definition;

			while(!read_model_file.eof()) {
				if(current_definition.compare("object") == 0) {
					read_model_file >> object_name;
					object_models.push_back(object_name);
					current_model.objects.push_back(object_name);

					read_model_file >> current_definition;
					while(!bad_input && current_definition.compare("v") == 0) {
						read_model_file >> model_vertices.x >> model_vertices.y >> model_vertices.z; 
					
						if(!read_model_file) {
							bad_input = 1;
							read_model_file.clear();
						} else {
							current_model.vertices.push_back(model_vertices);
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
							current_model.texture_vertices.push_back(model_texture_vertices);
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
							current_model.normal_vertices.push_back(model_normal_vertices);
						}

						read_model_file >> current_definition;
					}
					bad_input = 0;

					size_t position;
					int f;
					string smooth_value;
					
					if(current_definition.compare("usemtl") == 0)
						read_model_file >> current_face.texture_material;

					read_model_file >> current_definition;

					if(current_definition.compare("s") == 0) {
						read_model_file >> smooth_value;
						if(smooth_value.compare("off") == 0) {
							current_model.smooth_shading = 0;
						} else {
							current_model.smooth_shading = atoi(smooth_value.c_str());
						}
					}

					read_model_file >> current_definition;

					current_model.start_index.push_back(current_model.faces.size());

					while(current_definition.compare("f") == 0) {
						getline(read_model_file, current_face_values);

						TrimSpaces(current_face_values);

						array_size = 0;
						position = 0;
						while(current_face_values.find(' ', position) != string::npos) {
							++array_size;
							position = current_face_values.find(' ', position+1);
						}

						array_size *= 3;

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
						current_model.faces.push_back(current_face);
						current_face.face.clear();
						read_model_file >> current_definition;
						face_values.clear();
					}
				} else if(current_definition.compare("v") == 0) {
					object_models.push_back(models->at(i));

					while(!bad_input && current_definition.compare("v") == 0) {
						read_model_file >> model_vertices.x >> model_vertices.y >> model_vertices.z; 
					
						if(!read_model_file) {
							bad_input = 1;
							read_model_file.clear();
						} else {
							current_model.vertices.push_back(model_vertices);
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
							current_model.texture_vertices.push_back(model_texture_vertices);
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
							current_model.normal_vertices.push_back(model_normal_vertices);
						}

						read_model_file >> current_definition;
					}
					bad_input = 0;

					size_t position;
					int f;
					string smooth_value;

					current_model.start_index.push_back(current_model.faces.size());

					while(!read_model_file.eof()) {
						if(current_definition.compare("usemtl") == 0)
							read_model_file >> current_face.texture_material;

						if(current_definition.compare("s") == 0) {
							read_model_file >> smooth_value;
							if(smooth_value.compare("off") == 0) {
								current_model.smooth_shading = 0;
							} else {
								current_model.smooth_shading = atoi(smooth_value.c_str());
							}
						}

						read_model_file >> current_definition;

						while(current_definition.compare("f") == 0 && !read_model_file.eof()) {
							getline(read_model_file, current_face_values);

							TrimSpaces(current_face_values);

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
							current_model.faces.push_back(current_face);
							current_face.face.clear();
							read_model_file >> current_definition;
							face_values.clear();
						}
					}
				}

				current_model.end_index = current_model.faces.size();
			}

			read_model_file.close();

			game_models->push_back(current_model);

			current_model.vertices.clear();
			current_model.texture_vertices.clear();
			current_model.normal_vertices.clear();
			current_model.faces.clear();
			current_model.start_index.clear();
			current_model.objects.clear();
		}
	}
	
	int size = object_models.size();
	models->clear();
	//models->resize(size);

	for(int i = 0; i < size; i++) {
		models->push_back(object_models.at(i));
	}

	return 0;
}


int ModelLoader::LoadMaterials(vector<string>* material_files) {
	string temp_string, materials_file_name;
	int number_of_materials = material_files->size();

	MtlHandler model_materials;

	for(int i = 0; i < number_of_materials; i++) {
		temp_string = material_files->at(i);
		materials_file_name = "bin/models/";
		materials_file_name.append(temp_string);

		fstream read_material_file;

		read_material_file.open(materials_file_name, ios::in);

		if(!read_material_file) {
			return 1;
		}

		read_material_file.ignore(1024, '\n');
		read_material_file >> temp_string;

		int j = 0;
		while(temp_string.compare("newmtl") == 0 && !read_material_file.eof()) {
			read_material_file >> model_materials.newmtl;
			
			read_material_file >> temp_string;
			read_material_file >> model_materials.Ns;

			read_material_file >> temp_string;
			for(int i = 0; i < number_of_colors; i++)
					read_material_file >> model_materials.Ka[i];

			read_material_file >> temp_string;
			for(int i = 0; i < number_of_colors; i++)
					read_material_file >> model_materials.Kd[i];

			read_material_file >> temp_string;
			for(int i = 0; i < number_of_colors; i++)
					read_material_file >> model_materials.Ks[i];

			read_material_file >> temp_string;
			read_material_file >> model_materials.Ni;

			read_material_file >> temp_string;
			read_material_file >> model_materials.d;
						
			read_material_file >> temp_string;
			read_material_file >> model_materials.illum;

			read_material_file >> temp_string;
			read_material_file >> model_materials.map_Kd;

			read_material_file >> temp_string;

			materials->push_back(model_materials);
			j++;
		}

		read_material_file.close();
	}

	return 0;
}

//generate the call list from the model and the call list value
int ModelLoader::GenerateModelDisplayList(vector<GLuint>* model_display_list, GLuint* texture_images, vector<string>* texture_file_names) {
	int number_of_models = game_models->size();
	int number_of_objects;

	GLuint model_call_list;

	string current_material;
	string current_texture;

	GLfloat** vertex_vector = generate_vector(3, 3);
	GLfloat** normals_vector = generate_vector(3, 3);
	GLfloat** texture_vector = generate_vector(3, 2);

	for(int i = 0; i < number_of_models; i++) {
		number_of_objects = game_models->at(i).objects.size();
		if(number_of_objects != 0) {
			for(int j = 0; j < number_of_objects; j++) {
				model_call_list = glGenLists(1);

				glNewList(model_call_list, GL_COMPILE);
					int start_value = game_models->at(i).start_index.at(j);
					int end_value;

					if(j < number_of_objects - 1) 
						end_value = game_models->at(i).start_index.at(j + 1);
					else
						end_value = game_models->at(i).end_index;

					for(int k = start_value; k < end_value; k++) {
						//load materials and bind textures
						//if the current material is the same as in previous iteration do not relaoad the material values
						if(current_material.compare(game_models->at(i).faces.at(k).texture_material) != 0) {
							current_material = game_models->at(i).faces.at(k).texture_material;
							if(current_material.compare("(null)") != 0) {
								int material_index = 0;
								while(current_material.compare(materials->at(material_index).newmtl) != 0)
									material_index++;

								//assign material values
								glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materials->at(material_index).Ns);
								glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*) &materials->at(material_index).Ka);
								glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*) &materials->at(material_index).Kd);
								glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) &materials->at(material_index).Ks);

								//bind texture
								current_texture = materials->at(material_index).map_Kd;
								int texture_index = 0;
								while(current_texture.compare(texture_file_names->at(texture_index)) != 0) {
									texture_index++;
								}
						
								glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
							}
						}
				
						//load vectors
				
						int l = 0, vertex_index = 0, normals_index = 0, texture_index = 0, index = 0;
						while(l < game_models->at(i).faces.at(k).face.size()) {
							if(l%3 == 0) {
								index = game_models->at(i).faces.at(k).face.at(l) - 1;
						
								vertex_vector[vertex_index][0] = game_models->at(i).vertices.at(index).x;
								vertex_vector[vertex_index][1] = game_models->at(i).vertices.at(index).y;
								vertex_vector[vertex_index][2] = game_models->at(i).vertices.at(index).z;

								vertex_index++;
							} else if(l%3 == 1) {
								index = game_models->at(i).faces.at(k).face.at(l) - 1;

								texture_vector[texture_index][0] = game_models->at(i).texture_vertices.at(index).u;
								texture_vector[texture_index][1] = game_models->at(i).texture_vertices.at(index).v;	

								texture_index++;
							} else if(l%3 == 2) {
								index = game_models->at(i).faces.at(k).face.at(l) - 1;

								normals_vector[normals_index][0] = game_models->at(i).normal_vertices.at(index).x;
								normals_vector[normals_index][1] = game_models->at(i).normal_vertices.at(index).y;
								normals_vector[normals_index][2] = game_models->at(i).normal_vertices.at(index).z;

								normals_index++;
							}
							l++;
						}

						//draw the triangle
						glBegin(GL_TRIANGLES);
							//first point
							glTexCoord2fv(texture_vector[0]);
							glNormal3fv(normals_vector[0]);
							glVertex3fv(vertex_vector[0]);
							//second point
							glTexCoord2fv(texture_vector[1]);
							glNormal3fv(normals_vector[1]);
							glVertex3fv(vertex_vector[1]);
							//third point
							glTexCoord2fv(texture_vector[2]);
							glNormal3fv(normals_vector[2]);
							glVertex3fv(vertex_vector[2]);
						glEnd();
					}
				glEndList();

				model_display_list->push_back(model_call_list);
			}
		} else {
			model_call_list = glGenLists(1);

			glNewList(model_call_list, GL_COMPILE);
				int start_value = game_models->at(i).start_index.at(0);
				int end_value = game_models->at(i).end_index;

				for(int k = start_value; k < end_value; k++) {
					//load materials and bind textures
					//if the current material is the same as in previous iteration do not relaoad the material values
					if(current_material.compare(game_models->at(i).faces.at(k).texture_material) != 0) {
						current_material = game_models->at(i).faces.at(k).texture_material;
						if(current_material.compare("(null)") != 0) {
							int material_index = 0;
							while(current_material.compare(materials->at(material_index).newmtl) != 0)
								material_index++;

							//assign material values
							glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materials->at(material_index).Ns);
							glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*) &materials->at(material_index).Ka);
							glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*) &materials->at(material_index).Kd);
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) &materials->at(material_index).Ks);

							//bind texture
							current_texture = materials->at(material_index).map_Kd;
							int texture_index = 0;
							while(current_texture.compare(texture_file_names->at(texture_index)) != 0) {
								texture_index++;
							}
						
							glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
						}
					}
				
					//load vectors
				
					int l = 0, vertex_index = 0, normals_index = 0, texture_index = 0, index = 0;
					while(l < game_models->at(i).faces.at(k).face.size()) {
						if(l%3 == 0) {
							index = game_models->at(i).faces.at(k).face.at(l) - 1;
						
							vertex_vector[vertex_index][0] = game_models->at(i).vertices.at(index).x;
							vertex_vector[vertex_index][1] = game_models->at(i).vertices.at(index).y;
							vertex_vector[vertex_index][2] = game_models->at(i).vertices.at(index).z;

							vertex_index++;
						} else if(l%3 == 1) {
							index = game_models->at(i).faces.at(k).face.at(l) - 1;

							texture_vector[texture_index][0] = game_models->at(i).texture_vertices.at(index).u;
							texture_vector[texture_index][1] = game_models->at(i).texture_vertices.at(index).v;	

							texture_index++;
						} else if(l%3 == 2) {
							index = game_models->at(i).faces.at(k).face.at(l) - 1;

							normals_vector[normals_index][0] = game_models->at(i).normal_vertices.at(index).x;
							normals_vector[normals_index][1] = game_models->at(i).normal_vertices.at(index).y;
							normals_vector[normals_index][2] = game_models->at(i).normal_vertices.at(index).z;

							normals_index++;
						}
						l++;
					}

					//draw the triangle
					glBegin(GL_TRIANGLES);
						//first point
						glTexCoord2fv(texture_vector[0]);
						glNormal3fv(normals_vector[0]);
						glVertex3fv(vertex_vector[0]);
						//second point
						glTexCoord2fv(texture_vector[1]);
						glNormal3fv(normals_vector[1]);
						glVertex3fv(vertex_vector[1]);
						//third point
						glTexCoord2fv(texture_vector[2]);
						glNormal3fv(normals_vector[2]);
						glVertex3fv(vertex_vector[2]);
					glEnd();
				}
			glEndList();

			model_display_list->push_back(model_call_list);
		}
	}

	delete_vector(vertex_vector, 3);
	delete_vector(normals_vector, 3);
	delete_vector(texture_vector, 3);

	return 0;
}

int ModelLoader::GenerateModelVBO(vector<GLuint>* model_vbo_id, GLuint* texture_images, vector<string>* texture_file_names) {
	int number_of_models = game_models->size();
	GLuint vbo_id;


	
	return 0;
}

string ModelLoader::TrimSpaces(string& current_line) {
	int starting_position = current_line.find_first_of(" \t");
	int end_position = current_line.find_last_not_of(" \t");

	if(starting_position == end_position)
		current_line = "";
	else 
		current_line = current_line.substr(starting_position, end_position + 1);

	return current_line;
}