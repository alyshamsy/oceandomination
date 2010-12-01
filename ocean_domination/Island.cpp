#include "Island.h"

Island::Island() {
	
}

Island::~Island() {

}

int Island::InitializeIsland(Vertex location, char island_type, int island_number) {
	this->health = 100;
	this->under_attack = false;

	this->location.x = location.x;
	this->location.y = location.y;
	this->location.z = location.z;

	this->island_type = island_type;
	this->island_number = island_number;

	return 0;
}

int Island::UpdateIsland(bool under_attack, int health) {
	this->under_attack = under_attack;
	this->health = health;

	return 0;
}