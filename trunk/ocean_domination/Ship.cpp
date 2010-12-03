#include "Ship.h"

Ship::Ship() {
}

Ship::~Ship() {
}

int Ship::InitializeShip(Vertex& location) {
	this->health = 100;

	this->location.x = location.x;
	this->location.y = location.y;
	this->location.z = location.z;

	this->player_ammo.missiles = 15;
	this->player_ammo.super_missilies = 1;
	this->player_ammo.sniper_bullets = 5;

	return 0;
}

int Ship::UpdateShipLocation(Vertex& current_location) {
	this->location.x = current_location.x;
	this->location.y = current_location.y;
	this->location.z = current_location.z;

	return 0;
}

int Ship::UpdateHealth(int& current_health) {
	this->health = current_health;

	return 0;
}

int Ship::UpdateScore(int& current_score) {
	this->score = current_score;

	return 0;
}