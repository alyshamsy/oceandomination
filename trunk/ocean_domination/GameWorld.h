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
	int UpdateGameWorld();
	int RestartGame();

	void load_text(string text, FTPoint& position, unsigned int size);

	void draw_world();
	void draw_viewport();

	void setup_camera();

private:
	//game window dimensions
	int window_width, window_height;
	float aspect_ratio;

	//to calculate fps
	float prev_fps_time, max_time_step, fps_time;
	size_t fps_count, fps;

	//wind values in the world
	float wind_factor;
	
	//stores the current time elapsed since the start of the game
	GLfloat start_time;
	GLfloat current_time;

	//movement in the world
	GLfloat x_position, y_position, z_position, rotation_value;
	GLfloat total_x_position, total_z_position, total_rotation_value;

	//variables to draw movement in the world
	GLfloat scene_rotation, side_movement, forward_movement;
	GLfloat ship_bounce;

	int movement;

	//variables to detect collision
	int ship_collision, ammo_collision, ammo_ship_collision, power_up_collision;

	//stores which island is currently under attack
	int island_under_attack;

	//starting number of islands
	int starting_total_islands;

	//stores the current location of the ship
	Vertex current_ship_location;
	Vertex current_ammo_location;

	Vertex explosion_location;

	//missile movements
	float translation_x, translation_y, translation_z, scaling_factor;
	float missile_start_time, button_timeout;
	int ammo_number;

	//island missile movement variables
	float weapon_translation_x, weapon_translation_y, weapon_translation_z, weapon_scaling_factor;
	float island_weapon_time;

	//mode of firing 1 = regular, 2 = sniper, 3 = super missile
	int ammo_mode;

	//checks to see if a shot was fired
	bool shot_fired;

	//checks to see if the island should fire
	bool fire_next_missile;

	//checks to see if the island has been hit
	bool island_hit;

	//checks to see if the ship has been hit
	bool ship_hit;

	//checks to see if the sniper bullets were collected or if super missile was collected
	bool sniper_bullet_collected;
	float sniper_collected_time;

	bool super_missile_collected;
	float super_missile_collected_time;

	//Island AI variables
	float weapon_movement_angle;

	//boolean to keep track of whether to reduce light or increase light levels
	bool light_reduce;
	int prev_time_light;
	int current_time_light;

	//variable to keep track of time when the sniper bullet was fired
	GLfloat sniper_start_time;

	//Levels to be Loaded
	LevelLoader game_levels;

	//Models to be loaded
	ModelLoader sky;
	ModelLoader sun;
	ModelLoader small_island;
	ModelLoader medium_island;
	ModelLoader large_island;
	ModelLoader ship;
	ModelLoader bullet;
	ModelLoader missile;
	ModelLoader super_missile;
	ModelLoader weapon;
	ModelLoader canon;

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
	GLuint bullet_list;
	GLuint missile_list;
	GLuint super_missile_list;
	GLuint weapon_list;
	GLuint canon_list;
	GLuint health_powerup_list;
	GLuint missile_powerup_list;
	GLuint flare_list;
	GLuint cloud_list;

	//shader lists to be generated
	GLuint water_shader_list;

	//array to hold the water mesh
	float mesh_dimensions[ mesh_size ][ mesh_size ][3];
	float cloud_mesh[ mesh_size ][ mesh_size ][3];

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

	//struct of power up
	struct Collectables {
		Vertex power_up_locations;
		char type;
	};

	//collection of power ups
	vector<Collectables> power_ups;

	//List of ships to be used
	Ship player_ship;

	//Initialize Text displayer
	FTGLPixmapFont font;

	//initialization functionality
	int generate_model_display_list(ModelLoader& model, GLuint model_call_list);
	int generate_shader_display_list(string& vertex_shader_name, string& fragment_shader_name, GLuint& shader_call_list);
	int generate_health_powerup_list();
	int generate_missile_powerup_list();
	int generate_flare_list();

	int load_levels();
	int load_models();
	int load_textures(string& texture_file);
	void create_call_lists();
	void create_water_mesh();
	void create_smoke();
	void create_explosion();
	void create_missile_particles();
	void create_cloud_mesh();
	void create_rain();

	//Create game functionality
	void initialize_lighting();
	void initialize_islands();
	void initialize_ship();
	void initialize_power_ups();

	//game functionality
	void calculate_fps(float dtime);
	void update_wind_factor();
	void generate_quad_islands();
	void perform_island_AI(Vertex& ship_location);
	int detect_ship_collision(Vertex& ship_location);
	int detect_ammo_collision(Vertex& ammo_location, int& island_under_attack);
	int detect_ammo_ship_collision(Vertex& ship_location);
	int detect_power_ups(Vertex& ship_location, int& location);
	void reduce_island_health(int& island_number);
	void reduce_ship_health();
	void update_score(int& island_number);
	void update_lighting();
	void update_smoke();
	void update_explosion();
	void update_missile_particles();
	void update_rain();

	//draw functionality
	void draw_model(GLuint& model_list);
	void draw_top_world();
	void draw_bottom_world();
	void draw_water();
	void draw_islands();
	void draw_ship();
	void draw_ammo(bool shot_fired, int ammo_number, float angle);
	void draw_island_ammo();
	void draw_health_bar(int& health, float scale);
	void draw_island_health(float scale);
	void draw_powerups();
	void draw_smoke();
	void draw_explosion();
	void draw_missile_particles();
	void draw_clouds();
	void draw_rain();
};

#endif