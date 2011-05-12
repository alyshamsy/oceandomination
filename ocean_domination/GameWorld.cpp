#include "GameWorld.h"

string keysPressed;
static int letterCount = 0;
void  GLFWCALL getInputs( int key, int action );
void  GLFWCALL emptyCallBack( int key, int action );
void  GLFWCALL clearInputs();

//Lighting values
GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat diffusive_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specular_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_position[] = { 12.0f, 8.0f, -40.0f, 1.0f };

const size_t fps_sample_size = 100;
const int ParticleCount = 500;
const int SMParticleCount = 15;
const int trailParticles = 50;

typedef struct
{
	double Xpos;
	double Ypos;
	double Zpos;
	double Xmov;
	double Zmov;
	double Red;
	double Green;
	double Blue;
	double Direction;
	double Acceleration;
	double Deceleration;
	double Scalez;
	bool Visible;
}Particles;

Particles Smoke[ParticleCount];
Particles Explosion[ParticleCount];
Particles missile_particle[SMParticleCount];
Particles Rain[ParticleCount];
Particles trail_particles[trailParticles];

GameWorld::GameWorld():font("bin/fonts/CAMBRIA.ttf") {
	//game window dimensions
	window_width = 1280, window_height = 720;
	sniper_view_width = 400, sniper_view_height = 400;
	aspect_ratio = (float)window_width/window_height;

	prev_fps_time = 0, max_time_step = .2;
	fps_count = 0, fps = 0;
	
	//set the starting wind factor in the game (moves the waves)
	wind_factor = 0.5;
	
	//initialize the world movement values
	x_position = 0.0;
	y_position = -10.0;
	z_position = 0.0;
	rotation_value = 0.0;

	total_x_position = 0.0;
	total_z_position = 0.0;
	total_rotation_value = 0.0;

	//variables to draw movement in the world
	scene_rotation = 0.0; 
	side_movement = 0.0;
	forward_movement = 0.0;
	ship_bounce = 0.0;

	weapon_movement_angle = 0.0;

	translation_x = 0.0;
	translation_y = 0.0;
	translation_z = 0.0;
	scaling_factor = 0.0;

	weapon_translation_x = 0.0;
	weapon_translation_y = 0.0;
	weapon_translation_z = 0.0;
	weapon_scaling_factor = 0.0;

	missile_start_time = 0.0;
	button_timeout = 0.0;

	island_weapon_time = 0.0;

	ammo_number = 0;

	ammo_mode = 1;

	ship_collision = 0;
	ammo_collision = 0;
	ammo_ship_collision = 0;
	power_up_collision = 0;

	island_under_attack = 0;

	starting_total_islands = 0;

	shot_fired = false;
	fire_next_missile = false;
	island_hit = false;
	sniper_bullet_collected = false;
	super_missile_collected = false;
	
	explosion = false;
	damage = false;
	explosion_time = 0.0;

	sniper_collected_time = 0.0;
	super_missile_collected_time = 0.0;

	light_reduce = true;
	prev_time_light = 0;
	current_time_light = 0;

	movement = 0;

	sniper_start_time = 0.0;

	level_time = INITIAL_TIME;

	debug_draw = false;

	exit = 1;
	start_game = false;

	alutInit(0, 0);
	game_sound.initialize();
}

GameWorld::~GameWorld() {
	//clear all the heap memory
	delete texture_images;
	texture_images = NULL;

	delete texture_file_names;
	texture_file_names = NULL;

	delete models;
	models = NULL;

	delete materials;
	materials = NULL;
	
	delete model_display_list;
	model_display_list = NULL;
}

//initializes the variables in the game world
int GameWorld::InitializeGameWorld() {
	//create shader programs
	water_shader_program = glCreateProgram();

	string textureFileName ="textures.txt";
	loadTextures(textureFileName);

	string materials_file_name  = "materials.txt";
	loadMaterials(materials_file_name);

	string models_file_name = "models.txt";
	loadModels(models_file_name);

	generateModelLists();

	//load the game level filenames into memory and load the first level
	loadLevels();

	//create all call lists
	create_call_lists();

	//create water mesh
	create_water_mesh();

	create_smoke();

	create_explosion();

	//create_missile_particles();

	create_rain();

	return 0;
}

//creates the game world
int GameWorld::CreateGameWorld() {
	exit = 1;
	start_game = false;

	while(!start_game) {
		start_game = display_instructions();
	}

	//set the starting wind factor in the game (moves the waves)
	wind_factor = 0.5;
	
	//initialize the world movement values
	x_position = 0.0;
	y_position = -10.0;
	z_position = 0.0;
	rotation_value = 0.0;

	total_x_position = 0.0;
	total_z_position = 0.0;
	total_rotation_value = 0.0;

	//variables to draw movement in the world
	scene_rotation = 0.0; 
	side_movement = 0.0;
	forward_movement = 0.0;
	ship_bounce = 0.0;

	weapon_movement_angle = 0.0;

	translation_x = 0.0;
	translation_y = 0.0;
	translation_z = 0.0;
	scaling_factor = 0.0;

	weapon_translation_x = 0.0;
	weapon_translation_y = 0.0;
	weapon_translation_z = 0.0;
	weapon_scaling_factor = 0.0;

	missile_start_time = 0.0;
	button_timeout = 0.0;

	island_weapon_time = 0.0;

	ammo_number = 0;

	ammo_mode = 1;

	ship_collision = 0;
	ammo_collision = 0;
	ammo_ship_collision = 0;
	power_up_collision = 0;

	island_under_attack = 0;

	starting_total_islands = 0;

	shot_fired = false;
	fire_next_missile = false;
	island_hit = false;
	sniper_bullet_collected = false;
	super_missile_collected = false;
	
	explosion = false;
	damage = false;
	explosion_time = 0.0;

	sniper_collected_time = 0.0;
	super_missile_collected_time = 0.0;

	light_reduce = true;
	prev_time_light = 0;
	current_time_light = 0;

	movement = 0;

	sniper_start_time = 0.0;

	level_time = INITIAL_TIME;

	initialize_islands();
	initialize_ship();
	initialize_power_ups();
	initialize_lighting();

	x_position = player_ship.getLocation().x;
	z_position = player_ship.getLocation().z;

	starting_total_islands = islands.size();

	setFontSize(24);

	//randomize lighting at the beginning
	/*GLfloat initial_light_value = random_number_generator(0, 5, 0);
	initial_light_value = initial_light_value/10.0 + 0.5;

	ambient_light[0] = initial_light_value;
	ambient_light[1] = initial_light_value;
	ambient_light[2] = initial_light_value;

	diffusive_light[0] = initial_light_value;
	diffusive_light[1] = initial_light_value;
	diffusive_light[2] = initial_light_value;

	specular_light[0] = initial_light_value;
	specular_light[1] = initial_light_value;
	specular_light[2] = initial_light_value;*/

	//set the start time
	level_start_time = start_time = glfwGetTime();

	//update the current ship location
	current_ship_location.set_vector(0.0, 0.0, 0.0);
	player_ship.UpdateShipLocation(current_ship_location);

	int starting_score = 0;
	player_ship.UpdateScore(starting_score);

	clearInputs();

	load_high_scores();

	play_canon_sound = true;
	play_explosion_sound = true;
	play_storm_sound = true;

	game_paused = false;

	game_sound.setSoundSourcePosition(0.0, 0.0, 0.0);
	//game_sound.increaseSoundVolume("waves.wav", 0.25);
	game_sound.playSound("waves.wav");

	return 0;
}

