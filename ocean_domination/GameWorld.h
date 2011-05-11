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
#define INITIAL_TIME 180

using namespace std;

class GameWorld {
public:
	GameWorld();
	~GameWorld();

	int InitializeGameWorld();
	int CreateGameWorld();
	int UpdateGameWorld();
	int RestartGame();

	void load_text(string text, FTPoint& position);
	void setFontSize(unsigned int size);

	void draw_world();
	void draw_viewport();

	void setup_camera();

	int getTextureValue(string& texture_name);
private:
	//game window dimensions
	int window_width, window_height;
	int sniper_view_width, sniper_view_height;
	float aspect_ratio;

	//to calculate fps
	float prev_fps_time, max_time_step, fps_time;
	size_t fps_count, fps;
	bool debug_draw;
	//wind values in the world
	float wind_factor;
	
	//stores the current time elapsed since the start of the game
	GLfloat start_time;
	GLfloat current_time;

	//level timer
	int level_time;
	int level_start_time;

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
	Vector current_ship_location;
	Vector current_ammo_location;

	Vector explosion_location;

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
	
	bool damage;
	bool explosion;
	float explosion_time;

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

	//game running variables
	int exit;
	bool start_game;

	//variable to keep track of time when the sniper bullet was fired
	GLfloat sniper_start_time;

	//Levels to be Loaded
	LevelLoader game_levels;

	//Shaders to be loaded
	ShaderLoader water_shader;

	//textures to be loaded
	TextureLoader textures;

	//model lists to be generated
	GLuint water_list;
	GLuint health_powerup_list;
	GLuint missile_powerup_list;
	GLuint flare_list;
	GLuint cloud_list;
	GLuint sniper_target_list;
	GLuint sniper_meter_list;
	GLuint current_ammo_indicator_list;

	//shader lists to be generated
	GLuint water_shader_list;

	//array to hold the water mesh
	float mesh_dimensions[ mesh_size ][ mesh_size ][3];

	//programs to be used for the shader
	GLint water_shader_program;

	//list of texture images
	GLuint* texture_images;

	//textures to be loaded
	ModelLoader model;

	//list of texture file names
	vector<string>* texture_file_names;

	//list of models to be loaded
	vector<string>* models;
	vector<string>* materials;

	//array of display lists to be generated
	vector<GLuint>* model_display_list;

	//vector of islands to be drawns
	vector<Island> islands;
	vector<Island> quad_1_islands;
	vector<Island> quad_2_islands;
	vector<Island> quad_3_islands;
	vector<Island> quad_4_islands;

	//struct of power up
	struct Collectables {
		Vector power_up_locations;
		char type;
	};

	//collection of power ups
	vector<Collectables> power_ups;

	//List of ships to be used
	Ship player_ship;

	//Initialize Text displayer
	FTGLPixmapFont font;

	//initialization functionality
	//int generate_model_display_list(ModelLoader& model, GLuint model_call_list);
	int generate_shader_display_list(string& vertex_shader_name, string& fragment_shader_name, GLuint& shader_call_list);
	int generate_health_powerup_list();
	int generate_missile_powerup_list();
	int generate_flare_list();
	int generate_sniper_hud_list();

	int loadLevels();
	int loadModels(string& models_file);
	int loadMaterials(string& materials_file);
	int generateModelLists();
	int loadTextures(string& texture_file);
	
	void create_call_lists();
	void create_water_mesh();
	void create_smoke();
	void create_explosion();
	void create_missile_particles();
	void create_rain();
	void create_trail_particles();

	//Create game functionality
	void initialize_lighting();
	void initialize_islands();
	void initialize_ship();
	void initialize_power_ups();

	//game functionality
	void calculate_fps(float dtime);
	void update_wind_factor();
	void generate_quad_islands();
	void perform_island_AI(Vector& ship_location);
	int detect_ship_collision(Vector& ship_location);
	int detect_ammo_collision(Vector& ammo_location, int& island_under_attack);
	int detect_ammo_ship_collision(Vector& ship_location);
	int detect_power_ups(Vector& ship_location, int& location);
	void reduce_island_health(int& island_number);
	void reduce_ship_health();
	
	void update_score(int& island_number);
	void update_lighting();
	void update_smoke();
	void update_explosion();
	void update_missile_particles();
	void update_rain();
	void update_trail_particles();

	//high scores functionality
	int load_high_scores();
	int save_high_scores();
	int update_high_scores(string name, int score);

	//display functionality methods
	bool display_instructions();
	int display_end_of_game_screen();
	void pause_game();

	//draw functionality
	void draw_model(GLuint model_list);
	void draw_model(const char* model_name);
	void draw_top_world();
	void draw_bottom_world();
	void draw_hud();
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
	void draw_rain();
	void draw_trail_particles();
	void draw_sniper_hud();
	void draw_current_ammo();
};

#endif