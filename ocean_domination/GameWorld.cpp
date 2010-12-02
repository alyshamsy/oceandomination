#include "GameWorld.h"

//Lighting values
GLfloat ambient_light[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat diffusive_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specular_light[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 12.0f, 8.0f, -40.0f, 1.0f };

GameWorld::GameWorld() {
	//set the starting wind factor in the game (moves the waves)
	wind_factor = 0.5;
	
	//set the start time
	start_time = glfwGetTime();
	
	//initialize the world movement values
	x_position = 0.0;
	y_position = -10.0;
	z_position = 0.0;
	rotation_value = 0.0;

	//initialize enemy movements in the world
	enemy_x_position = 0.0;
	enemy_y_position = 0.0;
	enemy_z_position = 0.0;
	enemy_rotation_value = 0.0;

	total_x_position = 0.0;
	total_z_position = 0.0;
	total_rotation_value = 0.0;

	//variables to draw movement in the world
	scene_rotation = 0.0; 
	side_movement = 0.0;
	forward_movement = 0.0;

	enemy_rotation = 0.0;
	enemy_side_movement = 0.0;
	enemy_forward_movement = 0.0;

	translation_x = 0.0;
	translation_y = 0.0;
	translation_z = 0.0;
	scaling_factor = 0.0;

	missile_start_time = 0.0;
	button_timeout = 0.0;

	ammo_number = 0;

	ammo_mode = 1;

	ship_collision = 0;
	ammo_collision = 0;

	island_under_attack = 0;

	shot_fired = false;
}

GameWorld::~GameWorld() {
	//clear all the heap memory
	delete texture_images;
	texture_images = NULL;

	delete[] texture_file_names;
	texture_file_names = NULL;
}

int GameWorld::InitializeGameWorld() {
	//create shader programs
	water_shader_program = glCreateProgram();

	//load all models
	load_models();

	//load all textures
	string texture_file_name = "textures.txt";
	load_textures(texture_file_name);

	//load the game level filenames into memory and load the first level
	load_levels();

	//create all call lists
	create_call_lists();

	//create water mesh
	create_water_mesh();

	return 0;
}

int GameWorld::CreateGameWorld() {
	initialize_islands();
	initialize_ship();
	initialize_enemy();
	initialize_lighting();

	x_position = player_ship.getLocation().x;
	z_position = player_ship.getLocation().z;

	return 0;
}

void GameWorld::UpdateGameWorld() {
	Vertex target_location;
	Vertex next_ship_location = current_ship_location;

	//set the camera mode for different ammo mode
	if(ammo_mode != 2) {
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -7.0);
		glRotatef(17, 1.0, 0.0, 0.0);
	} else if(ammo_mode == 2) {
		glLoadIdentity();
		glTranslatef(0.0, -2.5, -3.7);
		glRotatef(-17, 1.0, 0.0, 0.0);
	}

	//if the up key is pressed, move in the positive x and z direction
	if(glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
		next_ship_location.x -= (float)sin(rotation_value*radian_conversion) * 0.2f;
		next_ship_location.z -= (float)cos(rotation_value*radian_conversion) * 0.2f;
	}

	//if the down key is pressed, move in the negative x and z direction
	if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
		next_ship_location.x += (float)sin(rotation_value*radian_conversion) * 0.2f;
		next_ship_location.z += (float)cos(rotation_value*radian_conversion) * 0.2f;
	}
	

	ship_collision = detect_ship_collision(next_ship_location);

	//check if there is ship collision
	if (!ship_collision){
		current_ship_location = next_ship_location;
	} else {
		rotation_value -= 0.5f;
	}

	//if the right key is pressed, rotate in the positive direction
	if(glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotation_value -= 0.5f;
	}

	//if the left key is pressed, rotate in the negative direction
	if(glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotation_value += 0.5f;
	}
	
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && shot_fired == false && glfwGetTime() - button_timeout > 0.5) {
		button_timeout = glfwGetTime();

		if(ammo_mode == 1) {
			ammo_mode = 2;
			ammo_number = 0;
		} else if(ammo_mode == 2) {
			if(player_ship.player_ammo.super_missilies > 0) {
				ammo_mode = 3;
			} else {
				ammo_mode = 1;
			}
			ammo_number = 0;
		} else if(ammo_mode == 3) {
			ammo_mode = 1;
			ammo_number = 0;
		}
	}

	//check if left button was pressed and fire the ammo if an ammo hasnt been fired 
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && shot_fired == false) {
		missile_start_time = glfwGetTime();
		shot_fired = true;
		ammo_number += 1;
	}

	//change the fire mode to either regular, sniper or super missile mode
	if(shot_fired == true) {
		current_time = glfwGetTime();
		if(ammo_mode == 1 && current_time - missile_start_time >= 0.5) {
			reduce_island_health(island_under_attack);
			missile_start_time = glfwGetTime();
			shot_fired = false;
			translation_x = 0.0;
			translation_y = 0.0;
			translation_z = 0.0;
			scaling_factor = 0.0;
		} else if(ammo_mode == 2 && current_time - missile_start_time >= 1.0) {
			reduce_island_health(island_under_attack);
			missile_start_time = glfwGetTime();
			shot_fired = false;
			translation_x = 0.0;
			translation_y = 0.0;
			translation_z = 0.0;
			scaling_factor = 0.0;
		} else if(ammo_mode == 3 && current_time - missile_start_time >= 1.5) {
			reduce_island_health(island_under_attack);
			missile_start_time = glfwGetTime();
			shot_fired = false;
			translation_x = 0.0;
			translation_y = 0.0;
			translation_z = 0.0;
			scaling_factor = 0.0;
		}
	}

	//adjusting rotation and forward and backward movement
	scene_rotation = 360.0 - rotation_value;
	side_movement = -current_ship_location.x;
	forward_movement = -current_ship_location.z;

	//update the current ship location
	player_ship.UpdateShipLocation(current_ship_location);

	//detect if there is ammo collision
	ammo_collision = detect_ammo_collision(current_ammo_location, island_under_attack);
	if(ammo_collision) {
		islands.at(island_under_attack).UpdateUnderAttack(true);
		current_time = glfwGetTime();
		if(ammo_mode == 1 && current_time - missile_start_time >= 0.5) {
			reduce_island_health(island_under_attack);
		} else if(ammo_mode == 2 && current_time - missile_start_time >= 1.0) {
			reduce_island_health(island_under_attack);

		} else if(ammo_mode == 3 && current_time - missile_start_time >= 1.5) {
			reduce_island_health(island_under_attack);
		}
	}

	perform_island_AI(current_ship_location);
	//IslandAI();
}