//updates the game world and detects collision
int GameWorld::UpdateGameWorld() {
	if(!glfwGetWindowParam( GLFW_OPENED )) {
		return 4;
	}

	if(glfwGetKey( GLFW_KEY_ESC )) {
		game_paused = true;
	}

	Vector next_ship_location = current_ship_location;
	int location = 0;
	float bounce_frequency = 0.0;

	float time = glfwGetTime(), dtime = min(time - prev_fps_time, max_time_step);
    prev_fps_time = time;

	calculate_fps(dtime);

	if(glfwGetKey('G')) {
		debug_draw = !debug_draw;
	}

	//if the up key is pressed, move in the positive x and z direction
	if(glfwGetKey(GLFW_KEY_UP) || glfwGetKey( 'W' )) {
		next_ship_location.x -= (float)sin(rotation_value*radian_conversion) * 0.3f;
		next_ship_location.z -= (float)cos(rotation_value*radian_conversion) * 0.3f;
	}

	//if the down key is pressed, move in the negative x and z direction
	if(glfwGetKey(GLFW_KEY_DOWN) || glfwGetKey( 'S' )) {
		next_ship_location.x += (float)sin(rotation_value*radian_conversion) * 0.3f;
		next_ship_location.z += (float)cos(rotation_value*radian_conversion) * 0.3f;
	}
	
	if(next_ship_location.x != 0 || next_ship_location.z != 0) {
		movement = 1;
	} else {
		movement = 0;
	}

	ship_collision = detect_ship_collision(next_ship_location);

	//check if there is ship collision
	if (!ship_collision){
		current_ship_location = next_ship_location;
	} else {
		rotation_value -= 1.0f;
	}

	//if the right key is pressed, rotate in the positive direction
	if(glfwGetKey(GLFW_KEY_RIGHT) || glfwGetKey( 'D' )) {
		rotation_value -= 0.75f;
	}

	//if the left key is pressed, rotate in the negative direction
	if(glfwGetKey(GLFW_KEY_LEFT) || glfwGetKey( 'A' )) {
		rotation_value += 0.75f;
	}

	//update the current ship location
	player_ship.UpdateShipLocation(current_ship_location);
	
	//switch the firing mode if the right button is pressed
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) && shot_fired == false && glfwGetTime() - button_timeout > 0.5) {
		button_timeout = glfwGetTime();

		if(ammo_mode == 1) {
			if(player_ship.player_ammo.sniper_bullets > 0) {
				ammo_mode = 2;
			} else if(player_ship.player_ammo.super_missiles > 0) {
				ammo_mode = 3;
			} else {
				ammo_mode = 1;
			}
			ammo_number = 0;
		} else if(ammo_mode == 2) {
			if(player_ship.player_ammo.super_missiles > 0) {
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
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) && shot_fired == false && glfwGetTime() - button_timeout > 0.5) {
		button_timeout = glfwGetTime();
		missile_start_time = glfwGetTime();
		shot_fired = true;

		//reduce the ammo after firing and increase the SB and SM based on models
		if(ammo_mode == 1) {
			ammo_number = player_ship.player_ammo.missiles;
			if(ammo_number > 0) {
				ammo_number--;
				player_ship.player_ammo.missiles = ammo_number;
				create_trail_particles();

				game_sound.setSoundSourcePosition(0.0, 0.0, 0.0/*current_ammo_location.x, current_ammo_location.y, current_ammo_location.z*/);
				game_sound.increaseSoundVolume("missile.wav", 10.0);
				game_sound.playSound("missile.wav");
			}
		} else if(ammo_mode == 2) {
			sniper_start_time = glfwGetTime();
			ammo_number = player_ship.player_ammo.sniper_bullets;
			if(ammo_number > 0) {
				ammo_number--;
				player_ship.player_ammo.sniper_bullets = ammo_number;

				game_sound.setSoundSourcePosition(0.0, 0.0, 0.0/*current_ammo_location.x, current_ammo_location.y, current_ammo_location.z*/);
				game_sound.increaseSoundVolume("sniper_bullet.wav", 100.0);
				game_sound.playSound("sniper_bullet.wav");
			}
		} else if(ammo_mode == 3) {
			ammo_number = player_ship.player_ammo.super_missiles;
			if(ammo_number > 0) {
				ammo_number--;
				player_ship.player_ammo.super_missiles = ammo_number;
				create_missile_particles();
				create_trail_particles();

				game_sound.setSoundSourcePosition(0.0, 0.0, 0.0/*current_ammo_location.x, current_ammo_location.y, current_ammo_location.z*/);
				game_sound.playSound("super_missile.wav");
			}
		}
	}

	//increase sniper bullets by 2 when killing 2 islands and 2 super missiles when killing 5 islands
	if((starting_total_islands - islands.size()) % 3 == 0 && (starting_total_islands - islands.size()) != 0 && sniper_bullet_collected == false) {
		player_ship.player_ammo.sniper_bullets += 5;
		sniper_bullet_collected = true;
		sniper_collected_time = glfwGetTime();
	} else if((starting_total_islands - islands.size()) % 5 == 0 && (starting_total_islands - islands.size()) != 0 && super_missile_collected == false) {
		player_ship.player_ammo.super_missiles += 2;
		super_missile_collected = true;
		super_missile_collected_time = glfwGetTime();
	}

	if((starting_total_islands - islands.size()) % 3 == 1 && sniper_bullet_collected == true) {
		sniper_bullet_collected = false;
	} else if((starting_total_islands - islands.size()) % 5 == 1 && super_missile_collected == true) {
		super_missile_collected = false;
	}

	//adjusting rotation and forward and backward movement
	if(rotation_value > 360) {
		rotation_value = rotation_value - 360;
	} else if(rotation_value <= 0) {
		rotation_value = 360 + rotation_value;
	}

	scene_rotation = 0.0 - rotation_value;
	if(scene_rotation > 360) {
		scene_rotation = 0.0  - rotation_value;
	} else if(scene_rotation <= 0) {
		scene_rotation = 360.0 - rotation_value;
	}

	side_movement = -current_ship_location.x;
	forward_movement = -current_ship_location.z;

	//rocks the boat up and down as the waves move
	bounce_frequency = -4.5f;
	if(ammo_mode == 2) {
		ship_bounce = 0.1 * sin(current_time + bounce_frequency*0.0) * sin(current_time + bounce_frequency*0.0);
	} else {
		ship_bounce = wind_factor * sin(current_time + bounce_frequency*30.0) * sin(current_time + bounce_frequency*30.0);
	}

	//detect if there is ammo collision
	ammo_collision = detect_ammo_collision(current_ammo_location, island_under_attack);

	//if an ammo was fired and there is collision with the island, reduce the island health
	//then check if the time difference between the last fire and current time is greater than a value then allow the next missile to be fired
	if(shot_fired == true) {
		if(ammo_collision) {
			islands.at(island_under_attack).UpdateUnderAttack(true);
			if(ammo_mode == 1 && glfwGetTime() - missile_start_time >= 0.25) {
				reduce_island_health(island_under_attack);
				//sniper_bullet_collected = false;
				//super_missile_collected = false;
				island_hit = true;
			} else if(ammo_mode == 2 && glfwGetTime() - missile_start_time >= 0.25) {
				reduce_island_health(island_under_attack);
				//sniper_bullet_collected = false;
				//super_missile_collected = false;
				island_hit = true;
			} else if(ammo_mode == 3 && glfwGetTime() - missile_start_time >= 0.5) {
				reduce_island_health(island_under_attack);
				//sniper_bullet_collected = false;
				//super_missile_collected = false;
				island_hit = true;
			}
		}
		
		if(ammo_mode == 1 && glfwGetTime() - missile_start_time >= 0.5) {
			missile_start_time = glfwGetTime();
			shot_fired = false;
			island_hit = false;
			translation_x = 0.0;
			translation_y = 0.0;
			translation_z = 0.0;
			scaling_factor = 0.0;
		} else if(ammo_mode == 2 && glfwGetTime() - missile_start_time >= 1.0) {
			missile_start_time = glfwGetTime();
			sniper_start_time = 0.0;
			shot_fired = false;
			island_hit = false;
			translation_x = 0.0;
			translation_y = 0.0;
			translation_z = 0.0;
			scaling_factor = 0.0;
		} else if(ammo_mode == 3 && glfwGetTime() - missile_start_time >= 1.0) {
			missile_start_time = glfwGetTime();
			shot_fired = false;
			island_hit = false;
			translation_x = 0.0;
			translation_y = 0.0;
			translation_z = 0.0;
			scaling_factor = 0.0;
		}
	}

	perform_island_AI(current_ship_location);

	//check if the island ammo has hit the ship
	ammo_ship_collision = detect_ammo_ship_collision(current_ship_location);
	
	if(fire_next_missile == true) {
		if(ammo_ship_collision && glfwGetTime() - island_weapon_time >= 1.5) {
			reduce_ship_health();	
			fire_next_missile = false;
		} else if(glfwGetTime() - island_weapon_time >= 1.5) {
			fire_next_missile = false;
		}
	}

	//if the difference between the current time and the time the last canon was fired, fire next missile
	if(fire_next_missile == false && glfwGetTime() - island_weapon_time >= 2.0) {
		weapon_translation_x = 0.0;
		weapon_translation_y = 0.0;
		weapon_translation_z = 0.0;
		weapon_scaling_factor = 0.0;
		fire_next_missile = true;
		
		int total_islands = islands.size();
		for(int i = 0; i < total_islands; i++) {
			islands.at(i).ResetAmmoLocation();
		}
		
		island_weapon_time = glfwGetTime();
	}
	
	//detect if the power up was picked up
	power_up_collision = detect_power_ups(current_ship_location, location);

	if(power_up_collision) {
		if(power_ups.at(location).type == 'H') {
			int ship_health = player_ship.getHealth();
			
			if(ship_health > 80)
				ship_health = 100;
			else
				ship_health += 20;

			player_ship.UpdateHealth(ship_health);
			power_ups.erase(power_ups.begin()+location);
		} else if(power_ups.at(location).type == 'M') {
			int missiles = player_ship.player_ammo.missiles;
			missiles += 3;
			player_ship.player_ammo.missiles = missiles;
			power_ups.erase(power_ups.begin()+location);
		}
	}

	//update the level timer
	if (glfwGetTime() - level_start_time >= 1) {
		level_time =  level_time + (int)level_start_time - int(glfwGetTime());
		
		if(level_time < 0){
			level_time = 0;
		}

		level_start_time += 1;
	}
	
	//dynamic lighting
	update_lighting();

	//update particles
	update_smoke();
	update_explosion();
	//update_missile_particles();
	update_rain();
	update_trail_particles();
	
	if(player_ship.getHealth() == 0 || level_time == 0 || islands.size() == 0) {
		game_sound.stopAllSounds();
		exit = display_end_of_game_screen();
	} 

	return exit;
}

//restarts the game
int GameWorld::RestartGame() {
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

//generates the list for the health power up
int GameWorld::generate_health_powerup_list() {
	if(health_powerup_list == 0) {
		string health_texture;
		health_powerup_list = glGenLists(1);

		glNewList(health_powerup_list, GL_COMPILE);
			health_texture = "health.tga";
			int texture_index = 0;
			while(health_texture.compare(texture_file_names->at(texture_index)) != 0) {
				texture_index++;
			}

			glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0);   //A
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.0, 0);  //B
				glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0); //C
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.5, 0);  //D
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0);     //A
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);  //F
				glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, -0.5); //G
				glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.0, 0);    //B
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.5, -0.5);  //E
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);   //F
				glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, -0.5);  //G
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, -0.5); //H
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.5, 0);     //D
				glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.5, -0.5);  //E
				glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, -0.5); //H
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, 0);    //C
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, -0.5); //H
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.0, -0.5);  //G
				glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, 0);     //B
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, 0);    //C
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.5, -0.5);  //E
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);   //F
				glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.5, 0);      //A		
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.5, 0);     //D
			}
			glEnd();
		glEndList();
	}

	return 0;
}

//generates the list for the missile powerup
int GameWorld::generate_missile_powerup_list() {
	if(missile_powerup_list == 0) {
		string missile_texture;
		missile_powerup_list = glGenLists(1);

		glNewList(missile_powerup_list, GL_COMPILE);
			missile_texture = "missile.tga";
			int texture_index = 0;
			while(missile_texture.compare(texture_file_names->at(texture_index)) != 0) {
				texture_index++;
			}

			glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0);   //A
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.0, 0);  //B
				glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0); //C
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.5, 0);  //D
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0);     //A
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);  //F
				glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, -0.5); //G
				glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.0, 0);    //B
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.5, -0.5);  //E
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);   //F
				glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, -0.5);  //G
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, -0.5); //H
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.5, 0);     //D
				glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.5, -0.5);  //E
				glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, -0.5); //H
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, 0);    //C
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, -0.5); //H
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.0, -0.5);  //G
				glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, 0);     //B
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, 0);    //C
		
				glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.5, -0.5);  //E
				glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);   //F
				glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.5, 0);      //A		
				glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.5, 0);     //D
			}
			glEnd();
		glEndList();
	}

	return 0;
}

//generates the list to deisplay the flare upon firing a sniper bullet
int GameWorld::generate_flare_list() {
	if(flare_list == 0) {
		string flare_texture;
		flare_list = glGenLists(1);

		glNewList(flare_list, GL_COMPILE);
			flare_texture = "flare.tga";
			int texture_index = 0;
			while(flare_texture.compare(texture_file_names->at(texture_index)) != 0) {
				texture_index++;
			}

			glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(1.0, 1.0); glVertex3f(10.0, 10.0, 0);   //A
				glTexCoord2f(1.0, 0.0); glVertex3f(10.0, -10.0, 0);  //B
				glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, 0); //C
				glTexCoord2f(0.0, 1.0); glVertex3f(-10.0, 10.0, 0);  //D
			}
			glEnd();
		glEndList();
	}

	return 0;
}

int GameWorld::generate_sniper_hud_list() {
	if(sniper_target_list == 0) {
		sniper_target_list = glGenLists(1);
		
		string sniper_target = "sniper-target.tga";

		glNewList(sniper_target_list, GL_COMPILE);
			int texture_index = 0;
			GLuint sniper_target_texture;
			while(sniper_target.compare(texture_file_names->at(texture_index)) != 0) {
				texture_index++;
			}
			sniper_target_texture = texture_images[texture_index];

			glBindTexture(GL_TEXTURE_2D, sniper_target_texture);
			glBegin (GL_QUADS);
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2d (0, 0); glVertex3f (-2.3, -2.3, -0.001);
				glTexCoord2d (1, 0); glVertex3f (2.3, -2.3, -0.001);
				glTexCoord2d (1, 1); glVertex3f (2.3, 2.3, -0.001);
				glTexCoord2d (0, 1); glVertex3f (-2.3, 2.3, -0.001);
			glEnd();
		glEndList();
	}

	if(sniper_meter_list == 0) {
		sniper_meter_list = glGenLists(1);

		string sniper_meter = "sniper-meter.tga";

		glNewList(sniper_meter_list, GL_COMPILE);
			int texture_index = 0;
			GLuint sniper_meter_texture;
			while(sniper_meter.compare(texture_file_names->at(texture_index)) != 0) {
				texture_index++;
			}
			sniper_meter_texture = texture_images[texture_index];

			glBindTexture(GL_TEXTURE_2D, sniper_meter_texture);
			glBegin (GL_QUADS);
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2d (0, 0); glVertex3f (-2.3, -2.3, 0.0);
				glTexCoord2d (1, 0); glVertex3f (2.3, -2.3, 0.0);
				glTexCoord2d (1, 1); glVertex3f (2.3, 2.3, 0.0);
				glTexCoord2d (0, 1); glVertex3f (-2.3, 2.3, 0.0);
			glEnd();
		glEndList();
	}

	return 0;
}

//sets the font size of the text on the screen
void GameWorld::setFontSize(unsigned int size) {
	font.FaceSize(size);
}

//displays the text on the screen
void GameWorld::load_text(string text, FTPoint& position) {
	font.Render(text.c_str(), -1, position);
}

//load the levels for the game
int GameWorld::loadLevels() {
	game_levels.LoadLevels();

	return 0;
}

//handle to read models.txt and load models in parallel
int GameWorld::loadModels(string& models_file) {
	int number_of_models = 0;
	string current_model;
	string current_model_file_location = "bin/models/";
	current_model_file_location.append(models_file);
	fstream model_loader;

	model_loader.open(current_model_file_location, ios::in);

	if(!model_loader) {
		return 1;
	}

	//ignore first line
	model_loader.ignore(1024, '\n');

	models = new vector<string>;

	while(!model_loader.eof()) {
		model_loader >> current_model;
		current_model = current_model.substr(0, current_model.find('.'));
		models->push_back(current_model);
	}

	model_loader.close();

	//number_of_models = models->size();
	//model_display_list = new vector<GLuint>;
	
	//load the model into a call list

	model.LoadModel(models);

	return 0;
}

int GameWorld::loadMaterials(string& materials_file) {
	int number_of_materials = 0;
	string current_material;
	string current_material_file_location = "bin/models/";
	current_material_file_location.append(materials_file);
	fstream material_loader;

	material_loader.open(current_material_file_location, ios::in);

	if(!material_loader) {
		return 1;
	}

	//ignore first line
	material_loader.ignore(1024, '\n');

	materials = new vector<string>;

	while(!material_loader.eof()) {
		material_loader >> current_material;
		materials->push_back(current_material);
	}

	material_loader.close();

	//number_of_materials = materials->size();

	model.LoadMaterials(materials);

	return 0;
}

int GameWorld::generateModelLists() {
	model_display_list = new vector<GLuint>;

	model.GenerateModelDisplayList(model_display_list, texture_images, texture_file_names);

	return 0;
}

