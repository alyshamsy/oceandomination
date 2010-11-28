#ifndef Game_World_H
#define Game_World_H

#include "ShaderLoader.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "LevelLoader.h"
#include "Helper.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <math.h>

#include <GL/glfw.h>
#include <FTGL/ftgl.h>

#define PI 3.14159265
#define mesh_size 1000
#define pi_conversion 0.0174532925

using namespace std;

class GameWorld {
public:
	GameWorld();
	~GameWorld();

	int InitializeGameWorld();
	int CreateGameWorld();
	void UpdateGameWorld();
	int RestartGame();

private:
	//wind values in the world
	float wind_factor;
	
	//stores the current time elapsed since the start of the game
	GLfloat start_time;
	GLfloat current_time;

	//movement in the world
	GLfloat x_position, z_position, rotation_value, viewing_value, camera_movement;

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
	ModelLoader weapon_base;
	ModelLoader weapon_head;

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
	GLuint weapon_base_list;
	GLuint weapon_head_list;

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

	//initialization functionality
	int generate_model_display_list(ModelLoader& model, GLuint model_call_list);
	int generate_shader_display_list(string& vertex_shader_name, string& fragment_shader_name, GLuint& shader_call_list);
	void load_text(string text, string font_type, FTPoint& position);
	int load_levels();
	int load_models();
	int load_textures(string& texture_file);
	void create_call_lists();
	void create_water_mesh();

	//game functionality
	void update_wind_factor();

	//draw functionality
	void draw_model(GLuint& model_list);
	void draw_top_world();
	void draw_bottom_world();
	void draw_water();
	void draw_islands();
	void draw_ship();
	void draw_enemy();
};

#endif