//restarts the game
int GameWorld::RestartGame() {
	return 0;
}

//generate the call list from the model and the call list value
int GameWorld::generate_model_display_list(ModelLoader& model, GLuint model_call_list) {
	if(model_call_list != 0) {
		string current_material;
		string current_texture;

		GLfloat** vertex_vector = generate_vector(3, 3);
		GLfloat** normals_vector = generate_vector(3, 3);
		GLfloat** texture_vector = generate_vector(3, 2);

		glNewList(model_call_list, GL_COMPILE);
			for(int i = 0; i < model.current_model.faces->size(); i++) {
				//load materials and bind textures
				//if the current material is the same as in previous iteration do not relaoad the material values
				if(current_material.compare(model.current_model.faces->at(i).texture_material) != 0) {
					current_material = model.current_model.faces->at(i).texture_material;
					if(current_material.compare("(null)") != 0) {
						int material_index = 0;
						while(current_material.compare(model.current_model.materials->at(material_index).newmtl) != 0)
							material_index++;

						//assign material values
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, model.current_model.materials->at(material_index).Ns);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*) &model.current_model.materials->at(material_index).Ka);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*) &model.current_model.materials->at(material_index).Kd);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) &model.current_model.materials->at(material_index).Ks);

						//bind texture
						current_texture = model.current_model.materials->at(material_index).map_Kd;
						int texture_index = 0;
						while(current_texture.compare(texture_file_names[texture_index]) != 0) {
							texture_index++;
						}

						glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
					}
				}
				
				//load vectors
				int j = 0, vertex_index = 0, normals_index = 0, texture_index = 0, index = 0;
				while(j < model.current_model.faces->at(i).face.size()) {
					if(j%3 == 0) {
						index = model.current_model.faces->at(i).face.at(j) - 1;
						
						vertex_vector[vertex_index][0] = model.current_model.vertices->at(index).x;
						vertex_vector[vertex_index][1] = model.current_model.vertices->at(index).y;
						vertex_vector[vertex_index][2] = model.current_model.vertices->at(index).z;

						vertex_index++;
					} else if(j%3 == 1) {
						index = model.current_model.faces->at(i).face.at(j) - 1;

						texture_vector[texture_index][0] = model.current_model.texture_vertices->at(index).u;
						texture_vector[texture_index][1] = model.current_model.texture_vertices->at(index).v;	

						texture_index++;
					} else if(j%3 == 2) {
						index = model.current_model.faces->at(i).face.at(j) - 1;

						normals_vector[normals_index][0] = model.current_model.normal_vertices->at(index).x;
						normals_vector[normals_index][1] = model.current_model.normal_vertices->at(index).y;
						normals_vector[normals_index][2] = model.current_model.normal_vertices->at(index).z;

						normals_index++;
					}
					j++;
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

		delete_vector(vertex_vector, 3);
		delete_vector(normals_vector, 3);
		delete_vector(texture_vector, 3);
	}

	return 0;
}