//read the texture file and load all the textures
int GameWorld::loadTextures(string& texture_file) {
	TextureLoader textures;
	int number_of_textures = 0;
	string texture_file_name;
	string texture_file_location = "bin/images/";
	texture_file_location.append(texture_file);
	fstream texture_loader;

	texture_loader.open(texture_file_location, ios::in);

	if(!texture_loader) {
		return 1;
	}

	texture_loader.ignore(1024, '\n');

	texture_file_names = new vector<string>;
	
	while(!texture_loader.eof()) {
		texture_loader >> texture_file_name;
		texture_file_names->push_back(texture_file_name);
	}

	texture_loader.close();

	number_of_textures = texture_file_names->size();
	texture_images = new GLuint[number_of_textures];
	textures.LoadTextures(texture_file_names, number_of_textures, texture_images);

	return 0;
}

//create and generate all display lists
void GameWorld::create_call_lists() {
	//create the shader display lists
	water_shader_list = glGenLists(1);

	//shows the list of shaders to be used
	string water_vertex_shader = "bin/shaders/water_shader.vert";
	string water_fragment_shader = "";

	//generate all the shader display lists
	generate_shader_display_list(water_vertex_shader, water_fragment_shader, water_shader_list);
	
	//generate the power up display lists
	generate_health_powerup_list();
	generate_missile_powerup_list();

	generate_sniper_hud_list();
	//generate_flare_list();
}

//create the water mesh to display water
void GameWorld::create_water_mesh() {
	for (int x = 0; x < mesh_size; x++) {
		for (int z = 0; z < mesh_size; z++) {
			mesh_dimensions[x][z][0] = x - (float) (mesh_size / 2);
			mesh_dimensions[x][z][1] = (float) 0.0;
			mesh_dimensions[x][z][2] = z - (float) (mesh_size / 2);
		}
	}
}

