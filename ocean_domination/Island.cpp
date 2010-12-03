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

	if(island_type == 'L') {
		this->defence_radius = small_island_defence_radius;
	} else if(island_type == 'M') {
		this->defence_radius = medium_island_defence_radius;
	} else if(island_type == 'S') {
		this->defence_radius = large_island_defence_radius;
	} 

	this->location = location;
	
	if(island_type == 'L') {
		this->island_ammo_location.x = location.x + 10.0;
		this->island_ammo_location.y = 20.0 + 3.0;
		this->island_ammo_location.z = location.z - 4.0;
	} else if(island_type == 'M') {
		this->island_ammo_location.x = location.x + 6.0;
		this->island_ammo_location.y = 10.0 + 3.0;
		this->island_ammo_location.z = location.z - 2.0;
	} else if(island_type == 'S') {
		this->island_ammo_location.x = location.x + 4.0;
		this->island_ammo_location.y = 5.0 + 3.0;
		this->island_ammo_location.z = location.z - 1.0;
	} 
	

	this->island_type = island_type;
	this->weapon_fire = false;
	
	this->weapon_rotation_angle = 0.0;

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

void Island::UpdateWeaponRotationAngle(float weapon_rotation_angle) {
	this->weapon_rotation_angle = weapon_rotation_angle;
}

void Island::UpdateAmmoLocation(Vertex island_ammo_location) {
	this->island_ammo_location = island_ammo_location;
}

void Island::ResetAmmoLocation() {
	if(this->island_type == 'L') {
		this->island_ammo_location.x = this->location.x + 10.0;
		this->island_ammo_location.y = 20.0 + 3.0;
		this->island_ammo_location.z = this->location.z - 4.0;
	} else if(this->island_type == 'M') {
		this->island_ammo_location.x = this->location.x + 6.0;
		this->island_ammo_location.y = 10.0 + 3.0;
		this->island_ammo_location.z = this->location.z - 2.0;
	} else if(this->island_type == 'S') {
		this->island_ammo_location.x = this->location.x + 4.0;
		this->island_ammo_location.y = 5.0 + 3.0;
		this->island_ammo_location.z = this->location.z - 1.0;
	}
}

void Island::CreateHealthBar() {

}