//generates the call lists for all the shaders used in the program
int GameWorld::generate_shader_display_list(string& vertex_shader_name, string& fragment_shader_name, GLuint& shader_call_list) {
	int return_value = 0;

	if(shader_call_list != 0) {
		glNewList(shader_call_list, GL_COMPILE);
			return_value = water_shader.LoadShader(vertex_shader_name, fragment_shader_name, water_shader_program);
		glEndList();
	}

	return return_value;
}

//displays the text on the screen
void GameWorld::load_text(string text, string font_type, FTPoint& position, unsigned int size) {
	FTGLPixmapFont font(font_type.c_str());
	font.FaceSize(size);
	font.Render(text.c_str(), -1, position);
}

//load the levels for the game
int GameWorld::load_levels() {
	game_levels.LoadLevels();

	return 0;
}

//handle to read models.txt and load models in parallel
int GameWorld::load_models() {
	string ship_model = "../models/ship.obj";
	string enemy_model = "enemy.obj";
	string sky_model = "../models/sky.obj";
	string sun_model = "../models/sun.obj";
	string small_island_model = "../models/small-island.obj";
	string medium_island_model = "../models/medium-island.obj";
	string large_island_model = "../models/large-island.obj";
	string bullet_model = "bullet.obj";
	string missile_model = "missile.obj";
	string super_missile_model = "super-missile.obj";
	string weapon_model = "weapon.obj";

	ship.LoadModel(ship_model);
	enemy.LoadModel(enemy_model);
	sky.LoadModel(sky_model);
	sun.LoadModel(sun_model);
	small_island.LoadModel(small_island_model);
	medium_island.LoadModel(medium_island_model);
	large_island.LoadModel(large_island_model);
	bullet.LoadModel(bullet_model);
	missile.LoadModel(missile_model);
	super_missile.LoadModel(super_missile_model);
	weapon.LoadModel(weapon_model);

	return 0;
}

//read the texture file and load all the textures
int GameWorld::load_textures(string& texture_file) {
	int number_of_textures = 0;
	string texture_file_name;
	fstream texture_loader;

	texture_loader.open(texture_file, ios::in);

	if(!texture_loader) {
		return 1;
	}

	texture_loader.ignore(1024, '\n');
	texture_loader >> number_of_textures;

	texture_file_names = new string[number_of_textures];
	texture_images = new GLuint[number_of_textures];

	for(int i = 0; i < number_of_textures; i++) {
		texture_loader >> texture_file_name;
		texture_file_names[i] = texture_file_name;
	}

	textures.LoadTextures(texture_file_names, number_of_textures, texture_images);

	return 0;
}

//create and generate all display lists
void GameWorld::create_call_lists() {
	//create the model display lists
	ship_list = glGenLists(1);
	enemy_list = glGenLists(1);
	sky_list = glGenLists(1);
	sun_list = glGenLists(1);
	small_island_list = glGenLists(1);
	medium_island_list = glGenLists(1);
	large_island_list = glGenLists(1);
	bullet_list = glGenLists(1);
	missile_list = glGenLists(1);
	super_missile_list = glGenLists(1);
	weapon_list = glGenLists(1);

	//create the shader display lists
	water_shader_list = glGenLists(1);

	//generate all the model display lists
	generate_model_display_list(ship, ship_list);
	generate_model_display_list(enemy, enemy_list);
	generate_model_display_list(sky, sky_list);
	generate_model_display_list(sun, sun_list);
	generate_model_display_list(small_island, small_island_list);
	generate_model_display_list(medium_island, medium_island_list);
	generate_model_display_list(large_island, large_island_list);
	generate_model_display_list(bullet, bullet_list);
	generate_model_display_list(missile, missile_list);
	generate_model_display_list(super_missile, super_missile_list);
	generate_model_display_list(weapon, weapon_list);

	//shows the list of shaders to be used
	string water_vertex_shader = "water_shader.vert";
	string water_fragment_shader = "";

	//generate all the shader display lists
	generate_shader_display_list(water_vertex_shader, water_fragment_shader, water_shader_list);
}

//create the water mesh to display water
void GameWorld::create_water_mesh() {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);					// Draw Our Mesh In Wireframe Mode
	
	// Create Our Mesh
	for (int x = 0; x < mesh_size; x++)
	{
		for (int z = 0; z < mesh_size; z++)
		{
			mesh_dimensions[x][z][0] = (float) (mesh_size / 2) - x;				// We Want To Center Our Mesh Around The Origin
			mesh_dimensions[x][z][1] = (float) 0.0;						// Set The Y Values For All Points To 0
			mesh_dimensions[x][z][2] = (float) (mesh_size / 2) - z;				// We Want To Center Our Mesh Around The Origin
		}
	}
}