//create the smoke particles
void GameWorld::create_smoke() {
	for (int i = 1; i < ParticleCount; i++) {
		Smoke[i].Xpos = 0;
		Smoke[i].Ypos = 0;
		Smoke[i].Zpos = 0;
		Smoke[i].Xmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		Smoke[i].Zmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		Smoke[i].Red = 1;
		Smoke[i].Green = 1;
		Smoke[i].Blue = 1;
		Smoke[i].Scalez = 5.0;
		Smoke[i].Direction = 0;
		Smoke[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.005;
		Smoke[i].Deceleration = 0.0000025;
	}
}

//create the explosion particles
void GameWorld::create_explosion() {
	for (int i = 1; i < ParticleCount; i++) {
		Explosion[i].Xpos = 0;
		Explosion[i].Ypos = 0;
		Explosion[i].Zpos = 0;
		Explosion[i].Xmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		Explosion[i].Zmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		Explosion[i].Red = 1;
		Explosion[i].Green = 1;
		Explosion[i].Blue = 1;
		Explosion[i].Scalez = 2.5;
		Explosion[i].Direction = 0;
		Explosion[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.02;
		Explosion[i].Deceleration = 0.0025;
	}
}

//create the missile particles
void GameWorld::create_missile_particles() {
	for (int i = 1; i < SMParticleCount; i++) {
		missile_particle[i].Xpos = 0;
		missile_particle[i].Ypos = 0;
		missile_particle[i].Zpos = 0;
		missile_particle[i].Xmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		missile_particle[i].Zmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		missile_particle[i].Red = 1;
		missile_particle[i].Green = 1;
		missile_particle[i].Blue = 1;
		missile_particle[i].Scalez = 0.25;
		missile_particle[i].Direction = 0;
		missile_particle[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.01;
		missile_particle[i].Deceleration = 0.025;
	}
}

//creates the rain particles
void GameWorld::create_rain() {
	int mid_count = ParticleCount * 0.5;

	for (int i = 1; i < ParticleCount; i++) {
		Rain[i].Xpos = (i - mid_count)*0.05;
		Rain[i].Ypos = 10;
		Rain[i].Zpos = 0;
		Rain[i].Xmov = 0;
		Rain[i].Zmov = 0;
		Rain[i].Red = 1;
		Rain[i].Green = 1;
		Rain[i].Blue = 1;
		Rain[i].Scalez = 0.25;
		Rain[i].Direction = 0;
		Rain[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1); //* 0.01;
		Rain[i].Deceleration = 0.025;
	}
}

void GameWorld::create_trail_particles() {
	for (int i = 1; i < trailParticles; i++) {
		trail_particles[i].Xpos = 0;
		trail_particles[i].Ypos = 0;
		trail_particles[i].Zpos = 0;
		trail_particles[i].Xmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		trail_particles[i].Zmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		trail_particles[i].Red = 1;
		trail_particles[i].Green = 1;
		trail_particles[i].Blue = 1;
		trail_particles[i].Scalez = 0.2;
		trail_particles[i].Direction = 0;
		trail_particles[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.02;
		trail_particles[i].Deceleration = 0.025;
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

	Vector island_location;

	vector<Vector>* game_island_coordinates = new vector<Vector>();
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
	Vector player_location;

	player_location.x = 0.0;
	player_location.y = -10.0;
	player_location.z = 0.0;

	player_ship.InitializeShip(player_location);
}

//initialize power ups
void GameWorld::initialize_power_ups() {
	vector<int> locations;
	int high_bound = islands.size() - 1;
	int value = 0;
	int location = 0;
	float seed = 0;

	Collectables current_power_up;

	for(int i = 0; i < 6; i++) {
		value = (int)random_number_generator(0, high_bound, seed);
		seed = value*2;
		locations.push_back(value);
		if(i > 0) {
			for(int j = 0; j < i; j++) {
				if(locations.at(j) == locations.at(i)) {
					if(value > high_bound*0.5) {
						seed *= 2;
						value = (int)random_number_generator(0, value, seed);
					} else {
						seed *= 4;
						value = (int)random_number_generator(value, high_bound, seed);
					}
					locations.at(i) = value;
					break;
				}
			}
		}
	}

	for(int i = 0; i < 6; i++) {
		location = locations.at(i);
		current_power_up.power_up_locations = islands.at(location).getLocation();

		if(i % 2 == 0)
			current_power_up.type = 'H';
		else
			current_power_up.type = 'M';

		power_ups.push_back(current_power_up);
	}
}

//used to calculate the frames per second of the game
void GameWorld::calculate_fps(float dtime) {
	//fps_time is the total time since the last time the fps was calculated
	fps_time += dtime;
	++fps_count;

	// When enough frames have elapsed, the fps is recalculated.
	if (fps_count == fps_sample_size){
		fps = static_cast<size_t>(static_cast<float>(fps_sample_size) / fps_time);
		fps_count = 0;
		fps_time = 0;
	}
}

//uses a random number generator to update wind factor
void GameWorld::update_wind_factor() {
	float random_number = random_number_generator(1, 2, 0);

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
void GameWorld::perform_island_AI(Vector& ship_location) {
	int start_defending = 0;
	float defence_radius = 0.0f;

	double within_distance = 0.00000000000001;
	double distance_squared = 0.0;
	double radii_squared = 0.0;
	double overlap = 0.0;

	Vector island_location;

	int total_islands = islands.size();

	for(int i = 0; i < total_islands; i++) {
		island_location = islands.at(i).getLocation();
		defence_radius = islands.at(i).getDefenceRadius();

		radii_squared = ((defence_radius + player_ship.getShipRadius()) * (defence_radius + player_ship.getShipRadius()));
		distance_squared = ((island_location.x - ship_location.x) * (island_location.x - ship_location.x)) + ((island_location.z - ship_location.z) * (island_location.z - ship_location.z));
		overlap = radii_squared - distance_squared;
		
		if (within_distance < overlap ) {
			start_defending = 1;
			Vector weapon_location = island_location;
			float current_weapon_rotation_angle;

			islands.at(i).UpdateWeaponFire(true);

			if(islands.at(i).getIslandType() == 'L') {
				weapon_location.set_vector(island_location.x + 10.0, 20.0, island_location.z - 4.0);
				current_weapon_rotation_angle = atan2(weapon_location.z - ship_location.z, weapon_location.x - ship_location.x)*degree_conversion;
				//current_weapon_rotation_angle = 180 - current_weapon_rotation_angle;
				islands.at(i).UpdateWeaponRotationAngle(current_weapon_rotation_angle);
			} else if(islands.at(i).getIslandType() == 'M') {
				weapon_location.set_vector(island_location.x + 6.0, 10.0, island_location.z - 2.0);				
				current_weapon_rotation_angle = atan2(weapon_location.z - ship_location.z, weapon_location.x - ship_location.x)*degree_conversion;			
				//current_weapon_rotation_angle = 180 - current_weapon_rotation_angle;
				islands.at(i).UpdateWeaponRotationAngle(current_weapon_rotation_angle);
			} else {
				weapon_location.set_vector(island_location.x + 4.0, 5.0, island_location.z - 1.0);
				current_weapon_rotation_angle = atan2(weapon_location.z - ship_location.z, weapon_location.x - ship_location.x)*degree_conversion;			
				//current_weapon_rotation_angle = 180 - current_weapon_rotation_angle;
				islands.at(i).UpdateWeaponRotationAngle(current_weapon_rotation_angle);
			}
		} else {
			islands.at(i).UpdateWeaponFire(false);
		}
	}	
}

//detect collision of the ship with the island
int GameWorld::detect_ship_collision(Vector& ship_location) {
	int collision = 0;

	double collision_distance = 0.00000000000001;
	double distance_squared = 0.0;
	double radii_squared = 0.0;
	double overlap = 0.0;

	Vector island_location;

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
int GameWorld::detect_ammo_collision(Vector& ammo_location, int& island_under_attack) {
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

	Vector island_location;

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
int GameWorld::detect_ammo_ship_collision(Vector& ship_location) {
	int collision = 0;
	
	float island_ammo_radius = 0.6f;

	double collision_distance = 0.00000000000001;
	double distance_squared = 0.0;
	double radii_squared = 0.0;
	double overlap = 0.0;

	Vector ammo_location;

	int total_islands = islands.size();

	for(int i = 0; i < total_islands; i++) {
		ammo_location = islands.at(i).getAmmoLocation();

		radii_squared = ((player_ship.getShipRadius() + island_ammo_radius) * (player_ship.getShipRadius() + island_ammo_radius));
		distance_squared = ((ship_location.x - ammo_location.x) * (ship_location.x - ammo_location.x)) + ((ship_location.z - ammo_location.z) * (ship_location.z - ammo_location.z));
		overlap = radii_squared - distance_squared;
		
		if (collision_distance < overlap ) {
			collision = 1;
		}
	}
	
	return collision;
}

//detect collision with the power ups
int GameWorld::detect_power_ups(Vector& ship_location, int& location) {
	int collision = 0;
	float power_up_radius = 0.6f;

	double collision_distance = 0.00000000000001;
	double distance_squared = 0.0;
	double radii_squared = 0.0;
	double overlap = 0.0;

	Vector power_up_location;

	int number_of_powerups = power_ups.size();

	for(int i = 0; i < number_of_powerups; i++) {
		power_up_location = power_ups.at(i).power_up_locations; //power_up_locations.at(i);

		radii_squared = ((player_ship.getShipRadius() + power_up_radius) * (player_ship.getShipRadius() + power_up_radius));
		distance_squared = ((ship_location.x - power_up_location.x) * (ship_location.x - power_up_location.x)) + ((ship_location.z - power_up_location.z) * (ship_location.z - power_up_location.z));
		overlap = radii_squared - distance_squared;
		
		if (collision_distance < overlap ) {
			collision = 1;
			location = i;
		}
	}
	return collision;
}

//reduce the health of the island
void GameWorld::reduce_island_health(int& island_number){
	int health = islands.at(island_number).getHealth();
	char island_type = islands.at(island_number).getIslandType();
	int total_islands = islands.size();

	for(int i = 0; i < total_islands; i++) {
		if(i != island_number) {
			islands.at(i).UpdateUnderAttack(false);
		} 
	}

	if(island_type == 'L') {
		if(ammo_mode == 1) {
			if(health <= 10) {
				health = 0;
				glPushMatrix();
				{
					explosion_location = islands.at(island_number).getLocation();
					damage = false;
					explosion = true;
					glTranslatef(islands.at(island_number).getLocation().x, 10, islands.at(island_number).getLocation().z);
					create_explosion();
					update_explosion();
					explosion_time = glfwGetTime();
				}
				glPopMatrix();
				update_score(island_number);
				islands.erase(islands.begin()+island_number);

				game_sound.setSoundSourcePosition(0.0, 0.0, 0.0/*player_ship.getLocation().x, player_ship.getLocation().y, player_ship.getLocation().z*/);
				game_sound.increaseSoundVolume("explosion.wav", 10.0);
				game_sound.playSound("explosion.wav");
			} else {
				health -= 10;
				explosion_location = islands.at(island_number).getLocation();
				damage = true;
				islands.at(island_number).UpdateHealth(health);
				explosion = false;
			}
		} else if(ammo_mode == 2) {
			if(health <= 10) {
				health = 0;
				glPushMatrix();
				{
					explosion_location = islands.at(island_number).getLocation();
					damage = false;
					explosion = true;
					glTranslatef(islands.at(island_number).getLocation().x, 10, islands.at(island_number).getLocation().z);
					create_explosion();
					update_explosion();
					explosion_time = glfwGetTime();
				}
				glPopMatrix();
				update_score(island_number);
				islands.erase(islands.begin()+island_number);

				game_sound.setSoundSourcePosition(0.0, 0.0, 0.0/*player_ship.getLocation().x, player_ship.getLocation().y, player_ship.getLocation().z*/);
				game_sound.increaseSoundVolume("explosion.wav", 10.0);
				game_sound.playSound("explosion.wav");
			} else {
				health -= 10;
				explosion_location = islands.at(island_number).getLocation();
				damage = true;
				islands.at(island_number).UpdateHealth(health);
				explosion = false;
			}
		} else {
			if(health <= 50) {
				health = 0;
				glPushMatrix();
				{
					explosion_location = islands.at(island_number).getLocation();
					damage = false;
					explosion = true;
					glTranslatef(islands.at(island_number).getLocation().x, 10, islands.at(island_number).getLocation().z);
					create_explosion();
					update_explosion();
					explosion_time = glfwGetTime();
				}
				glPopMatrix();
				update_score(island_number);
				islands.erase(islands.begin()+island_number);

				game_sound.setSoundSourcePosition(0.0, 0.0, 0.0/*player_ship.getLocation().x, player_ship.getLocation().y, player_ship.getLocation().z*/);
				game_sound.increaseSoundVolume("explosion.wav", 10.0);
				game_sound.playSound("explosion.wav");
			} else {
				health -= 50;
				explosion_location = islands.at(island_number).getLocation();
				damage = true;
				islands.at(island_number).UpdateHealth(health);
				explosion = false;
			}
		}
	} else {
		if(ammo_mode == 1) {
			if(health <= 20) {
				health = 0;
				glPushMatrix();
				{
					explosion_location = islands.at(island_number).getLocation();
					damage = false;
					explosion = true;
					glTranslatef(islands.at(island_number).getLocation().x, 10, islands.at(island_number).getLocation().z);
					create_explosion();
					update_explosion();
					explosion_time = glfwGetTime();
				}
				glPopMatrix();
				update_score(island_number);
				islands.erase(islands.begin()+island_number);

				game_sound.setSoundSourcePosition(0.0, 0.0, 0.0/*player_ship.getLocation().x, player_ship.getLocation().y, player_ship.getLocation().z*/);
				game_sound.increaseSoundVolume("explosion.wav", 10.0);
				game_sound.playSound("explosion.wav");
			} else {
				health -= 20;
				explosion_location = islands.at(island_number).getLocation();
				damage = true;
				islands.at(island_number).UpdateHealth(health);
				explosion = false;
			}
		} else if(ammo_mode == 2) {
			if(health <= 15) {
				health = 0;
				glPushMatrix();
				{
					explosion_location = islands.at(island_number).getLocation();
					damage = false;
					explosion = true;
					glTranslatef(islands.at(island_number).getLocation().x, 10, islands.at(island_number).getLocation().z);
					create_explosion();
					update_explosion();
					explosion_time = glfwGetTime();
				}
				glPopMatrix();
				update_score(island_number);
				islands.erase(islands.begin()+island_number);

				game_sound.setSoundSourcePosition(0.0, 0.0, 0.0/*player_ship.getLocation().x, player_ship.getLocation().y, player_ship.getLocation().z*/);
				game_sound.increaseSoundVolume("explosion.wav", 10.0);
				game_sound.playSound("explosion.wav");
			} else {
				health -= 15;
				explosion_location = islands.at(island_number).getLocation();
				damage = true;
				islands.at(island_number).UpdateHealth(health);
				explosion = false;
			}
		} else {
			if(health <= 100) {
				health = 0;
				glPushMatrix();
				{
					explosion_location = islands.at(island_number).getLocation();
					damage = false;
					explosion = true;
					glTranslatef(islands.at(island_number).getLocation().x, 10, islands.at(island_number).getLocation().z);
					create_explosion();
					update_explosion();
					explosion_time = glfwGetTime();
				}
				glPopMatrix();
				update_score(island_number);
				islands.erase(islands.begin()+island_number);

				game_sound.setSoundSourcePosition(0.0, 0.0, 0.0/*player_ship.getLocation().x, player_ship.getLocation().y, player_ship.getLocation().z*/);
				game_sound.increaseSoundVolume("explosion.wav", 10.0);
				game_sound.playSound("explosion.wav");
			} /*else {
				health -= 100;
				explosion_location = islands.at(island_number).getLocation();
				explosion = true;
				islands.at(island_number).UpdateHealth(health);
			}*/
		}
	}
}

//reduce the health of the ship
void GameWorld::reduce_ship_health() {
	int ship_health = 0;
	ship_health = player_ship.getHealth();

	if(ship_health <= 5) {
		ship_health = 0;
	} else {
		ship_health -= 5;
	}
	player_ship.UpdateHealth(ship_health);
}

//updates the players score
void GameWorld::update_score(int& island_number) {
	char island_type = islands.at(island_number).getIslandType();
	int current_score = player_ship.getCurrentScore();

	if(island_type == 'L') {
		current_score += 20;
		player_ship.UpdateScore(current_score);
	} else if(island_type == 'M') {
		current_score += 10;
		player_ship.UpdateScore(current_score);
	} else {
		current_score += 5;
		player_ship.UpdateScore(current_score);
	}
}

//resets the lighting values
void GameWorld::reset_lighting() {
	float reset_value = 1.0;

	ambient_light[0] = reset_value;
	ambient_light[1] = reset_value;
	ambient_light[2] = reset_value;

	diffusive_light[0] = reset_value;
	diffusive_light[1] = reset_value;
	diffusive_light[2] = reset_value;

	specular_light[0] = reset_value;
	specular_light[1] = reset_value;
	specular_light[2] = reset_value;

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusive_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
}

//updates the lighting values
void GameWorld::update_lighting() {
	current_time_light = (int)(glfwGetTime() - start_time);

	if( (current_time_light % 10 == 0) && ( (current_time_light - prev_time_light) > 1)) {
		float ambient_light_value = ambient_light[0];
		float diffuse_light_value = diffusive_light[0];
		float specular_light_value = specular_light[0];

		if(ambient_light_value <= 0.3) {
			light_reduce = false;
		} else if(ambient_light_value == 1.0) {
			light_reduce = true;
		}

		if(light_reduce == false) {
			ambient_light_value += 0.1f;
			diffuse_light_value += 0.1f;
			specular_light_value += 0.1f;
		} else {
			ambient_light_value -= 0.1f;
			diffuse_light_value -= 0.1f;
			specular_light_value -= 0.1f;
		}

		ambient_light[0] = ambient_light_value;
		ambient_light[1] = ambient_light_value;
		ambient_light[2] = ambient_light_value;

		diffusive_light[0] = diffuse_light_value;
		diffusive_light[1] = diffuse_light_value;
		diffusive_light[2] = diffuse_light_value;

		specular_light[0] = specular_light_value;
		specular_light[1] = specular_light_value;
		specular_light[2] = specular_light_value;
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusive_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

	if(current_time_light - prev_time_light > 1) {
		prev_time_light = current_time_light;
	}
}

//updates the smoke
void GameWorld::update_smoke() {
	for (int i = 1; i < ParticleCount; i++) {
		glColor3f (Smoke[i].Red, Smoke[i].Green, Smoke[i].Blue);

		Smoke[i].Ypos = Smoke[i].Ypos + Smoke[i].Acceleration + Smoke[i].Deceleration;
		Smoke[i].Deceleration = Smoke[i].Deceleration + 0.0025;
		Smoke[i].Xpos = Smoke[i].Xpos + Smoke[i].Xmov;
		Smoke[i].Zpos = Smoke[i].Zpos + Smoke[i].Zmov;
		Smoke[i].Direction = Smoke[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1);

		
		if (Smoke[i].Ypos > 10) {
			Smoke[i].Xpos = 0;
			Smoke[i].Ypos = 0;
			Smoke[i].Zpos = 0;
			Smoke[i].Red = 1;
			Smoke[i].Green = 1;
			Smoke[i].Blue = 1;
			Smoke[i].Direction = 0;
			Smoke[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.005;
			Smoke[i].Deceleration = 0.0000025;
		}	
	}
}

//updates the explosion
void GameWorld::update_explosion() {
	for (int i = 1; i < ParticleCount; i++) {
		glColor3f (Explosion[i].Red, Explosion[i].Green, Explosion[i].Blue);

		Explosion[i].Ypos = Explosion[i].Ypos + Explosion[i].Acceleration - Explosion[i].Deceleration;
		Explosion[i].Deceleration = Explosion[i].Deceleration + 0.0025;
		Explosion[i].Xpos = Explosion[i].Xpos + Explosion[i].Xmov;
		Explosion[i].Zpos = Explosion[i].Zpos + Explosion[i].Zmov;
		Explosion[i].Direction = Explosion[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1);
	}
}

//updates the missile particles
void GameWorld::update_missile_particles() {
	for (int i = 1; i < SMParticleCount; i++) {
		glColor3f (missile_particle[i].Red, missile_particle[i].Green, missile_particle[i].Blue);

		missile_particle[i].Ypos = missile_particle[i].Ypos + missile_particle[i].Acceleration - missile_particle[i].Deceleration;
		missile_particle[i].Deceleration = missile_particle[i].Deceleration + 0.0025;
		missile_particle[i].Xpos = missile_particle[i].Xpos + missile_particle[i].Xmov;
		missile_particle[i].Zpos = missile_particle[i].Zpos + missile_particle[i].Zmov;
		missile_particle[i].Direction = missile_particle[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1);
	}
}

//updates rain
void GameWorld::update_rain() {
	int mid_count = ParticleCount * 0.5;
	for (int i = 1; i < ParticleCount; i++) {
		glColor3f (Rain[i].Red, Rain[i].Green, Rain[i].Blue);

		Rain[i].Ypos = Rain[i].Ypos - Rain[i].Acceleration - Rain[i].Deceleration;
		Rain[i].Deceleration = Rain[i].Deceleration + 0.0025;
		Rain[i].Direction = Rain[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1);
		
		if (Rain[i].Ypos < -2)
		{
			Rain[i].Xpos = (i - mid_count)*0.1;
			Rain[i].Ypos = 10;
			Rain[i].Zpos = 0;//(i - mid_count)*0.25;
			Rain[i].Red = 1;
			Rain[i].Green = 1;
			Rain[i].Blue = 1;
			Rain[i].Direction = 0;
			Rain[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.03;
			Rain[i].Deceleration = 0.025;
		}
	}
}

void GameWorld::update_trail_particles() {
	for (int i = 1; i < trailParticles; i++) {
		glColor3f (trail_particles[i].Red, trail_particles[i].Green, trail_particles[i].Blue);

		trail_particles[i].Zpos = trail_particles[i].Zpos + trail_particles[i].Acceleration + trail_particles[i].Deceleration;
		trail_particles[i].Deceleration = trail_particles[i].Deceleration + 0.0025;
		trail_particles[i].Xpos = trail_particles[i].Xpos + trail_particles[i].Xmov;
		trail_particles[i].Ypos = trail_particles[i].Ypos + trail_particles[i].Zmov;
		trail_particles[i].Direction = trail_particles[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1);

		
		if (trail_particles[i].Zpos > 1) {
			trail_particles[i].Xpos = 0;
			trail_particles[i].Ypos = 0;
			trail_particles[i].Zpos = 0;
			trail_particles[i].Red = 1;
			trail_particles[i].Green = 1;
			trail_particles[i].Blue = 1;
			trail_particles[i].Direction = 0;
			trail_particles[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.005;
			trail_particles[i].Deceleration = 0.0000025;
		}	
	}
}

int GameWorld::getTextureValue(string& texture_name) {
	int texture_index = 0;
	while(texture_name.compare(texture_file_names->at(texture_index)) != 0) {
			texture_index++;
	}

	return texture_images[texture_index];
}

int GameWorld::load_high_scores() {
	//open the high scores file and load values into an array
	string name, high_scores_file_name = "bin/high_scores.txt";
	int score, i = 0;
	fstream read_high_scores;

	read_high_scores.open(high_scores_file_name, ios::in);
	
	if(!read_high_scores) {
		return 1;
	}

	while(!read_high_scores.eof()) {
		read_high_scores >> name >> score;

		names[i] = name;
		scores[i] = score;

		i++;
	}

	read_high_scores.close();

	return 0;
}

int GameWorld::save_high_scores() {
	//open the high scores file and load values into an array
	string name, high_scores_file_name = "bin/high_scores.txt";
	int score, number_of_high_scores = 0;
	ofstream save_high_scores;

	save_high_scores.open(high_scores_file_name, ios::out);
	
	if(!save_high_scores) {
		return 1;
	}

	for(int i = 0; i < MAX_HIGH_SCORES; i++) {
		if(scores[i] == 0) {
			number_of_high_scores = i;
			break;
		}
	}

	for(int i = 0; i < number_of_high_scores; i++) {
		name = names[i];
		score = scores[i];

		if(i < number_of_high_scores - 1) {
			save_high_scores << name << ' ' << score << endl;
		} else {
			save_high_scores << name << ' ' << score;
		}
	}

	save_high_scores.close();

	return 0;
}

int GameWorld::update_high_scores(string name, int score) {
	string current_names[MAX_HIGH_SCORES];
	int current_top_scores[MAX_HIGH_SCORES];

	int current_rank = 0;

	for(int i = 0; i < MAX_HIGH_SCORES; i++) {
		current_names[i] = names[i];
		current_top_scores[i] = scores[i];
	}

	for(int i = 0; i < MAX_HIGH_SCORES; i++) {
		if(score >= current_top_scores[i]) {
			current_rank = i;
			break;
		}
	}

	for(int i = 0; i < current_rank; i++) {
		names[i] = current_names[i];
		scores[i] = current_top_scores[i];
	}

	names[current_rank] = name;
	scores[current_rank] = score;

	for(int i = current_rank; i < MAX_HIGH_SCORES - 1; i++) {
		names[i + 1] = current_names[i];
		scores[i + 1] = current_top_scores[i];
	}

	return 0;
}

bool GameWorld::display_instructions() {
	bool start = false;
	int instructions, start_button;
	double delay_time = glfwGetTime();

	string instructions_image = "instructions.tga";
	string start_button_image = "begin_button.tga";

	instructions = getTextureValue(instructions_image);
	start_button = getTextureValue(start_button_image);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	while(!start) {
		if(!glfwGetWindowParam( GLFW_OPENED )) {
			start = true;
			exit = 4;
		}

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(75.0, aspect_ratio, 1.0, 20000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -5.0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, instructions);
		glBegin(GL_QUADS);
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-5.0, -3.0, 0.0);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-5.0, 3.0, 0.0);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(5.0, 3.0, 0.0);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(5.0, -3.0, 0.0);
		glEnd();   

		glBindTexture(GL_TEXTURE_2D, start_button);
		glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
		glBegin(GL_QUADS);
			glColor4f(1.0, 1.0, 0.0, 0.5);
			glTexCoord2f(0.0, 0.0); glVertex3f(-1.75, -2.5, 0.25);			
			glTexCoord2f(0.0, 1.0); glVertex3f(-1.75, -2.0, 0.25);			
			glTexCoord2f(1.0, 1.0); glVertex3f(1.75, -2.0, 0.25);
			glTexCoord2f(1.0, 0.0); glVertex3f(1.75, -2.5, 0.25);
		glEnd();
		glPopAttrib();

		glfwSwapBuffers();

		if(glfwGetKey( GLFW_KEY_ENTER ) && glfwGetTime() - delay_time >= 1.0) {
			start = true;
		}

		glDisable(GL_BLEND);
	}

	return start;
}

int GameWorld::display_end_of_game_screen() {
	int return_value = 0;
	int final_score = player_ship.getCurrentScore();
	int end_of_game_screen, return_button;
	bool return_to_menu = false;
	double delay_time = glfwGetTime();
	string game_score = intToString(final_score);

	//reset_lighting();
	glDisable(GL_LIGHTING);

	if(player_ship.getHealth() == 0 || level_time == 0) {
		FTPoint score_text_position((window_width*0.50), window_height*0.56);

		string end_of_game_image = "failure.tga";
		string return_button_image = "back_button.tga";

		end_of_game_screen = getTextureValue(end_of_game_image);
		return_button = getTextureValue(return_button_image);

		glClearColor(0.0, 0.0, 0.0, 1.0);

		while(!return_to_menu) {
			if(!glfwGetWindowParam( GLFW_OPENED )) {
				return_to_menu = true;
				return_value = 4;
			}

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(75.0, aspect_ratio, 1.0, 20000.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0, 0.0, -5.0);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindTexture(GL_TEXTURE_2D, end_of_game_screen);
			glBegin(GL_QUADS);
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -3.0, 0.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 3.0, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 3.0, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -3.0, 0.0);
			glEnd();   

			setFontSize(50);
			load_text(game_score, score_text_position);

			glBindTexture(GL_TEXTURE_2D, return_button);
			//glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
			glBegin(GL_QUADS);
				glColor4f(1.0, 1.0, 0.0, 0.5);
				glTexCoord2f(0.0, 0.0); glVertex3f(-1.75, -2.5, 0.25);			
				glTexCoord2f(0.0, 1.0); glVertex3f(-1.75, -2.0, 0.25);			
				glTexCoord2f(1.0, 1.0); glVertex3f(1.75, -2.0, 0.25);
				glTexCoord2f(1.0, 0.0); glVertex3f(1.75, -2.5, 0.25);
			glEnd();
			//glPopAttrib();

			glfwSwapBuffers();

			if(glfwGetKey( GLFW_KEY_ENTER ) && glfwGetTime() - delay_time >= 1.0) {
				return_to_menu = true;
			}

			glDisable(GL_BLEND);
		}

		return return_value;
	} else if(islands.size() == 0) {
		FTPoint score_text_position((window_width*0.55), window_height*0.595);
		FTPoint timer_score_text_position((window_width*0.55), window_height*0.53);
		FTPoint total_score_text_position((window_width*0.55), window_height*0.45);
		FTPoint high_score_text_position((window_width*0.35), window_height*0.20);

		FTPoint high_score_name_text_position((window_width*0.455), window_height*0.445);
		
		string end_of_game_image = "success.tga";
		string return_button_image = "back_button.tga";

		end_of_game_screen = getTextureValue(end_of_game_image);
		return_button = getTextureValue(return_button_image);

		int timer_score = level_time * 5;
		string timer_score_value = "5 * " + intToString(level_time) + " = " + intToString(timer_score);

		int total_score = final_score + timer_score;
		string total_score_value = intToString(total_score);

		glClearColor(0.0, 0.0, 0.0, 1.0);

		while(!return_to_menu) {
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(75.0, aspect_ratio, 1.0, 20000.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0, 0.0, -5.0);

			glBindTexture(GL_TEXTURE_2D, end_of_game_screen);
			glBegin(GL_QUADS);
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -3.0, 0.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 3.0, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 3.0, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -3.0, 0.0);
			glEnd();   

			setFontSize(40);
			load_text(game_score, score_text_position);
			load_text(timer_score_value, timer_score_text_position);

			setFontSize(50);
			load_text(total_score_value, total_score_text_position);

			//score timer
			/*int added_score = 0;
			while(level_time > 0) {
				added_score +=  5 * (level_time + (int)level_start_time - int(glfwGetTime()));

			}*/

			if(total_score > scores[MAX_HIGH_SCORES - 1]) {
				setFontSize(75);
				load_text("High Score!!", high_score_text_position);

				glfwSwapBuffers();

				float display_wait_time = glfwGetTime();
				while(glfwGetTime() - display_wait_time <= 5.0) {
					return_to_menu = true;
				}
			} else {
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBindTexture(GL_TEXTURE_2D, return_button);
				//glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
				glBegin(GL_QUADS);
					glColor4f(1.0, 1.0, 0.0, 0.5);
					glTexCoord2f(0.0, 0.0); glVertex3f(-1.75, -2.5, 0.25);			
					glTexCoord2f(0.0, 1.0); glVertex3f(-1.75, -2.0, 0.25);			
					glTexCoord2f(1.0, 1.0); glVertex3f(1.75, -2.0, 0.25);
					glTexCoord2f(1.0, 0.0); glVertex3f(1.75, -2.5, 0.25);
				glEnd();
				//glPopAttrib();

				glfwSwapBuffers();

				if(glfwGetKey( GLFW_KEY_ENTER ) && glfwGetTime() - delay_time >= 1.0) {
					return_to_menu = true;
				}

				glDisable(GL_BLEND);
			}
		}
			
		//check the score and if it is a high score then ask for name and call update_high_scores
		if(total_score > scores[MAX_HIGH_SCORES - 1]) {
			setFontSize(24);

			game_sound.setSoundSourcePosition(0.0, 0.0, 0.0);
			game_sound.increaseSoundVolume("tada.wav", 2.0);
			game_sound.playSound("tada.wav");

			bool name_entered = false;
			int high_score_name_box;
			string high_score_name_box_image = "high-score-box.tga";
			string high_score_name = "";

			high_score_name_box = getTextureValue(high_score_name_box_image);

			glfwSetKeyCallback(getInputs);

			while(!name_entered) {
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(75.0, aspect_ratio, 1.0, 20000.0);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0, 0.0, -5.0);

				glBindTexture(GL_TEXTURE_2D, high_score_name_box);
				glBegin(GL_QUADS);
					glColor3f(1.0, 1.0, 1.0);
					glTexCoord2f(0.0, 0.0); glVertex3f(-2.5, -1.5, 0.0);			
					glTexCoord2f(0.0, 1.0); glVertex3f(-2.5, 1.5, 0.0);			
					glTexCoord2f(1.0, 1.0); glVertex3f(2.5, 1.5, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(2.5, -1.5, 0.0);
				glEnd();

				high_score_name = keysPressed;
				load_text(high_score_name, high_score_name_text_position);

				glfwSwapBuffers();

				if(glfwGetKey( GLFW_KEY_ENTER )) {
					name_entered = true;
				}
			}
			glfwSetKeyCallback(emptyCallBack);
			update_high_scores(high_score_name, total_score);
		}

		//add fireworks particles and high score check
		/*end_game_time = glfwGetTime();
		
		while(glfwGetTime() - end_game_time <= 5.0) {
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			

			glfwSwapBuffers();
		}*/

		save_high_scores();

		return return_value;
	}
}

void GameWorld::pause_game() {
	int select_exit_value = 0;
	int exit_box;
	int button;

	double pause_time = glfwGetTime();
	double selection_time = pause_time;

	FTPoint paused_text_position((window_width*0.39), window_height*0.75);
	FTPoint yes_text_position((window_width*0.410), window_height*0.385);
	FTPoint no_text_position((window_width*0.565), window_height*0.385);

	string exit_box_image = "exit-box.tga";
	string button_image = "button.tga";

	exit_box = getTextureValue(exit_box_image);
	button = getTextureValue(button_image);

	game_sound.pauseAllSounds();

	while(game_paused) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_LIGHTING);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(75.0, aspect_ratio, 1.0, 20000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -5.0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, exit_box);
		glBegin(GL_QUADS);
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0.0, 0.0); glVertex3f(-2.25, -1.5, 0.0);			
			glTexCoord2f(0.0, 1.0); glVertex3f(-2.25, 1.5, 0.0);			
			glTexCoord2f(1.0, 1.0); glVertex3f(2.25, 1.5, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(2.25, -1.5, 0.0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, button);
		glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
		glBegin(GL_QUADS);
			glColor3f(1.0, 1.0, 1.0);
			if(select_exit_value == 1) {
				glColor4f(1.0, 1.0, 0.0, 0.5);
			}
			glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, -1.0, 0.25);			
			glTexCoord2f(0.0, 1.0); glVertex3f(-1.5, -0.5, 0.25);			
			glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5, 0.25);
			glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -1.0, 0.25);
		glEnd();
		glPopAttrib();

		glBindTexture(GL_TEXTURE_2D, button);
		glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
		glBegin(GL_QUADS);
			glColor3f(1.0, 1.0, 1.0);
			if(select_exit_value == 0) {
				glColor4f(1.0, 1.0, 0.0, 0.5);
			}
			glTexCoord2f(0.0, 0.0); glVertex3f(1.5, -1.0, 0.25);			
			glTexCoord2f(0.0, 1.0); glVertex3f(1.5, -0.5, 0.25);			
			glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.25);
			glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -1.0, 0.25);
		glEnd();
		glPopAttrib();

		glPixelTransferf(GL_RED_BIAS, 0.0f);
		glPixelTransferf(GL_GREEN_BIAS, 0.0f);
		glPixelTransferf(GL_BLUE_BIAS, 0.0f);

		setFontSize(50);
		load_text("Game Paused", paused_text_position);

		setFontSize(24);
		load_text("Yes", yes_text_position);
		load_text("No", no_text_position);

		glfwSwapBuffers();

		if(glfwGetKey( GLFW_KEY_RIGHT ) && glfwGetTime() - selection_time >= 0.25) {
			game_sound.setSoundSourcePosition(0.0, 0.0, 0.0);
			game_sound.playSound("menu-change.wav");
			select_exit_value = 0;
			selection_time = glfwGetTime();
		} else if(glfwGetKey( GLFW_KEY_LEFT ) && glfwGetTime() - selection_time >= 0.25) {
			game_sound.setSoundSourcePosition(0.0, 0.0, 0.0);
			game_sound.playSound("menu-change.wav");
			select_exit_value = 1;
			selection_time = glfwGetTime();
		}

		if(glfwGetKey( GLFW_KEY_ENTER ) && select_exit_value == 1) {
			game_paused = false;
			game_sound.stopAllSounds();

			ammo_mode = 1;
			exit = 0;
		} else if(glfwGetKey( GLFW_KEY_ENTER ) && select_exit_value == 0) {
			game_paused = false;

			glEnable(GL_LIGHTING);
			glDisable(GL_BLEND);

			glfwSetTime(pause_time);

			game_sound.stopSound("menu-change.wav");
			game_sound.playAllSounds();
		}

		if(!glfwGetWindowParam( GLFW_OPENED )) {
			exit = 4;
		}
	}
}

//draw the models by calling the display list
void GameWorld::draw_model(const char* model_name) {
	string current_model = model_name;
	int display_list_index = 0;
	GLuint model_list;

	while(current_model.compare(models->at(display_list_index)) != 0) {
		display_list_index++;
	}

	model_list = model_display_list->at(display_list_index);

	glCallList(model_list);
}

//draw the models by calling the display list
void GameWorld::draw_model(GLuint model_list) {
	glCallList(model_list);
}

//sets up the camera
void GameWorld::setup_camera() {
	//single player view
	glViewport(0,0, window_width, window_height);
	glScissor(0,0, window_width, window_height);
	glEnable(GL_SCISSOR_TEST);
		
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//set up the camera and the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, aspect_ratio, 1.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);
	if(ammo_mode != 2 || game_paused == true) {
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -10.0);
		glRotatef(17, 1.0, 0.0, 0.0);
	} else if(ammo_mode == 2) {
		glLoadIdentity();
		glTranslatef(0.0, -2.5, -3.5);
		glRotatef(-17, 1.0, 0.0, 0.0);
	}

	if(game_paused == true) {
		pause_game();
	}

	draw_world();

	if(ammo_mode == 2) {
		//2nd view
		glViewport(440.0, 97.0, sniper_view_width, sniper_view_height);
		glScissor(440.0, 97.0, sniper_view_width, sniper_view_height);
		glEnable(GL_SCISSOR_TEST);// OpenGL rendering goes here ...
		glClear( GL_DEPTH_BUFFER_BIT );

		//set up the camera and the viewport
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(75.0, 1.0, 1.0, 10000.0);

		glMatrixMode(GL_MODELVIEW);

		//transform for second player			
		draw_viewport();
	}
}

