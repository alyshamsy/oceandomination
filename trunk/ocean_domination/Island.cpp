#include "Island.h"

Island::Island() {
	
}

Island::~Island() {

}

int Island::InitializeIsland(Vertex location, char island_type) {
	this->health = 100;
	this->under_attack = false;

	if(island_type == 'L') {
		this->island_radius = large_island_radius;
	} else if(island_type == 'M') {
		this->island_radius = medium_island_radius;
	} else if(island_type == 'S') {
		this->island_radius = small_island_radius;
	} 

	this->location.x = location.x;
	this->location.y = location.y;
	this->location.z = location.z;

	this->island_type = island_type;
	this->weapon_fire = false;
	
	this->weapon_rotation_angle_side = 0.0;
	this->weapon_rotation_angle_up = 0.0;

	this->weapon_movement_angle_side = 0.0;
	this->weapon_movement_angle_up = 0.0;

	return 0;
}

void Island::UpdateHealth(int health) {
	this->health = health;
}

void Island::UpdateUnderAttack( bool under_attack) {
	this->under_attack = under_attack;
}

void Island::UpdateWeaponFire(bool weapon_fire) {
	this->weapon_fire = weapon_fire;
}

void Island::UpdateWeaponRotationAngleSide(float weapon_rotation_angle_side) {
	this->weapon_rotation_angle_side = weapon_rotation_angle_side;
}

void Island::UpdateWeaponRotationAngleUp(float weapon_rotation_angle_up) {
	this->weapon_rotation_angle_up = weapon_rotation_angle_up;
}

void Island::UpdateWeaponMovementAngleSide(float weapon_movement_angle_side) {
	this->weapon_movement_angle_side = weapon_movement_angle_side;
}

void Island::UpdateWeaponMovementAngleUp(float weapon_movement_angle_up) {
	this->weapon_movement_angle_up = weapon_movement_angle_up;
}