//initializes the lighting in the game world
void GameWorld::initialize_lighting() {
	glEnable(GL_LIGHTING); 

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusive_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
}

//initialize the islands
void GameWorld::initialize_islands() {
	int large_islands = game_levels.getLargeIslandNumbers();
	int medium_islands = game_levels.getMediumIslandNumbers();
	int small_islands = game_levels.getSmallIslandNumbers();
	int total_islands = large_islands + medium_islands + small_islands;

	Island current_island;

	Vertex island_location;

	vector<Vertex>* game_island_coordinates = new vector<Vertex>();
	game_island_coordinates = game_levels.getIslandCoordiantes();
	
	for(int i = 0; i < total_islands; i++) {
		island_location.x = game_island_coordinates->at(i).x;
		island_location.y = game_island_coordinates->at(i).y;
		island_location.z = game_island_coordinates->at(i).z;

		if(i < large_islands) {
			current_island.InitializeIsland(island_location, 'L');
		} else if(i >= large_islands && i < large_islands+medium_islands) {
			current_island.InitializeIsland(island_location, 'M');
		} else if(i >= large_islands+medium_islands && i < total_islands) {
			current_island.InitializeIsland(island_location, 'S');
		}

		islands.push_back(current_island);
	}
}

//initialize the ship
void GameWorld::initialize_ship() {
	Vertex player_location;

	player_location.x = 0.0;
	player_location.y = -10.0;
	player_location.z = 0.0;

	player_ship.InitializeShip(player_location);
}

//initialize the enemy by randomly selecting the location on the map
void GameWorld::initialize_enemy() {
	float x_location = 0.0;
	float y_location = -5.0;
	float z_location = 0.0;
	
	Vertex enemy_location;

    x_location = random_number_generator(-100, -150);
	z_location = random_number_generator(125, 150);

	enemy_location.x = 15; //x_location;
	enemy_location.y = y_location;
	enemy_location.z = -50; //z_location;

	enemy_ship.InitializeShip(enemy_location);
}

//initialize power ups
void GameWorld::initialize_power_ups() {

}

//uses a random number generator to update wind factor
void GameWorld::update_wind_factor() {
	float random_number = random_number_generator(1, 6);

	wind_factor = random_number * 0.25;
}

//generates islands in each quad
void GameWorld::generate_quad_islands() {
	int number_of_islands = islands.size();

	for(int i = 0; i < number_of_islands; i++) {
		if(islands.at(i).getLocation().x >= 0 && islands.at(i).getLocation().z <= 0) {
			quad_1_islands.push_back(islands.at(i));
		} else if(islands.at(i).getLocation().x >= 0 && islands.at(i).getLocation().z >= 0) {
			quad_2_islands.push_back(islands.at(i));
		} else if(islands.at(i).getLocation().x <= 0 && islands.at(i).getLocation().z >= 0) {
			quad_3_islands.push_back(islands.at(i));
		} else if(islands.at(i).getLocation().x <= 0 && islands.at(i).getLocation().z >= 0) {
			quad_4_islands.push_back(islands.at(i));
		}
	}
}

//perform the AI of the islands
void GameWorld::perform_island_AI(Vertex ship_location) {
	int start_defending = 0;
	float defence_radius = 60.0f;

	double within_distance = 0.00000000000001;
	double distance_squared = 0.0;
	double radii_squared = 0.0;
	double overlap = 0.0;

	Vertex island_location;

	int total_islands = islands.size();

	for(int i = 0; i < total_islands; i++) {
		island_location = islands.at(i).getLocation();

		radii_squared = ((defence_radius + player_ship.getShipRadius()) * (defence_radius + player_ship.getShipRadius()));
		distance_squared = ((island_location.x - ship_location.x) * (island_location.x - ship_location.x)) + ((island_location.z - ship_location.z) * (island_location.z - ship_location.z));
		overlap = radii_squared - distance_squared;
		
		if (within_distance < overlap ) {
			start_defending = 1;
			Vertex weapon_location = island_location;
			float current_weapon_rotation_angle_side;
			//float current_weapon_rotation_angle_up;
			//float hyp;

			islands.at(i).UpdateWeaponFire(true);

			if(islands.at(i).getIslandType() == 'L') {
				weapon_location.set_vertex(island_location.x + 10.0, 20.0, island_location.z - 4.0);
				current_weapon_rotation_angle_side = atan2(weapon_location.z - ship_location.z, weapon_location.x - ship_location.x)*degree_conversion;
				islands.at(i).UpdateWeaponRotationAngleSide(current_weapon_rotation_angle_side);
			} else if(islands.at(i).getIslandType() == 'M') {
				weapon_location.set_vertex(island_location.x + 6.0, 10.0, island_location.z - 2.0);				
				current_weapon_rotation_angle_side = atan2(weapon_location.z - ship_location.z, weapon_location.x - ship_location.x)*degree_conversion;			
				islands.at(i).UpdateWeaponRotationAngleSide(current_weapon_rotation_angle_side);
			} else {
				weapon_location.set_vertex(island_location.x + 4.0, 5.0, island_location.z - 1.0);
				current_weapon_rotation_angle_side = atan2(weapon_location.z - ship_location.z, weapon_location.x - ship_location.x)*degree_conversion;			
				islands.at(i).UpdateWeaponRotationAngleSide(current_weapon_rotation_angle_side);
			}
		}
	}	
}