//draw the entire game world
void GameWorld::draw_world() {
	//draw the world, the ammo and the particle effects
	glPushMatrix();
	{
		glRotatef(scene_rotation, 0.0, 1.0, 0.0);
		draw_top_world();
	}
	glPopMatrix();
	
	glPushMatrix();
	{
		glRotatef(scene_rotation, 0.0, 1.0, 0.0);
		glTranslatef(side_movement, 0.0, forward_movement);
		draw_bottom_world();
		draw_ammo(shot_fired, ammo_number, rotation_value);
	}
	glPopMatrix();

	//draw rain
	if(ambient_light[0] <= 0.5) {
		glDisable(GL_LIGHTING);
		draw_rain();
		glEnable(GL_LIGHTING);
	}

	//draw extras on screen
	glPushMatrix();
	{
		glTranslatef(0.0, -ship_bounce, 0.0);
		draw_ship();
		
		/*glPushMatrix();
		{
			glTranslatef(0.0, 0.0, -2.0);
			draw_missile_particles();
		}
		glPopMatrix();*/
	}
	glPopMatrix();

	//draw rain
	if(ambient_light[0] <= 0.5) {
		glDisable(GL_LIGHTING);
		glPushMatrix();
			if(ammo_mode != 2) {
				glTranslatef(0.0, 0.0, 5.0);
			}
			draw_rain();
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}

	//draw the game hud
	draw_hud();
}

