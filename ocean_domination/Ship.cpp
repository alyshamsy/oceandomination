#include "Ship.h"

Ship::Ship() {
}

Ship::~Ship() {
}

int Ship::InitializeShip(Vertex location) {
	this->health = 100;

	this->location.x = location.x;
	this->location.y = location.y;
	this->location.z = location.z;

	this->player_ammo.missiles = 15;
	this->player_ammo.super_missilies = 1;
	this->player_ammo.sniper_bullets = 5;

	return 0;
}

int Ship::UpdateShipLocation(Vertex current_location) {
	this->location.x = current_location.x;
	this->location.y = current_location.y;
	this->location.z = current_location.z;

	return 0;
}

int Ship::UpdateShip(int current_health, int current_missiles, int current_super_missiles, int current_sniper_bullets) {
	this->health = current_health;

	this->player_ammo.missiles = current_missiles;
	this->player_ammo.super_missilies = current_super_missiles;
	this->player_ammo.sniper_bullets = current_sniper_bullets;

	return 0;
}