//detect collision of the ship with the island
int GameWorld::detect_ship_collision(Vertex ship_location) {
	int collision = 0;

	double collision_distance = 0.00000000000001;
	double distance_squared = 0.0;
	double radii_squared = 0.0;
	double overlap = 0.0;

	Vertex island_location;

	int total_islands = islands.size();

	for(int i = 0; i < total_islands; i++) {
		island_location = islands.at(i).getLocation();

		radii_squared = ((islands.at(i).getIslandRadius() + player_ship.getShipRadius()) * (islands.at(i).getIslandRadius() + player_ship.getShipRadius()));
		distance_squared = ((island_location.x - ship_location.x) * (island_location.x - ship_location.x)) + ((island_location.z - ship_location.z) * (island_location.z - ship_location.z));
		overlap = radii_squared - distance_squared;
		
		if (collision_distance < overlap )
			collision = 1;
	}

	return collision;
}

//detect collision of islands with the ammo
int GameWorld::detect_ammo_collision(Vertex ammo_location, int& island_under_attack) {
	int collision = 0;
	
	float ammo_radius = 0.0;

	if(ammo_mode == 1) {
		ammo_radius = player_ship.getMissileRadius();
	} else if(ammo_mode == 2) {
		ammo_radius = player_ship.getBulletRadius();
	} else {
		ammo_radius = player_ship.getSuperMissileRadius();
	}

	double collision_distance = 0.00000000000001;
	double distance_squared = 0.0;
	double radii_squared = 0.0;
	double overlap = 0.0;

	Vertex island_location;

	int total_islands = islands.size();

	for(int i = 0; i < total_islands; i++) {
		island_location = islands.at(i).getLocation();

		radii_squared = ((islands.at(i).getIslandRadius() + ammo_radius) * (islands.at(i).getIslandRadius() + ammo_radius));
		distance_squared = ((island_location.x - ammo_location.x) * (island_location.x - ammo_location.x)) + ((island_location.z - ammo_location.z) * (island_location.z - ammo_location.z));
		overlap = radii_squared - distance_squared;
		
		if (collision_distance < overlap ) {
			collision = 1;
			island_under_attack = i;
		}
	}
	
	return collision;
}

//detects if the ammo from the island hit the ship
int GameWorld::detect_ammo_ship_location(Vertex ammo_location, Vertex ship_location) {
	int collision = 0;
	
	float ammo_radius = 0.0;

	ammo_radius = player_ship.getMissileRadius();

	double collision_distance = 0.00000000000001;
	double distance_squared = 0.0;
	double radii_squared = 0.0;
	double overlap = 0.0;

	radii_squared = ((player_ship.getShipRadius() + ammo_radius) * (player_ship.getShipRadius() + ammo_radius));
	distance_squared = ((ship_location.x - ammo_location.x) * (ship_location.x - ammo_location.x)) + ((ship_location.z - ammo_location.z) * (ship_location.z - ammo_location.z));
	overlap = radii_squared - distance_squared;
		
	if (collision_distance < overlap ) {
		collision = 1;
	}
	
	return collision;
}

//detect collision with the power ups
int GameWorld::detect_power_ups(Vertex ship_location) {
	return 0;
}