//display time, health bar, ammo numbers and debug values
void GameWorld::draw_hud() {
	//draw health bar on screen
	glPushMatrix();
	{
		//draw health bar in the bottom left corner
		int health_value = player_ship.getHealth();
		if(ammo_mode != 2) {
			glRotatef(-17, 1.0, 0.0, 0.0);
			glTranslatef(-2.5, -1.5, 7.85);
		} else {
			glRotatef(17, 1.0, 0.0, 0.0);
			glTranslatef(-4.0, 0.2, 0.20);
		}
		draw_health_bar(health_value, 0.25);
	}
	glPopMatrix();

	glPushMatrix();
	{
		if(ammo_mode == 1) {
			glRotatef(-17, 1.0, 0.0, 0.0);
			glTranslatef(0.32, -0.715, 9.0);
			glScalef(0.38, 0.13, 0.0);
		} else if(ammo_mode == 2) {
			glRotatef(17, 1.0, 0.0, 0.0);
			glTranslatef(1.925, 0.0, 0.0);
			glScalef(1.25, 0.5, 0.0);
		} else {
			glRotatef(-17, 1.0, 0.0, 0.0);
			glTranslatef(0.736, -0.715, 9.0);
			glScalef(0.38, 0.13, 0.0);
		}
		draw_current_ammo();
	}
	glPopMatrix();

	//draw the time on the top of the screen
	string timer = "Time: ";
	FTPoint time_position( window_width * 0.5, window_height * 0.95);
	char timeSTR [10];

	timer += itoa(level_time, timeSTR, 10);
	load_text(timer, time_position);

	if(debug_draw) {
		//display frames per second on top right corner
		FTPoint fps_text_position(window_width*0.88, window_height*0.95);
		load_text("FPS:", fps_text_position);

		FTPoint fps_value_position(window_width*0.94, window_height*0.95);
		string frames = intToString(fps);
		load_text(frames, fps_value_position);

		FTPoint angle_text_position(window_width*0.78, window_height*0.95);
		load_text(floatToString(scene_rotation), angle_text_position);
	}

	//draw the red line under the current ammo type

	//display ammo in centre bottom corner
	FTPoint ammo_text_position(window_width*0.55, window_height*0.05);
	load_text("Ammo", ammo_text_position);

	FTPoint missile_text_position(window_width*0.62, window_height*0.05);
	load_text("M:", missile_text_position);

	FTPoint missile_value_position(window_width*0.65, window_height*0.05);
	string missiles = intToString(player_ship.player_ammo.missiles);
	load_text(missiles, missile_value_position);

	FTPoint bullet_text_position(window_width*0.70, window_height*0.05);
	load_text("SB:", bullet_text_position);

	FTPoint bullet_value_position(window_width*0.73, window_height*0.05);
	string bullets = intToString(player_ship.player_ammo.sniper_bullets);
	load_text(bullets, bullet_value_position);

	FTPoint super_missile_text_position(window_width*0.77, window_height*0.05);
	load_text("SM:", super_missile_text_position);

	FTPoint super_missile_value_position(window_width*0.80, window_height*0.05);
	string super_missiles = intToString(player_ship.player_ammo.super_missiles);
	load_text(super_missiles, super_missile_value_position);

	//display score text
	FTPoint score_text_position(window_width*0.88, window_height*0.05);
	load_text("Score", score_text_position);

	FTPoint score_value_position(window_width*0.96, window_height*0.05);
	string score = intToString(player_ship.getCurrentScore());
	load_text(score, score_value_position);

	//display text when power up picked up centre mid screen
	if(sniper_bullet_collected && glfwGetTime() - sniper_collected_time < 2.0) {
		setFontSize(34);
		FTPoint sniper_collected_text_position(window_width*0.35, window_height*0.70);
		load_text("Sniper Bullet Collected", sniper_collected_text_position);
		setFontSize(24);
	}

	if(super_missile_collected && glfwGetTime() - super_missile_collected_time < 2.0) {
		setFontSize(34);
		FTPoint super_missile_collected_text_position(window_width*0.35, window_height*0.55);
		load_text("Super Missile Collected", super_missile_collected_text_position);
		setFontSize(24);
	}
}

//draw the sky, sun and clouds
void GameWorld::draw_top_world() {
	//draw sky
	glPushMatrix();
	{	
		glScalef(5, 5, 5);
		glTranslatef(0, -2, 0);
		draw_model("sky");
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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if(ambient_light[0] <= 0.6) {
				glColor4f(1.0, 1.0, 1.0, ambient_light[0] - 0.3);
			}
			draw_model("sun");
			glColor3f(1.0, 1.0, 1.0);
		glDisable(GL_BLEND);
	}
	glPopMatrix();

	//draw rain clouds
	glPushMatrix();
	{	
		if(ambient_light[0] <= 0.6) {	
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(1.0, 1.0, 1.0, 0.6 - ambient_light[0]);
				glScalef(4, 4, 4);
				glTranslatef(0, -2, 0);
				draw_model("grey_sky");
				glColor3f(1.0, 1.0, 1.0);
			glDisable(GL_BLEND);

			if(play_storm_sound == true) {
				game_sound.setSoundSourcePosition(0.0, 6.0, 0.0);
				game_sound.playSound("storm.wav");

				play_storm_sound = false;
			}
		} else {
			if(play_storm_sound == false) {
				play_storm_sound = true;
			}
		}
	}
	glPopMatrix();
}

//draw the water and islands
void GameWorld::draw_bottom_world() {
	GLfloat ship_location [] = {player_ship.getLocation().x, player_ship.getLocation().y, player_ship.getLocation().z};
	GLint location_time, location_wind, location_ship, location_mov;

	current_time = glfwGetTime();

	if((int)current_time % 300 == 0) {
		update_wind_factor();
	}
	
	//draw islands
	glPushMatrix();
	{
		draw_islands();


		if(damage) {
			glPushMatrix();
			{
				glTranslatef(explosion_location.x, 6, explosion_location.z);
				glRotatef(-scene_rotation, 0.0, 1.0, 0.0);
				draw_smoke();
			}
			glPopMatrix();
		}

		if(explosion) {
			glPushMatrix();
			{
				glTranslatef(explosion_location.x, 6.0, explosion_location.z);
				glRotatef(-scene_rotation, 0.0, 1.0, 0.0);
				if(glfwGetTime() - explosion_time >= 1.0 && glfwGetTime() - explosion_time <= 15.0) {
					draw_smoke();
				}
				draw_explosion();
			}
			glPopMatrix();

			/*if(play_explosion_sound == true) {
				play_explosion_sound = false;

				game_sound.setSoundSourcePosition(player_ship.getLocation().x, player_ship.getLocation().y, player_ship.getLocation().z);
				game_sound.increaseSoundVolume("explosion.wav", 10.0);
				game_sound.playSound("explosion.wav");
			}
		} else 
		
		if(play_explosion_sound == false && glfwGetTime() - explosion_time >= 3.0) {
			play_explosion_sound = true;*/
		}

		draw_island_health(0.5);
	}
	glPopMatrix();

	//draw water with waves
	glCallList(water_shader_list);
	
	location_time = glGetUniformLocation(water_shader_program, "time");
	glUniform1f(location_time, current_time);

	location_wind = glGetUniformLocation(water_shader_program, "wind_factor");
	glUniform1f(location_wind, wind_factor);

	location_mov = glGetUniformLocation(water_shader_program, "movement");
	glUniform1i(location_mov, movement);

	location_ship = glGetUniformLocation(water_shader_program, "ship_location");
	glUniform3fv(location_ship, 1, ship_location);

	glPushMatrix();
	{	
		glTranslatef(0, -2.0, 0);
		draw_water();
	}
	glPopMatrix();
	water_shader.DetachShader(water_shader_program);

	//draw islands ammo
	glPushMatrix();
	{
		draw_island_ammo();
	}
	glPopMatrix();

	//draw powerups
	glPushMatrix();
	{
		draw_powerups();
	}
	glPopMatrix();
}

