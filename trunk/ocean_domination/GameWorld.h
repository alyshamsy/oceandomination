#ifndef Game_World_H
#define Game_World_H

#include "ShaderLoader.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "LevelLoader.h"
#include "Helper.h"
#include "Island.h"
#include "Ship.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <math.h>

#include <GL/glfw.h>
#include <FTGL/ftgl.h>

#define mesh_size 1000
#define PI 3.14159265f
#define radian_conversion 0.0174532925f
#define degree_conversion 57.2957795785523f
#define light_size 4

using namespace std;

class GameWorld {
public:
	GameWorld();
	~GameWorld();

	int InitializeGameWorld();
	int CreateGameWorld();
	void UpdateGameWorld();
	int RestartGame();

	void draw_world();

private:
	//wind values in the world
	float wind_factor;
	
	//stores the current time elapsed since the start of the game
	GLfloat start_time;
	GLfloat current_time;

	//movement in the world
	GLfloat x_position, y_position, z_position, rotation_value;
	GLfloat enemy_x_position, enemy_y_position, enemy_z_position, enemy_rotation_value;
	GLfloat total_x_position, total_z_position, total_rotation_value;

	//variables to draw movement in the world
	GLfloat scene_rotation, enemy_rotation;
	GLfloat side_movement, enemy_side_movement;
	GLfloat forward_movement, enemy_forward_movement;

	//variables to detect collision
	int ship_collision, ammo_collision;

	//stores which island is currently under attack
	int island_under_attack;

	//stores the current location of the ship
	Vertex current_ship_location;
	Vertex current_ammo_location;
	Vertex current_island_ammo_location;

	//missile movements
	float translation_x, translation_y, translation_z, scaling_factor;
	float missile_start_time, button_timeout;
	int ammo_number;

	//mode of firing 1 = regular, 2 = sniper, 3 = super missile
	int ammo_mode;

	//checks to see if a shot was fired
	bool shot_fired;

	//Island AI variables
	

	//Levels to be Loaded
	LevelLoader game_levels;

	//Models to be loaded
	ModelLoader sky;
	ModelLoader sun;
	ModelLoader small_island;
	ModelLoader medium_island;
	ModelLoader large_island;
	ModelLoader ship;
	ModelLoader enemy;
	ModelLoader bullet;
	ModelLoader missile;
	ModelLoader super_missile;
	ModelLoader weapon;

	//Shaders to be loaded
	ShaderLoader water_shader;

	//textures to be loaded
	TextureLoader textures;

	//model lists to be generated
	GLuint sky_list;
	GLuint sun_list;
	GLuint small_island_list;
	GLuint medium_island_list;
	GLuint large_island_list;
	GLuint water_list;
	GLuint ship_list;
	GLuint enemy_list;
	GLuint bullet_list;
	GLuint missile_list;
	GLuint super_missile_list;
	GLuint weapon_list;

	//shader lists to be generated
	GLuint water_shader_list;

	//array to hold the water mesh
	float mesh_dimensions[ mesh_size ][ mesh_size ][3];

	//programs to be used for the shader
	GLint water_shader_program;

	//list of texture images
	GLuint* texture_images;

	//list of texture file names
	string* texture_file_names;

	//vector of islands to be drawns
	vector<Island> islands;
	vector<Island> quad_1_islands;
	vector<Island> quad_2_islands;
	vector<Island> quad_3_islands;
	vector<Island> quad_4_islands;

	//List of ships to be used
	Ship player_ship;
	Ship enemy_ship;

	//initialization functionality
	int generate_model_display_list(ModelLoader& model, GLuint model_call_list);
	int generate_shader_display_list(string& vertex_shader_name, string& fragment_shader_name, GLuint& shader_call_list);
	void load_text(string text, string font_type, FTPoint& position, unsigned int size);
	int load_levels();
	int load_models();
	int load_textures(string& texture_file);
	void create_call_lists();
	void create_water_mesh();

	//Create game functionality
	void initialize_lighting();
	void initialize_islands();
	void initialize_ship();
	void initialize_enemy();
	void initialize_power_ups();

	//game functionality
	void update_wind_factor();
	void generate_quad_islands();
	void perform_island_AI(Vertex ship_location);
	int detect_ship_collision(Vertex ship_location);
	int detect_ammo_collision(Vertex ammo_location, int& island_under_attack);
	int detect_ammo_ship_location(Vertex ammo_location, Vertex ship_location);
	int detect_power_ups(Vertex ship_location);
	void reduce_island_health(int island_number);

	//draw functionality
	void draw_model(GLuint& model_list);
	void draw_top_world();
	void draw_bottom_world();
	void draw_water();
	void draw_islands();
	void draw_ship();
	void draw_enemy();
	void draw_ammo(bool shot_fired, int ammo_number, float angle);
	void draw_island_ammo();
};

#endif