//reduce the health of the island
void GameWorld::reduce_island_health(int island_number){
	int health = islands.at(island_number).getHealth();
	char island_type = islands.at(island_number).getIslandType();
	
	for(int i = 0; i < islands.size(); i++) {
		if(i != island_number) {
			islands.at(i).UpdateUnderAttack(false);
		} 
	}

	if(island_type == 'L') {
		if(ammo_mode == 1) {
			if(health <= 10) {
				health = 0;
				islands.erase(islands.begin()+island_number);
			} else {
				health -= 10;
				islands.at(island_number).UpdateHealth(health);
			}
		} else if(ammo_mode == 2) {
			if(health == 5) {
				health = 0;
				islands.erase(islands.begin()+island_number);
			} else {
				health -= 5;
				islands.at(island_number).UpdateHealth(health);
			}
		} else {
			if(health <= 50) {
				health = 0;
				islands.erase(islands.begin()+island_number);
			} else {
				health -= 50;
				islands.at(island_number).UpdateHealth(health);
			}
		}
	} else {
		if(ammo_mode == 1) {
			if(health <= 20) {
				health = 0;
				islands.erase(islands.begin()+island_number);
			} else {
				health -= 20;
				islands.at(island_number).UpdateHealth(health);
			}
		} else if(ammo_mode == 2) {
			if(health <= 10) {
				health = 0;
				islands.erase(islands.begin()+island_number);
			} else {
				health -= 10;
				islands.at(island_number).UpdateHealth(health);
			}
		} else {
			if(health <= 100) {
				health = 0;
				islands.erase(islands.begin()+island_number);
			} else {
				health -= 100;
				islands.at(island_number).UpdateHealth(health);
			}
		}
	}
}

//draw the models by calling the display list
void GameWorld::draw_model(GLuint& model_list) {
	glCallList(model_list);
}

//draw the entire game world
void GameWorld::draw_world() {
	//draw the ship
	glPushMatrix();
	{
		draw_ship();
	}
	glPopMatrix();

	//draw the world, the ammo and the particle effects
	glPushMatrix();
	{
		glRotatef(scene_rotation, 0.0, 1.0, 0.0);
		draw_top_world();
		glTranslatef(side_movement, 0.0, forward_movement);
		draw_ammo(shot_fired, ammo_number, rotation_value);
		draw_bottom_world();
				
		if(ammo_collision) {
			//draw_smoke();
		}
	}
	glPopMatrix();
}

//draw the sky, sun and clouds
void GameWorld::draw_top_world() {
	//draw sky
	glPushMatrix();
	{	
		glScalef(5, 5, 5);
		glTranslatef(0, -2, 0);
		draw_model(sky_list);
	}
	glPopMatrix();
	
	//draw the billboarded sun
	GLfloat sun_starting_x = 12.0;
	GLfloat sun_starting_y = 8.0;
	GLfloat sun_starting_z = -400.0;
	GLfloat sun_starting_angle = atan(sun_starting_y/(-sun_starting_z));
	GLfloat sun_rotation_angle = atan((-sun_starting_z)/sun_starting_x);
	sun_rotation_angle += rotation_value*radian_conversion;
	glPushMatrix();
	{
		glScalef(15, 15, 1);
		glTranslatef(sun_starting_x, sun_starting_y, sun_starting_z);
		glRotatef(sun_rotation_angle, 0.0, 1.0, 0.0);
		glRotatef(sun_starting_angle, 1.0, 0.0, 0.0);
		draw_model(sun_list);
	}
	glPopMatrix();

	//draw clouds
	glPushMatrix();
	{	
		
	}
	glPopMatrix();
}

//draw the water and islands
void GameWorld::draw_bottom_world() {
	GLint location_time, location_wind;

	current_time = glfwGetTime();

	if((int)current_time % 300 == 0) {
		update_wind_factor();
	}

	glCallList(water_shader_list);
	
	location_time = glGetUniformLocation(water_shader_program, "time");
	glUniform1f(location_time, current_time);

	location_wind = glGetUniformLocation(water_shader_program, "wind_factor");
	glUniform1f(location_wind, wind_factor);

	//draw water
	glPushMatrix();
	{	
		glTranslatef(0, -2.0, 0);
		draw_water();
	}
	glPopMatrix();
	water_shader.DetachShader(water_shader_program);
	
	//draw islands
	glPushMatrix();
	{
		draw_islands();
	}
	glPopMatrix();
}

//draw the water and generate the display list if none exists
void GameWorld::draw_water() {
	if(water_list == 0) {
		string water_texture;
		water_list = glGenLists(1);

		glNewList(water_list, GL_COMPILE);
			water_texture = "blue_water.tga";
			int texture_index = 0;
			while(water_texture.compare(texture_file_names[texture_index]) != 0) {
				texture_index++;
			}

			glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);

			// Start Drawing Our Mesh
			float float_x, float_z, float_xb;
			for (int x = 0; x < mesh_size - 1; x++) {
				// Draw A Triangle Strip For Each Column Of Our Mesh
				glBegin(GL_TRIANGLE_STRIP);
				for (int z = 0; z < mesh_size - 1; z++) {
					// texture points to be used
					float_x = float(x)/mesh_size;		
					float_z = float(z)/mesh_size;
					float_xb = float(x+1)/mesh_size;
			
					// Set The Wave Parameter Of Our Shader To The Incremented Wave Value From Our Main Program
					glTexCoord2f( float_x, float_z);
					glVertex3f(mesh_dimensions[x][z][0], mesh_dimensions[x][z][1], mesh_dimensions[x][z][2]);	// Draw Vertex

					glTexCoord2f( float_xb, float_z );
					glVertex3f(mesh_dimensions[x+1][z][0], mesh_dimensions[x+1][z][1], mesh_dimensions[x+1][z][2]);	// Draw Vertex
				}
				glEnd();
			}
		glEndList();
	} else {
		glCallList(water_list);
	}
}