//draws the viewport
void GameWorld::draw_viewport() {
	Vector island_view_location;
	float zoom_distance = 125.0;
	float island_radius, distance_to_island, island_distance_comparison;
	float lower_angle, upper_angle, lower_limit_angle, upper_limit_angle, island_angle;
	float ship_angle = scene_rotation;
	int total_islands = islands.size();

	if(ship_angle > 90.0 && ship_angle < 180) {
		ship_angle = 180 - ship_angle;
	} else if(ship_angle > 180.0 && ship_angle < 270) {
		ship_angle -= 180;
	} else if(ship_angle > 270.0 && ship_angle < 360) {
		ship_angle = 360 - ship_angle;
	}

	for(int i = 0; i < total_islands; i++) {
		island_view_location = islands.at(i).getLocation();
		island_radius = islands.at(i).getIslandRadius();
		lower_angle = atan(((island_view_location.x - island_radius*2.5) - current_ship_location.x)/(current_ship_location.z - island_view_location.z)) * degree_conversion;
		upper_angle = atan(((island_view_location.x + island_radius*2.5) - current_ship_location.x)/(current_ship_location.z - island_view_location.z)) * degree_conversion;
		island_angle = atan((island_view_location.x - current_ship_location.x)/(current_ship_location.z - island_view_location.z)) * degree_conversion;

		if(island_angle < 0) {
			lower_angle = -lower_angle;
			upper_angle = -upper_angle;
			island_angle = -island_angle;
		}

		lower_limit_angle = min(lower_angle, upper_angle);
		upper_limit_angle = max(lower_angle, upper_angle);

		distance_to_island = sqrt((current_ship_location.x - island_view_location.x)*(current_ship_location.x - island_view_location.x) + (current_ship_location.z - island_view_location.z)*(current_ship_location.z - island_view_location.z));
		if((int)island_angle % 90 == 0) {
			island_distance_comparison = distance_to_island - island_radius*1.5;
		} else {
			island_distance_comparison = distance_to_island*cos(island_angle*radian_conversion);
		}

		if(island_distance_comparison < 140.0 && ship_angle > lower_limit_angle && ship_angle < upper_limit_angle) {			
			zoom_distance = min(125.0, distance_to_island - island_radius*1.5);
		}
	}

	//draw the flare, sky, water, sun, islands, sniper bullet
	//check if island is in the middle
	glPushMatrix();
	{
		glTranslatef(0.0, 0.0, zoom_distance);

		glRotatef(scene_rotation, 0.0, 1.0, 0.0);
		draw_top_world();
		
		glTranslatef(side_movement, 0.0, forward_movement);
		draw_bottom_world();
		draw_ammo(shot_fired, ammo_number, rotation_value);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -3.0);

		if(ambient_light[0] <= 0.5) {
			glDisable(GL_LIGHTING);
			draw_rain();
			glEnable(GL_LIGHTING);
		}

		draw_sniper_hud();
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
			while(water_texture.compare(texture_file_names->at(texture_index)) != 0) {
				texture_index++;
			}

			glPolygonMode(GL_FRONT, GL_LINE);
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
			glPolygonMode(GL_FRONT, GL_FILL);
		glEndList();
	} else {
		glCallList(water_list);
	}
}

//draw all the islands
void GameWorld::draw_islands() {
	int total_islands = islands.size();

	for(int i = 0; i < total_islands; i++) {
		Vector island_location = islands.at(i).getLocation();

		if(islands.at(i).getIslandType() == 'L') {
			//draw large islands
			glPushMatrix();
			{	
				glTranslatef(island_location.x, island_location.y, island_location.z);
				glScalef(1.67, 1.55, 1.67);
				draw_model("island");
			}
			glPopMatrix();
			
			glPushMatrix();
			{	
				glTranslatef(island_location.x + 10.0, 20.0, island_location.z - 4.0);

				float weapon_rotation_angle = islands.at(i).getWeaponRotationAngle();
				weapon_rotation_angle = 180 - weapon_rotation_angle;

				glRotatef(weapon_rotation_angle, 0.0, 1.0, 0.0);
				draw_model("weapon");
			}
			glPopMatrix();		
		} else if(islands.at(i).getIslandType() == 'M') {
			//draw medium islands
			glPushMatrix();
			{	
				glTranslatef(island_location.x, island_location.y, island_location.z);
				draw_model("island");
			}
			glPopMatrix();
			
			glPushMatrix();
			{	
				glTranslatef(island_location.x + 6.0, 10.0, island_location.z - 2.0);
				
				float weapon_rotation_angle = islands.at(i).getWeaponRotationAngle();
				weapon_rotation_angle = 180 - weapon_rotation_angle;

				glRotatef(weapon_rotation_angle, 0.0, 1.0, 0.0);
				draw_model("weapon");
			}
			glPopMatrix();			
		} else if(islands.at(i).getIslandType() == 'S') {
			//draw small islands
			glPushMatrix();
			{	
				glTranslatef(island_location.x, island_location.y, island_location.z);
				glScalef(0.67, 0.75, 0.67);
				draw_model("island");
			}
			glPopMatrix();
			
			glPushMatrix();
			{	
				glTranslatef(island_location.x + 4.0, 5.0, island_location.z - 1.0);
				
				float weapon_rotation_angle = islands.at(i).getWeaponRotationAngle();
				weapon_rotation_angle = 180 - weapon_rotation_angle;

				glRotatef(weapon_rotation_angle, 0.0, 1.0, 0.0);
				draw_model("weapon");
			}
			glPopMatrix();			
		} 
	}
}

//draw the ship
void GameWorld::draw_ship() {
	glPushMatrix();
	{
		glScalef(0.30, 0.30, 0.30);
		//glScalef(0.25, 0.25, 0.25);
		draw_model("enemy");
	}
	glPopMatrix();
}

//draws the missile on the screen
void GameWorld::draw_ammo(bool shot_fired, int ammo_number, float angle) {
	int current_missile_numbers = player_ship.player_ammo.missiles;
	int current_sniper_numbers =  player_ship.player_ammo.sniper_bullets;
	int current_super_missile_numbers =  player_ship.player_ammo.super_missiles;

	current_ammo_location = player_ship.getLocation();
	
	if(!island_hit) {
		if(ammo_mode == 1 && shot_fired == true && current_missile_numbers > 0) {	
			translation_x -= (float)sin(angle*radian_conversion) * 1.5f;
			translation_y += 0.02f;
			translation_z -= (float)cos(angle*radian_conversion) * 1.5f;
			scaling_factor += 0.2;
		
			if(ammo_number % 2 == 0) {
				current_ammo_location.x += 0.095 + translation_x;
				current_ammo_location.y += translation_y;
				current_ammo_location.z += translation_z;

				glPushMatrix();
				{
					glTranslatef(current_ammo_location.x, current_ammo_location.y, current_ammo_location.z);
					glRotatef(angle, 0.0, 1.0, 0.0);
					draw_trail_particles();
					glScalef(1.0 + scaling_factor, 1.0 + scaling_factor, 1.0 + scaling_factor);
					draw_model("missile");
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
					draw_trail_particles();
					glScalef(1.0 + scaling_factor, 1.0 + scaling_factor, 1.0 + scaling_factor);
					draw_model("missile");
				}
				glPopMatrix();
			}
		} else if(ammo_mode == 2  && shot_fired == true && current_sniper_numbers > 0) {
			translation_x -= (float)sin(angle*radian_conversion) * 3.0f;
			translation_y += 0.0f;
			translation_z -= (float)cos(angle*radian_conversion) * 3.0f;
			scaling_factor += 0.05;
			
			current_ammo_location.x += translation_x;
			current_ammo_location.y += 2.5 + translation_y;
			current_ammo_location.z += translation_z;

			glPushMatrix();
			{
				glTranslatef(current_ammo_location.x, current_ammo_location.y, current_ammo_location.z);
				glRotatef(angle, 0.0, 1.0, 0.0);
				glScalef(0.5 + scaling_factor, 0.5 + scaling_factor, 0.5 + scaling_factor);
				draw_model("bullet");
			}
			glPopMatrix();
		} else if(ammo_mode == 3  && shot_fired == true && current_super_missile_numbers > 0) {	
			translation_x -= (float)sin(angle*radian_conversion) * 1.5f;
			translation_y += 0.02f;
			translation_z -= (float)cos(angle*radian_conversion) * 1.5f;
			scaling_factor += 0.1;
			
			current_ammo_location.x += translation_x;
			current_ammo_location.y += translation_y;
			current_ammo_location.z += translation_z;

			glPushMatrix();
			{
				glTranslatef(current_ammo_location.x, current_ammo_location.y, current_ammo_location.z);
				glRotatef(angle, 0.0, 1.0, 0.0);
				
				glPushMatrix();
				{
					glScalef(1.0 + scaling_factor, 1.0 + scaling_factor, 1.0 + scaling_factor);
					draw_model("super-missile");
				}
				glPopMatrix();

				draw_trail_particles();
			}
			glPopMatrix();
		}
	}
}

//this draws the bullets fired by the island
void GameWorld::draw_island_ammo() {
	int total_islands = islands.size();
	float weapon_rotation_angle;
	float weapon_rotation_angle_down = 0.0;
	float hyp = 0.0;
	Vector current_island_ammo_location;

	if(!ammo_ship_collision) {
		for(int i = 0; i < total_islands; i++) {
			if(islands.at(i).doWeaponFire() == true && fire_next_missile == true) {
				weapon_rotation_angle = islands.at(i).getWeaponRotationAngle();
				current_island_ammo_location = islands.at(i).getAmmoLocation();

				weapon_rotation_angle = 90 - weapon_rotation_angle;

				hyp = getHypotenuse((current_ship_location.x - current_island_ammo_location.x), (current_ship_location.z - current_island_ammo_location.z));
				weapon_rotation_angle_down = atan2(hyp, current_island_ammo_location.y)*degree_conversion;

				if(islands.at(i).getIslandType() != 'L') {
					weapon_translation_x -= (float)sin(weapon_rotation_angle*radian_conversion) * 0.2f;
					weapon_translation_y -= (float)cos(weapon_rotation_angle_down*radian_conversion) * 0.3f;
					weapon_translation_z -= (float)cos(weapon_rotation_angle*radian_conversion) * 0.2f;
					weapon_scaling_factor += 0.1;
				} else {
					weapon_translation_x -= (float)sin(weapon_rotation_angle*radian_conversion) * 0.6f;
					weapon_translation_y -= (float)cos(weapon_rotation_angle_down*radian_conversion) * 0.6f;
					weapon_translation_z -= (float)cos(weapon_rotation_angle*radian_conversion) * 0.6f;
					weapon_scaling_factor += 0.2;
				}

				current_island_ammo_location.x += weapon_translation_x;
				current_island_ammo_location.y += weapon_translation_y;
				current_island_ammo_location.z += weapon_translation_z;

				islands.at(i).UpdateAmmoLocation(current_island_ammo_location);

				glPushMatrix();
				{
					glTranslatef(current_island_ammo_location.x, current_island_ammo_location.y, current_island_ammo_location.z);
					glRotatef(-weapon_rotation_angle_down, 0.0, 0.0, 1.0);
					glScalef(1.0 + scaling_factor, 1.0 + scaling_factor, 1.0 + scaling_factor);
					draw_model("canon");
				}
				glPopMatrix();

				if(play_canon_sound == true) {
					game_sound.setSoundSourcePosition(current_island_ammo_location.x, current_island_ammo_location.y, current_island_ammo_location.z);
					game_sound.increaseSoundVolume("canon.wav", 10.0);
					game_sound.playSound("canon.wav");

					play_canon_sound = false;
				}
			}
		}
	} else {
		play_canon_sound = true;
	}
	
}

//draws the health bar on the screen and scales it up or down to the preferred size
void GameWorld::draw_health_bar(int& health, float scale) {
	float health_value = ((float)health/10)*(scale);
	string health_icon = "health.tga";

	int health_icon_index = 0;
	int texture_index = 0;

	glPolygonMode(GL_FRONT, GL_FILL);
	while(health_icon.compare(texture_file_names->at(texture_index)) != 0) {
			texture_index++;
	}
	health_icon_index = texture_index;
	
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture_images[health_icon_index]);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0*scale, 0.5*scale, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0*scale, 0.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.5*scale, 0.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.5*scale, 0.5*scale, 0.0);
	glEnd();

	glPolygonMode(GL_FRONT, GL_LINE);

	if(health_value <= 2*scale) {
		glColor3f(1.0, 0.0, 0.0);
	} else if(health_value <= 4*scale) {
		glColor3f(1.0, 1.0, 0.0);
	} else {
		glColor3f(0.0, 1.0, 0.0);
	}

	glBegin(GL_QUADS);	
		glVertex3f(0.0 - 0.02, 0.5*scale + 0.02, 0.0);
		glVertex3f(0.0 - 0.02, 0.0 - 0.02, 0.0);
		glVertex3f(10.0*scale + 0.02, 0.0 - 0.02, 0.0);
		glVertex3f(10.0*scale + 0.02, 0.5*scale + 0.02, 0.0);
	glEnd();

	glPolygonMode(GL_FRONT, GL_FILL);

	glBegin(GL_QUADS);
		for(float i = 0.0; i < (health_value); i +=(0.5*scale)) {
			glVertex3f(i, 0.5*scale, 0.0);
			glVertex3f(i, 0.0, 0.0);
			glVertex3f(i+(0.5*scale), 0.0, 0.0);
			glVertex3f(i+(0.5*scale), 0.5*scale, 0.0);
		}
	glEnd();
	glEnable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
}