//draw all the islands
void GameWorld::draw_islands() {
	int total_islands = islands.size();
	float leeway = 1.0;

	for(int i = 0; i < total_islands; i++) {
		Vertex island_location = islands.at(i).getLocation();

		if(islands.at(i).getIslandType() == 'L') {
			//draw large islands
			glPushMatrix();
			{	
				glTranslatef(island_location.x, island_location.y, island_location.z);
				draw_model(large_island_list);
			}
			glPopMatrix();
			
			glPushMatrix();
			{	
				glTranslatef(island_location.x + 10.0, 20.0, island_location.z - 4.0);
				/*
				float movement_angle_side = islands.at(i).getWeaponMovementAngleSide();
				float rotation_angle_side = islands.at(i).getWeaponRotationAngleSide();
				if(movement_angle_side != rotation_angle_side) {
					if(rotation_angle_side < 0)
						movement_angle_side += 0.5f;
					else
						movement_angle_side -= 0.5f;

					islands.at(i).UpdateWeaponMovementAngleSide(movement_angle_side);
					glRotatef(movement_angle_side, 0.0, 1.0, 0.0);
				}

				float movement_angle_up = islands.at(i).getWeaponMovementAngleUp();
				float rotation_angle_up = islands.at(i).getWeaponRotationAngleUp();
				if(movement_angle_up != rotation_angle_side) {
					if(rotation_angle_side > rotation_angle_up)
						movement_angle_up += 0.5f;
					else
						movement_angle_up -= 0.5f;

					islands.at(i).UpdateWeaponMovementAngleUp(movement_angle_up);
					glRotatef(movement_angle_up, 1.0, 0.0, 0.0);
				}
				*/
				draw_model(weapon_list);
			}
			glPopMatrix();		
		} else if(islands.at(i).getIslandType() == 'M') {
			//draw large islands
			glPushMatrix();
			{	
				glTranslatef(island_location.x, island_location.y, island_location.z);
				draw_model(medium_island_list);
			}
			glPopMatrix();
			
			glPushMatrix();
			{	
				glTranslatef(island_location.x + 6.0, 10.0, island_location.z - 2.0);
				/*
				float movement_angle_side = islands.at(i).getWeaponMovementAngleSide();
				float rotation_angle_side = islands.at(i).getWeaponRotationAngleSide();
				if(movement_angle_side != rotation_angle_side) {
					if(rotation_angle_side < 0)
						movement_angle_side += 0.5f;
					else
						movement_angle_side -= 0.5f;

					islands.at(i).UpdateWeaponMovementAngleSide(movement_angle_side);
					glRotatef(movement_angle_side, 0.0, 1.0, 0.0);
				}

				float movement_angle_up = islands.at(i).getWeaponMovementAngleUp();
				float rotation_angle_up = islands.at(i).getWeaponRotationAngleUp();
				if(movement_angle_up != rotation_angle_side) {
					if(rotation_angle_side > rotation_angle_up)
						movement_angle_up += 0.5f;
					else
						movement_angle_up -= 0.5f;

					islands.at(i).UpdateWeaponMovementAngleUp(movement_angle_up);
					glRotatef(movement_angle_up, 1.0, 0.0, 0.0);
				}
				*/
				draw_model(weapon_list);
			}
			glPopMatrix();			
		} else if(islands.at(i).getIslandType() == 'S') {
			//draw large islands
			glPushMatrix();
			{	
				glTranslatef(island_location.x, island_location.y, island_location.z);
				draw_model(small_island_list);
			}
			glPopMatrix();
			
			glPushMatrix();
			{	
				glTranslatef(island_location.x + 4.0, 5.0, island_location.z - 1.0);
				
				float movement_angle_side = islands.at(i).getWeaponMovementAngleSide();
				float rotation_angle_side = islands.at(i).getWeaponRotationAngleSide();
				if(rotation_angle_side < 0) {
					if(rotation_angle_side < movement_angle_side) {
						movement_angle_side -= 1.0f;
						
						islands.at(i).UpdateWeaponMovementAngleSide(movement_angle_side);
						movement_angle_side = 360 - movement_angle_side;
					}
				} else {
					if(rotation_angle_side > movement_angle_side) {
						movement_angle_side += 1.0f;

						islands.at(i).UpdateWeaponMovementAngleSide(movement_angle_side);
					}
				}
				/*
				float movement_angle_up = islands.at(i).getWeaponMovementAngleUp();
				float rotation_angle_up = islands.at(i).getWeaponRotationAngleUp();
				if(movement_angle_up != rotation_angle_side) {
					if(rotation_angle_side > rotation_angle_up)
						movement_angle_up += 0.5f;
					else
						movement_angle_up -= 0.5f;

					islands.at(i).UpdateWeaponMovementAngleUp(movement_angle_up);
					glRotatef(movement_angle_up, 1.0, 0.0, 0.0);
				}
				*/
				glRotatef(movement_angle_side, 0.0, 1.0, 0.0);
				draw_model(weapon_list);
			}
			glPopMatrix();			
		} 
	}
}

//draw the ship
void GameWorld::draw_ship() {
	glPushMatrix();
	{
		glScalef(0.25, 0.25, 0.25);
		draw_model(ship_list);
	}
	glPopMatrix();
}

//draw the enemy ship
void GameWorld::draw_enemy() {
	Vertex enemy_location = enemy_ship.getLocation();

	glPushMatrix();
	{
		glScalef(0.25, 0.25, 0.25);
		glTranslatef(enemy_location.x, enemy_location.y, enemy_location.z);
		draw_model(enemy_list);
	}
	glPopMatrix();
}

//draws the missile on the screen
void GameWorld::draw_ammo(bool shot_fired, int ammo_number, float angle) {
	current_ammo_location = player_ship.getLocation();
	if(!ammo_collision) {
		if(ammo_mode == 1) {
			if(shot_fired == true) {	
				translation_x -= (float)sin(angle*radian_conversion) * 0.8f;
				translation_y += 0.02f;
				translation_z -= (float)cos(angle*radian_conversion) * 0.8f;
				scaling_factor += 0.05;
		
				if(ammo_number % 2 == 0) {
					current_ammo_location.x += 0.095 + translation_x;
					current_ammo_location.y += translation_y;
					current_ammo_location.z += translation_z;

					glPushMatrix();
					{
						glTranslatef(current_ammo_location.x, current_ammo_location.y, current_ammo_location.z);
						glRotatef(angle, 0.0, 1.0, 0.0);
						glScalef(1.0 + scaling_factor, 1.0 + scaling_factor, 1.0 + scaling_factor);
						draw_model(missile_list);
					}
					glPopMatrix();
				} else {
					current_ammo_location.x += -0.095 + translation_x;
					current_ammo_location.y += translation_y;
					current_ammo_location.z += translation_z;

					glPushMatrix();
					{
						glTranslatef(current_ammo_location.x, current_ammo_location.y, current_ammo_location.z);
						glRotatef(angle, 0.0, 1.0, 0.0);
						glScalef(1.0 + scaling_factor, 1.0 + scaling_factor, 1.0 + scaling_factor);
						draw_model(missile_list);
					}
					glPopMatrix();
				}
			}
		} else if(ammo_mode == 2) {
			if(shot_fired == true) {
				translation_x -= (float)sin(angle*radian_conversion) * 3.0f;
				translation_y += 0.0f;
				translation_z -= (float)cos(angle*radian_conversion) * 3.0f;
				scaling_factor += 0.05;
			
				current_ammo_location.x += translation_x;
				current_ammo_location.y += 1.4 + translation_y;
				current_ammo_location.z += translation_z;

				glPushMatrix();
				{
					glTranslatef(current_ammo_location.x, current_ammo_location.y, current_ammo_location.z);
					glRotatef(angle, 0.0, 1.0, 0.0);
					glScalef(0.5 + scaling_factor, 0.5 + scaling_factor, 0.5 + scaling_factor);
					draw_model(bullet_list);
				}
				glPopMatrix();
			}
		} else if(ammo_mode == 3) {
			if(shot_fired == true) {	
				translation_x -= (float)sin(angle*radian_conversion) * 1.5f;
				translation_y += 0.02f;
				translation_z -= (float)cos(angle*radian_conversion) * 1.5f;
				scaling_factor += 0.04;
			
				current_ammo_location.x += translation_x;
				current_ammo_location.y += translation_y;
				current_ammo_location.z += translation_z;

				glPushMatrix();
				{
					glTranslatef(current_ammo_location.x, current_ammo_location.y, current_ammo_location.z);
					glRotatef(angle, 0.0, 1.0, 0.0);
					glScalef(1.0 + scaling_factor, 1.0 + scaling_factor, 1.0 + scaling_factor);
					draw_model(super_missile_list);
				}
				glPopMatrix();
			}
		}
	}
}

//this draws the bullets fired by the island
void GameWorld::draw_island_ammo() {

}