void GameWorld::draw_island_health(float scale) {
	float health_value;
	string health_icon = "health.tga";
	Vector health_bar_translation;

	int health_icon_index = 0;
	int texture_index = 0;
	int total_islands = islands.size();
	
	glDisable(GL_LIGHTING);
	for(int i = 0; i < total_islands; i++) {
		if(islands.at(i).isUnderAttack() == true) {
			health_value = ((float)islands.at(i).getHealth()/10)*(scale);
			health_bar_translation = islands.at(i).getLocation();
			
			glPushMatrix();
			{
				if(islands.at(i).getIslandType() == 'L') {
					glTranslatef(health_bar_translation.x, 22.0, health_bar_translation.z);
				} else if(islands.at(i).getIslandType() == 'M') {
					glTranslatef(health_bar_translation.x, 15.0, health_bar_translation.z);
				} else {
					glTranslatef(health_bar_translation.x, 10.0, health_bar_translation.z);
				}

				//billboards the island health bar
				glRotatef(-scene_rotation, 0.0, 1.0, 0.0);
			
				glPolygonMode(GL_FRONT, GL_FILL);
				while(health_icon.compare(texture_file_names->at(texture_index)) != 0) {
						texture_index++;
				}
				health_icon_index = texture_index;
				glBindTexture(GL_TEXTURE_2D, texture_images[health_icon_index]);
				glBegin(GL_QUADS);	
					glTexCoord2f(0.0, 1.0); glVertex3f(-1.0*scale, 0.5*scale, 0.0);
					glTexCoord2f(0.0, 0.0); glVertex3f(-1.0*scale, 0.0, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(-0.5*scale, 0.0, 0.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(-0.5*scale, 0.5*scale, 0.0);
				glEnd();

				glPolygonMode(GL_FRONT, GL_LINE);
	
				if(health_value <= 2*scale) {
					glColor3f(1.0, 0.0, 0.0);
				} else if(health_value <= 4*scale) {
					glColor3f(1.0, 1.0, 0.0);
				} else {
					glColor3f(0.0, 1.0, 0.0);
				}

				glBegin(GL_QUADS);	
					glVertex3f(0.0 - 0.02, 0.5*scale + 0.02, 0.0);
					glVertex3f(0.0 - 0.02, 0.0 - 0.02, 0.0);
					glVertex3f(10.0*scale + 0.02, 0.0 - 0.02, 0.0);
					glVertex3f(10.0*scale + 0.02, 0.5*scale + 0.02, 0.0);
				glEnd();

				glPolygonMode(GL_FRONT, GL_FILL);

				glBegin(GL_QUADS);
					for(float i = 0.0; i < (health_value); i +=(0.5*scale)) {
						glVertex3f(i, 0.5*scale, 0.0);
						glVertex3f(i, 0.0, 0.0);
						glVertex3f(i+(0.5*scale), 0.0, 0.0);
						glVertex3f(i+(0.5*scale), 0.5*scale, 0.0);
					}
				glEnd();
			}
			glPopMatrix();
		}
	}
	glEnable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
}

//draws the power ups
void GameWorld::draw_powerups() {
	int number_of_powerups = power_ups.size();
	Vector location;

	for(int i = 0; i < number_of_powerups; i++) {
		location = power_ups.at(i).power_up_locations;
		if(power_ups.at(i).type == 'H') {
			glPushMatrix();
			{	
				glTranslatef(location.x, location.y + 5, location.z);
				draw_model(health_powerup_list);
			}
			glPopMatrix();
		} else if(power_ups.at(i).type == 'M') {
			glPushMatrix();
			{	
				glTranslatef(location.x, location.y + 5, location.z);
				draw_model(missile_powerup_list);
			}
			glPopMatrix();
		}
	}
}

//draw the smoke
void GameWorld::draw_smoke() {
	string smoke_texture;
	smoke_texture = "smoke.tga";
	int texture_index = 0;
	while(smoke_texture.compare(texture_file_names->at(texture_index)) != 0) {
		texture_index++;
	}

	for (int i = 1; i < ParticleCount; i++) {
		glPushMatrix();
			glTranslatef (Smoke[i].Xpos, Smoke[i].Ypos, Smoke[i].Zpos);
			glRotatef (Smoke[i].Direction - 90, 0, 0, 1);
   
			glScalef (Smoke[i].Scalez, Smoke[i].Scalez, Smoke[i].Scalez);
   
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);

			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
    
			glBegin (GL_QUADS);
			glTexCoord2d (0, 0);
			glVertex3f (-0.5, -0.5, 0);
			glTexCoord2d (1, 0);
			glVertex3f (0.5, -0.5, 0);
			glTexCoord2d (1, 1);
			glVertex3f (0.5, 0.5, 0);
			glTexCoord2d (0, 1);
			glVertex3f (-0.5, 0.5, 0);
			glEnd();

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
		glPopMatrix();
	}
}

//draw the explosion
void GameWorld::draw_explosion() {
	string explosion_texture;
	explosion_texture = "explosions.tga";
	int texture_index = 0;
	while(explosion_texture.compare(texture_file_names->at(texture_index)) != 0) {
		texture_index++;
	}

	for (int i = 1; i < ParticleCount; i++) {
		if(Explosion[i].Ypos >= -3) {
			glPushMatrix();
				glTranslatef (Explosion[i].Xpos, Explosion[i].Ypos, Explosion[i].Zpos);
				glRotatef (Explosion[i].Direction - 90, 0, 0, 1);
   
				glScalef (Explosion[i].Scalez, Explosion[i].Scalez, Explosion[i].Scalez);
   
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
			
				glColor4f(1.0, 1.0, 1.0, 1.0);
				glBegin (GL_QUADS);
					glTexCoord2d (0, 0); glVertex3f (-0.5, -0.5, 0);
					glTexCoord2d (1, 0); glVertex3f (0.5, -0.5, 0);
					glTexCoord2d (1, 1); glVertex3f (0.5, 0.5, 0);
					glTexCoord2d (0, 1); glVertex3f (-0.5, 0.5, 0);
				glEnd();

				glEnable(GL_DEPTH_TEST);
				glDisable(GL_BLEND);
			glPopMatrix();
		}
	}
}

//draw the missile particles
void GameWorld::draw_missile_particles() {
	string missile_particle_texture;
	missile_particle_texture = "explosion.tga";
	int texture_index = 0;
	while(missile_particle_texture.compare(texture_file_names->at(texture_index)) != 0) {
		texture_index++;
	}

	for (int i = 1; i < SMParticleCount; i++) {
		if (missile_particle[i].Ypos >= -2) {
			glPushMatrix();
				glTranslatef (missile_particle[i].Xpos, missile_particle[i].Ypos, missile_particle[i].Zpos);
				glRotatef (missile_particle[i].Direction - 90, 0, 0, 1);
   
				glScalef (missile_particle[i].Scalez, missile_particle[i].Scalez, missile_particle[i].Scalez);
   
				glDisable (GL_DEPTH_TEST);
				glEnable (GL_BLEND);

				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
    
				glBegin (GL_QUADS);
					glTexCoord2d (0, 0); glVertex3f (-0.5, -0.5, 0);
					glTexCoord2d (1, 0); glVertex3f (0.5, -0.5, 0);
					glTexCoord2d (1, 1); glVertex3f (0.5, 0.5, 0);
					glTexCoord2d (0, 1); glVertex3f (-0.5, 0.5, 0);
				glEnd();

				glDisable (GL_BLEND);
				glEnable(GL_DEPTH_TEST);
			glPopMatrix();
		}
	}
}

//draw the rain
void GameWorld::draw_rain() {
	string rain_texture;
	rain_texture = "ice.tga";

	int texture_index = 0;
	while(rain_texture.compare(texture_file_names->at(texture_index)) != 0) {
		texture_index++;
	}

	for (int i = 1; i < ParticleCount; i++) {
		glPushMatrix();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glTranslatef (Rain[i].Xpos, Rain[i].Ypos, Rain[i].Zpos);
   
			glScalef (Rain[i].Scalez, Rain[i].Scalez, Rain[i].Scalez);
   
			glDisable (GL_DEPTH_TEST);
			glEnable (GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
    
			glBegin (GL_QUADS);
				glTexCoord2d (0, 0); glVertex3f (-0.05, -0.35, 0);
				glTexCoord2d (1, 0); glVertex3f (0.05, -0.35, 0);
				glTexCoord2d (1, 1); glVertex3f (0.05, 0.35, 0);
				glTexCoord2d (0, 1); glVertex3f (-0.05, 0.35, 0);
			glEnd();

			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}
}

//draw the trail particles
void GameWorld::draw_trail_particles() {
	string rain_texture;
	rain_texture = "trail_smoke.tga";

	int texture_index = 0;
	while(rain_texture.compare(texture_file_names->at(texture_index)) != 0) {
		texture_index++;
	}

	for (int i = 1; i < trailParticles; i++) {
		glPushMatrix();
			glTranslatef (trail_particles[i].Xpos, trail_particles[i].Ypos, trail_particles[i].Zpos);
			glRotatef (trail_particles[i].Direction - 90, 0, 0, 1);
   
			glScalef (trail_particles[i].Scalez, trail_particles[i].Scalez, trail_particles[i].Scalez);
   
			glDisable (GL_DEPTH_TEST);
			glEnable (GL_BLEND);

			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, texture_images[texture_index]);
    
			glBegin (GL_QUADS);
				glTexCoord2d (0, 0); glVertex3f (-0.5, -0.5, 0);
				glTexCoord2d (1, 0); glVertex3f (0.5, -0.5, 0);
				glTexCoord2d (1, 1); glVertex3f (0.5, 0.5, 0);
				glTexCoord2d (0, 1); glVertex3f (-0.5, 0.5, 0);
			glEnd();

			glDisable (GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}
}

void GameWorld::draw_sniper_hud() {
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCallList(sniper_target_list);

	glPushMatrix();
		glRotatef(scene_rotation, 0.0, 0.0, 1.0);
		glCallList(sniper_meter_list);
	glPopMatrix();

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}

void GameWorld::draw_current_ammo() {
	if(current_ammo_indicator_list == 0) {
		current_ammo_indicator_list = glGenLists(1);
		glNewList(current_ammo_indicator_list, GL_COMPILE);
			glDisable(GL_LIGHTING);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_QUADS);	
				glVertex3f(0.0, 0.05, 0.0);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(0.15, 0.0, 0.0);
				glVertex3f(0.15, 0.05, 0.0);
			glEnd();
			glEnable(GL_LIGHTING);
		glEndList();
	} else {
		glCallList(current_ammo_indicator_list);
	}
}

void  GLFWCALL clearInputs() {
	keysPressed.clear();
	letterCount = 0;
}

void  GLFWCALL emptyCallBack( int key, int action ){

}

void  GLFWCALL getInputs( int key, int action ){
	if( action != GLFW_PRESS ) {
        return;
    }

	if(letterCount < 8) {
		//	glfwSetKeyCallback( keyInputs.getInputs );
		switch( key ){
			case GLFW_KEY_ESC: break;
			case 'A':keysPressed+='A';letterCount++;break;
			case 'B':keysPressed+='B';letterCount++;break;
			case 'C':keysPressed+='C';letterCount++;break;
			case 'D':keysPressed+='D';letterCount++;break;
			case 'E':keysPressed+='E';letterCount++;break;
			case 'F':keysPressed+='F';letterCount++;break;
			case 'G':keysPressed+='G';letterCount++;break;
			case 'H':keysPressed+='H';letterCount++;break;
			case 'I':keysPressed+='I';letterCount++;break;
			case 'J':keysPressed+='J';letterCount++;break;
			case 'K':keysPressed+='K';letterCount++;break;
			case 'L':keysPressed+='L';letterCount++;break;
			case 'M':keysPressed+='M';letterCount++;break;
			case 'N':keysPressed+='N';letterCount++;break;
			case 'O':keysPressed+='O';letterCount++;break;
			case 'P':keysPressed+='P';letterCount++;break;
			case 'Q':keysPressed+='Q';letterCount++;break;
			case 'R':keysPressed+='R';letterCount++;break;
			case 'S':keysPressed+='S';letterCount++;break;
			case 'T':keysPressed+='T';letterCount++;break;
			case 'U':keysPressed+='U';letterCount++;break;
			case 'V':keysPressed+='V';letterCount++;break;
			case 'W':keysPressed+='W';letterCount++;break;
			case 'X':keysPressed+='X';letterCount++;break;
			case 'Y':keysPressed+='Y';letterCount++;break;
			case 'Z':keysPressed+='Z';letterCount++;break;
			case GLFW_KEY_DEL:
			case GLFW_KEY_BACKSPACE:
				if(keysPressed.size()>0) {
					keysPressed.erase(keysPressed.size()-1);
					letterCount--;
				}
			break;
		}
	 } else{
		switch(key) {
			case GLFW_KEY_BACKSPACE:
			case GLFW_KEY_DEL:
				keysPressed.erase(keysPressed.size()-1);
				letterCount--;
		}
	}
}